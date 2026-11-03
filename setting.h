#ifndef __SETTING_H__
#define __SETTING_H__

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <functional>
#include <Preferences.h>
#include "src/generated/gui_guider.h"
#include "src/custom/custom.h"

#define DEFAULT_SCREEN_BRIGHTNESS 60
#define DEFAULT_SCREEN_TIMEOUT_MS 300000
#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 0
#define TFT_BL_PIN 45
#define LEDC_FREQ 5000
#define LEDC_RESOLUTION_BITS 8

class Audio;

class Setting {
public:
    static Setting& GetInstance();

    void Init();

    void SetScreenBrightness(uint8_t brightness);
    uint8_t GetScreenBrightness() const;

    void SetVolume(uint16_t volume);
    uint16_t GetVolume() const;

    void SetScreenTimeout(uint32_t timeout_ms);
    uint32_t GetScreenTimeout() const;

    void ApplyBrightness();
    void ApplyVolume();
    // MP3/电台页：在 disable_i2s() 之前调用，把硬件音量拉满，让响度只由软件音量决定
    void PreparePlaybackVolume();
    // MP3/电台页：把滑块百分比映射成播放器的软件音量（音量条拖动时也走这里）
    void ApplySoftwareVolume(Audio* player);
    // 退出 MP3/电台页面、codec 归还给 AI_VOX 后调用，把播放期间改过的音量补写到硬件
    void FlushVolumeToHardware();

    void SaveSettings();
    void LoadSettings();

    void LoadOnlineImageUrl();
    const String& GetOnlineImageUrlBase() const;

    std::pair<String, String> getWifiConfig();
    void saveWifiConfig(const char* ssid,const char* pass);

private:
    Setting();
    ~Setting() = default;
    Setting(const Setting&) = delete;
    Setting& operator=(const Setting&) = delete;

    void ApplyHardwareVolume();

    Preferences prefs_;
    uint8_t brightness_;
    uint16_t volume_;
    uint32_t screen_timeout_ms_;
    String online_img_url_base_;
    bool hw_volume_dirty_ = false;   // 播放页里音量只改到软件侧，退出时需补写硬件
};

#endif
