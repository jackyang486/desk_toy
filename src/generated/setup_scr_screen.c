/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "../custom/custom.h"



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_main
    ui->screen_main = lv_obj_create(ui->screen);
    lv_obj_set_pos(ui->screen_main, 0, 0);
    lv_obj_set_size(ui->screen_main, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_main, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_battery
    ui->screen_battery = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_battery, "" LV_SYMBOL_BATTERY_FULL "");
    lv_label_set_long_mode(ui->screen_battery, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_battery, 215, 3);
    lv_obj_set_size(ui->screen_battery, 20, 20);

    //Write style for screen_battery, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_battery, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_battery, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_battery, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_battery, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_battery, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_icon
    ui->screen_wifi_icon = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_wifi_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_wifi_icon, &_wifiEnbale_alpha_20x20);
    lv_img_set_pivot(ui->screen_wifi_icon, 50,50);
    lv_img_set_angle(ui->screen_wifi_icon, 0);
    lv_obj_set_pos(ui->screen_wifi_icon, 3, 3);
    lv_obj_set_size(ui->screen_wifi_icon, 20, 20);
    lv_obj_add_flag(ui->screen_wifi_icon, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_wifi_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_wifi_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_wifi_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_wifi_icon, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_no_wifi_icon
    ui->screen_no_wifi_icon = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_no_wifi_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_no_wifi_icon, &_NOWIFI_alpha_20x20);
    lv_img_set_pivot(ui->screen_no_wifi_icon, 50,50);
    lv_img_set_angle(ui->screen_no_wifi_icon, 0);
    lv_obj_set_pos(ui->screen_no_wifi_icon, 3, 3);
    lv_obj_set_size(ui->screen_no_wifi_icon, 20, 20);

    //Write style for screen_no_wifi_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_no_wifi_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_no_wifi_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_no_wifi_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_no_wifi_icon, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_ip_address
    ui->screen_ip_address = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_ip_address, "");
    lv_label_set_long_mode(ui->screen_ip_address, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_ip_address, 70, 3);
    lv_obj_set_size(ui->screen_ip_address, 120, 20);

    //Write style for screen_ip_address, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_ip_address, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_ip_address, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_ip_address, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_ip_address, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_ip_address, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_humidity
    ui->screen_img_humidity = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_humidity, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_humidity, &_humidity_24x24);
    lv_img_set_pivot(ui->screen_img_humidity, 50,50);
    lv_img_set_angle(ui->screen_img_humidity, 0);
    lv_obj_set_pos(ui->screen_img_humidity, 10, 243);
    lv_obj_set_size(ui->screen_img_humidity, 24, 24);

    //Write style for screen_img_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_humidity, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_temperature
    ui->screen_img_temperature = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_temperature, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_temperature, &_temperature_24x24);
    lv_img_set_pivot(ui->screen_img_temperature, 50,50);
    lv_img_set_angle(ui->screen_img_temperature, 0);
    lv_obj_set_pos(ui->screen_img_temperature, 10, 213);
    lv_obj_set_size(ui->screen_img_temperature, 24, 24);

    //Write style for screen_img_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_temperature, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_bar_temp
    ui->screen_bar_temp = lv_bar_create(ui->screen_main);
    lv_obj_set_style_anim_time(ui->screen_bar_temp, 1000, 0);
    lv_bar_set_mode(ui->screen_bar_temp, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_bar_temp, 0, 52);
    lv_bar_set_value(ui->screen_bar_temp, 25, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_bar_temp, 40, 222);
    lv_obj_set_size(ui->screen_bar_temp, 52, 6);

    //Write style for screen_bar_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bar_temp, 113, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_bar_temp, lv_color_hex(0x85e2a4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_bar_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bar_temp, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bar_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_bar_temp, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bar_temp, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_bar_temp, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_bar_temp, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bar_temp, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_bar_hum
    ui->screen_bar_hum = lv_bar_create(ui->screen_main);
    lv_obj_set_style_anim_time(ui->screen_bar_hum, 1000, 0);
    lv_bar_set_mode(ui->screen_bar_hum, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_bar_hum, 0, 100);
    lv_bar_set_value(ui->screen_bar_hum, 25, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_bar_hum, 40, 252);
    lv_obj_set_size(ui->screen_bar_hum, 52, 6);

    //Write style for screen_bar_hum, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bar_hum, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_bar_hum, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_bar_hum, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bar_hum, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_bar_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_bar_hum, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_bar_hum, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_bar_hum, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_bar_hum, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_bar_hum, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_label_temp
    ui->screen_label_temp = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_temp, "12℃");
    lv_label_set_long_mode(ui->screen_label_temp, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_temp, 95, 218);
    lv_obj_set_size(ui->screen_label_temp, 58, 24);

    //Write style for screen_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_temp, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_hum
    ui->screen_label_hum = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_hum, "80%");
    lv_label_set_long_mode(ui->screen_label_hum, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_hum, 95, 244);
    lv_obj_set_size(ui->screen_label_hum, 58, 24);

    //Write style for screen_label_hum, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_hum, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_hum, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_hum, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_hum, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_qulaity
    ui->screen_label_qulaity = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_qulaity, "优");
    lv_label_set_long_mode(ui->screen_label_qulaity, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_qulaity, 100, 38);
    lv_obj_set_size(ui->screen_label_qulaity, 56, 24);

    //Write style for screen_label_qulaity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_qulaity, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_qulaity, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_qulaity, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_qulaity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_qulaity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_qulaity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_label_qulaity, lv_color_hex(0x9CCA7F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_label_qulaity, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_qulaity, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_weather0
    ui->screen_img_weather0 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_weather0, LV_OBJ_FLAG_CLICKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_img_set_src(ui->screen_img_weather0, "D:\\guider-gui-workspace\\my_controller\\import\\image\\t0.jpg");
#else
    lv_img_set_src(ui->screen_img_weather0, "S:/image/t0.jpg");
#endif
    lv_img_set_pivot(ui->screen_img_weather0, 50,50);
    lv_img_set_angle(ui->screen_img_weather0, 0);
    lv_obj_set_pos(ui->screen_img_weather0, 170, 35);
    lv_obj_set_size(ui->screen_img_weather0, 60, 60);

    //Write style for screen_img_weather0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_weather0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_weather0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_weather0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_weather0, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_weather_banner
    ui->screen_label_weather_banner = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_weather_banner, "");
    lv_label_set_long_mode(ui->screen_label_weather_banner, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_weather_banner, 10, 65);
    lv_obj_set_size(ui->screen_label_weather_banner, 155, 30);

    //Write style for screen_label_weather_banner, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_weather_banner, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_weather_banner, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_weather_banner, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_weather_banner, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_city
    ui->screen_label_city = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_city, "");
    lv_label_set_long_mode(ui->screen_label_city, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_city, 5, 35);
    lv_obj_set_size(ui->screen_label_city, 94, 30);

    //Write style for screen_label_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_city, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_city, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_city, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_week
    ui->screen_label_week = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_week, "周一二三四五六日");
    lv_label_set_long_mode(ui->screen_label_week, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_week, 182, 170);
    lv_obj_set_size(ui->screen_label_week, 58, 30);

    //Write style for screen_label_week, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_week, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_week, &lv_font_SourceHanSerifSC_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_week, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_week, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_year_month
    ui->screen_label_year_month = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_year_month, "2026年05月20日");
    lv_label_set_long_mode(ui->screen_label_year_month, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_year_month, 5, 170);
    lv_obj_set_size(ui->screen_label_year_month, 175, 30);

    //Write style for screen_label_year_month, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_year_month, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_year_month, &lv_font_SourceHanSerifSC_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_year_month, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_year_month, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_9
    ui->screen_img_hour_9 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_9, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_9, &_W_3660_i9_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_9, 50,50);
    lv_img_set_angle(ui->screen_img_hour_9, 0);
    lv_obj_set_pos(ui->screen_img_hour_9, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_9, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_9, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_9, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_8
    ui->screen_img_hour_8 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_8, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_8, &_W_3660_i8_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_8, 50,50);
    lv_img_set_angle(ui->screen_img_hour_8, 0);
    lv_obj_set_pos(ui->screen_img_hour_8, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_8, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_8, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_8, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_7
    ui->screen_img_hour_7 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_7, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_7, &_W_3660_i7_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_7, 50,50);
    lv_img_set_angle(ui->screen_img_hour_7, 0);
    lv_obj_set_pos(ui->screen_img_hour_7, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_7, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_7, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_7, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_6
    ui->screen_img_hour_6 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_6, &_W_3660_i6_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_6, 50,50);
    lv_img_set_angle(ui->screen_img_hour_6, 0);
    lv_obj_set_pos(ui->screen_img_hour_6, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_6, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_6, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_5
    ui->screen_img_hour_5 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_5, &_W_3660_i5_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_5, 50,50);
    lv_img_set_angle(ui->screen_img_hour_5, 0);
    lv_obj_set_pos(ui->screen_img_hour_5, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_5, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_5, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_4
    ui->screen_img_hour_4 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_4, &_W_3660_i4_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_4, 50,50);
    lv_img_set_angle(ui->screen_img_hour_4, 0);
    lv_obj_set_pos(ui->screen_img_hour_4, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_4, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_4, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_3
    ui->screen_img_hour_3 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_3, &_W_3660_i3_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_3, 50,50);
    lv_img_set_angle(ui->screen_img_hour_3, 0);
    lv_obj_set_pos(ui->screen_img_hour_3, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_3, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_0
    ui->screen_img_hour_0 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_hour_0, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_0, &_W_3660_i0_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_0, 50,50);
    lv_img_set_angle(ui->screen_img_hour_0, 0);
    lv_obj_set_pos(ui->screen_img_hour_0, 70, 110);
    lv_obj_set_size(ui->screen_img_hour_0, 36, 60);
    lv_obj_add_flag(ui->screen_img_hour_0, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_hour_0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_0, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute9
    ui->screen_img_minute9 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute9, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute9, &_O_3660_i9_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute9, 50,50);
    lv_img_set_angle(ui->screen_img_minute9, 0);
    lv_obj_set_pos(ui->screen_img_minute9, 70, 110);
    lv_obj_set_size(ui->screen_img_minute9, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute9, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute9, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_8
    ui->screen_img_minute_8 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_8, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_8, &_O_3660_i8_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_8, 50,50);
    lv_img_set_angle(ui->screen_img_minute_8, 0);
    lv_obj_set_pos(ui->screen_img_minute_8, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_8, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_8, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_8, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_7
    ui->screen_img_minute_7 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_7, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_7, &_O_3660_i7_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_7, 50,50);
    lv_img_set_angle(ui->screen_img_minute_7, 0);
    lv_obj_set_pos(ui->screen_img_minute_7, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_7, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_7, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_7, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_6
    ui->screen_img_minute_6 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_6, &_O_3660_i6_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_6, 50,50);
    lv_img_set_angle(ui->screen_img_minute_6, 0);
    lv_obj_set_pos(ui->screen_img_minute_6, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_6, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_6, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_5
    ui->screen_img_minute_5 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_5, &_O_3660_i5_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_5, 50,50);
    lv_img_set_angle(ui->screen_img_minute_5, 0);
    lv_obj_set_pos(ui->screen_img_minute_5, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_5, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_5, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_4
    ui->screen_img_minute_4 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_4, &_O_3660_i4_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_4, 50,50);
    lv_img_set_angle(ui->screen_img_minute_4, 0);
    lv_obj_set_pos(ui->screen_img_minute_4, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_4, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_4, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_3
    ui->screen_img_minute_3 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_3, &_O_3660_i3_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_3, 50,50);
    lv_img_set_angle(ui->screen_img_minute_3, 0);
    lv_obj_set_pos(ui->screen_img_minute_3, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_3, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_0
    ui->screen_img_minute_0 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_minute_0, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_0, &_O_3660_i0_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_0, 50,50);
    lv_img_set_angle(ui->screen_img_minute_0, 0);
    lv_obj_set_pos(ui->screen_img_minute_0, 70, 110);
    lv_obj_set_size(ui->screen_img_minute_0, 36, 60);
    lv_obj_add_flag(ui->screen_img_minute_0, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_minute_0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_0, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_9
    ui->screen_img_second_9 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_9, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_9, &_W_1830_i9_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_9, 50,50);
    lv_img_set_angle(ui->screen_img_second_9, 0);
    lv_obj_set_pos(ui->screen_img_second_9, 70, 110);
    lv_obj_set_size(ui->screen_img_second_9, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_9, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_9, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_8
    ui->screen_img_second_8 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_8, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_8, &_W_1830_i8_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_8, 50,50);
    lv_img_set_angle(ui->screen_img_second_8, 0);
    lv_obj_set_pos(ui->screen_img_second_8, 70, 110);
    lv_obj_set_size(ui->screen_img_second_8, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_8, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_8, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_7
    ui->screen_img_second_7 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_7, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_7, &_W_1830_i7_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_7, 50,50);
    lv_img_set_angle(ui->screen_img_second_7, 0);
    lv_obj_set_pos(ui->screen_img_second_7, 70, 110);
    lv_obj_set_size(ui->screen_img_second_7, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_7, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_7, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_6
    ui->screen_img_second_6 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_6, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_6, &_W_1830_i6_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_6, 50,50);
    lv_img_set_angle(ui->screen_img_second_6, 0);
    lv_obj_set_pos(ui->screen_img_second_6, 70, 110);
    lv_obj_set_size(ui->screen_img_second_6, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_6, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_6, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_5
    ui->screen_img_second_5 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_5, &_W_1830_i5_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_5, 50,50);
    lv_img_set_angle(ui->screen_img_second_5, 0);
    lv_obj_set_pos(ui->screen_img_second_5, 70, 110);
    lv_obj_set_size(ui->screen_img_second_5, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_5, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_4
    ui->screen_img_second_4 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_4, &_W_1830_i4_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_4, 50,50);
    lv_img_set_angle(ui->screen_img_second_4, 0);
    lv_obj_set_pos(ui->screen_img_second_4, 70, 110);
    lv_obj_set_size(ui->screen_img_second_4, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_4, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_3
    ui->screen_img_second_3 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_3, &_W_1830_i3_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_3, 50,50);
    lv_img_set_angle(ui->screen_img_second_3, 0);
    lv_obj_set_pos(ui->screen_img_second_3, 70, 110);
    lv_obj_set_size(ui->screen_img_second_3, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_0
    ui->screen_img_second_0 = lv_img_create(ui->screen_main);
    lv_obj_add_flag(ui->screen_img_second_0, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_0, &_W_1830_i0_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_0, 50,50);
    lv_img_set_angle(ui->screen_img_second_0, 0);
    lv_obj_set_pos(ui->screen_img_second_0, 70, 110);
    lv_obj_set_size(ui->screen_img_second_0, 18, 30);
    lv_obj_add_flag(ui->screen_img_second_0, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_second_0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_0, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_second_1
    ui->screen_cont_second_1 = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_second_1, 215, 132);
    lv_obj_set_size(ui->screen_cont_second_1, 18, 30);
    lv_obj_set_scrollbar_mode(ui->screen_cont_second_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_second_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_second_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_second_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_second_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_1
    ui->screen_img_second_1 = lv_img_create(ui->screen_cont_second_1);
    lv_obj_add_flag(ui->screen_img_second_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_1, &_W_1830_i1_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_1, 50,50);
    lv_img_set_angle(ui->screen_img_second_1, 0);
    lv_obj_set_pos(ui->screen_img_second_1, 0, 0);
    lv_obj_set_size(ui->screen_img_second_1, 18, 30);

    //Write style for screen_img_second_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_ge
    ui->screen_img_second_ge = lv_img_create(ui->screen_cont_second_1);
    lv_obj_add_flag(ui->screen_img_second_ge, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_ge, &_W_1830_i0_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_ge, 50,50);
    lv_img_set_angle(ui->screen_img_second_ge, 0);
    lv_obj_set_pos(ui->screen_img_second_ge, 0, 0);
    lv_obj_set_size(ui->screen_img_second_ge, 18, 30);

    //Write style for screen_img_second_ge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_ge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_ge, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_second_2
    ui->screen_cont_second_2 = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_second_2, 195, 132);
    lv_obj_set_size(ui->screen_cont_second_2, 18, 30);
    lv_obj_set_scrollbar_mode(ui->screen_cont_second_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_second_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_second_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_second_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_second_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_2
    ui->screen_img_second_2 = lv_img_create(ui->screen_cont_second_2);
    lv_obj_add_flag(ui->screen_img_second_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_2, &_W_1830_i2_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_2, 50,50);
    lv_img_set_angle(ui->screen_img_second_2, 0);
    lv_obj_set_pos(ui->screen_img_second_2, 0, 0);
    lv_obj_set_size(ui->screen_img_second_2, 18, 30);

    //Write style for screen_img_second_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_second_shi
    ui->screen_img_second_shi = lv_img_create(ui->screen_cont_second_2);
    lv_obj_add_flag(ui->screen_img_second_shi, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_second_shi, &_W_1830_i0_alpha_18x30);
    lv_img_set_pivot(ui->screen_img_second_shi, 50,50);
    lv_img_set_angle(ui->screen_img_second_shi, 0);
    lv_obj_set_pos(ui->screen_img_second_shi, 0, 0);
    lv_obj_set_size(ui->screen_img_second_shi, 18, 30);

    //Write style for screen_img_second_shi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_second_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_second_shi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_second_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_second_shi, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_minute_1
    ui->screen_cont_minute_1 = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_minute_1, 145, 102);
    lv_obj_set_size(ui->screen_cont_minute_1, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_cont_minute_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_minute_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_minute_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_minute_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_minute_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_1
    ui->screen_img_minute_1 = lv_img_create(ui->screen_cont_minute_1);
    lv_obj_add_flag(ui->screen_img_minute_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_1, &_O_3660_i1_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_1, 50,50);
    lv_img_set_angle(ui->screen_img_minute_1, 0);
    lv_obj_set_pos(ui->screen_img_minute_1, 0, 0);
    lv_obj_set_size(ui->screen_img_minute_1, 36, 60);

    //Write style for screen_img_minute_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_ge
    ui->screen_img_minute_ge = lv_img_create(ui->screen_cont_minute_1);
    lv_obj_add_flag(ui->screen_img_minute_ge, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_ge, &_O_3660_i0_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_ge, 50,50);
    lv_img_set_angle(ui->screen_img_minute_ge, 0);
    lv_obj_set_pos(ui->screen_img_minute_ge, 0, 0);
    lv_obj_set_size(ui->screen_img_minute_ge, 36, 60);

    //Write style for screen_img_minute_ge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_ge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_ge, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_minute_2
    ui->screen_cont_minute_2 = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_minute_2, 105, 102);
    lv_obj_set_size(ui->screen_cont_minute_2, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_cont_minute_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_minute_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_minute_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_minute_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_minute_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_2
    ui->screen_img_minute_2 = lv_img_create(ui->screen_cont_minute_2);
    lv_obj_add_flag(ui->screen_img_minute_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_2, &_O_3660_i2_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_2, 50,50);
    lv_img_set_angle(ui->screen_img_minute_2, 0);
    lv_obj_set_pos(ui->screen_img_minute_2, 0, 0);
    lv_obj_set_size(ui->screen_img_minute_2, 36, 60);

    //Write style for screen_img_minute_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_minute_shi
    ui->screen_img_minute_shi = lv_img_create(ui->screen_cont_minute_2);
    lv_obj_add_flag(ui->screen_img_minute_shi, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_minute_shi, &_O_3660_i0_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_minute_shi, 50,50);
    lv_img_set_angle(ui->screen_img_minute_shi, 0);
    lv_obj_set_pos(ui->screen_img_minute_shi, 0, 0);
    lv_obj_set_size(ui->screen_img_minute_shi, 36, 60);

    //Write style for screen_img_minute_shi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_minute_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_minute_shi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_minute_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_minute_shi, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_hour_2
    ui->screen_cont_hour_2 = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_hour_2, 10, 102);
    lv_obj_set_size(ui->screen_cont_hour_2, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_cont_hour_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_hour_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_hour_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_hour_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_hour_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_2
    ui->screen_img_hour_2 = lv_img_create(ui->screen_cont_hour_2);
    lv_obj_add_flag(ui->screen_img_hour_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_2, &_W_3660_i2_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_2, 50,50);
    lv_img_set_angle(ui->screen_img_hour_2, 0);
    lv_obj_set_pos(ui->screen_img_hour_2, 0, 0);
    lv_obj_set_size(ui->screen_img_hour_2, 36, 60);

    //Write style for screen_img_hour_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_shi
    ui->screen_img_hour_shi = lv_img_create(ui->screen_cont_hour_2);
    lv_obj_add_flag(ui->screen_img_hour_shi, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_shi, &_W_3660_i0_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_shi, 50,50);
    lv_img_set_angle(ui->screen_img_hour_shi, 0);
    lv_obj_set_pos(ui->screen_img_hour_shi, 0, 0);
    lv_obj_set_size(ui->screen_img_hour_shi, 36, 60);

    //Write style for screen_img_hour_shi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_shi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_shi, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_hour_1
    ui->screen_cont_hour_1 = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_hour_1, 50, 102);
    lv_obj_set_size(ui->screen_cont_hour_1, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_cont_hour_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_hour_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_hour_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_hour_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_hour_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_1
    ui->screen_img_hour_1 = lv_img_create(ui->screen_cont_hour_1);
    lv_obj_add_flag(ui->screen_img_hour_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_1, &_W_3660_i1_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_1, 50,50);
    lv_img_set_angle(ui->screen_img_hour_1, 0);
    lv_obj_set_pos(ui->screen_img_hour_1, 0, 0);
    lv_obj_set_size(ui->screen_img_hour_1, 36, 60);

    //Write style for screen_img_hour_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_hour_ge
    ui->screen_img_hour_ge = lv_img_create(ui->screen_cont_hour_1);
    lv_obj_add_flag(ui->screen_img_hour_ge, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_hour_ge, &_W_3660_i0_alpha_36x60);
    lv_img_set_pivot(ui->screen_img_hour_ge, 50,50);
    lv_img_set_angle(ui->screen_img_hour_ge, 0);
    lv_obj_set_pos(ui->screen_img_hour_ge, 0, 0);
    lv_obj_set_size(ui->screen_img_hour_ge, 36, 60);

    //Write style for screen_img_hour_ge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_hour_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_hour_ge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_hour_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_hour_ge, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_animimg_1
    ui->screen_animimg_1 = lv_animimg_create(ui->screen_main);
    lv_animimg_set_src(ui->screen_animimg_1, (const void **) screen_animimg_1_imgs, 10);
    lv_animimg_set_duration(ui->screen_animimg_1, 37*10);
    lv_animimg_set_repeat_count(ui->screen_animimg_1, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(ui->screen_animimg_1);
    lv_obj_set_pos(ui->screen_animimg_1, 160, 205);
    lv_obj_set_size(ui->screen_animimg_1, 70, 70);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}
