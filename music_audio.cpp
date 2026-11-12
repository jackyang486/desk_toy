#include "music_audio.h"
#include "esp_heap_caps.h"

#include <atomic>
#include <memory>

#include "ESP32_board.h"
#include "SD_MMC.h"
#include "setting.h"
#include "HTTPClient.h"
#include "WiFiClient.h"
#include "WiFiClientSecure.h"
#include "components/espressif/esp_audio_codec/esp_mp3_dec.h"
#include "components/espressif/esp_audio_codec/esp_audio_simple_dec.h"

#define DEC_INBUF_SIZE   4096               // 输入缓冲：单帧最大 ~1.5KB，4KB 足够
#define DEC_OUTBUF_SIZE  8192               // 输出缓冲：最大帧 1152 样本 x 2ch x 2B = 4608B
#define DEC_TASK_STACK   (1024 * 10)        // 40KB：文件播放 24KB 够用，但 HTTPS 电台的 TLS
                                            // 握手要 ~10KB 栈，统一按 40KB 分配（PSRAM 不缺）
#define STREAM_TIMEOUT_MS      8000         // 连续这么久读不到数据 → 判定断流并重连
#define STREAM_RETRY_MIN_MS    1500         // 重连最小间隔（失败次数越多退避越久）
#define STREAM_RETRY_MAX_MS    8000

enum PlayerCmd : uint8_t { PCMD_IDLE = 0, PCMD_PLAY, PCMD_PLAY_STREAM, PCMD_ABORT };

static SemaphoreHandle_t g_cmd_lock = NULL;       // 保护 g_cmd / g_cmd_path
static volatile PlayerCmd g_cmd = PCMD_IDLE;      // LVGL 线程写、解码任务读
static char     g_cmd_path[256] = {0};
static volatile bool g_run = false;               // 解码任务生命周期（音乐页打开期间为 true）
static volatile bool g_playing = false;           // 当前文件解码循环进行中
static volatile bool g_file_open = false;         // 任务正占用 SD 卡（停歌后计时器清掉）
static volatile bool g_is_stream = false;         // 当前音源是网络流（切台/暂停语义不同）
static volatile bool g_paused = false;
static volatile bool g_eof = false;               // 播完/打不开，待 LVGL 线程取走
static volatile uint32_t g_pos_ms = 0;            // 当前播放位置 ms（任务推进）
static volatile uint32_t g_dur_ms = 0;            // 时长估算 ms（任务推进）
static volatile uint8_t  g_volume = 100;          // 0~100 软件音量
static std::atomic<uint8_t> g_level{0};           // 响度 0~255（任务写、LVGL 线程读）
static volatile uint32_t g_level_ms = 0;          // 最近一次更新响度的时刻
// 三块大缓冲运行时从 PSRAM 一次性惰性分配、永不释放（不产生碎片）：
// MP3 解码的输入/输出/任务栈对访存速度不敏感，放 PSRAM 归还 ~18KB 内部 SRAM。
// 注：不能用 EXT_RAM_BSS_ATTR——本环境 sdkconfig 未开
// CONFIG_SPIRAM_ALLOW_BSS_SEG_EXTERNAL_MEMORY，linker 也没有 .ext_ram.bss 段。
static uint8_t    *g_inbuf  = nullptr;   // DEC_INBUF_SIZE
static uint8_t    *g_outbuf = nullptr;   // DEC_OUTBUF_SIZE
static StackType_t *g_stack = nullptr;   // DEC_TASK_STACK（任务栈，已开 ALLOW_STACK_EXTERNAL）
static StaticTask_t g_task_buf;          // TCB 很小，留内部
static TaskHandle_t g_task_handle = NULL;

MusicAudio music_audio;

static void post_cmd(PlayerCmd cmd, const char *path) {
    if (g_cmd_lock == NULL) return;
    xSemaphoreTake(g_cmd_lock, portMAX_DELAY);
    g_cmd = cmd;
    if (path != NULL) strlcpy(g_cmd_path, path, sizeof(g_cmd_path));
    xSemaphoreGive(g_cmd_lock);
}

// 解码 PCM 的响度（RMS）→ 0~255，供 RGB 灯使用
static void vu_from_pcm(const int16_t *pcm, size_t samples) {
    if (samples == 0) return;
    uint64_t acc = 0;
    for (size_t i = 0; i < samples; i++) acc += (int32_t)pcm[i] * pcm[i];
    const float rms = sqrtf((float)(acc / samples));
    float lvl = rms / 32767.0f * 255.0f * 3.0f;      // x3 灵敏度，小音量也能亮
    if (lvl > 255.0f) lvl = 255.0f;
    g_level.store((uint8_t)lvl, std::memory_order_relaxed);
    g_level_ms = millis();
}

// 解码一个文件直到播完/被打断。返回 false 表示文件打不开或解码器初始化失败。
static bool decode_one_file(const char *full_path,
                            const std::shared_ptr<ai_vox::AudioDeviceEs8311> &dev) {
    File f = SD_MMC.open(full_path);
    if (!f) {
        printf("[player] open failed: %s\n", full_path);
        return false;
    }
    g_file_open = true;
    g_playing = true;
    g_pos_ms = 0;
    g_dur_ms = 0;

    // 每次开文件前重新注册：Board::playMp3() 播完会 esp_audio_dec_unregister(MP3)，
    // 不补这一步的话，通知音播过一次之后，换下一首 open 就会失败（注册是幂等的）。
    esp_mp3_dec_register();

    esp_audio_simple_dec_cfg_t cfg = {
        .dec_type = ESP_AUDIO_SIMPLE_DEC_TYPE_MP3,
        .dec_cfg = NULL,
        .cfg_size = 0,
    };
    esp_audio_simple_dec_handle_t dec = NULL;
    const esp_audio_err_t open_ret = esp_audio_simple_dec_open(&cfg, &dec);
    const bool ok = (open_ret == ESP_AUDIO_ERR_OK);
    if (!ok) printf("[player] dec open fail: %d\n", (int)open_ret);

    uint32_t pcm_rate = 44100;
    uint8_t  pcm_ch = 2;
    bool info_set = false;
    bool file_eof = false;
    int32_t fill = 0;              // inbuf 里未消费的数据量
    uint32_t consumed_total = 0;   // 解码器实际消费（含其内部缓存）的字节数：用于定位音频数据起点
    uint32_t audio_start = 0;      // 音频数据起点（ID3 等头部之后的字节偏移，首帧时确定）
    uint64_t play_frames = 0;      // 已解码帧数（每样本一声道）：逐帧整除重算，无累积误差
    uint32_t fade_frames = 0;      // 开播淡入总帧数
    uint32_t fade_done = 0;        // 已淡入帧数
    int stall = 0;                 // 连续无产出计数（尾部垃圾数据防死循环）

    while (ok && g_run) {
        if (g_cmd != PCMD_IDLE) break;                  // 切歌（PLAY）/退出请求（ABORT）：立即停
        if (g_paused) { vTaskDelay(pdMS_TO_TICKS(20)); continue; }   // 暂停：不喂数据，DMA 自动清空出静音

        if (fill < DEC_INBUF_SIZE && !file_eof) {   // 尽量填满：解码器会把输入缓存到内部，必须持续供给
            const int n = f.read(g_inbuf + fill, DEC_INBUF_SIZE - fill);
            if (n <= 0) file_eof = true;
            else fill += n;
        }
        if (fill == 0 && file_eof) break;                // 文件读完

        esp_audio_simple_dec_raw_t raw = {};
        raw.buffer = g_inbuf;
        raw.len = fill;
        raw.eos = file_eof;
        esp_audio_simple_dec_out_t out = {};
        out.buffer = g_outbuf;
        out.len = DEC_OUTBUF_SIZE;
        esp_audio_simple_dec_process(dec, &raw, &out);

        if (raw.consumed == 0 && out.decoded_size == 0) {
            // 解码器既不消费也不产出：多半是输入不足一帧，已被它内部缓存（正常现象），
            // 继续读文件补充数据即可；只有文件读完（或缓冲已满）仍无产出才是真垃圾，
            // 丢一个字节推进防死循环，连续多次直接放弃。
            const bool can_read = (!file_eof && fill < DEC_INBUF_SIZE);
            if (can_read) continue;
            if (++stall > 64) { printf("[player] too many stalls, stop\n"); break; }
            if (fill == 0) break;
            memmove(g_inbuf, g_inbuf + 1, (size_t)(fill - 1));
            fill--;
            continue;
        }
        stall = 0;
        consumed_total += raw.consumed;
        fill -= raw.consumed;
        if (raw.consumed > 0 && fill > 0) memmove(g_inbuf, g_inbuf + raw.consumed, (size_t)fill);

        if (out.decoded_size > 0) {
            // 首帧：拿到真实采样率/声道数 → 重开 codec 输出端（44.1k 立体声直连）+ 定位音频数据起点
            if (!info_set) {
                esp_audio_simple_dec_info_t info = {};
                if (esp_audio_simple_dec_get_info(dec, &info) == ESP_AUDIO_ERR_OK) {
                    pcm_rate = info.sample_rate ? info.sample_rate : 44100;
                    pcm_ch = info.channel ? info.channel : 2;
                    if (dev) dev->ReopenOutput(pcm_rate, pcm_ch);
                    audio_start = consumed_total;   // 首帧时解码器已消费掉 ID3 等头部
                    info_set = true;
                }
            }
            const size_t nsamples = out.decoded_size / 2;
            // 逐帧整除重算（不能累加每帧毫秒值：1152×1000/44100 截断成 26，每帧丢 0.12ms，
            // 4 分钟累计漂移 ~1.1s，正是"歌词晚一秒"的根源）
            play_frames += nsamples / pcm_ch;
            g_pos_ms = (uint32_t)(play_frames * 1000ULL / pcm_rate);
            // 总时长精修：按"剩余字节 / 已消费音频字节"比例外推，VBR 也随播放逐渐收敛
            const uint64_t audio_bytes = consumed_total - audio_start;
            if (audio_bytes > 0) {
                g_dur_ms = (uint64_t)(f.size() - audio_start) * g_pos_ms / audio_bytes;
            }

            // 开播淡入 150ms：掩掉 codec 重开换时钟时的 DAC 电平跳变（进页面音爆）
            if (fade_frames == 0) fade_frames = pcm_rate * 150 / 1000;
            int16_t *p = (int16_t *)g_outbuf;
            if (fade_done < fade_frames) {
                for (size_t i = 0; i < nsamples / pcm_ch; i++) {
                    const int32_t sc = (int32_t)fade_done * 10000 / (int32_t)fade_frames;   // 0~10000
                    for (uint8_t c = 0; c < pcm_ch; c++) {
                        int16_t *s = &p[i * pcm_ch + c];
                        *s = (int16_t)((int32_t)*s * sc / 10000);
                    }
                    fade_done++;
                }
            }

            // 响度取样必须在音量衰减之前：滑块音量一低，PCM 被平方曲线压得很小，
            // 测出来 level 只有个位数，RGB 灯就永远只有底部几颗绿灯在闪
            vu_from_pcm(p, nsamples);
            // 软件音量：滑块百分比平方曲线，等价于原 Audio 库的 setVolume 行为
            const uint8_t vol = g_volume;
            if (vol < 100) {
                const int32_t sc = (int32_t)vol * vol;   // /10000 = (vol/100)^2
                for (size_t i = 0; i < nsamples; i++) p[i] = (int16_t)((int32_t)p[i] * sc / 10000);
            }
            if (dev) dev->Write(p, nsamples);   // 阻塞写，DMA 满自然节流
        }
    }

    if (dec) esp_audio_simple_dec_close(dec);
    // 把 DMA 残尾排空成静音：切歌打断时半帧截断、收尾的咔哒声都消掉
    if (dev && info_set) {
        int16_t z[576] = {0};
        dev->Write(z, 576);
    }
    f.close();
    g_file_open = false;
    g_playing = false;
    return ok;
}

// ---------------- 网络电台流（MP3 over HTTP/HTTPS）----------------
// 一次 HTTP 会话的播放循环。返回 true 表示连接异常、可重连；false 表示收到停止命令。
static bool stream_session(const char *url, const std::shared_ptr<ai_vox::AudioDeviceEs8311> &dev) {
    const bool use_tls = (strncmp(url, "https://", 8) == 0);
    WiFiClient plain;
    WiFiClientSecure tls;
    HTTPClient http;
    if (use_tls) {
        tls.setInsecure();                 // 电台流不做证书校验：省 RAM、免 NTP 时间依赖
        http.begin(tls, url);
    } else {
        http.begin(plain, url);
    }
    http.setConnectTimeout(STREAM_TIMEOUT_MS);
    http.setTimeout(STREAM_TIMEOUT_MS);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);   // 电台 CDN 常见 302 跳转
    http.setUserAgent("ESP32-DeskToy/1.0");

    const int code = http.GET();
    if (code != HTTP_CODE_OK) {
        printf("[radio] http %d, retry\n", code);
        http.end();
        return true;
    }
    WiFiClient *st = http.getStreamPtr();
    if (st == nullptr) {
        http.end();
        return true;
    }

    // 单次播放页里 emos 通知音可能重注册过 MP3 解码器，这里补注册（幂等）
    esp_mp3_dec_register();
    esp_audio_simple_dec_cfg_t cfg = {
        .dec_type = ESP_AUDIO_SIMPLE_DEC_TYPE_MP3,
        .dec_cfg = NULL,
        .cfg_size = 0,
    };
    esp_audio_simple_dec_handle_t dec = NULL;
    if (esp_audio_simple_dec_open(&cfg, &dec) != ESP_AUDIO_ERR_OK) {
        printf("[radio] dec open fail\n");
        http.end();
        return true;
    }

    uint32_t pcm_rate = 44100;
    uint8_t  pcm_ch = 2;
    bool info_set = false;
    int32_t fill = 0;                 // inbuf 未消费字节
    int stall = 0;                    // 连续无产出计数（跳过 ICY 元数据等垃圾）
    uint32_t last_data_ms = millis();
    bool want_exit = false;

    g_playing = true;

    while (g_run) {
        if (g_cmd != PCMD_IDLE) { want_exit = true; break; }   // 切台/退出

        if (g_paused) {
            // 暂停：停止读取让 TCP 窗口反压服务端（不消耗流量），恢复时把积压的旧数据丢掉，
            // 保证继续播放时听到的是"实时"内容而不是几秒前的缓冲
            while (g_paused && g_run && g_cmd == PCMD_IDLE) vTaskDelay(pdMS_TO_TICKS(20));
            uint8_t tmp[128];
            int remain = st->available();
            while (remain > 0) {
                const int want = remain > (int)sizeof(tmp) ? (int)sizeof(tmp) : remain;
                const int n = st->read(tmp, want);
                if (n <= 0) break;
                remain -= n;
            }
            fill = 0;
            last_data_ms = millis();
            continue;
        }

        // 1) 填输入缓冲
        if (fill < DEC_INBUF_SIZE) {
            const int avail = st->available();
            if (avail > 0) {
                int want = DEC_INBUF_SIZE - fill;
                if (want > avail) want = avail;
                const int n = st->read(g_inbuf + fill, want);
                if (n > 0) { fill += n; last_data_ms = millis(); }
            } else if (!st->connected()) {
                printf("[radio] connection closed\n");
                break;                                  // 断流：交给外层重连
            } else if (millis() - last_data_ms > STREAM_TIMEOUT_MS) {
                printf("[radio] read timeout\n");
                break;                                  // 断流：交给外层重连
            } else {
                vTaskDelay(pdMS_TO_TICKS(2));           // 等数据，避免空转
                continue;
            }
        }

        // 2) 解码（与文件路径同一套逻辑，只是没有 eos）
        esp_audio_simple_dec_raw_t raw = {};
        raw.buffer = g_inbuf;
        raw.len = fill;
        raw.eos = false;
        esp_audio_simple_dec_out_t out = {};
        out.buffer = g_outbuf;
        out.len = DEC_OUTBUF_SIZE;
        esp_audio_simple_dec_process(dec, &raw, &out);

        if (raw.consumed == 0 && out.decoded_size == 0) {
            if (fill < DEC_INBUF_SIZE) continue;        // 输入不足一帧，继续读
            if (++stall > 64) {                         // 连续 64 次仍无产出：流已不可用
                printf("[radio] no audio, reconnect\n");
                break;
            }
            memmove(g_inbuf, g_inbuf + 1, (size_t)(fill - 1));   // 丢 1 字节重新对齐帧头
            fill--;
            continue;
        }
        stall = 0;
        fill -= raw.consumed;
        if (raw.consumed > 0 && fill > 0) memmove(g_inbuf, g_inbuf + raw.consumed, (size_t)fill);

        if (out.decoded_size > 0) {
            if (!info_set) {                            // 首帧：按流真实采样率/声道重开输出端
                esp_audio_simple_dec_info_t info = {};
                if (esp_audio_simple_dec_get_info(dec, &info) == ESP_AUDIO_ERR_OK) {
                    pcm_rate = info.sample_rate ? info.sample_rate : 44100;
                    pcm_ch = info.channel ? info.channel : 2;
                    if (dev) dev->ReopenOutput(pcm_rate, pcm_ch);
                    info_set = true;
                    printf("[radio] stream %u Hz %u ch\n", (unsigned)pcm_rate, (unsigned)pcm_ch);
                }
            }
            const size_t nsamples = out.decoded_size / 2;
            int16_t *p = (int16_t *)g_outbuf;
            vu_from_pcm(p, nsamples);
            const uint8_t vol = g_volume;
            if (vol < 100) {
                const int32_t sc = (int32_t)vol * vol;
                for (size_t i = 0; i < nsamples; i++) p[i] = (int16_t)((int32_t)p[i] * sc / 10000);
            }
            if (dev) dev->Write(p, nsamples);           // 阻塞写，DMA 满即节流（电台天然实时）
        }
    }

    if (dec) esp_audio_simple_dec_close(dec);
    if (dev && info_set) {
        int16_t z[576] = {0};
        dev->Write(z, 576);                             // 排空 DMA 残尾
    }
    http.end();
    g_playing = false;
    return !want_exit;
}

// 电台流外壳：断线自动重连（退避 1.5s → 8s），直到收到停止命令
static void decode_stream(const char *url, const std::shared_ptr<ai_vox::AudioDeviceEs8311> &dev) {
    int fails = 0;
    while (g_run && g_cmd == PCMD_IDLE) {
        if (!stream_session(url, dev)) break;
        if (++fails > 100) { printf("[radio] too many retries, give up\n"); break; }
        uint32_t delay_ms = STREAM_RETRY_MIN_MS * (uint32_t)fails;
        if (delay_ms > STREAM_RETRY_MAX_MS) delay_ms = STREAM_RETRY_MAX_MS;
        for (uint32_t waited = 0; waited < delay_ms && g_run && g_cmd == PCMD_IDLE; waited += 50) {
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

// 解码任务：等命令 → 解码文件/网络流 → 播完/被打断后回到等待。栈与输入输出缓冲全静态。
static void decode_task(void *pvParam) {
    (void)pvParam;
    esp_mp3_dec_register();                       // 与 Board::playMp3 共用同一注册（幂等）
    auto dev = board.getAudioDeviceEs8311();
    while (g_run) {
        PlayerCmd cmd;
        char path[256];
        xSemaphoreTake(g_cmd_lock, portMAX_DELAY);
        cmd = g_cmd;
        g_cmd = PCMD_IDLE;
        strlcpy(path, g_cmd_path, sizeof(path));
        xSemaphoreGive(g_cmd_lock);

        if (cmd == PCMD_PLAY) {
            g_is_stream = false;
            decode_one_file(path, dev);
            g_eof = true;                         // 播完/失败都上抛，由 LVGL 线程切下一曲
            continue;
        }
        if (cmd == PCMD_PLAY_STREAM) {
            g_is_stream = true;
            decode_stream(path, dev);             // 内部自带重连，只在停止命令/彻底失败时返回
            g_is_stream = false;
            g_eof = true;                         // 交给上层（电台页不轮询，音乐页会清零）
            continue;
        }
        vTaskDelay(pdMS_TO_TICKS(10));            // ABORT 或 IDLE：停在等待区
    }
    g_task_handle = NULL;                         // 先清句柄，通知主线程本任务已退出
    vTaskDelete(NULL);
}

// ---------------- MusicAudio ----------------

bool MusicAudio::begin() {
    if (g_run) return true;
    // PSRAM 惰性分配：成功后常驻不释放，后续 begin() 直接复用，无碎片风险
    if (g_inbuf == nullptr) {
        g_inbuf = static_cast<uint8_t*>(heap_caps_malloc(DEC_INBUF_SIZE, MALLOC_CAP_SPIRAM));
    }
    if (g_outbuf == nullptr) {
        g_outbuf = static_cast<uint8_t*>(heap_caps_malloc(DEC_OUTBUF_SIZE, MALLOC_CAP_SPIRAM));
    }
    if (g_stack == nullptr) {
        // 16 字节对齐，满足 FreeRTOS 静态任务栈的对齐要求；栈在 PSRAM 需要的
        // CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY 本环境已开启
        g_stack = static_cast<StackType_t*>(heap_caps_aligned_alloc(16,
            DEC_TASK_STACK * sizeof(StackType_t), MALLOC_CAP_SPIRAM));
    }
    if (g_inbuf == nullptr || g_outbuf == nullptr || g_stack == nullptr) {
        printf("[music_audio] PSRAM alloc failed\n");
        return false;
    }
    if (g_cmd_lock == NULL) g_cmd_lock = xSemaphoreCreateMutex();
    if (g_cmd_lock == NULL) return false;
    g_cmd = PCMD_IDLE;
    g_paused = false;
    g_eof = false;
    g_playing = false;
    g_file_open = false;
    g_is_stream = false;
    g_level.store(0, std::memory_order_relaxed);
    g_run = true;
    g_task_handle = xTaskCreateStaticPinnedToCore(decode_task, "music_dec", DEC_TASK_STACK,
                                                 NULL, 2, g_stack, &g_task_buf, 1);
    if (g_task_handle == NULL) {
        g_run = false;
        return false;
    }
    return true;
}

void MusicAudio::end() {
    if (!g_run) return;
    g_run = false;
    post_cmd(PCMD_ABORT, NULL);
    const uint32_t t0 = millis();
    while (g_task_handle != NULL && millis() - t0 < 1000) vTaskDelay(pdMS_TO_TICKS(10));
    g_paused = false;
    g_playing = false;
    g_eof = false;
    g_level.store(0, std::memory_order_relaxed);
    // 恢复 codec 默认输出配置（16k 单声道）：通知音/AI_VOX 立即可用，无需引脚让渡
    if (auto dev = board.getAudioDeviceEs8311()) dev->CloseOutput();
}

bool MusicAudio::is_active() const {
    return g_run;
}

void MusicAudio::stop_current(uint32_t timeout_ms) {
    if (!g_run) return;
    post_cmd(PCMD_ABORT, NULL);
    const uint32_t t0 = millis();
    while (g_file_open && millis() - t0 < timeout_ms) vTaskDelay(pdMS_TO_TICKS(5));
}

void MusicAudio::play(const char *path) {
    if (!g_run || path == NULL || path[0] == '\0') return;
    g_paused = false;
    g_eof = false;
    post_cmd(PCMD_PLAY, path);
}

void MusicAudio::play_stream(const char *url) {
    if (!g_run || url == NULL || url[0] == '\0') return;
    g_paused = false;
    g_eof = false;
    post_cmd(PCMD_PLAY_STREAM, url);
}

bool MusicAudio::is_stream() const {
    return g_is_stream;
}

void MusicAudio::toggle_pause() {
    if (!g_run || !g_playing) return;
    g_paused = !g_paused;
}

void MusicAudio::set_paused(bool paused) {
    g_paused = paused;
}

bool MusicAudio::paused() const {
    return g_paused;
}

uint32_t MusicAudio::position_ms() const {
    return g_pos_ms;
}

uint32_t MusicAudio::duration_ms() const {
    return g_dur_ms;
}

bool MusicAudio::is_playing() const {
    return g_playing;
}

bool MusicAudio::take_eof() {
    if (!g_eof) return false;
    g_eof = false;
    return true;
}

void MusicAudio::set_volume(uint8_t percent) {
    g_volume = percent > 100 ? 100 : percent;
}

uint8_t MusicAudio::volume() const {
    return g_volume;
}

uint8_t MusicAudio::level() const {
    return g_level.load(std::memory_order_relaxed);
}

uint32_t MusicAudio::level_ms() const {
    return g_level_ms;
}

// ---------------- 给 setting.cpp 的音量路由钩子 ----------------

// 音乐页播放时：软件音量直接作用在解码 PCM 上，不碰 codec 硬件音量
bool music_apply_volume(void) {
    if (!g_run) return false;
    music_audio.set_volume((uint8_t)Setting::GetInstance().GetVolume());
    return true;
}

bool music_is_active(void) {
    return g_run;
}
