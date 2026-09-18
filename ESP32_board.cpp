#include <algorithm>
#include <cctype>
#include "ESP32_board.h"
#include "driver/i2c_master.h"
#include "driver/i2s_std.h"
#include "driver/i2s.h"
#include "esp_log.h"
#include "SD_MMC.h"  
#include "src/generated/gui_guider.h"
#include "components/espressif/esp_audio_codec/esp_mp3_dec.h"
#include "components/espressif/esp_audio_codec/esp_audio_simple_dec.h"

static const char *TAG = "BOARD";

Board board;
Audio* audio = nullptr;

void Board::init_sd_card()
{
  
  if(!SD_MMC.setPins(SD_CLK, SD_CMD, SD_D0, SD_D1, SD_D2, SD_D3))
  {
        Serial.println("Pin change failed!");
        return;
  }
  // Initialise SD before TFT
  if (!SD_MMC.begin("/sdcard", true)) 
  {
    Serial.println(F("SD.begin failed!"));
    return;
  }
  // 获取 SD 卡信息
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("未检测到 SD 卡");
    return;
  }
}

// 递归扫描指定目录
void Board::scanDir(const char *dirPath,std::vector<String> * filelist,FILE_TYPE file_type)
{
    lv_fs_dir_t dir;
    lv_fs_res_t res;
    // 打开目录
    res = lv_fs_dir_open(&dir, dirPath);
    if (res != LV_FS_RES_OK) {
        return;
    }
    // 用于存储文件名
    char fname[128];
    // 读取目录下每一项（文件/文件夹）
    while (1)
    {
        res = lv_fs_dir_read(&dir, fname);
        if (res != LV_FS_RES_OK || strlen(fname) <=1) {  //过滤掉，空、乱码、不可见字符）
            break;
        }
        // 跳过 . 和 .. 目录
        if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0 || strcmp(fname, "/") ==0 || strcmp(fname, "/image") == 0)
            continue;
        // 构建完整路径
        char fullPath[256];
        if (strchr(fname, '.') == NULL) {
             
        } else { //如果是文件，则分别读取图片文件和音乐文件
           lv_snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, fname);
           size_t len = strlen(fname);
           const std::string lower_name = [&]() {
               std::string s = fname;
               std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
                   return static_cast<char>(std::tolower(c));
               });
               return s;
           }();
                if(file_type == IMG_TYPE && len > 4){
                    if(lower_name.find(".jpg") != std::string::npos ||
                       lower_name.find(".jpeg") != std::string::npos ||
                       lower_name.find(".png") != std::string::npos ||
                       lower_name.find(".bmp") != std::string::npos ||
                       lower_name.find(".bin") != std::string::npos ||
                       lower_name.find(".gif") != std::string::npos){
                        filelist->push_back(String(fullPath));
                    }
                }else  if(file_type == MP3_TYPE && len > 4){
                     if(lower_name.find(".wav") != std::string::npos || lower_name.find(".mp3") != std::string::npos){
                        filelist->push_back(String(fullPath));
                     }
                }
        }
    }
    // 关闭目录
    lv_fs_dir_close(&dir);
}

void Board::es8311_initial(){
     g_audio_device_es8311 = std::make_shared<ai_vox::AudioDeviceEs8311>(g_i2c_master_bus_handle,
                                                                      kEs8311I2cAddress,  // ES8311 I2C address
                                                                      kI2CPort,           // I2C port
                                                                      kAudioSampleRate,   // sample rate
                                                                      kEs8311Mclk,        // ES8311 Master clock (MCLK)
                                                                      kEs8311Sclk,    // ES8311 Serial data bit clock/DMIC bit clock (SCLK/DMIC_SCL)
                                                                      kEs8311Lrck,    // ES8311 Serial data left and right channel frame clock (LRCK)
                                                                      kEs8311Asdout,  // ES8311 ADC serial data output (ASDOUT)
                                                                      kEs8311Dsdin);    // ES8311 DAC serial data input (DSDIN)
}

void Board::i2c_driver_init(const gpio_num_t sda_pin, const gpio_num_t scl_pin) {
    const i2c_master_bus_config_t i2c_master_bus_config = {
        .i2c_port = IIC_MASTER_NUM,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags = {
            .enable_internal_pullup = true,
        },
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_bus_config, &g_i2c_master_bus_handle));
    ESP_LOGI(TAG, "I2C bus initialized on port %d", IIC_MASTER_NUM);
}

//播放录音
void Board:: play_audio(uint16_t *record_buffer,size_t recorded_size) {
    size_t bytes_written = 0;
    esp_err_t ret = i2s_channel_write(tx_handle_, record_buffer, recorded_size, &bytes_written, 1000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2s write failed, %s", esp_err_to_name(ret));
    }
    ESP_LOGI(TAG, "Playback finished, %d bytes written", bytes_written);
}

//录音
 size_t  Board::record_audio(uint16_t *audio_buffer,uint32_t samples) {
    size_t bytes_recorded = 0;
    esp_err_t ret = i2s_channel_read(rx_handle_, audio_buffer, samples, &bytes_recorded, portMAX_DELAY);
    if (ret != ESP_OK) {
            ESP_LOGE(TAG, "i2s read failed, %s", esp_err_to_name(ret));
    }
    ESP_LOGI(TAG, "Recording finished, %d bytes recorded", bytes_recorded);
    return bytes_recorded;
}

void Board::board_init(){
  
  wifi_scanning_ = false;
  wifi_scan_mutex_ = xSemaphoreCreateMutex();
  pinMode(AP_ENABLE, OUTPUT);
  digitalWrite(AP_ENABLE, LOW);  
  i2c_driver_init(kI2cSda, kI2cScl);
  es8311_initial();
  init_sd_card();
  //ESP_ERROR_CHECK(es8311_codec_init(g_i2c_master_bus_handle));
}

void  Board::playMp3(const uint8_t* data, size_t size) {
  auto ret = esp_mp3_dec_register();
  if (ret != ESP_AUDIO_ERR_OK) {
    printf("Failed to register mp3 decoder: %d\n", ret);
    abort();
  }

  esp_audio_simple_dec_handle_t decoder = nullptr;
  esp_audio_simple_dec_cfg_t audio_dec_cfg{
      .dec_type = ESP_AUDIO_SIMPLE_DEC_TYPE_MP3,
      .dec_cfg = nullptr,
      .cfg_size = 0,
  };
  ret = esp_audio_simple_dec_open(&audio_dec_cfg, &decoder);
  if (ret != ESP_AUDIO_ERR_OK) {
    printf("Failed to open mp3 decoder: %d\n", ret);
    abort();
  }
  g_audio_device_es8311->OpenOutput(16000);

  esp_audio_simple_dec_raw_t raw = {
      .buffer = const_cast<uint8_t*>(data),
      .len = size,
      .eos = true,
      .consumed = 0,
      .frame_recover = ESP_AUDIO_SIMPLE_DEC_RECOVERY_NONE,
  };

  uint8_t* frame_data = (uint8_t*)malloc(4096);
  esp_audio_simple_dec_out_t out_frame = {
      .buffer = frame_data,
      .len = 4096,
      .needed_size = 0,
      .decoded_size = 0,
  };

  while (raw.len > 0) {
    const auto ret = esp_audio_simple_dec_process(decoder, &raw, &out_frame);
    if (ret == ESP_AUDIO_ERR_BUFF_NOT_ENOUGH) {
      // Handle output buffer not enough case
      out_frame.buffer = reinterpret_cast<uint8_t*>(realloc(out_frame.buffer, out_frame.needed_size));
      if (out_frame.buffer == nullptr) {
        break;
      }
      out_frame.len = out_frame.needed_size;
      continue;
    }

    if (ret != ESP_AUDIO_ERR_OK) {
      break;
    }

    g_audio_device_es8311->Write(reinterpret_cast<int16_t*>(out_frame.buffer), out_frame.decoded_size >> 1);
    raw.len -= raw.consumed;
    raw.buffer += raw.consumed;
  }

  free(frame_data);

  g_audio_device_es8311->CloseOutput();
  esp_audio_simple_dec_close(decoder);
  esp_audio_dec_unregister(ESP_AUDIO_TYPE_MP3);
}

//获取wifi列表
void Board::GetWifiList() {
    if (xSemaphoreTake(wifi_scan_mutex_, pdMS_TO_TICKS(5000)) != pdTRUE) {
        return; // 超时返回缓存
    }
    // 有缓存且未过期，直接返回（可加时间戳判断是否需要刷新）
    if (!wifi_list_.empty()) {
        String connected_ssid = WiFi.SSID();
        xSemaphoreGive(wifi_scan_mutex_);
        on_wifi_scaned_(wifi_list_);
        return;
    }

    // ⭐ 在锁内同步完成扫描，保证返回的是最新结果
    WiFi.mode(WIFI_STA);
    // 不要 disconnect，避免断网
    int n = WiFi.scanNetworks(false, false, false, 300);
    std::vector<WifiInfo> temp_list;
    if (n > 0) {
        for (int i = 0; i < n && i < MAX_WIFI_SCAN_NUM; i++) {
            WifiInfo info;
            info.ssid = WiFi.SSID(i);
            info.rssi = WiFi.RSSI(i);
            temp_list.push_back(std::move(info)); // ⭐ move 避免深拷贝
        }
        WiFi.scanDelete();
    }
    printf("scan finished...\n");
    wifi_list_ = std::move(temp_list); // ⭐ 已在锁内，安全赋值
    xSemaphoreGive(wifi_scan_mutex_);
    on_wifi_scaned_(wifi_list_);
    return;
}

//wifi获取完之后回调
void  Board::onWifiListScaned(std::function<void(const std::vector<WifiInfo>& wifi_list)> callback) {
    on_wifi_scaned_ = callback;
}


