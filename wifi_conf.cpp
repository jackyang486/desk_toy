#include <Arduino.h>
#include <WiFi.h>
#include "wifi_conf.h"
#include "src/generated/gui_guider.h"
#include "src/generated/events_init.h"
#include "ai_vox_engine.h"
#include "components/wifi_configurator/wifi_configurator.h"
#include "weather.h"
#include "setting.h"
#include "ESP32_board.h"
#define MAX_SCAN_WIFI 10   // 最多显示10个WiFi
#define WIFI_CONNECT_TIMEOUT 5000  // 连接超时5秒
#define WIFI_NAMESPACE "wifi_data"
#include "network_connected_mp3.h"

constexpr smartconfig_type_t kSmartConfigType = SC_TYPE_ESPTOUCH_AIRKISS;


wifi_config& wifi_config::GetInstance() {
    static wifi_config instance;
    return instance;
}

void wifi_config::initial(){
    //注册回调事件
    board.onWifiListScaned([this](const std::vector<WifiInfo>& wifi_list){
      lv_obj_clean(guider_ui.screen_wifi_list);
      for(int i=0; i<wifi_list.size();i++){
          String ssid = wifi_list[i].ssid;
          int8_t rssi = wifi_list[i].rssi;
          char buf[64];
          snprintf(buf, sizeof(buf), "%s(%d)", ssid.c_str(),rssi);
          lv_obj_t *btn = lv_list_add_btn(guider_ui.screen_wifi_list, LV_SYMBOL_WIFI, buf);
          lv_obj_t *btnText = lv_obj_get_child(btn, 1); 
          lv_obj_set_style_text_font(btnText, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
          char *ssid_copy = strdup(ssid.c_str());
          lv_obj_add_event_cb(btn, wifi_config::wifi_list_click_event, LV_EVENT_CLICKED, (void *)ssid_copy);
          if (WiFi.status() == WL_CONNECTED) {
               if(wifi_list[i].ssid == WiFi.SSID()){
                  lv_obj_set_style_bg_color(btn, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
              }
          }
      }
   });
}
// WiFi 扫描任务
void wifi_config::wifi_scan_start()
{
   vTaskDelay(pdMS_TO_TICKS(200));
   static lv_style_t style_wifi_btn_text;
   lv_style_init(&style_wifi_btn_text);
   lv_style_set_text_font(&style_wifi_btn_text, &font_puhui_16_4);
   lv_obj_add_style(guider_ui.screen_wifi_list, &style_wifi_btn_text, LV_PART_ITEMS | LV_STATE_DEFAULT);
   printf("scan begin");
   xTaskCreate([](void* arg) {
       board.GetWifiList();
       vTaskDelete(NULL);
    }, "wifi_scan_task",4096, NULL, 3,NULL);
}

// WiFi 连接任务
void wifi_config::wifi_connect_task(){
    auto *params = new WifiConnectParams{
        this,  // ✅ 直接传 this，不依赖局部引用
        lv_textarea_get_text(guider_ui.screen_wifi_wifi_name_value),
        lv_textarea_get_text(guider_ui.screen_wifi_wifi_pass_value)
    };
    xTaskCreate([](void *arg){
      auto *p = static_cast<WifiConnectParams*>(arg);
      p->instance->wifi_connect(p->ssid.c_str(), p->password.c_str());
      vTaskDelay(pdMS_TO_TICKS(1000));
      lv_obj_add_flag(guider_ui.screen_wifi_form, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(guider_ui.screen_wifi_list, LV_OBJ_FLAG_HIDDEN);
      if(WiFi.status() == WL_CONNECTED){
         auto& setting = Setting::GetInstance();
         setting.saveWifiConfig(p->ssid.c_str(),p->password.c_str());
      }
      delete p;
      // 退出任务
      vTaskDelete(NULL);
    }, "wifi_connect_task", 4096, params, 3, NULL);
}



//wifi连接方法
void  wifi_config::wifi_connect(const char* ssid, const char* pass){
  printf("configure wifi\n");
  WiFi.begin(ssid, pass);
  WiFi.setSleep(false);
  uint32_t start = millis();
  while (1)
  {   // 超时判断
      if (millis() - start > WIFI_CONNECT_TIMEOUT)
      {
          WiFi.disconnect();
          WiFi.mode(WIFI_OFF); 
          break;
      }
        // 连接成功
      if (WiFi.status() == WL_CONNECTED)
      {
          board.playMp3(kNetworkConnectedMp3, sizeof(kNetworkConnectedMp3));
          break;
      }
        // 每100ms检查一次
      vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// 1. WiFi列表点击事件：自动填充SSID到输入框
void wifi_config::wifi_list_click_event(lv_event_t *e) {
   char *ssid = (char *)lv_event_get_user_data(e);
   if (ssid) {
        // 自动填充SSID到输入框
        lv_obj_add_flag(guider_ui.screen_wifi_list, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_wifi_form, LV_OBJ_FLAG_HIDDEN);
        lv_textarea_set_text(guider_ui.screen_wifi_wifi_name_value, ssid);
  }
}
//主屏加载后,自动连接wifi
void wifi_config::main_sreen_loaded_task(){
    // 创建信号量
    if(WiFi.status() == WL_CONNECTED){
       //String ipAddrss =  WiFi.localIP().toString();
       lv_image_set_src(page_head.wifi_img,&wifi_fill);
    }else{
       xTaskCreate([](void *arg){
        auto *p = static_cast<wifi_config*>(arg);
        auto& setting = Setting::GetInstance();
         std::pair<String, String>  params =  setting.getWifiConfig();
         if(!params.first.isEmpty() && !params.second.isEmpty()){
            p->wifi_connect(params.first.c_str(), params.second.c_str());
            if (WiFi.status() == WL_CONNECTED) {
              String ipAddrss =  WiFi.localIP().toString();
              lv_image_set_src(page_head.wifi_img,&wifi_fill);
              vTaskDelay(pdMS_TO_TICKS(200));
              wifi_udp_init();
            } else {
               lv_image_set_src(page_head.wifi_img,&no_wifi);
            }
        }
        // 退出任务
        vTaskDelete(NULL);
      }, "main_sreen_loaded_task", 1024*5,this, 2, NULL); 
    }
}

// =========== 扫描WiFi事件 ==========
void wifi_screen_load(SCREEN_LOAD_STAUS status) {
   // 2. 启动WiFi任务（关键！不卡UI）
  auto& wifiConfig = wifi_config::GetInstance(); 
  if(status == LOADED){
    wifiConfig.wifi_scan_start(); //开始扫描
  }else{
    lv_obj_clean(guider_ui.screen_wifi_list);
  }
}
// =========== wifi连接事件 ============
void connect_wifi()
{
   auto& wifiConfig = wifi_config::GetInstance(); 
   wifiConfig.wifi_connect_task(); //开始连接
}








