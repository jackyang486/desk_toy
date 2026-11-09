#include "setting.h"
#include "ESP32_board.h"

// music.cpp 提供的音量路由钩子：音乐页解码任务直连 codec，软件音量作用在解码 PCM 上
extern bool music_apply_volume(void);
extern bool music_is_active(void);
#include "ESP32_board.h"
#include <Preferences.h>
#include "ai_vox_engine.h"
#include "esp32-hal-ledc.h"
#include "notification_0_mp3.h"
#include "SD_MMC.h"


static const char* SETTINGS_NAMESPACE = "device_settings";
static const char* DEFAULT_ONLINE_IMG_URL_BASE = "http://192.168.66.68:5000/proxy?w=240&h=320";
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
      volume_(50),
      screen_timeout_ms_(DEFAULT_SCREEN_TIMEOUT_MS),
      online_img_url_base_(DEFAULT_ONLINE_IMG_URL_BASE){
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
    setting.ApplyVolume(); //播放页里走软件音量，其余场景写硬件音量
    // 电台播放页里 AI_VOX 的 I2S 通道已被 disable_i2s() 停掉，音乐页里解码任务正独占 codec，
    // 此时 playMp3() 与播放并发写/重开 codec 会失败甚至崩溃，所以播放期间不出提示音
    if (audio == nullptr && !music_is_active()) {
        board.playMp3(kNotification0mp3, sizeof(kNotification0mp3));
    }
}

void sun_value_change(uint8_t brightness) {
    auto& setting = Setting::GetInstance();
    setting.SetScreenBrightness(static_cast<uint8_t>(brightness));
    setting.ApplyBrightness();
}

//设置亮屏时间
void time_value_change(uint8_t expire_time) {
    auto& setting = Setting::GetInstance();
    setting.SetScreenTimeout(expire_time*1000*60);
}

void Setting::ApplyBrightness() {
    uint16_t raw = static_cast<uint16_t>(brightness_* 255 / 100);
    ledcWrite(TFT_BL_PIN, raw);
}

// 只有 AI_VOX 独占 codec 时才能安全写 ES8311 寄存器
void Setting::ApplyHardwareVolume() {
    auto audio_device = board.getAudioDeviceEs8311();
    if (audio_device) {
        uint16_t raw = static_cast<uint16_t>(volume_* 255 / 100);
        audio_device->set_volume(raw);
    }
    hw_volume_dirty_ = false;
}

void Setting::ApplyVolume() {
    // 音乐播放页：解码任务直连 AudioDeviceEs8311，软件音量作用在解码 PCM 上（music.cpp 提供）
    if (music_apply_volume()) {
        return;
    }
    // 电台页面播放期间，codec 的 I2S 时钟与 GPIO 由 ESP32-audioI2S 接管：
    // AI_VOX 的 I2S_NUM_1 已被 disable_i2s() 停掉，Audio 又把同一组引脚重配成 I2S_NUM_0，
    // 这时再走 esp_codec_dev_set_out_vol() 去写 ES8311（依赖 MCLK/I2S 时钟的 codec），
    // 实测会直接崩溃。所以播放期间只调库的软件音量，硬件音量等退出播放页后再补写。
    if (audio != nullptr) {
        ApplySoftwareVolume(audio);
        return;
    }
    ApplyHardwareVolume();
}

// 进播放页、还没 disable_i2s() 时调用：硬件音量拉满，响度完全交给软件音量，
// 这样同一个滑块位置在播放页和其它页面听起来一致（避免两层衰减叠加）。
void Setting::PreparePlaybackVolume() {
    auto dev = board.getAudioDeviceEs8311();
    if (dev == nullptr) return;
    dev->set_volume(255);
    hw_volume_dirty_ = true;   // 退出播放页时按 volume_ 恢复硬件音量
}

void Setting::ApplySoftwareVolume(Audio* player) {
    if (player == nullptr) return;
    hw_volume_dirty_ = true;
    uint8_t steps = player->getVolumeSteps();
    if (steps == 0) steps = 21;
    player->setVolume(static_cast<uint8_t>(volume_ * steps / 100));
}

// 退出播放页（audio 已 delete、reconfigure_gpio() 已把 codec 让渡回 AI_VOX）后调用
void Setting::FlushVolumeToHardware() {
    if (audio != nullptr) return;   // 还在播放页，硬件仍归另一个库管
    if (!hw_volume_dirty_) return;
    ApplyHardwareVolume();
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

void Setting::LoadOnlineImageUrl() {
    File file = SD_MMC.open("/param.txt", FILE_READ);
    if (!file) {
        online_img_url_base_ = DEFAULT_ONLINE_IMG_URL_BASE;
        printf("[Setting] Failed to open /param.txt; using default image URL");
        return;
    }

    String url = file.readStringUntil('\n');
    file.close();
    url.trim();
    if (url.length() == 0) {
        online_img_url_base_ = DEFAULT_ONLINE_IMG_URL_BASE;
        printf("[Setting] /param.txt first line is empty; using default image URL");
        return;
    }
    online_img_url_base_ = url;
    printf("[Setting] /param.txt first line：%s\n",online_img_url_base_.c_str());
}

const String& Setting::GetOnlineImageUrlBase() const {
    return online_img_url_base_;
}
