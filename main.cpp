#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp32-hal-ledc.h>
#include <src/MY_FT6336U.h>
#include <Preferences.h>
#include <TimeLib.h>
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
#include "online_img.h"
#include <vector>

#define LV_LVGL_H_INCLUDE_SIMPLE
#define CONFIG_SPIRAM_SUPPORT 1
#define TOUCH_FT6336_INT 17
#define TOUCH_FT6336_RST 18
#define DRAW_BUF_SIZE (240 * 320)  



static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 320;
static lv_disp_draw_buf_t disp_buf;
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
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    my_lcd.startWrite();
    my_lcd.setAddrWindow(area->x1, area->y1,  w, h);
    my_lcd.pushColors((uint16_t *)&color_p->full, w*h, true);
    my_lcd.endWrite();
    lv_disp_flush_ready(disp);
}
/* Reading input device (capacitive touch here) */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
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


void lvgl_user_init(void)
{
    my_lcd.init();
    my_lcd.fillScreen(0xFFFF);
    my_lcd.setRotation(0); 

    ft6336u.begin();// 初始化电容触摸
    lv_init();
    delay(10);

    buf1 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
    buf2 = (lv_color_t *)heap_caps_malloc(DRAW_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);

    lv_disp_draw_buf_init(&disp_buf, buf1,buf2, DRAW_BUF_SIZE);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = my_lcd.width();
    disp_drv.ver_res = my_lcd.height();
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /**srceen load change */
    
    lv_screen_param.main_screen_load = main_sreen_loaded;
    lv_screen_param.wifi_screen_load = wifi_screen_load;
    lv_screen_param.photo_screen_load = photo_win_loaded;
    lv_screen_param.robot_screen_load = robot_win_load;
    lv_screen_param.music_screen_load = music_player_load;
    lv_screen_param.setting_screen_load = setting_win_load;
    lv_screen_param.radio_screen_load = radio_win_load;
    lv_screen_param.img_screen_load = online_img_win_load;
    screen_load_register(&lv_screen_param);

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);    
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
}
time_t prevDisplay = 0; // 显示时间
void loop() {
   lv_timer_handler(); // v8 和 v9 都使用这个
   delay(5);
   if(time_enable){
    if(now() != prevDisplay) {
        prevDisplay = now();
        digitalClockDisplay(prevDisplay); 
    }
  }
  uint32_t timeout = Setting::GetInstance().GetScreenTimeout();
  if (screen_on && timeout > 0 && (millis() - last_touch_time > timeout)) {
      my_lcd.writecommand(TFT_DISPOFF);
      ledcWrite(TFT_BL_PIN, 0);
      screen_on = false;
  }
}
