#include "online_img.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "esp_heap_caps.h"
#include "ESP32_board.h"
#include "SD_MMC.h"

// 图片源：使用 LVGL 内置 sjpg 解码器（LV_USE_SJPG=1）自动解码 JPEG
// 同时支持 baseline 和 progressive JPEG，无需第三方 JPEG 库
// picsum.photos 的 CDN 返回 progressive JPEG，LVGL sjpg 也能正确解码
#define IMG_URL_BASE "https://onlineimage-production.up.railway.app/proxy?w=240&h=320"



OnlineImg& OnlineImg::GetInstance() {
    static OnlineImg instance;
    return instance;
}

//==================== 缓冲分配/释放 ====================
bool OnlineImg::AllocateBuf(ImgBuf_t& b) {
    if (b.jpg_buf == nullptr) {
        b.jpg_buf = (uint8_t*)heap_caps_malloc(ONLINE_JPG_BUF_SIZE, MALLOC_CAP_SPIRAM);
        if (!b.jpg_buf) b.jpg_buf = (uint8_t*)malloc(ONLINE_JPG_BUF_SIZE);
    }
    b.jpg_len = 0;
    b.valid = false;
    // 用 LV_IMG_CF_RAW 包装原始 JPEG 字节流，LVGL sjpg 解码器会自动识别并解码
    memset(&b.dsc, 0, sizeof(b.dsc));
    b.dsc.header.always_zero = 0;
    b.dsc.header.reserved = 0;
    b.dsc.header.cf = LV_IMG_CF_RAW;        // 关键：RAW 格式让 LVGL 用 sjpg 解码器
    b.dsc.header.w = ONLINE_IMG_W;
    b.dsc.header.h = ONLINE_IMG_H;
    b.dsc.data = nullptr;                   // 下载完成后才指向 jpg_buf
    b.dsc.data_size = 0;
    return (b.jpg_buf != nullptr);
}

void OnlineImg::FreeBuf(ImgBuf_t& b) {
    if (b.jpg_buf) { free(b.jpg_buf); b.jpg_buf = nullptr; }
    b.jpg_len = 0;
    b.valid = false;
    b.dsc.data = nullptr;
    b.dsc.data_size = 0;
}

// 将下载的 JPEG 保存到 SD 卡 /photo 目录（供相册浏览）
void OnlineImg::saveToSD() {

 // 加锁，防止下载任务在读取过程中修改缓冲状态
    if (!xSemaphoreTake(save_mutex_, pdMS_TO_TICKS(500))) {
        Serial.println("[OnlineImg] saveToSD: mutex timeout");
        return;
    }
    ImgBuf_t& work = (active_idx_ == 0) ? bufA_ : bufB_;
    if (!work.valid || work.jpg_buf == nullptr || work.jpg_len == 0) {
        Serial.printf("[OnlineImg] saveToSD: buffer invalid (valid=%d, len=%u)\n",
                      work.valid, (unsigned)work.jpg_len);
        xSemaphoreGive(save_mutex_);
        return;
    }
  // 拷贝一份数据到栈/临时缓冲，尽快释放锁
    // JPEG 最大 ONLINE_JPG_BUF_SIZE，若太大可改为分块写
    size_t len = work.jpg_len;
    uint8_t* tmp = (uint8_t*)heap_caps_malloc(len, MALLOC_CAP_SPIRAM);
    if (!tmp) tmp = (uint8_t*)malloc(len);
    if (!tmp) {
        Serial.println("[OnlineImg] saveToSD: malloc failed");
        xSemaphoreGive(save_mutex_);
        return;
    }
    memcpy(tmp, work.jpg_buf, len);
    xSemaphoreGive(save_mutex_);  // ✅ 拷贝完立即释放锁

    // ---- SD 卡写入（在锁外执行，不阻塞下载任务）----
    if (SD_MMC.cardType() == CARD_NONE) {
        Serial.println("[OnlineImg] SD card not mounted");
        free(tmp);
        return;
    }
    if (!SD_MMC.exists("/photo")) SD_MMC.mkdir("/photo");

    char path[64];
    int idx = 0;
    do {
        snprintf(path, sizeof(path), "/photo/online_%ld_%d.jpg", (long)millis(), idx++);
    } while (SD_MMC.exists(path));

    File f = SD_MMC.open(path, FILE_WRITE);
    if (f) {
        f.write(tmp, len);
        f.close();
        Serial.printf("[OnlineImg] saved: %s (%u bytes)\n", path, (unsigned)len);
    } else {
        Serial.printf("[OnlineImg] saveToSD: open failed: %s\n", path);
    }
    free(tmp);
}

//==================== 下载任务 (FreeRTOS) ====================
// 仅下载 JPEG 原始字节流，解码交给 LVGL 主线程的 sjpg 解码器
void OnlineImg::DownloadTask(void* pvParam) {
    OnlineImg* self = static_cast<OnlineImg*>(pvParam);
    HTTPClient http;
    WiFiClientSecure wificlient;
    wificlient.setInsecure();

    while (self->active_) {
        // 选择非活跃缓冲作为本次下载目标
        ImgBuf_t& work = (self->active_idx_ == 0) ? self->bufB_ : self->bufA_;
        work.valid = false;
        work.dsc.data = nullptr;
        work.dsc.data_size = 0;
        // 拼接递增 seed，让每次刷新取不同图片
        String url = String(IMG_URL_BASE);
        if (!http.begin(wificlient, url)) {
            vTaskDelay(pdMS_TO_TICKS(ONLINE_IMG_REFRESH_MS));
            continue;
        }
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);  // picsum 会 302 重定向到 CDN
        http.setTimeout(10000);
        http.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
        http.addHeader("Accept-Encoding", "identity");  // 禁止 gzip 压缩

        int code = http.GET();
        if (code == HTTP_CODE_OK) {
            int64_t totalLen = http.getSize();
            if (totalLen > 0 && totalLen <= ONLINE_JPG_BUF_SIZE) {
                WiFiClient* stream = http.getStreamPtr();
                size_t readed = 0;
                while (http.connected() && readed < (size_t)totalLen) {
                    size_t avail = stream->available();
                    if (avail) {
                        size_t to_read = avail;
                        if (to_read > (size_t)totalLen - readed) to_read = totalLen - readed;
                        readed += stream->readBytes(work.jpg_buf + readed, to_read);
                    } else {
                        vTaskDelay(1); 
                    }
                }
                xSemaphoreTake(self->save_mutex_, portMAX_DELAY);
                work.jpg_len = readed;
                work.dsc.header.cf = LV_IMG_CF_RAW;
                work.dsc.header.always_zero = 0;
                work.dsc.header.reserved = 0;
                work.dsc.header.w = ONLINE_IMG_W;
                work.dsc.header.h = ONLINE_IMG_H;
                work.dsc.data = (const uint8_t*)work.jpg_buf;
                work.dsc.data_size = readed;
                Serial.printf("[OnlineImg] downloaded: %u bytes, dsc=%dx%d cf=%d az=%d\n",
                                  (unsigned)readed, ONLINE_IMG_W, ONLINE_IMG_H,
                                  (int)work.dsc.header.cf, (int)work.dsc.header.always_zero);
                work.valid = true;
                xSemaphoreGive(self->save_mutex_);
                // 通知 LVGL 主线程切换显示缓冲
                xSemaphoreGive(self->img_sem_);
            } else {
               Serial.printf("[OnlineImg] GET failed: %d\n", code);
            }
        } else {
            Serial.printf("[OnlineImg] GET failed: %d\n", code);
        }
        http.end();

        // 分片延时以便及时响应退出
        for (int i = 0; i < (ONLINE_IMG_REFRESH_MS / 100) && self->active_; i++) {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
    self->task_handle_ = nullptr;
    vTaskDelete(NULL);
}

//==================== LVGL 定时器回调 (主线程) ====================
void OnlineImg::LvTimerCallback(lv_timer_t* timer) {
    OnlineImg* self = &GetInstance();
    if (!self->active_) return;

    // 非阻塞检查信号量，无信号则跳过
    if (xSemaphoreTake(self->img_sem_, 0) != pdTRUE) {
        return;
    }

    // 取刚下载完成的非活跃缓冲作为新的显示缓冲
    ImgBuf_t& new_buf = (self->active_idx_ == 0) ? self->bufB_ : self->bufA_;
    if (!new_buf.valid || new_buf.jpg_buf == nullptr || new_buf.dsc.data == nullptr) {
        return;
    }

    xSemaphoreTake(self->save_mutex_, portMAX_DELAY);
    // 切换活跃索引：现在 new_buf 成为显示缓冲，旧的活跃缓冲可被下次下载覆盖
    self->active_idx_ = !self->active_idx_;
    xSemaphoreGive(self->save_mutex_);

    lv_obj_t *img = guider_ui.online_img_win_img_online;
    // 给 img 对象本身设置白色背景，防止 dsc 区域外露出默认黑色
    lv_obj_set_style_bg_color(img, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(img, 255, LV_PART_MAIN);
    // lv_img_set_src 在主线程调用，LVGL sjpg 解码器会在此处自动解码 JPEG
    lv_img_set_src(img, &new_buf.dsc);
    lv_obj_invalidate(img);
}



void OnlineImg::img_win_click(lv_event_t *e){
    OnlineImg* pThis = (OnlineImg*)lv_event_get_user_data(e);
    pThis->saveToSD();
    show_toast(lv_scr_act(),"保存成功");
}

//==================== 页面加载/卸载 ====================
void OnlineImg::img_screen_load(SCREEN_LOAD_STAUS status) {
    if (status == LOADED) {
        lv_obj_add_event_cb(guider_ui.online_img_win_img_online, OnlineImg::img_win_click, LV_EVENT_LONG_PRESSED,this);
        // 分配双缓冲
        AllocateBuf(bufA_);
        AllocateBuf(bufB_);
        // 创建信号量
        if (img_sem_ == nullptr) {
            img_sem_ = xSemaphoreCreateBinary();
        }
        //创建锁
        if (save_mutex_ == nullptr) {
            save_mutex_ = xSemaphoreCreateMutex();
        }

        active_ = true;
        active_idx_ = 0;

        // 创建 LVGL 定时器，100ms 检查一次新图片信号
        if (lv_timer_ == nullptr) {
            lv_timer_ = lv_timer_create(LvTimerCallback, 100, nullptr);
        }

        // 创建下载任务（不再需要解码，任务栈可以减小）
        if (task_handle_ == nullptr) {
            xTaskCreatePinnedToCore(
                DownloadTask,
                "online_img_task",
                1024 * 8,           // 不再做 JPEG 解码，8KB 栈足够
                this,
                2 | portPRIVILEGE_BIT,
                &task_handle_,
                1
            );
        }
    } else {
        // 页面卸载：停止任务、定时器并释放缓冲
        active_ = false;
        if (lv_timer_ != nullptr) {
            lv_timer_del(lv_timer_);
            lv_timer_ = nullptr;
        }
        FreeBuf(bufA_);
        FreeBuf(bufB_);
    }
}

void online_img_win_load(SCREEN_LOAD_STAUS status) {
    OnlineImg::GetInstance().img_screen_load(status);
}
