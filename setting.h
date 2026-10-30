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

    void SaveSettings();
    void LoadSettings();

    std::pair<String, String> getWifiConfig();
    void saveWifiConfig(const char* ssid,const char* pass);

private:
    Setting();
    ~Setting() = default;
    Setting(const Setting&) = delete;
    Setting& operator=(const Setting&) = delete;

    Preferences prefs_;
    uint8_t brightness_;
    uint16_t volume_;
    uint32_t screen_timeout_ms_;
};

#endif
