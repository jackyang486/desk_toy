#ifndef __WIFI_CONF_H_
#define __WIFI_CONF_H_
#include "src/custom/custom.h"
#include "src/generated/gui_guider.h"

void wifi_screen_load(SCREEN_LOAD_STAUS status);
void main_sreen_loaded(SCREEN_LOAD_STAUS loadStatus);



class wifi_config{
  private:
    bool scan_finished;
    static void wifi_list_click_event(lv_event_t *e);
  public:
    SemaphoreHandle_t wifi_sem_ = nullptr;
    static wifi_config& GetInstance();
    void initial();
    void screen_load();
    void wifi_scan_start();
    void wifi_connect_task();
    void wifi_connect(const char* ssid, const char* pass);
    void main_sreen_loaded_task();
};
// 1. 定义任务参数结构体（放在类私有或 cpp 文件顶部）
struct WifiConnectParams {
    wifi_config* instance;      // 安全持有实例指针
    String ssid;
    String password;
};
#endif