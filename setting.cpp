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
      screen_timeout_ms_(DEFAULT_SCREEN_TIMEOUT_MS),
      hasInitial(false){
      LoadSettings();
}

void Setting::Init() {
    // 初始化背光LEDC PWM
    ledcAttach(TFT_BL_PIN, LEDC_FREQ, LEDC_RESOLUTION_BITS);
    // 应用初始值
    ApplyBrightness();
    ApplyVolume();
    board.playMp3(kNotification0mp3, sizeof(kNotification0mp3));
}

void setting_win_load(SCREEN_LOAD_STAUS status){
  auto& setting = Setting::GetInstance();
  setting.screen_loaded(status);
}

void Setting::screen_loaded(SCREEN_LOAD_STAUS status){
  if(status == LOADED){ 
        // 设置滑块范围
        if(!hasInitial){
            hasInitial = true;
            lv_slider_set_range(guider_ui.screen_setting_slider_bright, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
            lv_label_set_text(guider_ui.screen_setting_label_vol_title,"音量大小");
            lv_label_set_text(guider_ui.screen_setting_label_bright_title,"屏幕亮度");
            lv_label_set_text(guider_ui.screen_setting_label_light_switch_title,"熄屏时间");
            lv_obj_set_style_text_font(guider_ui.screen_setting_label_vol_title, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(guider_ui.screen_setting_label_bright_title, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(guider_ui.screen_setting_label_light_switch_title, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(guider_ui.screen_setting_label_minute, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);

            // 设置初始值
            lv_slider_set_value(guider_ui.screen_setting_slider_bright, brightness_, LV_ANIM_OFF);
            lv_slider_set_value(guider_ui.screen_setting_slider_volume, volume_, LV_ANIM_OFF);
            uint8_t screen_off_minute = static_cast<uint16_t>(screen_timeout_ms_/(1000*60));
            lv_slider_set_value(guider_ui.screen_setting_slider_switch, screen_off_minute, LV_ANIM_OFF);

            // 更新百分比标签
            UpdateBrightnessLabel(brightness_);
            UpdateVolumeLabel(volume_);
            UpdateScreenOffMinute(screen_off_minute);

            // 绑定事件回调
            lv_obj_add_event_cb(guider_ui.screen_setting_slider_volume, volume_slider_event_handler, LV_EVENT_VALUE_CHANGED, nullptr);
            lv_obj_add_event_cb(guider_ui.screen_setting_slider_bright, brightness_slider_event_handler, LV_EVENT_VALUE_CHANGED, nullptr);
            lv_obj_add_event_cb(guider_ui.screen_setting_slider_switch, light_switch_event_handler, LV_EVENT_VALUE_CHANGED, nullptr);
        }
    }else{
        // 仅当 NVS 中存储的值与当前对象属性有差异时才写入，避免无谓的擦写
        prefs_.begin(SETTINGS_NAMESPACE, true); // 只读模式
        uint8_t  stored_brightness       = prefs_.getUChar (KEY_BRIGHTNESS,     DEFAULT_SCREEN_BRIGHTNESS);
        uint16_t stored_volume           = prefs_.getUShort(KEY_VOLUME,          100);
        uint32_t stored_screen_timeout   = prefs_.getUInt  (KEY_SCREEN_TIMEOUT,  DEFAULT_SCREEN_TIMEOUT_MS);
        prefs_.end();

        if (stored_brightness != brightness_ ||
            stored_volume != volume_ ||
            stored_screen_timeout != screen_timeout_ms_) {
            SaveSettings();
        }
    }
}

void Setting::volume_slider_event_handler(lv_event_t *e) {
    auto& setting = Setting::GetInstance();
    int32_t value = lv_slider_get_value(guider_ui.screen_setting_slider_volume);
    setting.SetVolume(static_cast<uint16_t>(value)); 
    setting.UpdateVolumeLabel(static_cast<uint16_t>(value));
    setting.ApplyVolume();
    board.playMp3(kNotification0mp3, sizeof(kNotification0mp3));
}

void Setting::brightness_slider_event_handler(lv_event_t *e) {
    auto& setting = Setting::GetInstance();
    int32_t value = lv_slider_get_value(guider_ui.screen_setting_slider_bright);
    setting.SetScreenBrightness(static_cast<uint8_t>(value));
    setting.UpdateBrightnessLabel(static_cast<uint8_t>(value));
    setting.ApplyBrightness();
}

void Setting::light_switch_event_handler(lv_event_t *e) {
    auto& setting = Setting::GetInstance();
    int32_t value = lv_slider_get_value(guider_ui.screen_setting_slider_switch);
    setting.SetScreenTimeout(value*1000*60);
    setting.UpdateScreenOffMinute(static_cast<uint8_t>(value));
}

void Setting::ApplyBrightness() {
    ledcWrite(TFT_BL_PIN, brightness_);
}

void Setting::ApplyVolume() {
    auto audio_device = board.getAudioDeviceEs8311();
    if (audio_device) {
        audio_device->set_volume(volume_);
    }
}

void Setting::UpdateVolumeLabel(uint16_t volume) {
    if (guider_ui.screen_setting_label_vol_percent) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d%%", volume);
        lv_label_set_text(guider_ui.screen_setting_label_vol_percent, buf);
    }
}

void Setting::UpdateBrightnessLabel(uint8_t brightness) {
    if (guider_ui.screen_setting_label_bright_percent) {
        uint8_t percent = (brightness * 100) / MAX_BRIGHTNESS;
        char buf[8];
        snprintf(buf, sizeof(buf), "%d%%", percent);
        lv_label_set_text(guider_ui.screen_setting_label_bright_percent, buf);
    }
}

void Setting::UpdateScreenOffMinute(uint8_t minute){
    if (guider_ui.screen_setting_label_minute) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d分钟", minute);
        lv_label_set_text(guider_ui.screen_setting_label_minute, buf);
    }
}

void Setting::SetScreenBrightness(uint8_t brightness) {
    if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    }
    brightness_ = brightness;
}

uint8_t Setting::GetScreenBrightness() const {
    return brightness_;
}

void Setting::SetVolume(uint16_t volume) {
    if (volume > 100) {
        volume = 100;
    }
    volume_ = volume;
}

uint16_t Setting::GetVolume() const {
    return volume_;
}

void Setting::SetScreenTimeout(uint32_t timeout_ms) {
    if (timeout_ms != 0 && timeout_ms < 5000) {
        timeout_ms = 5000;
    }
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
