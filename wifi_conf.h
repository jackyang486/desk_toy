#ifndef __WIFI_CONF_H_
#define __WIFI_CONF_H_
#include <vector>
#include "ESP32_board.h"        // WifiInfo（必须先于 custom.h 引入，避免被其 extern "C" 包住）
#include "src/custom/custom.h"
#include "src/generated/gui_guider.h"

void wifi_screen_load(SCREEN_LOAD_STAUS status);
void main_sreen_loaded(SCREEN_LOAD_STAUS loadStatus);
// 定义在 main.cpp：创建 NTP 同步任务并完成 setTime()。WiFi 连上后调用。
void wifi_udp_init(void);
// 把工作线程投递过来的 WiFi 全局状态（顶栏图标/文字、提示、表单复位）刷到界面。
// 只允许在 ui_task 线程调用；由 main.cpp 的 time_update_cb 周期驱动。
void wifi_head_ui_update(void);

class wifi_config{
  private:
    // ---- 工作线程 → UI 线程的事件位 ----
    enum : uint8_t {
        kEvtHeadState = 1u << 0,   // WiFi 图标/状态文字（全局）
        kEvtNoCfg     = 1u << 1,   // "WIFI未配置!" 提示（全局）
        kEvtForm      = 1u << 2,   // 配置页切回列表（wifi 页内，走全局通道避免丢事件）
        kEvtScanList  = 1u << 3,   // 扫描结果就绪，请 UI 线程重建列表（页面级）
    };
    static constexpr uint8_t kHeadEvtMask = kEvtHeadState | kEvtNoCfg | kEvtForm;

    bool scan_finished = false;
    bool inited_ = false;
    uint8_t ui_events_ = 0;            // 只经由 ui_post()/ui_take() 原子访问
    bool ui_connected_ = false;
    volatile bool boot_busy_ = false;  // 防止连点开关起两个任务

    lv_timer_t *list_timer_ = nullptr; // 列表渲染定时器，跟随 wifi 页面
    SemaphoreHandle_t list_mutex_ = nullptr;
    std::vector<WifiInfo> list_pending_;   // 扫描线程写入，UI 线程取走
    std::vector<char *> ssid_cache_;       // 列表按钮的 user_data，重建/清空时统一释放

    // 事件位原子操作
    void ui_post(uint8_t bits);
    uint8_t ui_take(uint8_t mask);
    void ui_clear(uint8_t bits);

    // WiFi 列表（除 on_scan_result 外都只能在 UI 线程调用）
    void list_clear();
    void list_build(const std::vector<WifiInfo> &wifi_list);
    void on_scan_result(const std::vector<WifiInfo> &wifi_list);  // 跑在 wifi_scan_task
    static void list_timer_cb(lv_timer_t *t);
    static void list_click_cb(lv_event_t *e);

  public:
    static wifi_config& GetInstance();
    void initial();
    void wifi_connect(const char* ssid, const char* pass);
    void wifi_connect_task();

    // ---- 工作线程调用：只投递，不碰 LVGL ----
    void post_connected_state(bool connected);
    void post_no_config_hint();
    void post_form_reset();
    bool boot_busy() const { return boot_busy_; }
    void set_boot_busy(bool busy) { boot_busy_ = busy; }

    // ---- UI 线程调用 ----
    void head_ui_update();      // 顶栏 WiFi 图标/文字、提示、表单复位
    void screen_list_enter();   // 进入 wifi 页面：起列表定时器
    void screen_list_exit();    // 退出 wifi 页面：停定时器 + 清列表
};
// 1. 定义任务参数结构体（放在类私有或 cpp 文件顶部）
struct WifiConnectParams {
    wifi_config* instance;      // 安全持有实例指针
    String ssid;
    String password;
};
#endif
