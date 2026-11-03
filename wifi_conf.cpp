#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <freertos/semphr.h>
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

static constexpr uint32_t kWifiWorkerStack = 12 * 1024;
static constexpr uint32_t kWifiScanTaskStack = 4 * 1024;

wifi_config& wifi_config::GetInstance() {
    static wifi_config instance;
    return instance;
}

// ============ 事件位：工作线程置位、UI 线程取走，必须原子访问 ============
void wifi_config::ui_post(uint8_t bits) {
    __atomic_fetch_or(&ui_events_, bits, __ATOMIC_SEQ_CST);
}

// 只取走（并清零）mask 指定的位，其它位不受影响
uint8_t wifi_config::ui_take(uint8_t mask) {
    return (uint8_t)(__atomic_fetch_and(&ui_events_, (uint8_t)(~mask), __ATOMIC_SEQ_CST) & mask);
}

void wifi_config::ui_clear(uint8_t bits) {
    __atomic_fetch_and(&ui_events_, (uint8_t)(~bits), __ATOMIC_SEQ_CST);
}

// ============ 工作线程投递口 ============
void wifi_config::post_connected_state(bool connected) {
    ui_connected_ = connected;
    ui_post(kEvtHeadState);
}

void wifi_config::post_no_config_hint() { 
    ui_post(kEvtNoCfg);
}

void wifi_config::post_form_reset() {
    ui_post(kEvtForm);
}

// ============ 全局状态消费：由 main.cpp 的 time_update_cb（ui_task）驱动 ============
void wifi_config::head_ui_update() {
    const uint8_t evt = ui_take(kHeadEvtMask);
    if (evt == 0) return;

    if (evt & kEvtHeadState) {
        if (ui_connected_) {
            lv_image_set_src(page_head.wifi_img, &wifi_fill);
            lv_label_set_text(page_head.wifi_status_label, "on");
        } else {
            lv_image_set_src(page_head.wifi_img, &no_wifi);
            lv_obj_remove_state(page_head.wifi_switch, LV_STATE_CHECKED);
            lv_label_set_text(page_head.wifi_status_label, "off");
        }
    }
    if (evt & kEvtNoCfg) {
        show_toast(lv_scr_act(), "WIFI未配置!");
    }
    if (evt & kEvtForm) {
        lv_obj_add_flag(guider_ui.screen_wifi_form, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_wifi_list, LV_OBJ_FLAG_HIDDEN);
    }
}

// ============ WiFi 列表（页面级，UI 线程独占） ============
// board.GetWifiList() 的完成回调跑在 wifi_scan_task（非 UI 线程）。
// 原来直接在回调里 lv_obj_clean / lv_list_add_btn / lv_obj_add_event_cb，
// 和 ui_task 的 lv_timer_handler() 并发改同一棵对象树，会随机花屏或崩溃。
// 现在回调只深拷贝结果 + 置标志，列表由 UI 线程的定时器重建。
void wifi_config::list_clear() {
    if (guider_ui.screen_wifi_list != nullptr) {
        lv_obj_clean(guider_ui.screen_wifi_list);
    }
    for (char *s : ssid_cache_) free(s);
    ssid_cache_.clear();
}

// 列表点击：自动填充 SSID 到输入框
void wifi_config::list_click_cb(lv_event_t *e) {
    char *ssid = static_cast<char *>(lv_event_get_user_data(e));
    if (ssid == nullptr) return;
    lv_obj_add_flag(guider_ui.screen_wifi_list, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(guider_ui.screen_wifi_form, LV_OBJ_FLAG_HIDDEN);
    lv_textarea_set_text(guider_ui.screen_wifi_wifi_name_value, ssid);
}

void wifi_config::list_build(const std::vector<WifiInfo> &wifi_list) {
    list_clear();
    const bool connected = (WiFi.status() == WL_CONNECTED);
    const String cur_ssid = connected ? WiFi.SSID() : String();
    for (size_t i = 0; i < wifi_list.size(); i++) {
        char buf[64];
        lv_snprintf(buf, sizeof(buf), "%s(%d)", wifi_list[i].ssid.c_str(), (int)wifi_list[i].rssi);
        lv_obj_t *btn = lv_list_add_btn(guider_ui.screen_wifi_list, LV_SYMBOL_WIFI, buf);
        if (btn == nullptr) continue;
        lv_obj_t *btnText = lv_obj_get_child(btn, 1);
        if (btnText != nullptr) {
            lv_obj_set_style_text_font(btnText, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        char *ssid_copy = strdup(wifi_list[i].ssid.c_str());
        if (ssid_copy != nullptr) {
            ssid_cache_.push_back(ssid_copy);
            lv_obj_add_event_cb(btn, list_click_cb, LV_EVENT_CLICKED, ssid_copy);
        }
        if (connected && wifi_list[i].ssid == cur_ssid) {
            lv_obj_set_style_bg_color(btn, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

// 扫描线程：只搬数据，不碰界面
void wifi_config::on_scan_result(const std::vector<WifiInfo> &wifi_list) {
    if (list_mutex_ == nullptr) return;
    if (xSemaphoreTake(list_mutex_, pdMS_TO_TICKS(500)) != pdTRUE) {
        printf("[WIFI] scan result dropped (busy)\n");
        return;
    }
    list_pending_ = wifi_list;   // 深拷贝，String 各自持有堆内存
    xSemaphoreGive(list_mutex_);
    ui_post(kEvtScanList);
}

// UI 线程：消费扫描结果并重建列表
void wifi_config::list_timer_cb(lv_timer_t *t) {
    (void)t;
    wifi_config &self = GetInstance();
    if (self.ui_take(kEvtScanList) == 0) return;

    std::vector<WifiInfo> list;
    if (self.list_mutex_ != nullptr &&
        xSemaphoreTake(self.list_mutex_, pdMS_TO_TICKS(50)) == pdTRUE) {
        list.swap(self.list_pending_);
        xSemaphoreGive(self.list_mutex_);
    } else {
        self.ui_post(kEvtScanList);   // 下个周期再试
        return;
    }
    self.list_build(list);
}

void wifi_config::screen_list_enter() {
    if (list_timer_ == nullptr) {
        list_timer_ = lv_timer_create(list_timer_cb, 100, NULL);
    }
   static lv_style_t style_wifi_btn_text;
   static bool style_added = false;
   if (!style_added) {          // 每次进页面都 add 会重复挂载样式
       style_added = true;
       lv_style_init(&style_wifi_btn_text);
       lv_style_set_text_font(&style_wifi_btn_text, &font_puhui_16_4);
       lv_obj_add_style(guider_ui.screen_wifi_list, &style_wifi_btn_text, LV_PART_ITEMS | LV_STATE_DEFAULT);
   }
   printf("scan begin\n");
   xTaskCreate([](void* arg) {
       vTaskDelay(pdMS_TO_TICKS(200));   // 让页面切换动画先跑完，别占着 UI 线程
       board.GetWifiList();
       vTaskDelete(NULL);
    }, "wifi_scan_task", kWifiScanTaskStack, NULL, 3, NULL);
}

void wifi_config::screen_list_exit() {
    ui_clear(kEvtScanList);   // 丢弃过期扫描结果，避免刚清完又重建
    if (list_timer_ == nullptr) return;
    lv_timer_delete(list_timer_);
    list_timer_ = nullptr;       // 退出销毁
    list_clear();             // 清列表 + 释放 ssid 缓存
}

void wifi_config::initial(){
    if (inited_) return;
    inited_ = true;
    if (list_mutex_ == nullptr) {
        list_mutex_ = xSemaphoreCreateMutex();
    }
    //注册回调事件：这里只搬数据，界面由 UI 线程的定时器重建
    board.onWifiListScaned([this](const std::vector<WifiInfo>& wifi_list){
        on_scan_result(wifi_list);
    });
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
      // 切回 ui_task 再动 LVGL
      p->instance->post_form_reset();
      if(WiFi.status() == WL_CONNECTED){
         auto& setting = Setting::GetInstance();
         setting.saveWifiConfig(p->ssid.c_str(),p->password.c_str());
      }
      printf("[WIFI] connect task stack high water: %u bytes\n",
             (unsigned)(uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t)));
      delete p;
      // 退出任务
      vTaskDelete(NULL);
    }, "wifi_connect_task", kWifiWorkerStack, params, 3, NULL);
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

//主屏加载后,自动连接wifi
void start_connect_wifi(){
    // 创建信号量
    if(WiFi.status() == WL_CONNECTED){
       WiFi.disconnect();
       WiFi.mode(WIFI_OFF); 
    }else{
       auto& wifiConfig = wifi_config::GetInstance();
       if (wifiConfig.boot_busy()) {
          printf("[WIFI] connect already running, skip\n");
          return;
       }
       wifiConfig.set_boot_busy(true);
       // 顶栏状态由 wifi_head_ui_update()（挂在 time_update_cb 上）消费，这里不用建定时器
       BaseType_t ok = xTaskCreate([](void *arg){
        auto *p = static_cast<wifi_config*>(arg);
        auto& setting = Setting::GetInstance();
         std::pair<String, String>  params =  setting.getWifiConfig();
         if(!params.first.isEmpty() && !params.second.isEmpty()){
            p->wifi_connect(params.first.c_str(), params.second.c_str());
            const bool connected = (WiFi.status() == WL_CONNECTED);
            p->post_connected_state(connected);   // 交给 ui_task 改界面
            if (connected) {
              vTaskDelay(pdMS_TO_TICKS(200));
              wifi_udp_init();
            }
        }else{
          p->post_no_config_hint();
        }
        printf("[WIFI] boot task stack high water: %u bytes\n",
               (unsigned)(uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t)));
        p->set_boot_busy(false);
        // 退出任务
        vTaskDelete(NULL);
      }, "wifi_boot_task", kWifiWorkerStack, &wifiConfig, 2, NULL);
       if (ok != pdPASS) {
          wifiConfig.set_boot_busy(false);
          printf("[WIFI] failed to create boot task\n");
       }
    }
}

// =========== 扫描WiFi事件 ==========
void wifi_screen_load(SCREEN_LOAD_STAUS status) {
   // 2. 启动WiFi任务（关键！不卡UI）
  auto& wifiConfig = wifi_config::GetInstance(); 
   if(status == LOADED){
    wifiConfig.screen_list_enter();   // 进页面：起列表定时器 + 开始扫描
  }else{
    wifiConfig.screen_list_exit();    // 退出：停定时器 + 清列表
  }
}

// 供 main.cpp 的 time_update_cb 调用（ui_task 线程）
void wifi_head_ui_update(void) {
    wifi_config::GetInstance().head_ui_update();
}
// =========== wifi连接事件 ============
void connect_wifi()
{
   auto& wifiConfig = wifi_config::GetInstance(); 
   wifiConfig.wifi_connect_task(); //开始连接
}








