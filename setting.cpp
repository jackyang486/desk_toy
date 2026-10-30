#include "setting.h"
#include "ESP32_board.h"
#include <Preferences.h>
#include "ai_vox_engine.h"
#include "esp32-hal-ledc.h"
#include "notification_0_mp3.h"


static const char* SETTINGS_NAMESPACE = "device_settings";
static const char* KEY_BRIGHTNESS = "brightness";
static const char* KEY_VOLUME = "volume";
static const char* KEY_SCREEN_TIMEOUT = "screen_timeout";
static const char* KEY_SCREEN_TIMEOUT_ENABLED = "screen_timeout_en";
//wifi存取键
static const char* WIFI_SSID_KEY = "ssid";
static const char* WIFI_PASS_KEY = "password";


Setting& Setting::GetInstance() {
    static Setting instance;
    return instance;
}

Setting::Setting()
    : brightness_(DEFAULT_SCREEN_BRIGHTNESS),
      volume_(80),
      screen_timeout_ms_(DEFAULT_SCREEN_TIMEOUT_MS){
      LoadSettings();
}

void Setting::Init() {
    // 初始化背光LEDC PWM
    ledcAttach(TFT_BL_PIN, LEDC_FREQ, LEDC_RESOLUTION_BITS);
    // 应用初始值
    ApplyBrightness();
    ApplyVolume();
    uint8_t screen_off_minute = static_cast<uint16_t>(screen_timeout_ms_/(1000*60));
    page_head_value_load((uint8_t)volume_,brightness_,screen_off_minute);
    board.playMp3(kNotification0mp3, sizeof(kNotification0mp3));
}

//设置声音
void volume_value_change(uint8_t volume) {
    auto& setting = Setting::GetInstance();
    setting.SetVolume(static_cast<uint16_t>(volume)); 
    setting.ApplyVolume(); //硬件设置声音
    board.playMp3(kNotification0mp3, sizeof(kNotification0mp3));
    setting.SaveSettings();
}

void sun_value_change(uint8_t brightness) {
    auto& setting = Setting::GetInstance();
    setting.SetScreenBrightness(static_cast<uint8_t>(brightness));
    setting.ApplyBrightness();
    setting.SaveSettings();
}

//设置亮屏时间
void time_value_change(uint8_t expire_time) {
    auto& setting = Setting::GetInstance();
    setting.SetScreenTimeout(expire_time*1000*60);
    setting.SaveSettings();
}

void Setting::ApplyBrightness() {
    uint16_t raw = static_cast<uint16_t>(brightness_* 255 / 100);
    ledcWrite(TFT_BL_PIN, raw);
}

void Setting::ApplyVolume() {
    auto audio_device = board.getAudioDeviceEs8311();
    if (audio_device) {
        uint16_t raw = static_cast<uint16_t>(volume_* 255 / 100);
        audio_device->set_volume(raw);
    }
}


void Setting::SetScreenBrightness(uint8_t brightness) {
    brightness_ = brightness;
}

// 返回原始 PWM 值（0-255），与 ApplyBrightness() 里的换算保持一致：
// main.cpp 直接拿它做 ledcWrite()，chat_robot.cpp 再 *100/255 反算百分比
uint8_t Setting::GetScreenBrightness() const {
    return static_cast<uint8_t>(brightness_ * 255 / 100);
}

void Setting::SetVolume(uint16_t volume) {
    volume_ = volume;
}

uint16_t Setting::GetVolume() const {
    return volume_;
}


void Setting::SetScreenTimeout(uint32_t timeout_ms) {
    screen_timeout_ms_ = timeout_ms;
}

uint32_t Setting::GetScreenTimeout() const {
    return screen_timeout_ms_;
}

//读取wifi配置信息
std::pair<String, String> Setting::getWifiConfig(){
    prefs_.begin(SETTINGS_NAMESPACE, true); // 读模式
    String ssid_raw = prefs_.getString(WIFI_SSID_KEY);
    String pass_raw = prefs_.getString(WIFI_PASS_KEY);
    prefs_.end();
    return {ssid_raw, pass_raw};
}
//保存wifi配置信息
void Setting::saveWifiConfig(const char* ssid,const char* pass){
    prefs_.begin(SETTINGS_NAMESPACE, false); // 读写模式
    prefs_.putString(WIFI_SSID_KEY,ssid);
    prefs_.putString(WIFI_PASS_KEY,pass);
    prefs_.end();
}

void Setting::SaveSettings() {
    prefs_.begin(SETTINGS_NAMESPACE, false); // 读写模式
    prefs_.putUChar(KEY_BRIGHTNESS, brightness_);
    prefs_.putUShort(KEY_VOLUME, volume_);
    prefs_.putUInt(KEY_SCREEN_TIMEOUT, screen_timeout_ms_);
    prefs_.end();
}

void Setting::LoadSettings() {
    prefs_.begin(SETTINGS_NAMESPACE, true);
    brightness_ = prefs_.getUChar(KEY_BRIGHTNESS, DEFAULT_SCREEN_BRIGHTNESS);
    volume_ = prefs_.getUShort(KEY_VOLUME, 80);
    screen_timeout_ms_ = prefs_.getUInt(KEY_SCREEN_TIMEOUT, DEFAULT_SCREEN_TIMEOUT_MS);
    prefs_.end();
}
