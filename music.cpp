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
#include "id3_parser.h"
#include "esp_heap_caps.h"
#include <JPEGDEC.h>
#include <new>
#include <random>
#include <chrono>
#include "Ticker.h"
#define I2S_MCK   4
#define I2S_BCK   5
#define I2S_DOUT  8
#define I2S_WS    7

void check_list_item(uint16_t idx, bool check);
#define LIST_VISIBLE_ITEMS 30 
#define LIST_ITEM_HEIGHT 35    // 与 fill_music_list_panel() 里 lv_obj_set_height(btn, 35) 保持一致
static bool list_is_touching = false;
static int32_t list_scroll_start_y = 0;   // 手势开始时的滚动位置，用于算"实际滚过了几条"
static bool list_swipe_registered = false;
static bool list_cb_registered = false;
static volatile bool g_eof_pending = false;   // 播放结束，待切下一曲
static uint32_t      g_play_start_ms = 0;     // 本次 connecttoFS 的时刻，用于识别"刚切歌就来的旧 EOF"
static uint32_t      g_last_curr_sec = 0;     // 定时器最近一次读到的播放位置（库在抛 EOF 前会把它清零）
static char          g_cur_file_path[128] = {0}; // 当前播放文件路径（相对 SD_MMC 挂载点）
static const char *basePath = "S:/music";

// ---------------- ID3 标签（歌词 / 专辑封面） ----------------
// 库的 evt_lyrics 解析 UTF-8 编码的 SYLT 有 off-by-one（时间戳全错），evt_image 只给偏移不读数据，
// 所以这里用 id3_parser 自己解析：歌词按播放进度显示，封面取 APIC 数据自己缩小解码后显示。
#define COVER_BOX_SIZE 180        // 与 setup_scr_screen_music.c 里 screen_music_album_img 的尺寸保持一致
// 当前歌曲的标签。注意生命周期：进音乐页 new，退音乐页 delete。
// 不能只靠 clear()：vector.clear() 不还元素数组容量，String 赋 "" 不还缓冲，
// 退出后仍会常驻 2~6KB 且正好散在主区的空闲洞里，阻碍空闲块合并。
static id3::Tag *g_id3_ptr = NULL;
#define g_id3 (*g_id3_ptr)          // 既有 17 处 g_id3.xxx 引用无需改动
static int32_t   g_lyric_index = -2; // 当前显示的歌词行下标（-2 表示还没写过文本）

// 封面：LVGL 的内存池只有 128KB，直接让它解码 500x500 的 jpg 要整张解码图 + 缩放变换，必然失败。
// 这里用 JPEGDEC 按 1/2、1/4、1/8 缩小解码，再最近邻采样成 180x180 的 RGB565 交给 LVGL。
static uint16_t *g_cover_decoded = NULL;   // 缩小解码后的整图（RGB565）
static int16_t   g_cover_dec_w = 0;
static int16_t   g_cover_dec_h = 0;
static uint16_t *g_cover_pixels = NULL;    // 180x180 RGB565，直接作为 LVGL 图片源
static lv_image_dsc_t g_cover_dsc;
static JPEGDEC *g_jpeg_ctx = NULL;         // JPEGDEC 解码器（约 16KB，按需分配，优先放 PSRAM）

static void cover_free_decoded(void) {
    if (g_cover_decoded != NULL) {
        free(g_cover_decoded);
        g_cover_decoded = NULL;
    }
    g_cover_dec_w = 0;
    g_cover_dec_h = 0;
}

// JPEGDEC 按 MCU 块回调，把每块拷进缩小后的整图缓冲
static int cover_draw_cb(JPEGDRAW *draw) {
    if (g_cover_decoded == NULL || draw == NULL || draw->pPixels == NULL) return 0;
    for (int row = 0; row < draw->iHeight; row++) {
        const int dy = draw->y + row;
        if (dy < 0 || dy >= g_cover_dec_h) continue;

        const uint16_t *src = draw->pPixels + (int32_t)row * draw->iWidth;
        int dx = draw->x;
        int off = 0;
        int cw = draw->iWidth;
        if (dx < 0) {
            off = -dx;
            cw += dx;
            dx = 0;
        }
        if (dx + cw > g_cover_dec_w) cw = g_cover_dec_w - dx;
        if (cw <= 0) continue;

        memcpy(&g_cover_decoded[(int32_t)dy * g_cover_dec_w + dx], src + off, (size_t)cw * 2);
    }
    return 1;
}

static void cover_reset(void) {
    lv_obj_t *img = guider_ui.screen_music_album_img;
    if (img != NULL) {
        lv_image_set_src(img, &_cover_2_RGB565A8_180x180);
        lv_image_set_pivot(img, 50, 50);
        lv_image_set_scale(img, LV_SCALE_NONE);
    }
    cover_free_decoded();
}

// 退出播放页：恢复默认封面并释放所有封面缓冲
static void cover_release(void) {
    cover_reset();
    if (g_cover_pixels != NULL) {
        free(g_cover_pixels);
        g_cover_pixels = NULL;
    }
    if (g_jpeg_ctx != NULL) {
        g_jpeg_ctx->~JPEGDEC();   // PSRAM 上 placement new 出来的对象
        free(g_jpeg_ctx);
        g_jpeg_ctx = NULL;
    }
}

// 把当前歌曲的 APIC 图片解码成 180x180 显示
static void cover_show(void) {
    lv_obj_t *img = guider_ui.screen_music_album_img;
    if (img == NULL || g_id3_ptr == NULL || !g_id3.has_cover() || g_cur_file_path[0] == '\0') return;

    uint8_t *jpg = (uint8_t *)heap_caps_malloc(g_id3.cover.length, MALLOC_CAP_SPIRAM);
    if (jpg == NULL) jpg = (uint8_t *)malloc(g_id3.cover.length);
    if (jpg == NULL) {
        printf("[cover] alloc %lu fail\n", (unsigned long)g_id3.cover.length);
        return;
    }

    File f = SD_MMC.open(g_cur_file_path);
    const size_t jpg_len = f ? id3::read_cover(f, g_id3, jpg, g_id3.cover.length) : 0;
    if (f) f.close();
    if (jpg_len != g_id3.cover.length) {
        printf("[cover] read %u != %u（文件可能被截断/损坏）\n",
               (unsigned)jpg_len, (unsigned)g_id3.cover.length);
        free(jpg);
        return;
    }

    // JPEGDEC 上下文约 16KB，优先放 PSRAM，别占内部 DRAM
    if (g_jpeg_ctx == NULL) {
        void *mem = heap_caps_malloc(sizeof(JPEGDEC), MALLOC_CAP_SPIRAM);
        if (mem == NULL) mem = malloc(sizeof(JPEGDEC));
        if (mem == NULL) {
            printf("[cover] jpeg ctx alloc fail\n");
            free(jpg);
            return;
        }
        g_jpeg_ctx = new (mem) JPEGDEC();
    }
    if (!g_jpeg_ctx->openRAM(jpg, (int)jpg_len, cover_draw_cb)) {
        printf("[cover] openRAM fail (%u bytes)\n", (unsigned)jpg_len);
        free(jpg);
        return;
    }

    const int w = g_jpeg_ctx->getWidth();
    const int h = g_jpeg_ctx->getHeight();
    if (w <= 0 || h <= 0) {
        g_jpeg_ctx->close();
        free(jpg);
        return;
    }

    // 选一个"缩完仍不小于 180x180"的最大缩小比例
    int scale = 1;
    while (scale < 8 && (w / (2 * scale)) >= COVER_BOX_SIZE && (h / (2 * scale)) >= COVER_BOX_SIZE) {
        scale *= 2;
    }
    g_jpeg_ctx->setPixelType(RGB565_LITTLE_ENDIAN);   // 和 LVGL 的 RGB565 字节序一致
    int options = 0;
    if (scale == 8) options |= JPEG_SCALE_EIGHTH;
    else if (scale == 4) options |= JPEG_SCALE_QUARTER;
    else if (scale == 2) options |= JPEG_SCALE_HALF;

    g_cover_dec_w = (int16_t)((w + scale - 1) / scale);
    g_cover_dec_h = (int16_t)((h + scale - 1) / scale);
    const size_t dec_bytes = (size_t)g_cover_dec_w * g_cover_dec_h * 2;
    g_cover_decoded = (uint16_t *)heap_caps_malloc(dec_bytes, MALLOC_CAP_SPIRAM);
    if (g_cover_decoded == NULL) g_cover_decoded = (uint16_t *)malloc(dec_bytes);
    if (g_cover_decoded == NULL) {
        printf("[cover] decode buf alloc %u fail\n", (unsigned)dec_bytes);
        g_cover_dec_w = 0;
        g_cover_dec_h = 0;
        g_jpeg_ctx->close();
        free(jpg);
        return;
    }
    memset(g_cover_decoded, 0, dec_bytes);   // 边缘 MCU 可能不满，先清零避免采样到脏数据

    const int decoded = g_jpeg_ctx->decode(0, 0, options);
    g_jpeg_ctx->close();
    free(jpg);
    if (!decoded) {
        printf("[cover] jpeg decode fail\n");
        cover_free_decoded();
        return;
    }

    if (g_cover_pixels == NULL) {
        g_cover_pixels = (uint16_t *)heap_caps_malloc(COVER_BOX_SIZE * COVER_BOX_SIZE * 2, MALLOC_CAP_SPIRAM);
        if (g_cover_pixels == NULL) g_cover_pixels = (uint16_t *)malloc(COVER_BOX_SIZE * COVER_BOX_SIZE * 2);
    }
    if (g_cover_pixels == NULL) {
        cover_free_decoded();
        return;
    }

    // 最近邻采样到 180x180（缩略图本身已经很小，画质够用）
    for (int y = 0; y < COVER_BOX_SIZE; y++) {
        const int sy = (int)((int32_t)y * g_cover_dec_h / COVER_BOX_SIZE);
        for (int x = 0; x < COVER_BOX_SIZE; x++) {
            const int sx = (int)((int32_t)x * g_cover_dec_w / COVER_BOX_SIZE);
            g_cover_pixels[y * COVER_BOX_SIZE + x] = g_cover_decoded[(int32_t)sy * g_cover_dec_w + sx];
        }
    }
    cover_free_decoded();

    lv_memzero(&g_cover_dsc, sizeof(g_cover_dsc));
    g_cover_dsc.header.magic = LV_IMAGE_HEADER_MAGIC;
    g_cover_dsc.header.cf = LV_COLOR_FORMAT_RGB565;
    g_cover_dsc.header.w = COVER_BOX_SIZE;
    g_cover_dsc.header.h = COVER_BOX_SIZE;
    g_cover_dsc.header.stride = COVER_BOX_SIZE * 2;
    g_cover_dsc.data_size = COVER_BOX_SIZE * COVER_BOX_SIZE * 2;
    g_cover_dsc.data = (const uint8_t *)g_cover_pixels;

    lv_obj_set_size(img, COVER_BOX_SIZE, COVER_BOX_SIZE);
    lv_image_set_scale(img, LV_SCALE_NONE);   // 源就是 180x180，不需要缩放
    lv_image_set_src(img, &g_cover_dsc);
}


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
    if (parent == NULL) return;

    g_lyric_cont = lv_obj_create(parent);
    if (g_lyric_cont == NULL) {          // LVGL 内存池耗尽时会返回 NULL，不判空会直接崩
        printf("[lyric] create container fail\n");
        return;
    }
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
        if (l == NULL) {
            printf("[lyric] create row %d fail\n", i);
            break;
        }
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
    if (g_lyric_cont == NULL || g_id3_ptr == NULL) return;
    const int32_t n = (int32_t)g_id3.lyrics.size();
    for (int i = 0; i < LYRIC_ROWS; i++) {
        if (g_lyric_row[i] == NULL) continue;
        const int32_t idx = cur + (i - 4);
        const char *txt = (idx >= 0 && idx < n) ? g_id3.lyrics[idx].text.c_str() : "";
        lv_label_set_text(g_lyric_row[i], txt);
    }
}

// 封面 / 整页歌词 之间切换（由 screen_music_cont_cover 的点击事件驱动，LVGL 线程内执行）
static void lyric_page_toggle(void) {
    if (guider_ui.screen_music_cont_cover == NULL) return;

    if (!g_lyric_page_on) {
        if (g_id3_ptr == NULL || !g_id3.has_lyrics()) return;   // 没有歌词就不切，保持封面
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
        const int32_t cur = g_id3.line_at(g_last_curr_sec * 1000u);
        g_lyric_page_cur = cur;
        lyric_page_fill(cur);
    } else {
        if (g_lyric_cont != NULL) lv_obj_add_flag(g_lyric_cont, LV_OBJ_FLAG_HIDDEN);
        if (guider_ui.screen_music_album_img != NULL) {
            lv_obj_remove_flag(guider_ui.screen_music_album_img, LV_OBJ_FLAG_HIDDEN);
        }
        if (guider_ui.screen_music_label_music_name != NULL) {
            if (g_id3_ptr != NULL && g_id3.has_lyrics()) {
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
        if (g_id3_ptr != NULL && g_id3.has_lyrics()) {
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
    if (g_id3_ptr == NULL || g_id3.lyrics.empty()) return;
    const int idx = g_id3.line_at(curr_sec * 1000u);

    if (g_lyric_page_on) {                 // 整页歌词：内容整体上移一行
        if (idx == g_lyric_page_cur) return;
        g_lyric_page_cur = idx;
        lyric_page_fill(idx);
        return;
    }
    if (lv_obj_has_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN)) return;
    if (idx == g_lyric_index) return;
    g_lyric_index = idx;
    lv_label_set_text(guider_ui.screen_music_label_music_name, idx >= 0 ? g_id3.lyrics[idx].text.c_str() : "");
}

// 解锁当前歌曲的标签：更新歌名、封面、歌词（在 LVGL 线程调用）
static void load_id3_info(const char *full_path) {
    if (g_id3_ptr == NULL) return;   // 不在音乐页时标签对象不存在
    g_id3.clear();
    g_lyric_index = -1;

    lv_label_set_text(guider_ui.screen_music_label_music_name, "");
    lv_obj_add_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
    cover_reset();   // 先恢复默认封面，避免解析失败时还留着上一首的图

    if (!id3::parse_file(SD_MMC, full_path, g_id3) || !g_id3.valid) return;

    if (!g_id3.title.isEmpty()) {
        String name = g_id3.artist.isEmpty() ? g_id3.title : (g_id3.artist + " - " + g_id3.title);
        if (guider_ui.screen_music_label_title != NULL) lv_label_set_text(guider_ui.screen_music_label_title, name.c_str());   // 歌名进顶栏
    }
    printf("[id3] title=%s artist=%s cover=%lu@%lu lyrics=%u plain=%u\n",
           g_id3.title.c_str(), g_id3.artist.c_str(),
           (unsigned long)g_id3.cover.length, (unsigned long)g_id3.cover.offset,
           (unsigned)g_id3.lyrics.size(), (unsigned)g_id3.plain_lyrics.length());
    if (g_id3.has_lyrics()) {
        printf("[id3] lyric first=%ums last=%ums \"%s\"\n",
               (unsigned)g_id3.lyrics.front().time_ms,
               (unsigned)g_id3.lyrics.back().time_ms,
               g_id3.lyrics.front().text.c_str());
    } else {
        printf("[id3] 没解析到同步歌词（若 plain>0 说明只有无时间轴歌词）\n");
    }

    cover_show();
    if (guider_ui.screen_music_label_music_name != NULL && g_id3.has_lyrics()) lv_obj_clear_flag(guider_ui.screen_music_label_music_name, LV_OBJ_FLAG_HIDDEN);
    // 正在看整页歌词却换到了没有歌词的歌：自动退回封面，别停在一屏空白上
    if (g_lyric_page_on && !g_id3.has_lyrics()) lyric_page_toggle();
}

Music_player::Music_player():playEnable(false),playSequence(false),list_start_index(0),touch_start_Y(0){
      
};

void on_audio_info(Audio::msg_t msg) {
    switch (msg.e) {
//S        case Audio::evt_spectrum:
//S            // vec1 = display_vec（15 个对数分贝频段，已内置 attack/release/hold 平滑），各 0~255
//S            // 线性插值映射到 25 根柱
//S            if (msg.vec1.size() >= SPECTRUM_SRC_BANDS) {
//S                for (int i = 0; i < SPECTRUM_BARS; i++) {
//S                    float pos  = (float)i * (SPECTRUM_SRC_BANDS - 1) / (SPECTRUM_BARS - 1);
//S                    int   idx0 = (int)pos;
//S                    int   idx1 = (idx0 + 1 < SPECTRUM_SRC_BANDS) ? idx0 + 1 : idx0;
//S                    float frac = pos - idx0;
//S                    float val  = msg.vec1[idx0] * (1.0f - frac) + msg.vec1[idx1] * frac;
//S                    spectrum_target_heights[i] = (uint8_t)(val * SPECTRUM_MAX_HEIGHT / 255)+1;
//S                }
//S            }
//S            break;
        case Audio::evt_eof:
            g_eof_pending = true;   // 交给主线程切下一曲
            break;
        case Audio::evt_image:
        case Audio::evt_lyrics:
            // 封面 / 歌词统一由 id3_parser 在切歌时解析：
            // evt_image 只给 [pos, len] 不给数据，evt_lyrics 解析 UTF-8 的 SYLT 有 off-by-one，
            // 这里不再使用，避免重复读文件。
            break;
        case Audio::evt_id3data:
            // 非同步歌词 USLT 也通过 ID3 数据事件上抛，避免依赖固定的 "Lyrics:" 前缀。
            if (msg.msg) {
                printf("id3: %s\n", msg.msg);
            }
            break;
        default:
            break;
    }
}

int getRandomInt(int min, int max);

Music_player music_player;
static lv_anim_t album_anim;
uint32_t total_sec = 1;
static bool music_screen_load = false;
static lv_timer_t *music_timer = NULL;  // 静态，只在本文件可见
#define MUSIC_TASK_STACK_WORDS  (1024 * 6)
static TaskHandle_t music_back_task_handle = NULL;

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
    const String& musicFile = music_files.at(current_music);  // 不再拷贝 String，避免每次切歌多分配一块 DRAM
    lv_obj_clear_flag(guider_ui.screen_music_pause_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.screen_music_playing_img, LV_OBJ_FLAG_HIDDEN);
    char fullPath[256];
    lv_snprintf(fullPath, sizeof(fullPath), "%s/%s","/music", musicFile.c_str());
    strncpy(g_cur_file_path, fullPath, sizeof(g_cur_file_path) - 1);
    g_cur_file_path[sizeof(g_cur_file_path) - 1] = '\0';
    // 先用文件名占位；解析到 ID3 后再换成"歌手 - 歌名"
    lv_label_set_text(guider_ui.screen_music_label_title,musicFile.c_str());
    audio->stopSong();   // 必须先停歌再解析：SDMMC 不支持并发事务，音频任务还在读卡时解析 ID3 会读到坏数据（症状：封面在、歌词丢）
    load_id3_info(g_cur_file_path);   // 解析标签：歌名 / 专辑封面 / 同步歌词
    lv_slider_set_value(guider_ui.screen_music_slider, 0, LV_ANIM_OFF); //切歌时，滑块归0
    check_list_item(current_music,true);
    g_eof_pending = false;              // 清掉可能残留的旧 EOF 事件，避免刚开播就被切走
    g_last_curr_sec = 0;
    g_play_start_ms = millis();
    audio->connecttoFS(SD_MMC, fullPath);
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
    audio->pauseResume();
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
void music_back_task(void *pvParam){
  Music_player *p = (Music_player*)pvParam;
  while (p->getPlayEnable())
  {
    audio->loop();
    vTaskDelay(1);
  }
  music_back_task_handle = NULL;  // 先清句柄，通知主线程本任务已退出
  vTaskDelete(NULL);
}
// LVGL 定时器：刷新播放时间（安全，不会崩溃）
static void lv_timer_refresh_time(lv_timer_t *timer)
{
    // 只有播放中才刷新
    if (audio && audio->isRunning())
    {
       if(total_sec == 1){
         total_sec = audio->getAudioFileDuration();
         if(total_sec == 0)
         total_sec = 1;
         lv_slider_set_range(guider_ui.screen_music_slider, 0, total_sec);
       }
        uint32_t curr = audio->getAudioCurrentTime();
        g_last_curr_sec = curr;   // 供 EOF 处理判断"是否真的放完"
        char buf[32];
        sprintf(buf, "%02d:%02d / %02d:%02d", 
            curr / 60, curr % 60, 
            total_sec / 60, total_sec % 60
        );
        lv_label_set_text(guider_ui.screen_music_label_slider_time, buf);
        lv_slider_set_value(guider_ui.screen_music_slider, curr, LV_ANIM_OFF);
        lyric_update(curr);
    }
    // 处理音频任务上抛的跨线程事件（EOF 切歌）
    if (g_eof_pending) {
        g_eof_pending = false;
        // 库在抛 evt_eof 前已经把当前时间清零，这里用定时器最后一次读到的位置判断"是否放完"
        const uint32_t elapsed_ms = millis() - g_play_start_ms;
        if (elapsed_ms < 1000) {
            // 刚开播就来 EOF：这是上一首歌遗留的陈旧事件（比如手动切歌与上一曲结束撞在一起），
            // 不能跟着切，否则会出现"刚播就跳下一首"
            printf("[eof] 丢弃陈旧 EOF：本曲仅播放 %lu ms\n", (unsigned long)elapsed_ms);
        } else {
            printf("[eof] 切歌：已播 %lu s / 总时长 %u s（%lu ms）\n",
                   (unsigned long)g_last_curr_sec, (unsigned)total_sec, (unsigned long)elapsed_ms);
            if (total_sec > 10 && g_last_curr_sec + 5 < total_sec) {
                // 明显没放完就收到 EOF，说明库算出的音频数据长度偏小（ID3 标签解析/文件损坏）
                printf("[eof] !! 提前结束：差 %u s，检查该文件的 ID3 标签与文件完整性\n",
                       (unsigned)(total_sec - g_last_curr_sec));
            }
            handle_eof_next();
        }
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
            if (i < 0 || i >= total) continue;   // 歌曲数不足一屏时 at() 越界会直接 abort()
            const char* songName = music_files.at(i).c_str();
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
        if (audio != nullptr) return;   // 重入保护：重复 LOADED 会覆盖 audio 指针，导致 PSRAM 与 I2S 通道泄漏
        g_eof_pending = false;          // 清掉上一轮遗留的切歌标志
        if (g_id3_ptr == NULL) g_id3_ptr = new id3::Tag();  // 进页面创建（退页面销毁），退出后标签内存整体归还堆
        list_start_index = 0;           // 列表回到首页，避免与上次退出时的滑动位置错位
        list_is_touching = false;
        music_files.reserve(128);       // 预分配，减少扫描期间 vector 反复 realloc 造成的 DRAM 碎片
        board.scanDir(basePath,&music_files,MP3_TYPE);
        // 事件回调只注册一次：重复注册会让 handler 被重复调用并永久多占一份内存
        if (!list_cb_registered) {
            lv_obj_add_event_cb(guider_ui.screen_music_list_music, music_list_swipe_event_handler, LV_EVENT_ALL, NULL);
            list_cb_registered = true;
        }
        fill_music_list_panel();        // 内部依据 list_swipe_registered 决定重建还是复用
        list_swipe_registered = true;
        lv_obj_set_style_text_font(guider_ui.screen_music_label_music_name, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT); 
        lv_obj_set_style_text_font(guider_ui.screen_music_label_title, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT); 
        cover_reset();                  // 默认封面资源是 100x100，先缩回原尺寸居中显示
        lyric_click_init();             // 封面容器点击 → 封面/整页歌词切换
        lyric_page_reset();
        if (music_timer == NULL) {
            music_timer = lv_timer_create(lv_timer_refresh_time, 50, NULL);
        }
            playEnable = true;
            // 先（codec 还归 AI_VOX 时）把硬件音量拉满，响度只由软件音量决定
            Setting::GetInstance().PreparePlaybackVolume();
            // 让渡前先停掉 AI_VOX(I2S_NUM_1)，避免两套 I2S 同时使能驱动同一组 codec 引脚
            if (auto dev = board.getAudioDeviceEs8311()) {
                dev->disable_i2s();
            }
            // 每次进入页面都创建全新的 Audio 对象，退出时 delete 销毁
            // 析构函数会 stopSong + disable/del I2S_NUM_0 通道 + stopAudioTask，确保状态不累积
            audio = new Audio();
            audio->setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_MCK);
            audio->setTone(0, 0, 0);                   // 触发计算 0dB 平坦响应系数，避免 IIR coeffs 全 0 导致静音
            audio->settings.IIR_FILTER = false;        // 绕过 biquad IIR（旧 esp-dsp 与新版库系数约定不兼容造成静音）
            //S audio->settings.SPECTRUM = true;         // 使能频谱分析（evt_spectrum，新版库默认 false）—— 频谱已关闭，不再开启 FFT 计算
            Setting::GetInstance().ApplySoftwareVolume(audio);  // 滑块百分比 → 软件音量（v4.0.0 默认 0 即静音）
            audio->audio_info_callback = on_audio_info; // 注册事件回调，接收频谱/EOF/封面/歌词数据
            const BaseType_t xCoreID = 1;
            xTaskCreatePinnedToCore(
                        music_back_task,
                        "music_back_task",
                        MUSIC_TASK_STACK_WORDS,
                        this,
                        2 | portPRIVILEGE_BIT,
                        &music_back_task_handle,
                        xCoreID
            );
            // 创建失败时句柄仍为 NULL，后续 loop 会在下一次 tick 自然返回，不影响其它清理
            vTaskDelay(pdMS_TO_TICKS(200));
            play_music();
    }else{
        lv_obj_clear_flag(page_head.time_label,LV_OBJ_FLAG_HIDDEN);
        playEnable = false;
        // 1) 先停掉 LVGL 定时器（内部会访问 audio->），避免后续销毁过程中再被触发
        if (music_timer != NULL) {
            lv_timer_del(music_timer);
            music_timer = NULL;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
        lyric_page_reset();             // 整页歌词可能正显示着，先退回封面模式
        cover_release();                // 恢复默认封面并释放封面缓冲
        // 整个销毁标签对象：clear() 只清内容，vector 的元素数组容量和 String 的缓冲
        // （合计约 2~6KB，散在主区空闲洞里）不会归还；delete 才能全部还给堆。
        if (g_id3_ptr != NULL) {
            delete g_id3_ptr;
            g_id3_ptr = NULL;
        }
        // 3) 销毁 Audio 对象：析构函数 stopSong + disable/del I2S_NUM_0 通道 + stopAudioTask + 释放互斥锁
        if (audio != nullptr) {
            audio->stopSong();
            vTaskDelay(pdMS_TO_TICKS(30));  // 给库内 "PeriodicTask" 留出退出窗口，降低 UAF 概率
            delete audio;
            audio = nullptr;
        }
        // 把 codec 的 I2S 引脚让渡回 AI_VOX(I2S_NUM_1)，保证退出后通知音/语音可用
        if (auto dev = board.getAudioDeviceEs8311()) {
            dev->reconfigure_gpio();
        }
        // 播放页里调过的音量只落在 Audio 的软件音量上，这里补写回 ES8311 硬件音量
        Setting::GetInstance().FlushVolumeToHardware();
        music_files.clear();
        music_files.shrink_to_fit();
        lv_obj_clean(guider_ui.screen_music_list_music);
        list_swipe_registered = false;
    }
}
