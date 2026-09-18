#ifndef __ESP32_BOARD_H_
#define __ESP32_BOARD_H_
#include <arduino.h>
#include "Audio.h"
#include "driver/i2c_master.h"
#include <vector>
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
  MP3_TYPE
} FILE_TYPE; 

typedef struct WifiInfo {
    String ssid;
    int8_t rssi;
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
    void playMp3(const uint8_t* data, size_t size);
    void GetWifiList();
    void onWifiListScaned(std::function<void(const std::vector<WifiInfo>& wifi_list)> callback);
};
extern Board board;
// 改为指针：进入音乐/电台页面时 new，退出时 delete，确保 I2S 通道与音频任务状态每次全新
extern Audio* audio;
#endif