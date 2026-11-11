#include "music.h"
#include "SPI.h"
#include "FS.h"
#include "esp_err.h"
#include "esp_check.h"
#include "src/generated/gui_guider.h"
#include "src/generated/events_init.h"
#include "SD_MMC.h"
#include "ESP32_board.h"
#include "ai_vox_engine.h"
#include "setting.h"
#include "music_tag.h"
#include "music_audio.h"
#include <new>
#include <random>
#include <chrono>
#include "Ticker.h"
#include <Adafruit_NeoPixel.h>
#include <atomic>
#include "esp_heap_caps.h"

void check_list_item(uint16_t idx, bool check);
#define LIST_VISIBLE_ITEMS 30 
#define LIST_ITEM_HEIGHT 35    // 与 fill_music_list_panel() 里 lv_obj_set_height(btn, 35) 保持一致

// ---------------- 歌曲路径池（PSRAM） ----------------
// MusicPathList 的实现已上移到 ESP32_board.h（photo_album/lottie/game 页共用）。

// scanDirEach 的回调：直接拷进路径池，不做任何内部堆分配
static bool music_path_sink(void *ctx, const char *name) {
    return static_cast<MusicPathList *>(ctx)->push(name);
}

static bool list_is_touching = false;
static int32_t list_scroll_start_y = 0;   // 手势开始时的滚动位置，用于算"实际滚过了几条"
static bool list_swipe_registered = false;
static bool list_cb_registered = false;
static uint32_t      g_last_curr_sec = 0;     // 定时器最近一次读到的播放位置（歌词整页切换用）
static uint32_t      g_shown_sec = UINT32_MAX; // 上次已刷新到界面的秒数（去重，避免 50ms 级重排）
static char          g_cur_file_path[128] = {0}; // 当前播放文件路径（相对 SD_MMC 挂载点）
static const char *basePath = "S:/music";

// ---------------- ID3 标签（歌词 / 专辑封面） ----------------
// 标签解析（id3_parser）与封面 JPEG 解码全部收敛到 music_tag.h/.cpp 的 MusicTag 对象，
// 这里只把它接到 UI 上。生命周期：进音乐页 load()，退音乐页 unload()（内存随 delete 归还堆）。
static MusicTag musicTag;
static int32_t   g_lyric_index = -2; // 当前显示的歌词行下标（-2 表示还没写过文本）

// ---------------- 整页歌词（点击封面容器切换） ----------------
// 9 行铺满 240x205（容器在 setup_scr_screen_music.c:311，宽 240 高 205）：
// 中间一行 18px 黄色，向上下依次 16/14/12/10，水平/垂直均居中。
// 注意：工程里只有 16px 的中文字体(font_puhui_16_4)，没有 10/12/14/18 的中文字模，
// 所以这里统一用 16px 字体 + transform_scale 缩放得到其它字号（256 = 100%）。
// 若后续用 lv_font_conv 生成真字号的字体，只需替换 LYRIC_BASE_FONT 并按 px 直接设字号。
#define LYRIC_ROWS      9
#define LYRIC_ROW_W     240
#define LYRIC_ROW_H     205
#define LYRIC_BASE_FONT font_puhui_16_4
#define LYRIC_YELLOW    0xffb900

static lv_obj_t *g_lyric_cont = NULL;               // 9 行歌词容器（首次点击时创建）
static lv_obj_t *g_lyric_row[LYRIC_ROWS] = {0};
static bool      g_lyric_page_on   = false;         // false=封面模式，true=整页歌词
static int32_t   g_lyric_page_cur  = -2;            // 整页歌词当前居中行（-2 表示待刷新）

// 每行的字号、缩放比、行高、Y 坐标。
// 行高按"字号+6"排布，总高 176 + 8 段间距 3 = 200；容器高 205，上留 2 下留 3，垂直居中。
static const struct {
    uint8_t  px;      // 目标字号
    int16_t  scale;   // 相对 16px 的缩放（256 = 100%）
    int16_t  h;       // 行高
    int16_t  y;       // 相对容器顶部的 Y
} kLyricRowStyle[LYRIC_ROWS] = {
    {10, 160, 16,   2},
    {12, 192, 18,  21},
    {14, 224, 20,  42},
    {16, 256, 22,  65},
    {18, 288, 24,  90},   // 中间行：当前歌词（90..114，中心 102 ≈ 容器中心 102.5）
    {16, 256, 22, 117},
    {14, 224, 20, 142},
    {12, 192, 18, 165},
    {10, 160, 16, 186},   // 186+16=202，底部留 3
};

static void lyric_page_create(void) {
    if (g_lyric_cont != NULL) return;
    lv_obj_t *parent = guider_ui.screen_music_cont_cover;
    g_lyric_cont = lv_obj_create(parent);
    lv_obj_set_pos(g_lyric_cont, 0, 0);
    lv_obj_set_size(g_lyric_cont, LYRIC_ROW_W, LYRIC_ROW_H);
    lv_obj_set_scrollbar_mode(g_lyric_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(g_lyric_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(g_lyric_cont, LV_OBJ_FLAG_CLICKABLE);  // 不接收点击，事件落到下层封面容器
    lv_obj_set_style_bg_opa(g_lyric_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(g_lyric_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(g_lyric_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(g_lyric_cont, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(g_lyric_cont, LV_OBJ_FLAG_HIDDEN);

    for (int i = 0; i < LYRIC_ROWS; i++) {
        lv_obj_t *l = lv_label_create(g_lyric_cont);
        lv_label_set_text(l, "");
        lv_label_set_long_mode(l, LV_LABEL_LONG_CLIP);   // 歌词超宽时裁掉，不撑破容器
        lv_obj_set_pos(l, 0, kLyricRowStyle[i].y);
        lv_obj_set_size(l, LYRIC_ROW_W, kLyricRowStyle[i].h);
        lv_obj_set_style_text_font(l, &LYRIC_BASE_FONT, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(l, lv_color_hex(i == 4 ? LYRIC_YELLOW : 0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(l, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(l, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (kLyricRowStyle[i].scale != 256) {
            // 缩放轴必须用**像素**（宽/2、行高/2），不能用 lv_pct()：
            // pivot 是长度属性，百分比可能被当作原始数值解析（50px），导致缩放后
            // 内容整体偏移、重绘区域跑到容器外，是切页闪屏的可疑来源之一。
            lv_obj_set_style_transform_pivot_x(l, LYRIC_ROW_W / 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_transform_pivot_y(l, kLyricRowStyle[i].h / 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_transform_scale(l, kLyricRowStyle[i].scale, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        g_lyric_row[i] = l;
    }
}

// 以 cur 行为中心填充 9 行；越界的位置留空，形成"首尾渐入/渐出"的效果
static void lyric_page_fill(int32_t cur) {
    if (g_lyric_cont == NULL || !musicTag.has_lyrics()) return;
    const int32_t n = musicTag.lyric_count();
    for (int i = 0; i < LYRIC_ROWS; i++) {
        if (g_lyric_row[i] == NULL) continue;
        const int32_t idx = cur + (i - 4);
        const char *txt = (idx >= 0 && idx < n) ? musicTag.lyric_text(idx) : "";
        lv_label_set_text(g_lyric_row[i], txt);
    }
}

// 封面 / 整页歌词 之间切换（由 screen_music_cont_cover 的点击事件驱动，LVGL 线程内执行）
static void lyric_page_toggle(void) {
    if (guider_ui.screen_music_cont_cover == NULL) return;

    if (!g_lyric_page_on) {
        if (!musicTag.has_lyrics()) return;   // 没有歌词就不切，保持封面
        lyric_page_create();
        if (g_lyric_cont == NULL) return;
        if (guider_ui.screen_music_album_img != NULL) {
            lv_obj_add_flag(guider_ui.screen_music_album_img, LV_OBJ_FLAG_HIDDEN);
        }
        if (guider_ui.screen_music_label_music_name != NULL) {
            lv_obj_add_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
        }
        lv_obj_remove_flag(g_lyric_cont, LV_OBJ_FLAG_HIDDEN);
        g_lyric_page_on = true;
        // 立即按最近一次读到的播放位置填一次，避免等下一次定时器（最多 1s）才出字
        const int32_t cur = musicTag.line_at(g_last_curr_sec * 1000u);
        g_lyric_page_cur = cur;
        lyric_page_fill(cur);
    } else {
        if (g_lyric_cont != NULL) lv_obj_add_flag(g_lyric_cont, LV_OBJ_FLAG_HIDDEN);
        if (guider_ui.screen_music_album_img != NULL) {
            lv_obj_remove_flag(guider_ui.screen_music_album_img, LV_OBJ_FLAG_HIDDEN);
        }
        if (guider_ui.screen_music_label_music_name != NULL) {
            if (musicTag.has_lyrics()) {
                lv_obj_remove_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
            }
        }
        g_lyric_page_on  = false;
        g_lyric_page_cur = -2;
        g_lyric_index    = -2;   // 让单行歌词重新写一次
    }
}

static void lyric_cover_click_cb(lv_event_t *e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;
    lyric_page_toggle();
}

// 关闭主题在"按下/松开"上的过渡动画。
// 坑：不能写 lv_obj_set_style_transition(obj, NULL, ...) —— LVGL 会把这个属性存成
// "存在但值为 NULL"，点击时 DEFAULT→PRESSED 的 update_obj_state() 取到 tr = NULL
// 后直接读 tr->props[0]，触发 LoadProhibited（EXCVADDR=0）崩溃。
// 必须给一个 props 为空表的"真"描述符：props[0]==0，LVGL 的循环会立刻退出。
static lv_style_prop_t kNoTransProps[] = {(lv_style_prop_t)0};
static lv_style_transition_dsc_t kNoTransition;

// 注册封面点击事件（只注册一次：重复注册会让一次点击触发多次切换）
static void lyric_click_init(void) {
    static bool registered = false;
    if (registered || guider_ui.screen_music_cont_cover == NULL) return;
    lv_obj_add_flag(guider_ui.screen_music_cont_cover, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(guider_ui.screen_music_cont_cover, lyric_cover_click_cb, LV_EVENT_CLICKED, NULL);
    lv_style_transition_dsc_init(&kNoTransition, kNoTransProps, lv_anim_path_linear, 0, 0, NULL);
    lv_obj_set_style_transition(guider_ui.screen_music_cont_cover, &kNoTransition,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_transition(guider_ui.screen_music_cont_cover, &kNoTransition,
                                LV_PART_MAIN | LV_STATE_PRESSED);
    // 封面图自己带 CLICKABLE，点在图上的事件要靠冒泡才能到容器
    if (guider_ui.screen_music_album_img != NULL) {
        lv_obj_add_flag(guider_ui.screen_music_album_img, LV_OBJ_FLAG_EVENT_BUBBLE);
    }
    registered = true;
}

// 退出音乐页时回到封面模式，下次进来状态干净
static void lyric_page_reset(void) {
    if (g_lyric_cont != NULL) lv_obj_add_flag(g_lyric_cont, LV_OBJ_FLAG_HIDDEN);
    if (guider_ui.screen_music_album_img != NULL) lv_obj_remove_flag(guider_ui.screen_music_album_img, LV_OBJ_FLAG_HIDDEN);
    if (guider_ui.screen_music_label_music_name != NULL) {
        if (musicTag.has_lyrics()) {
            lv_obj_remove_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
        }
    }
    g_lyric_page_on  = false;
    g_lyric_page_cur = -2;
    g_lyric_index    = -2;
}

// 按播放进度切换歌词行（curr_sec 是秒）
static void lyric_update(uint32_t curr_sec) {
    if (!musicTag.has_lyrics()) return;
    const int idx = musicTag.line_at(curr_sec * 1000u);

    if (g_lyric_page_on) {                 // 整页歌词：内容整体上移一行
        if (idx == g_lyric_page_cur) return;
        g_lyric_page_cur = idx;
        lyric_page_fill(idx);
        return;
    }
    if (lv_obj_has_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN)) return;
    if (idx == g_lyric_index) return;
    g_lyric_index = idx;
    lv_label_set_text(guider_ui.screen_music_label_music_name, musicTag.lyric_text(idx));
}

// 解锁当前歌曲的标签：更新歌名、封面、歌词（在 LVGL 线程调用）
static void load_id3_info(const char *full_path) {
    g_lyric_index = -1;

    lv_label_set_text(guider_ui.screen_music_label_music_name, "");
    lv_obj_add_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
    musicTag.reset_cover(guider_ui.screen_music_album_img);   // 先恢复默认封面，避免解析失败时还留着上一首的图

    if (!musicTag.load(full_path)) {
        musicTag.dump();   // 解析失败/无标签时也打一行日志，便于排查
        return;
    }

    if (!musicTag.title().isEmpty() && guider_ui.screen_music_label_title != NULL) {
        lv_label_set_text(guider_ui.screen_music_label_title, musicTag.display_name().c_str());   // "歌手 - 歌名"进顶栏
    }
    musicTag.dump();

    musicTag.show_cover(guider_ui.screen_music_album_img, full_path);
    if (guider_ui.screen_music_label_music_name != NULL && musicTag.has_lyrics()) {
        lv_obj_clear_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
    }
    // 正在看整页歌词却换到了没有歌词的歌：自动退回封面，别停在一屏空白上
    if (g_lyric_page_on && !musicTag.has_lyrics()) lyric_page_toggle();
}

Music_player::Music_player():playEnable(false),playSequence(false),list_start_index(0),touch_start_Y(0){
      
};

// ---------------- WS2812B 响度灯（GPIO42） ----------------
// 随音乐响度变化的 RGB 灯。数据链路：
//   解码任务算 PCM RMS → 原子变量响度 → lv_timer(LVGL 线程，50ms) → NeoPixel 上色刷新
// NeoPixel(RMT) 只在 LVGL 线程写，避免解码线程与主线程并发操作 RMT。
#define RGB_LED_PIN     42
#define RGB_LED_COUNT   60       // 板上是 60 颗 WS2812B（RGBW，见厂商 Example_06_RGB_LED 示例）
#define RGB_LED_MIN_LVL 6        // 低于该响度视为静音，灯全灭（过滤底噪）
#define RGB_LED_HOLD_MS 300      // 超过这么久没有新 PCM 帧（暂停/停止）就把灯熄掉

// 注意必须是 NEO_GRBW：这颗板子是 RGBW 灯珠（每颗 4 字节）。之前按 NEO_GRB 发 3 字节，
// 数据长度与灯珠不匹配，整条灯都不亮。
static Adafruit_NeoPixel g_rgb(RGB_LED_COUNT, RGB_LED_PIN, NEO_GRBW + NEO_KHZ800);
static bool g_rgb_inited = false;
static uint8_t  g_rgb_shown_count = 0xFF;     // 当前点亮的灯珠数（0xFF = 尚未绘制）

static void rgb_led_init(void) {
    if (g_rgb_inited) return;
    g_rgb.begin();               // 占用 RMT 通道
    g_rgb.clear();
    g_rgb.show();
    g_rgb_inited = true;
}

// 响度 → 灯条：点亮的颗数随响度增长；
// 颜色跟着响度走（不按条上绝对位置）：level 低→绿、中→黄、高→红，
// 点亮的每颗同色，整条随节拍绿→黄→红地闪。之前按位置取色，小音量
// 只点亮底部几颗，那里永远是绿，看起来就"只有绿灯在闪"。
static void rgb_led_show(uint8_t level) {
    if (!g_rgb_inited) return;
    if (level < RGB_LED_MIN_LVL) level = 0;
    const uint8_t lit = (uint16_t)level * RGB_LED_COUNT / 255;
    if (lit == g_rgb_shown_count) return;   // 没有变化就不动 RMT
    g_rgb_shown_count = lit;

    if (lit == 0) {
        g_rgb.clear();
        g_rgb.show();
        return;
    }
    // 颜色：0~255 映射到 绿(0,255) → 黄(255,255) → 红(255,0)
    const uint16_t hue = (uint16_t)level * 2;       // 0~510，两段各 255
    uint8_t r, g;
    if (hue <= 255) {                               // 绿 → 黄
        r = (uint8_t)hue;
        g = 255;
    } else {                                        // 黄 → 红
        r = 255;
        g = (uint8_t)(510 - hue);
    }
    // 亮度跟随响度，留 40 的底亮，小声时也看得见
    const uint8_t bright = 40 + (uint16_t)level * 215 / 255;
    r = (uint8_t)((uint16_t)r * bright / 255);
    g = (uint8_t)((uint16_t)g * bright / 255);
    for (int i = 0; i < RGB_LED_COUNT; i++) {
        g_rgb.setPixelColor(i, (i < (int)lit) ? ((uint32_t)r << 16 | (uint32_t)g << 8) : 0);
    }
    g_rgb.show();
}

static void rgb_led_off(void) {
    if (!g_rgb_inited || g_rgb_shown_count == 0) return;
    g_rgb_shown_count = 0;
    g_rgb.clear();
    g_rgb.show();
}

// 播放引擎（解码任务 / 软件音量 / 进度 / 响度）已收敛到 music_audio.h/.cpp 的 MusicAudio 单例。
// 这里只负责把它的状态接到 UI 上：进度条、时间文字、歌词、WS2812B 灯。

int getRandomInt(int min, int max);

Music_player music_player;
static lv_anim_t album_anim;
uint32_t total_sec = 1;
static bool music_screen_load = false;
static lv_timer_t *music_timer = NULL;  // 静态，只在本文件可见

//音乐播放上一曲
void music_prev(){
    music_player.music_left_btn();
}
//音乐播放下一曲
void music_next(){
    music_player.music_right_btn();
}
//播放/暂停
void music_play(){
    music_player.playPauseResume();
}
void music_player_load(SCREEN_LOAD_STAUS playerLoadStatus){
    music_player.music_loop(playerLoadStatus);     
}
//播放音乐
void Music_player::play_music()
{
    if(music_files.size() == 0) return;
    total_sec = 1;
    if(current_music < 0) current_music = music_files.size() - 1;
    if(current_music >= music_files.size()) current_music = 0;
    const char *musicFile = music_files.at(current_music);  // 路径池里的指针，不产生任何分配
    lv_obj_clear_flag(guider_ui.screen_music_pause_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_music_playing_img, LV_OBJ_FLAG_HIDDEN);
    char fullPath[256];
    lv_snprintf(fullPath, sizeof(fullPath), "%s/%s","/music", musicFile);
    strncpy(g_cur_file_path, fullPath, sizeof(g_cur_file_path) - 1);
    g_cur_file_path[sizeof(g_cur_file_path) - 1] = '\0';
    // 先用文件名占位；解析到 ID3 后再换成"歌手 - 歌名"
    lv_label_set_text(guider_ui.screen_music_label_title,musicFile);
    // 必须先停歌再解析：SDMMC 不支持并发事务，解码任务还在读卡时解析 ID3 会读到坏数据（症状：封面在、歌词丢）。
    // stop_current 让解码任务立刻退出当前文件并关掉 SD 句柄，任务随后回到等待区（不退出任务本身）。
    music_audio.stop_current();
    load_id3_info(g_cur_file_path);   // 解析标签：歌名 / 专辑封面 / 同步歌词
    lv_slider_set_value(guider_ui.screen_music_slider, 0, LV_ANIM_OFF); //切歌时，滑块归0
    check_list_item(current_music,true);
    g_last_curr_sec = 0;
    g_shown_sec = UINT32_MAX;   // 换歌后强制刷新一次时间文本
    lv_slider_set_range(guider_ui.screen_music_slider, 0, 1);
    // 下发新播放命令（任务收到后重新开文件，首帧解码成功时自动按真实采样率/声道数重开 codec）
    music_audio.play(fullPath);
}
// 生成 [min, max] 范围内的随机整数（闭区间）
int getRandomInt(int min, int max) {
    // 使用静态变量确保引擎和分布只初始化一次，提高效率
    static std::mt19937 engine(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

// 播放结束后切歌逻辑（在 LVGL 主线程调用，会操作 UI）
static void handle_eof_next() {
    if (music_player.getPlaySequence()) {
        music_player.music_right_btn();
    } else {
        // 随机播放：直接播放随机到的曲目。
        // 原来这里又调了一次 music_right_btn()，会把 current_music 再 +1，实际播的不是随机的那首。
        music_player.setCurrentMusic(getRandomInt(0, music_player.getMusicFilesSize() - 1));
        music_player.play_music();
    }
}

//音乐随机播放
void music_random(){
    music_player.setPlaySequence(false);
    int rndNum = getRandomInt(0,music_player.getMusicFilesSize()-1);
    music_player.setCurrentMusic(rndNum);
    music_player.play_music();   // 直接播随机到的那首（原 music_right_btn() 会多 +1）
}
//音乐顺序播放
void music_sequence(){
    music_player.setPlaySequence(true);
}
//暂停/播放
void Music_player::playPauseResume(){
    // 任务暂停时不喂数据，I2S DMA auto_clear 播完缓冲后自然静音；恢复后从暂停点继续
    music_audio.toggle_pause();
}

//上一曲
void Music_player::music_left_btn(){
    current_music--;
    play_music();
}
//下一曲
void Music_player::music_right_btn(){
    current_music++;
    play_music();
}
// LVGL 定时器：刷新播放时间（安全，不会崩溃）
static void lv_timer_refresh_time(lv_timer_t *timer)
{
    // 播放引擎推进的时间/时长（总时长按比例精修，收敛后稳定）
    const uint32_t dur_sec = music_audio.duration_ms() / 1000;
    if (dur_sec > 0 && dur_sec != total_sec) {
        total_sec = dur_sec;
        lv_slider_set_range(guider_ui.screen_music_slider, 0, total_sec);
    }
    // 只有播放中才刷新
    if (music_audio.is_playing() && !music_audio.paused())
    {
        const uint32_t curr = music_audio.position_ms() / 1000;
        g_last_curr_sec = curr;   // 供整页歌词切换时取当前进度
        // 只在秒数变化时刷新文本/滑块/歌词：原来每 50ms 无条件 lv_label_set_text，
        // 每秒 20 次重排 LVGL 文本缓冲，播放期间内部堆被反复切碎（实测 largest 从 114KB 掉到 98KB）。
        if (curr != g_shown_sec) {
            g_shown_sec = curr;
            char buf[32];
            sprintf(buf, "%02d:%02d / %02d:%02d",
                curr / 60, curr % 60,
                total_sec / 60, total_sec % 60
            );
            lv_label_set_text(guider_ui.screen_music_label_slider_time, buf);
            lv_slider_set_value(guider_ui.screen_music_slider, curr, LV_ANIM_OFF);
            lyric_update(curr);
        }
    }
    // WS2812B 响度灯：有新 PCM 帧就跟响度；暂停/停止超时则熄灭
    if (g_rgb_inited) {
        if (music_audio.is_playing() && !music_audio.paused() &&
            millis() - music_audio.level_ms() < RGB_LED_HOLD_MS) {
            rgb_led_show(music_audio.level());
        } else {
            rgb_led_off();
        }
    }
    // 处理播放引擎上抛的跨线程事件（EOF 切歌）：只在文件真正解码完（或打不开）时置位
    if (music_audio.take_eof()) {
        handle_eof_next();
    }
}

void change_list_other_item_style(uint16_t idx) {
    lv_obj_t *list = guider_ui.screen_music_list_music;
    if (!list) return;
    for (int i = 0; i < LIST_VISIBLE_ITEMS; i++) {
        lv_obj_t *otherItem = lv_obj_get_child(list, i);
        // 歌曲数不足 LIST_VISIBLE_ITEMS 时 child 数会少于循环次数，这里必须判空
        if (!otherItem) break;
        int real_idx = (int)(uintptr_t)lv_obj_get_user_data(otherItem);
        if (real_idx == (int)idx) continue;
        lv_obj_set_style_bg_color(otherItem, lv_color_hex(0x343247), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void music_list_item_click(lv_event_t *e) {
    // 必须取对象上的 user_data：注册回调时传入的索引是"创建那一刻"的，
    // 列表回收复用后 item 与歌曲的对应关系会变，继续用注册时的值会点到别的歌。
    lv_obj_t *btn = lv_event_get_target_obj(e);   // LVGL 9 里 lv_event_get_target() 返回 void*
    if (!btn) return;
    int idx = (int)(uintptr_t)lv_obj_get_user_data(btn);
    music_player.setCurrentMusic(idx);
    music_player.play_music();
}

void check_list_item(uint16_t idx,bool check) {
    lv_obj_t *list = guider_ui.screen_music_list_music;
    if (!list) return;
    int listStartIndex = music_player.getListStartIndex();
    if(idx < listStartIndex || idx >= (listStartIndex+LIST_VISIBLE_ITEMS)){  //如果当前歌曲索引大于list的开始和结束索引，则重新刷新播放列表
        return;
    }
    lv_obj_t *selectItem = NULL;
    for (int i = 0; i < LIST_VISIBLE_ITEMS; i++) {
        lv_obj_t *item = lv_obj_get_child(list, i);
        if (!item) break;   // 歌曲数不足 LIST_VISIBLE_ITEMS 时 child 会少于循环次数
        int real_idx = (int)(uintptr_t)lv_obj_get_user_data(item);
        if (real_idx == (int)idx) {
            selectItem = item;
            break;
        }
    } 
    if (selectItem) {
        lv_obj_set_style_bg_color(selectItem, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if(check){
        change_list_other_item_style(idx);
    }
}



void music_list_swipe_event_handler(lv_event_t *e);


//加载音乐列表
void  Music_player::fill_music_list_panel(void) {
    lv_obj_t *list = guider_ui.screen_music_list_music;
    if (!list) return;
    if(!list_swipe_registered){
        lv_obj_clean(list);
    }
    const int total = (int)music_files.size();
    for (int i = list_start_index; i < (list_start_index+LIST_VISIBLE_ITEMS); i++) {
            if (i < 0 || i >= total) continue;   // 越界保护：at() 越界返回空串，这里直接跳过
            const char* songName = music_files.at(i);
            lv_obj_t *btn = NULL;
            if(list_swipe_registered){  //已加载过了
                btn = lv_obj_get_child(list, (i-list_start_index));
                if (btn == NULL) continue;
                lv_obj_t *label = lv_obj_get_child(btn,1);
                if (label) lv_label_set_text(label,songName);
                if(music_player.getCurrentMusic() == i){   //如果是播放中的歌曲，则改变背景色和按钮状态
                    lv_obj_set_style_bg_color(btn, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
                }else{
                    lv_obj_set_style_bg_color(btn, lv_color_hex(0x343247), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                // 复用分支不再重复注册回调：每次滑动刷新都 add 一条，会导致 handler 被调用 N 次且节点常驻内存
            }else{
                btn = lv_list_add_btn(list, &Music, songName);
                //lv_obj_t *icon = lv_obj_get_child(btn, 0);
                //if (icon) lv_image_set_scale(icon, LV_SCALE_NONE / 2);
                lv_obj_set_style_bg_color(btn, lv_color_hex(0x343247), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_t *label = lv_obj_get_child(btn, 1);
                if (label) {
                    lv_obj_set_style_text_font(label, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                lv_obj_set_height(btn, 35);
                // 事件回调只在新建按钮时注册一次
                lv_obj_add_event_cb(btn, music_list_item_click, LV_EVENT_LONG_PRESSED, (void*)(uintptr_t)i);
            }
            lv_obj_set_user_data(btn, (void*)(uintptr_t)i);
    }
}

// 列表 item 回收复用：不销毁/新建对象，只把滑出视野的 item 搬到另一端并改文本。
//   n > 0（向上滑，往下翻）：顶部滑出的 n 个 item 依次搬到末尾，下面的 item 整体上移，
//                            再从 music_files 里取后面的元素补进末尾，补满 count 个。
//   n < 0（向下滑，往上翻）：末尾的 |n| 个 item 依次搬到顶部，上面的 item 整体下移，
//                            再从 music_files 里取前面的元素补进顶部。
// 返回的 real_n 会被夹到合法范围（到头/到尾时实际推进量可能小于 n）。
static int recycle_music_list(int n)
{
    lv_obj_t *list = guider_ui.screen_music_list_music;
    if (!list || n == 0) return 0;

    const int count = (int)lv_obj_get_child_cnt(list);
    if (count == 0) return 0;

    const int total = music_player.getMusicFilesSize();
    const int start = music_player.getListStartIndex();

    // 目标窗口起点，夹到合法范围（歌曲数不足一屏时始终为 0）
    int new_start = start + n;
    if (new_start + count > total) new_start = total - count;
    if (new_start < 0) new_start = 0;

    const int real_n = new_start - start;
    if (real_n == 0) return 0;   // 已经到头/到尾，无需搬移

    if (real_n > 0) {
        for (int i = 0; i < real_n; i++) {
            lv_obj_t *item = lv_obj_get_child(list, 0);
            if (!item) break;
            lv_obj_move_to_index(item, count - 1);
        }
    } else {
        for (int i = 0; i < -real_n; i++) {
            lv_obj_t *item = lv_obj_get_child(list, count - 1);
            if (!item) break;
            lv_obj_move_to_index(item, 0);
        }
    }

    music_player.setListStartIndex(new_start);
    // 按新窗口刷新文本/索引/播放高亮：被搬到另一端的 item 会从 music_files 取到新元素
    music_player.fill_music_list_panel();
    return real_n;
}

void music_list_swipe_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SCROLL_BEGIN) {
        lv_indev_t *indev = lv_indev_get_act();
        if (indev) {
            lv_point_t point;
            lv_indev_get_point(indev, &point);
            if(!list_is_touching){
                music_player.setTouchStartY(point.y);
                list_is_touching = true;
                lv_obj_t *list = guider_ui.screen_music_list_music;
                list_scroll_start_y = list ? lv_obj_get_scroll_y(list) : 0;
            }
        }
    } else if (code == LV_EVENT_SCROLL_END) {
        if (!list_is_touching) return;   // 程序滚动触发的事件不再处理，避免递归
        list_is_touching = false;
        lv_indev_t *indev = static_cast<lv_indev_t *>(lv_event_get_param(e));
        if (!indev) return;

        lv_point_t point;
        lv_indev_get_point(indev, &point);
        const int16_t delta_y = music_player.getTouchStartY() - point.y;

        lv_obj_t *list = guider_ui.screen_music_list_music;
        if (!list) return;

        if (abs(delta_y) >= LIST_ITEM_HEIGHT) {
            // 用"滚动位置的位移"而不是绝对 scroll_y 来算 n：
            // 列表静止时可能停在中间，绝对 scroll_y 是十几条，直接拿它当翻页量会跳。
            // >0 表示内容上移（看后面的歌），<0 表示内容下移（看前面的歌）。
            int32_t moved = lv_obj_get_scroll_y(list) - list_scroll_start_y;
            if (moved == 0) moved = delta_y;   // 顶/底被夹住时滚不动，用手指位移兜底
            const int n = (int)(moved / LIST_ITEM_HEIGHT);
            if (n != 0) recycle_music_list(n);
        }

        // 回收复用后按当前窗口复位滚动位置：
        // 含第 1 首 → 置顶；含最后 1 首 → 置底；否则居中，让视野落在窗口正中
        // （两端都还有内容，下一次往哪个方向滑都能立刻看到新元素）
        lv_obj_update_layout(list);   // 先刷新布局，scroll 边界才算得准
        const int32_t scroll_max = lv_obj_get_scroll_y(list) + lv_obj_get_scroll_bottom(list);
        const int total_songs = music_player.getMusicFilesSize();
        const int count = (int)lv_obj_get_child_cnt(list);
        const int start = music_player.getListStartIndex();

        if (start <= 0) {                                   // 窗口含第 1 首
            lv_obj_scroll_to_y(list, 0, LV_ANIM_OFF);
        } else if (count > 0 && (start + count) >= total_songs) {  // 窗口含最后 1 首
            lv_obj_scroll_to_y(list, scroll_max, LV_ANIM_OFF);
        } else {
            lv_obj_scroll_to_y(list, scroll_max / 2, LV_ANIM_OFF);
        }
    }
}

//循环播放
void Music_player::music_loop(SCREEN_LOAD_STAUS playerLoadStatus){
    if(playerLoadStatus == LOADED){
        lv_obj_add_flag(page_head.time_label,LV_OBJ_FLAG_HIDDEN);
        if (music_audio.is_active()) return;   // 重入保护：重复 LOADED 会重复创建解码任务
        musicTag.unload();              // 进页面先清掉上一轮残留的标签（下次 load 时才分配）
        list_start_index = 0;           // 列表回到首页，避免与上次退出时的滑动位置错位
        list_is_touching = false;
        // 路径全部写进 PSRAM 池，内部堆里不再有几十个 String 小块
        board.scanDirEach(basePath, music_path_sink, &music_files, MP3_TYPE);
        printf("[MEM][music] scanDir n=%u\n", (unsigned)music_files.size());
        // 事件回调只注册一次：重复注册会让 handler 被重复调用并永久多占一份内存
        if (!list_cb_registered) {
            lv_obj_add_event_cb(guider_ui.screen_music_list_music, music_list_swipe_event_handler, LV_EVENT_ALL, NULL);
            list_cb_registered = true;
        }
        fill_music_list_panel();        // 内部依据 list_swipe_registered 决定重建还是复用
        list_swipe_registered = true;
        lv_obj_set_style_text_font(guider_ui.screen_music_label_music_name, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT); 
        lv_obj_set_style_text_font(guider_ui.screen_music_label_title, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT); 
        musicTag.reset_cover(guider_ui.screen_music_album_img);   // 默认封面资源先缩回原尺寸居中显示
        lyric_click_init();             // 封面容器点击 → 封面/整页歌词切换
        lyric_page_reset();
        if (music_timer == NULL) {
            music_timer = lv_timer_create(lv_timer_refresh_time, 50, NULL);
        }
            playEnable = true;
            // 先把硬件音量拉满，响度只由软件音量（解码 PCM 缩放）决定
            Setting::GetInstance().PreparePlaybackVolume();
            // 启动播放引擎：静态栈解码任务把 PCM 直接 Write 给 AudioDeviceEs8311
            music_audio.set_volume((uint8_t)Setting::GetInstance().GetVolume());
            music_audio.begin();
            rgb_led_init();                 // WS2812B 响度灯（GPIO42）
            vTaskDelay(pdMS_TO_TICKS(50));
            play_music();
    }else{
        lv_obj_clear_flag(page_head.time_label,LV_OBJ_FLAG_HIDDEN);
        playEnable = false;
        // 1) 先停掉 LVGL 定时器（内部会访问 audio->），避免后续销毁过程中再被触发
        if (music_timer != NULL) {
            lv_timer_del(music_timer);
            music_timer = NULL;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        lyric_page_reset();             // 整页歌词可能正显示着，先退回封面模式
        musicTag.release_cover(guider_ui.screen_music_album_img);
        musicTag.unload();
        music_audio.end();
        rgb_led_off();
        // 播放页里调过的音量只落在解码 PCM 上，这里补写回 ES8311 硬件音量
        Setting::GetInstance().FlushVolumeToHardware();
        // 只 clear 不收容量：反复 clear/shrink_to_fit 会让内部堆每次进出页面都做一次
        // "分配→释放"，留下无法合并的小洞；128 槽容量仅占 512B，留着更省事也更少碎片
        music_files.clear();
        lv_obj_clean(guider_ui.screen_music_list_music);
        list_swipe_registered = false;
    }
}
