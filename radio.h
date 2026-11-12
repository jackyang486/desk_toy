#ifndef __RADIO_H_
#define __RADIO_H_

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include "src/generated/gui_guider.h"
#include "src/custom/custom.h"

typedef struct RadioInfo {
    const char* name;
    const char* url;
};

class Radio {
public:
    static Radio& GetInstance();
    void Init();
    void PlayStation(size_t index);   // 切台并播放（HTTP 流，断线自动重连）
    void TogglePlay();                // 播放/暂停切换
    void StopPlay();
    bool IsPlaying() const { return playing_; }
    size_t GetCurrentIndex() const { return current_index_; }
    void radio_screen_load(SCREEN_LOAD_STAUS status);

private:
    Radio() = default;
    ~Radio() = default;
    Radio(const Radio&) = delete;
    Radio& operator=(const Radio&) = delete;

    size_t current_index_ = 0;
    bool playing_ = false;
    void ui_refresh();                // 把当前台名/状态/播放键状态刷到 screen_radio 播放页
    bool ensure_engine();             // 惰性启动 music_audio 解码任务（含硬件音量拉满）
};

void radio_win_load(SCREEN_LOAD_STAUS status);
// 供 events_init.c（C 文件）调用的 UI 动作，声明在 custom.h 的 extern "C" 里
void radio_prev(void);
void radio_next(void);
void radio_toggle_play(void);

#endif
