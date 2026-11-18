#include <Arduino.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>
#include "battery.h"
#include "battery_low_notice.h"
#include "src/generated/gui_guider.h"
#include "src/generated/events_init.h"
#include "src/custom/custom.h"      // Page_Head / page_head（顶栏电量图标）
#include "ESP32_board.h"

/*==================== 电量检测（参考 Example_13_Get_Battery_Voltage） ====================*/
// 电池经分压（×2）接到 ADC：ADC_UNIT_1 + ADC_CHANNEL_8（ESP32-S3 ADC1_CH8 = GPIO9）。
// 电压映射：<=2500mV -> 0%，2500~4200mV 线性 -> (v-2500)/17，>=4200mV -> 100%。
#define BAT_ADC_UNIT       ADC_UNIT_1
#define BAT_ADC_CHANNEL    ADC_CHANNEL_8
#define BAT_ADC_ATTEN      ADC_ATTEN_DB_12
#define BAT_ADC_BITWIDTH   ADC_BITWIDTH_12
#define BAT_LOW_PERCENT    20     // 低于该百分比：提示音 + 红色图标（只报一次）
#define BAT_REARM_PERCENT  30     // 回升到该百分比以上才重新武装报警（防临界抖动反复播报）

static adc_oneshot_unit_handle_t s_bat_adc_handle = NULL;
static adc_cali_handle_t s_bat_cali_handle = NULL;
static bool s_bat_adc_ok = false;
static volatile bool s_bat_low_notified = false;

static void battery_adc_init(void) {
  esp_err_t ret;
  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = BAT_ADC_UNIT,
  };
  ret = adc_oneshot_new_unit(&init_config, &s_bat_adc_handle);
  if (ret != ESP_OK) {
    printf("[BAT] adc unit init failed: %d\n", ret);
    return;
  }
  adc_oneshot_chan_cfg_t channel_config = {
      .atten = BAT_ADC_ATTEN,
      .bitwidth = BAT_ADC_BITWIDTH,
  };
  adc_oneshot_config_channel(s_bat_adc_handle, BAT_ADC_CHANNEL, &channel_config);
  adc_cali_curve_fitting_config_t cali_config = {
      .unit_id = BAT_ADC_UNIT,
      .atten = BAT_ADC_ATTEN,
      .bitwidth = BAT_ADC_BITWIDTH,
  };
  ret = adc_cali_create_scheme_curve_fitting(&cali_config, &s_bat_cali_handle);
  if (ret != ESP_OK) {
    printf("[BAT] adc cali create failed: %d\n", ret);
    return;
  }
  s_bat_adc_ok = true;
}

// 读电量百分比；失败返回 -1
static int battery_read_percent(void) {
  if (!s_bat_adc_ok) return -1;
  int raw = 0, mv = 0;
  if (adc_oneshot_read(s_bat_adc_handle, BAT_ADC_CHANNEL, &raw) != ESP_OK) return -1;
  if (adc_cali_raw_to_voltage(s_bat_cali_handle, raw, &mv) != ESP_OK) return -1;
  const int v = mv * 2;  // 考虑分压
  if (v <= 2500) return 0;
  if (v <= 4200) return (v - 2500) / 17;
  return 100;
}

// 低电提示音：单独任务里播（playMp3 同步解码播放，不能占着 LVGL 线程）
static void battery_notice_task(void *param) {
  (void)param;
  // 等切页动画/卸载回调完成（音乐/电台页停止播放释放 codec）
  vTaskDelay(pdMS_TO_TICKS(1000));
  board.playMp3(battery_low_notice, battery_low_notice_len);
  vTaskDelete(NULL);
}

// 电量检测定时器（LVGL 线程）：30s 一次，更新顶栏图标 + 低电播报
static void battery_check_cb(lv_timer_t *t) {
  (void)t;
  const int pct = battery_read_percent();
  if (pct < 0) return;

  // 顶栏电量图标（去重，避免每 30s 重排一次 label）
  const char *sym = pct > 80 ? LV_SYMBOL_BATTERY_FULL
                  : pct > 60 ? LV_SYMBOL_BATTERY_3
                  : pct > 40 ? LV_SYMBOL_BATTERY_2
                  : pct > 10 ? LV_SYMBOL_BATTERY_1
                             : LV_SYMBOL_BATTERY_EMPTY;
  static const char *last_sym = nullptr;
  if (sym != last_sym) {
    lv_label_set_text(page_head.battery_label, sym);
    lv_obj_set_style_text_color(page_head.battery_label,
                                pct <= BAT_LOW_PERCENT ? lv_color_hex(0xD00000)
                                                       : lv_color_hex(0x000000),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    last_sym = sym;
  }

  // 低电播报：一次性；若在音乐/电台页先回首页（卸载回调会停止播放、释放 codec）
  if (pct <= BAT_LOW_PERCENT && !s_bat_low_notified) {
    s_bat_low_notified = true;
    printf("[BAT] low battery %d%%, play notice\n", pct);
    const lv_obj_t *act = lv_screen_active();
    if (act == guider_ui.screen_music || act == guider_ui.screen_radio) {
      ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del,
                            act == guider_ui.screen_music ? &guider_ui.screen_music_del
                                                          : &guider_ui.screen_radio_del,
                            setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
    }
    xTaskCreate(battery_notice_task, "bat_notice", 4096, NULL, 1, NULL);
  } else if (pct > BAT_REARM_PERCENT) {
    s_bat_low_notified = false;  // 电量回升，重新武装
  }
}

void battery_init(void) {
  battery_adc_init();
  // 电量检测定时器：30s 一次（本函数须在 lvgl_user_init() 之后调用）
  lv_timer_create(battery_check_cb, 10000, NULL);
}
/*================== 电量检测结束 ==================*/
