#include <Arduino.h>
#define LV_CONF_INCLUDE_SIMPLE
#include <TFT_eSPI.h>
#include <esp32-hal-ledc.h>
#include <src/MY_FT6336U.h>
#include <Preferences.h>
#include <TimeLib.h>
#include <esp_heap_caps.h>
#include <esp32-hal-psram.h>
#include "src/generated/gui_guider.h"
#include "src/generated/events_init.h"
#include "src/custom/custom.h"
#include "photo_album.h"
#include "music.h"
#include "chat_robot.h"
#include "wifi_conf.h"
#include "weather.h"
#include "ESP32_board.h"
#include "setting.h"
#include "radio.h"
#include "lottie.h"
#include "game.h"
#include <TimeLib.h>
#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>

/*==================== NTP 时间同步（已从 weather.cpp 迁移到此处） ====================*/
// NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
static const int timeZone = 8;             // 东八区
#define NTP_SYNC_INTERVAL_SEC  300         // 同步周期（秒）
#define NTP_SYNC_TASK_STACK    4096
#define NTP_PACKET_SIZE        48          // NTP 时间位于消息前 48 字节
#define NTP_LOCAL_PORT         8000

static WiFiUDP ntpUdp;
static byte ntpPacketBuffer[NTP_PACKET_SIZE];  // 收发 NTP 报文的缓冲
static TaskHandle_t ntp_task_handle = NULL;
static SemaphoreHandle_t ntp_udp_mutex = NULL; // 保护 ntpUdp，避免多任务并发
static bool ntp_task_started = false;
static volatile bool ntp_time_ready = false;   // 是否已成功同步过一次

static void sendNTPpacket(IPAddress &address);
static time_t getNtpTime(void);

bool ntp_is_time_ready(void) { return ntp_time_ready; }

// 向 NTP 服务器发送请求
static void sendNTPpacket(IPAddress &address)
{
  memset(ntpPacketBuffer, 0, NTP_PACKET_SIZE);
  ntpPacketBuffer[0]  = 0b11100011;  // LI, Version, Mode
  ntpPacketBuffer[1]  = 0;           // Stratum, or type of clock
  ntpPacketBuffer[2]  = 6;           // Polling Interval
  ntpPacketBuffer[3]  = 0xEC;        // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  ntpPacketBuffer[12] = 49;
  ntpPacketBuffer[13] = 0x4E;
  ntpPacketBuffer[14] = 49;
  ntpPacketBuffer[15] = 52;
  ntpUdp.beginPacket(address, 123);  // NTP requests are to port 123
  ntpUdp.write(ntpPacketBuffer, NTP_PACKET_SIZE);
  ntpUdp.endPacket();
}

static time_t getNtpTime(void)
{
  IPAddress ntpServerIP;

  // 原来是 "while (Udp.parsePacket() > 0);" —— 无超时、不让出 CPU，
  // 一旦 UDP 状态异常就会变成死循环。这里加上时间上限并让出 CPU。
  uint32_t drainStart = millis();
  while (ntpUdp.parsePacket() > 0 && (millis() - drainStart < 100)) {
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  WiFi.hostByName(ntpServerName, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    vTaskDelay(pdMS_TO_TICKS(2));   // 原来是完全忙等，会独占一个核
    int size = ntpUdp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      ntpUdp.read(ntpPacketBuffer, NTP_PACKET_SIZE);
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900  =  (unsigned long)ntpPacketBuffer[40] << 24;
      secsSince1900 |= (unsigned long)ntpPacketBuffer[41] << 16;
      secsSince1900 |= (unsigned long)ntpPacketBuffer[42] << 8;
      secsSince1900 |= (unsigned long)ntpPacketBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0;  // 无法获取时间时返回 0
}

static void ntp_sync_task(void *param) {
  (void)param;
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      if (ntp_udp_mutex && xSemaphoreTake(ntp_udp_mutex, pdMS_TO_TICKS(2000)) == pdTRUE) {
        time_t t = getNtpTime();
        xSemaphoreGive(ntp_udp_mutex);
        if (t != 0) {
          setTime(t);
          ntp_time_ready = true;
          Serial.println("NTP time synced");
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(NTP_SYNC_INTERVAL_SEC * 1000));
  }
}

// WiFi 连上后由 wifi_conf.cpp 调用
void wifi_udp_init(void) {
  ntpUdp.begin(NTP_LOCAL_PORT);
  // 不使用 setSyncProvider：now() 一旦注册了 provider，就可能在任意调用点
  // （包括 Arduino loop()）触发 getNtpTime()，而该函数内含阻塞式 DNS + 最长 1.5s
  // 等待，会直接把调用线程卡死。改为后台任务定时同步 + setTime()，
  // 让 now() 退化为纯本地计算，永不阻塞。
  if (!ntp_task_started) {
    ntp_task_started = true;
    if (ntp_udp_mutex == NULL) ntp_udp_mutex = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(
        ntp_sync_task,
        "ntp_sync_task",
        NTP_SYNC_TASK_STACK,
        NULL,
        2,
        &ntp_task_handle,
        tskNO_AFFINITY);
  }
}
/*================== NTP 结束 ==================*/


#define LV_LVGL_H_INCLUDE_SIMPLE
#define TOUCH_FT6336_INT 17
#define TOUCH_FT6336_RST 18
#define DRAW_BUF_SIZE (240 * 320)  

static lv_display_t *display = NULL;
static lv_color_t *buf1 = NULL;
static lv_color_t *buf2 = NULL;

// 顶栏/天气页时间刷新定时器回调（定义在文件后部）
static void time_update_cb(lv_timer_t *t);

lv_ui guider_ui;
screen_load lv_screen_param;
/*------------ 液晶屏对象 ------------*/
TFT_eSPI my_lcd = TFT_eSPI();
/*------------ 触摸驱动对象 ------------*/
FT6336U ft6336u(TOUCH_FT6336_RST, TOUCH_FT6336_INT);
FT6336U_TouchPointType tp;

#define SCREEN_TIMEOUT_MS 60000
static unsigned long last_touch_time = 0;
static bool screen_on = true;

extern "C" {
    void lv_mem_init(void)
    {
        // PSRAM allocator requires no initialization.
    }

    void* lv_malloc_core(size_t size)
    {
        return ps_malloc(size);
    }

    void lv_free_core(void* ptr)
    {
        heap_caps_free(ptr);
    }

    void* lv_realloc_core(void* ptr, size_t new_size)
    {
        return ps_realloc(ptr, new_size);
    }
}

/* Display flushing */
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    my_lcd.startWrite();
    my_lcd.setAddrWindow(area->x1, area->y1,  w, h);
    my_lcd.pushColors(reinterpret_cast<uint16_t *>(color_p), w*h, true);
    my_lcd.endWrite();
    lv_display_flush_ready(disp);
}
/* Reading input device (capacitive touch here) */
void my_touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
    (void)indev_driver;
    tp = ft6336u.scan(); 
    /* 判断屏幕是否被按下 */
    bool touched = tp.touch_count;
    if (touched) {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = tp.tp[0].x;
        data->point.y = tp.tp[0].y;
        last_touch_time = millis();
        if (!screen_on) {
            my_lcd.writecommand(TFT_DISPON);
            ledcWrite(TFT_BL_PIN, Setting::GetInstance().GetScreenBrightness());
            screen_on = true;
        }
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

static uint32_t lvgl_tick_get(void)
{
    return static_cast<uint32_t>(millis());
}

void lvgl_user_init(void)
{
    my_lcd.init();
    my_lcd.fillScreen(0xFFFF);
    my_lcd.setRotation(0); 

    ft6336u.begin();// 初始化电容触摸
    lv_init();
    lv_tick_set_cb(lvgl_tick_get);
    delay(10);

    const size_t draw_buf_bytes = DRAW_BUF_SIZE * sizeof(lv_color_t);
    buf1 = static_cast<lv_color_t*>(heap_caps_malloc(draw_buf_bytes, MALLOC_CAP_SPIRAM)); //| MALLOC_CAP_DMA
    buf2 = static_cast<lv_color_t*>(heap_caps_malloc(draw_buf_bytes, MALLOC_CAP_SPIRAM));
    if (buf1 == nullptr || buf2 == nullptr) {
        printf("[MEM] LVGL draw buffer allocation failed (buf1=%p, buf2=%p)\n",
               static_cast<void*>(buf1), static_cast<void*>(buf2));
        heap_caps_free(buf1);
        heap_caps_free(buf2);
        abort();
    }

    display = lv_display_create(my_lcd.width(), my_lcd.height());
    if (display == nullptr) {
        printf("[LVGL] Display allocation failed\n");
        abort();
    }
    lv_display_set_flush_cb(display, my_disp_flush);
    lv_display_set_buffers(display,buf1,buf2,DRAW_BUF_SIZE * sizeof(lv_color_t),LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    if (indev == nullptr) {
        printf("[LVGL] Input device allocation failed\n");
        abort();
    }
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    /**srceen load change */
    
    lv_screen_param.main_screen_load = main_sreen_loaded;
    lv_screen_param.wifi_screen_load = wifi_screen_load;
    lv_screen_param.photo_screen_load = photo_win_loaded;
    lv_screen_param.robot_screen_load = robot_win_load;
    //lv_screen_param.fly_mouse_screen_load = fly_mouse_win_load;
    lv_screen_param.music_screen_load = music_player_load;
    lv_screen_param.setting_screen_load = setting_win_load;
    lv_screen_param.radio_screen_load = radio_win_load;
    lv_screen_param.lottie_screen_load = lottie_page_load;
    lv_screen_param.weather_screen_load = weather_screen_load;
    lv_screen_param.game_screen_load = game_screen_load;
    screen_load_register(&lv_screen_param);

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);

    // 顶栏时间交给 LVGL 定时器（运行在 ui_task 线程），1s 触发一次
    lv_timer_create(time_update_cb, 1000, NULL);
}

static void ui_task(void *parameter);
static TaskHandle_t ui_task_handle = NULL;   // 用于观测 ui_task 栈水位，便于把栈调到合适大小

//=========== 主屏加载完成时间 ==========
void main_sreen_loaded(SCREEN_LOAD_STAUS loadStatus){
   if(loadStatus == LOADED){
    auto& wifiConfig = wifi_config::GetInstance(); 
    wifiConfig.main_sreen_loaded_task(); 
    printf("Free INTERNAL heap: %lu\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
    printf("Largest INTERNAL free block: %lu\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT));
   }else{
    
   }
}

void setup() {
  Serial.begin(115200);
  lvgl_user_init();    //lvgl初始化
  board.board_init();  //ESP32S3硬件初始化
  auto& wifiConfig = wifi_config::GetInstance(); 
  wifiConfig.initial(); 
  ft6336u.begin(board.get_I2C_master_bus_handle());
  Setting::GetInstance().Init();
  Radio::GetInstance().Init();

  const BaseType_t task_result = xTaskCreatePinnedToCore(
      ui_task,
      "uiTask",
      20 * 1024,
      nullptr,
      1,
      &ui_task_handle,
      1);
  if (task_result != pdPASS) {
      printf("[TASK] Failed to create UI task\n");
      abort();
  }
}

// ============ LVGL 心跳归属交接 ============
// lottie 动画（rlottie 解析 + 逐帧渲染）需要很大的栈，如果常驻在 ui_task 上，
// 就必须把 ui_task 的栈一直开到很大，白白占用 internal DRAM。
// 现在的做法：进入 lottie 页面时，由 lottie.cpp 创建一个专用任务接管 lv_timer_handler()，
// ui_task 在此期间完全不进入 LVGL；退出页面时该任务自行退出并把心跳交还 ui_task。
// 这样"大栈"只在 lottie 页面存在，退出后 40KB 栈连同 TCB 一起还给堆。
volatile bool g_lvgl_external_active  = false;  // true：外部任务正在驱动 lv_timer_handler()

static void ui_task(void *parameter) {
    (void)parameter;
    uint32_t last_resource_log = 0;
    while(1) {
        if (!g_lvgl_external_active) {
            lv_timer_handler();
        }
        vTaskDelay(pdMS_TO_TICKS(5));  
        game_process_pending();
    }
}

// 顶栏时间刷新：运行在 LVGL 定时器里（即 ui_task 线程），
// 避免在 Arduino loop() 线程里跨线程调用 lv_label_set_text()。
static void time_update_cb(lv_timer_t *t) {
    (void)t;
    // NTP 还没同步成功时不刷新，避免顶栏先闪一个 1970 的开机秒数
    if (!ntp_time_ready) return;
    time_t n = now();
    if (lv_screen_active() == guider_ui.screen_weather) {
        digitalClockDisplay(n);   // 天气页大表盘，内部自行按 时/分/秒 去重
        return;
    }
    // 自己维护去重，不再复用 weather.cpp 的 Hour_sign / Minute_sign，
    // 避免天气页把它们同步成当前时间后导致顶栏要等整分钟才刷新。
    static int last_h = -1;
    static int last_m = -1;
    const int h = hour(n);
    const int m = minute(n);
    if (h == last_h && m == last_m) return;
    last_h = h;
    last_m = m;

    char time_buff[16];
    snprintf(time_buff, sizeof(time_buff), "%02d:%02d", h, m);
    lv_label_set_text(page_head.time_label, time_buff);
}

// 设为 1 可周期性打印 ui_task 栈剩余量：lottie 交给独立任务后，
// 可据此把 setup() 里 ui_task 的 20*1024 调到一个安全的小值
#define UI_STACK_DEBUG 1

void loop() {
#if UI_STACK_DEBUG
    static uint32_t last_stack_log = 0;
    if (ui_task_handle != NULL && (millis() - last_stack_log > 5000)) {
        last_stack_log = millis();
        Serial.printf("[loop] uiTask stack free=%lu bytes\n",
                      (unsigned long)(uxTaskGetStackHighWaterMark(ui_task_handle) * sizeof(StackType_t)));
    }
#endif
      static bool game_was_active = false;
      if (game_is_active()) {
          game_was_active = true;
          delay(5);
          return;
      }
      if (game_was_active) {
          last_touch_time = millis();
          game_was_active = false;
      }
    const uint32_t timeout = Setting::GetInstance().GetScreenTimeout();
    if (screen_on && timeout > 0 && (millis() - last_touch_time > timeout)) {
            my_lcd.writecommand(TFT_DISPOFF);
            ledcWrite(TFT_BL_PIN, 0);
            screen_on = false;
    }
}
