#ifndef __ESP32_BOARD_H_
#define __ESP32_BOARD_H_
#include <arduino.h>
#include "driver/i2c_master.h"
#include <vector>
#include "esp_heap_caps.h"
#include "ai_vox_engine.h"
#include "audio_device/audio_device_es8311.h"
#define IIC_MASTER_NUM I2C_NUM_0
#define IIC_MASTER_FREQ_HZ 400000
#define IIC_MASTER_TX_BUF_DISABLE 0
#define IIC_MASTER_RX_BUF_DISABLE 0

//I2C相关引脚配置
constexpr auto kI2cScl = GPIO_NUM_15;  // ES8311 CCLK
constexpr auto kI2cSda = GPIO_NUM_16;  // ES8311 CDATA
constexpr auto kEs8311Mclk = GPIO_NUM_4;   // ES8311 Master clock (MCLK) -- I2S MCLK/MCK
constexpr auto kEs8311Sclk = GPIO_NUM_5;   // ES8311 Serial data bit clock/DMIC bit clock (SCLK/DMIC_SCL) -- I2S SCLK/BCK
constexpr auto kEs8311Lrck = GPIO_NUM_7;    // ES8311 Serial data left and right channel frame clock (LRCK) --  I2S WS
constexpr auto kEs8311Dsdin = GPIO_NUM_8;   // ES8311 DAC serial data input (DSDIN) -- I2S DOUT
constexpr auto kEs8311Asdout = GPIO_NUM_6;  // ES8311 ADC serial data output (ASDOUT) -- I2S DIN
constexpr auto kI2CPort = I2C_NUM_0;
constexpr uint8_t kEs8311I2cAddress = 0x30;
constexpr uint32_t kAudioSampleRate = 16000;

//I2S相关引脚配置
#define AP_ENABLE 1
#define IIS_NUM   I2S_NUM_1
#define MCLK_MULTIPLE I2S_MCLK_MULTIPLE_384
#define RECORD_TIME_MS 5000
#define IIS_MCK   4
#define IIS_BCK   5
#define IIS_DINT  6 
#define IIS_DOUT  8
#define IIS_WS    7

//SD卡相关引脚配置
#define SD_CLK 38
#define SD_CMD 40
#define SD_D0  39
#define SD_D1  41
#define SD_D2  48
#define SD_D3  47
//最大wifi数量
#define MAX_WIFI_SCAN_NUM 10

typedef enum {
  IMG_TYPE,
  MP3_TYPE,
  JSON_TYPE,
  NES_TYPE
} FILE_TYPE; 

typedef struct WifiInfo {
    String ssid;
    int8_t rssi;
};


// 路径池：路径和索引都放在 PSRAM 上，一块大缓冲存全部路径。
// 替代 std::vector<String>：避免几十个 String 小块把内部堆的最大连续块切碎。
// push()/release() 的实现在下方 inline，music/photo_album/lottie/game 四个页面共用。
class MusicPathList {
  public:
    ~MusicPathList() { release(); }
    void clear() { count_ = 0; pool_used_ = 0; }   // 保留池容量，下次扫描直接复用
    bool empty() const { return count_ == 0; }
    size_t size() const { return count_; }
    const char *at(size_t i) const { return (i < count_) ? index_[i] : ""; }
    bool push(const char *name) {
        if (name == nullptr) return false;
        const size_t len = strlen(name) + 1;
        if (count_ == index_cap_) {
            const size_t ncap = index_cap_ ? index_cap_ * 2 : 128;
            const char **ni = (const char **)psram_realloc_((void *)index_, ncap * sizeof(const char *));
            if (ni == nullptr) return false;
            index_ = ni;
            index_cap_ = ncap;
        }
        if (pool_used_ + len > pool_cap_) {
            size_t ncap = pool_cap_ ? pool_cap_ : 4096;
            while (ncap < pool_used_ + len) ncap *= 2;
            char *np = (char *)psram_realloc_(pool_, ncap);
            if (np == nullptr) return false;
            pool_ = np;
            pool_cap_ = ncap;
        }
        memcpy(pool_ + pool_used_, name, len);
        index_[count_++] = pool_ + pool_used_;
        pool_used_ += len;
        return true;
    }
    void remove_at(size_t i) {
        if (i >= count_) return;
        for (size_t j = i; j + 1 < count_; ++j) index_[j] = index_[j + 1];
        --count_;
    }
    void release() {
        if (pool_) { heap_caps_free(pool_); pool_ = nullptr; }
        if (index_) { heap_caps_free((void *)index_); index_ = nullptr; }
        pool_cap_ = pool_used_ = index_cap_ = count_ = 0;
    }
  private:
    static void *psram_realloc_(void *p, size_t size) {
        void *np = heap_caps_realloc(p, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (np == nullptr) np = heap_caps_realloc(p, size, MALLOC_CAP_8BIT);   // PSRAM 不够时退回内部堆
        return np;
    }
    char  *pool_ = nullptr;
    size_t pool_cap_ = 0;
    size_t pool_used_ = 0;
    const char **index_ = nullptr;
    size_t index_cap_ = 0;
    size_t count_ = 0;
};

class Board{
  private:
    i2c_master_bus_handle_t g_i2c_master_bus_handle = nullptr;
    i2s_chan_handle_t tx_handle_ = nullptr;
    i2s_chan_handle_t rx_handle_ = nullptr;
    std::shared_ptr<ai_vox::AudioDeviceEs8311> g_audio_device_es8311;
    std::function<void(const std::vector<WifiInfo>& wifi_list)> on_wifi_scaned_;
    std::vector<WifiInfo> wifi_list_;
    bool wifi_scanning_;
    SemaphoreHandle_t wifi_scan_mutex_;
    void i2c_driver_init(const gpio_num_t sda_pin,const gpio_num_t scl_pin);
    void i2s_driver_init(void);
    void init_sd_card(void);
  public:
    void board_init();
    void es8311_initial(void);
    i2c_master_bus_handle_t get_I2C_master_bus_handle(){
      return g_i2c_master_bus_handle;
    }
     std::shared_ptr<ai_vox::AudioDeviceEs8311> getAudioDeviceEs8311(){
      return g_audio_device_es8311;
     }
    size_t record_audio(uint16_t *audio_buffer,uint32_t samples);
    void play_audio(uint16_t *record_buffer,size_t recorded_size);
    void scanDir(const char *dirPath,std::vector<String> *filelist,FILE_TYPE file_type);
    // 回调版目录扫描：每命中一个符合类型的文件名就回调一次（name 只在回调期间有效），
    // 调用方可直接拷进 PSRAM 池，避免几十个 String 小块把内部堆切碎。返回 false 可提前结束。
    typedef bool (*DirFileCallback)(void *ctx, const char *name);
    void scanDirEach(const char *dirPath,DirFileCallback onFile,void *ctx,FILE_TYPE file_type);
    void playMp3(const uint8_t* data, size_t size);
    void GetWifiList();
    void onWifiListScaned(std::function<void(const std::vector<WifiInfo>& wifi_list)> callback);
};
extern Board board;
#endif