#include <Arduino.h>
#define LV_CONF_INCLUDE_SIMPLE
#include <TFT_eSPI.h>
#include <esp32-hal-ledc.h>
#include <src/MY_FT6336U.h>
#include <Preferences.h>
#include <TimeLib.h>
#include <esp_heap_caps.h>
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
//#include "fly_mouse.h"
#include <vector>

#define LV_LVGL_H_INCLUDE_SIMPLE
#define TOUCH_FT6336_INT 17
#define TOUCH_FT6336_RST 18
#define DRAW_BUF_SIZE (240 * 320)  



static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 320;
static lv_display_t *display = NULL;
static lv_color_t *buf1 = NULL;
static lv_color_t *buf2 = NULL;

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
    printf("[MEM] PSRAM total=%u free=%u, internal free=%u, largest DMA block=%u, draw buffer=%u each\n",
           static_cast<unsigned>(ESP.getPsramSize()),
           static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)),
           static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)),
           static_cast<unsigned>(heap_caps_get_largest_free_block(MALLOC_CAP_DMA)),
           static_cast<unsigned>(draw_buf_bytes));

    buf1 = static_cast<lv_color_t*>(heap_caps_malloc(
        draw_buf_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA));
    buf2 = static_cast<lv_color_t*>(heap_caps_malloc(
        draw_buf_bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA));
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
    lv_display_set_buffers(
        display,
        buf1,
        buf2,
        DRAW_BUF_SIZE * sizeof(lv_color_t),
        LV_DISPLAY_RENDER_MODE_PARTIAL);

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
    screen_load_register(&lv_screen_param);

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);    
}

static void ui_task(void *parameter);

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
      nullptr,
      1);
  if (task_result != pdPASS) {
      printf("[TASK] Failed to create UI task\n");
      abort();
  }
}
time_t prevDisplay = 0; // 显示时间
static void ui_task(void *parameter) {
    (void)parameter;
    uint32_t last_resource_log = 0;
    for (;;) {
        lv_timer_handler();
        delay(5);
        if (time_enable && now() != prevDisplay) {
            prevDisplay = now();
            digitalClockDisplay(prevDisplay);
        }
        const uint32_t timeout = Setting::GetInstance().GetScreenTimeout();
        if (screen_on && timeout > 0 && (millis() - last_touch_time > timeout)) {
            my_lcd.writecommand(TFT_DISPOFF);
            ledcWrite(TFT_BL_PIN, 0);
            screen_on = false;
        }
    }
}

void loop() {
    delay(1);
}
