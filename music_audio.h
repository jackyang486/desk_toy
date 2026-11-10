#ifndef __MUSIC_AUDIO_H_
#define __MUSIC_AUDIO_H_

#include <Arduino.h>

// MP3 播放引擎（单例 music_audio）
//
// 数据链路：SD_MMC → esp_audio_simple_dec 解码 → 软件音量/淡入 → AudioDeviceEs8311::Write()
// 不再使用 ESP32-audioI2S 的 Audio 对象（49KB 堆借还 + 独占 I2S_NUM_0 + 抢占 codec 引脚），
// codec 全程归 AI_VOX 的 I2S_NUM_1，没有任何引脚让渡动作。
//
// 线程模型：
//   LVGL 线程：begin/end/play/stop_current/toggle_pause/set_volume、读时间与响度
//   解码任务：静态栈 24KB，静态输入输出缓冲，只跑 decode 循环
class MusicAudio {
public:
    // 进入播放页：创建静态解码任务。返回 false 表示任务创建失败
    bool begin();
    // 退出播放页：停任务、等待退出、恢复 codec 默认输出（16k 单声道）
    void end();
    bool is_active() const;

    // 让解码任务退出当前文件并释放 SD 句柄（ID3 解析前必须调用：SDMMC 不支持并发事务）
    void stop_current(uint32_t timeout_ms = 500);
    // 开始播放指定文件（路径相对 SD_MMC 挂载点）
    void play(const char *path);

    // 暂停/继续
    void toggle_pause();
    void set_paused(bool paused);
    bool paused() const;

    uint32_t position_ms() const;
    uint32_t duration_ms() const;
    bool     is_playing() const;

    // 播完/打不开：任务置位，LVGL 线程取走（取走即清零）
    bool take_eof();

    // 0~100 软件音量（平方曲线，作用在解码 PCM 上）
    void set_volume(uint8_t percent);
    uint8_t volume() const;

    // 当前响度 0~255（解码任务算 PCM RMS），供 WS2812B 灯用；level_ms() 是最近更新时刻
    uint8_t  level() const;
    uint32_t level_ms() const;
};

extern MusicAudio music_audio;

#endif  // __MUSIC_AUDIO_H_
