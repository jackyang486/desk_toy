#ifndef __ONLINE_IMG_H_
#define __ONLINE_IMG_H_

#include <Arduino.h>
#include "src/generated/gui_guider.h"
#include "src/custom/custom.h"

// 图片尺寸（与 picsum.photos 请求尺寸一致）
#define ONLINE_IMG_W 240
#define ONLINE_IMG_H 320
// JPEG 原始数据缓冲上限（240x320 JPEG 通常 10~40KB，留余量到 100KB）
#define ONLINE_JPG_BUF_SIZE (100 * 1024)
// 刷新间隔
#define ONLINE_IMG_REFRESH_MS  10000

// 双缓冲：A/B 交替，下载任务写非活跃缓冲，LVGL 显示活跃缓冲
typedef struct {
    uint8_t* jpg_buf;     // JPEG 原始数据缓冲 (PSRAM)
    size_t jpg_len;       // JPEG 实际长度
    bool valid;           // 是否已下载完成可显示
    lv_img_dsc_t dsc;     // 用 LV_IMG_CF_RAW 包装 jpg_buf，LVGL 自动解码
} ImgBuf_t;

class OnlineImg {
public:
    static OnlineImg& GetInstance();

    // 页面加载/卸载入口
    void img_screen_load(SCREEN_LOAD_STAUS status);
    static void img_win_click(lv_event_t *e);

private:
    OnlineImg() = default;
    ~OnlineImg() = default;
    OnlineImg(const OnlineImg&) = delete;
    OnlineImg& operator=(const OnlineImg&) = delete;

    // 下载任务（写非活跃缓冲，完成后 give 信号量）
    static void DownloadTask(void* pvParam);
    // lv_timer 回调：在 LVGL 主线程检查信号量，切换活跃缓冲并刷新图片
    static void LvTimerCallback(lv_timer_t* timer);

    bool AllocateBuf(ImgBuf_t& b);
    void FreeBuf(ImgBuf_t& b);
    // 将下载的 JPEG 保存到 SD 卡 /photo 目录
    void saveToSD();

    ImgBuf_t bufA_;
    ImgBuf_t bufB_;
    uint8_t active_idx_ = 0;            // 0:A 显示  1:B 显示
    bool active_ = false;
    TaskHandle_t task_handle_ = nullptr;
    SemaphoreHandle_t img_sem_ = nullptr;
    SemaphoreHandle_t save_mutex_ = nullptr;
    lv_timer_t* lv_timer_ = nullptr;
    uint32_t seed_counter_ = 0;         // 用于变化 seed，每次刷新取不同图片
};

void online_img_win_load(SCREEN_LOAD_STAUS status);

#endif
