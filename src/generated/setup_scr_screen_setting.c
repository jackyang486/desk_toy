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



void setup_scr_screen_setting(lv_ui *ui)
{
    //Write codes screen_setting
    ui->screen_setting = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_setting, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_setting, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting, lv_color_hex(0xd1e8f9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_cont_light_switch
    ui->screen_setting_cont_light_switch = lv_obj_create(ui->screen_setting);
    lv_obj_set_pos(ui->screen_setting_cont_light_switch, 10, 228);
    lv_obj_set_size(ui->screen_setting_cont_light_switch, 220, 60);
    lv_obj_set_scrollbar_mode(ui->screen_setting_cont_light_switch, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting_cont_light_switch, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_cont_light_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_cont_light_switch, 22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_cont_light_switch, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_cont_light_switch, lv_color_hex(0x9966EE), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_cont_light_switch, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_cont_light_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_cont_light_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_cont_light_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_cont_light_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_cont_light_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_light_switch_title
    ui->screen_setting_label_light_switch_title = lv_label_create(ui->screen_setting_cont_light_switch);
    lv_obj_set_pos(ui->screen_setting_label_light_switch_title, 55, 10);
    lv_obj_set_size(ui->screen_setting_label_light_switch_title, 80, 24);
    lv_label_set_text(ui->screen_setting_label_light_switch_title, "");
    lv_label_set_long_mode(ui->screen_setting_label_light_switch_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_light_switch_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_light_switch_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_light_switch_title, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_light_switch_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_light_switch_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_light_switch_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_slider_switch
    ui->screen_setting_slider_switch = lv_slider_create(ui->screen_setting_cont_light_switch);
    lv_obj_set_pos(ui->screen_setting_slider_switch, 50, 42);
    lv_obj_set_size(ui->screen_setting_slider_switch, 156, 8);
    lv_slider_set_range(ui->screen_setting_slider_switch, 0, 60);
    lv_slider_set_mode(ui->screen_setting_slider_switch, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_setting_slider_switch, 50, LV_ANIM_OFF);

    //Write style for screen_setting_slider_switch, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_switch, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_switch, lv_color_hex(0x34ffa4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_switch, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_switch, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_setting_slider_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_slider_switch, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_switch, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_switch, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_switch, lv_color_hex(0x34ff98), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_switch, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_switch, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_switch, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_switch, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_switch, lv_color_hex(0x00ed5c), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_switch, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_switch, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_light_switch_img
    ui->screen_setting_label_light_switch_img = lv_label_create(ui->screen_setting_cont_light_switch);
    lv_obj_set_pos(ui->screen_setting_label_light_switch_img, 9, 7);
    lv_obj_set_size(ui->screen_setting_label_light_switch_img, 40, 40);
    lv_label_set_text(ui->screen_setting_label_light_switch_img, "" LV_SYMBOL_EYE_OPEN " ");
    lv_label_set_long_mode(ui->screen_setting_label_light_switch_img, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_light_switch_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_light_switch_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_light_switch_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_light_switch_img, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_light_switch_img, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_light_switch_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_light_switch_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_light_switch_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_light_switch_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_light_switch_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_light_switch_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_light_switch_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_light_switch_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_light_switch_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_light_switch_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_minute
    ui->screen_setting_label_minute = lv_label_create(ui->screen_setting_cont_light_switch);
    lv_obj_set_pos(ui->screen_setting_label_minute, 155, 10);
    lv_obj_set_size(ui->screen_setting_label_minute, 55, 24);
    lv_label_set_text(ui->screen_setting_label_minute, "");
    lv_label_set_long_mode(ui->screen_setting_label_minute, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_minute, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_minute, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_minute, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_minute, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_minute, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_minute, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_cont_volume
    ui->screen_setting_cont_volume = lv_obj_create(ui->screen_setting);
    lv_obj_set_pos(ui->screen_setting_cont_volume, 10, 56);
    lv_obj_set_size(ui->screen_setting_cont_volume, 220, 60);
    lv_obj_set_scrollbar_mode(ui->screen_setting_cont_volume, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting_cont_volume, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_cont_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_cont_volume, 22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_cont_volume, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_cont_volume, lv_color_hex(0x4896fb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_cont_volume, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_cont_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_cont_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_cont_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_cont_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_cont_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_vol_title
    ui->screen_setting_label_vol_title = lv_label_create(ui->screen_setting_cont_volume);
    lv_obj_set_pos(ui->screen_setting_label_vol_title, 55, 10);
    lv_obj_set_size(ui->screen_setting_label_vol_title, 80, 24);
    lv_label_set_text(ui->screen_setting_label_vol_title, "");
    lv_label_set_long_mode(ui->screen_setting_label_vol_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_vol_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_vol_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_vol_title, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_vol_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_vol_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_vol_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_slider_volume
    ui->screen_setting_slider_volume = lv_slider_create(ui->screen_setting_cont_volume);
    lv_obj_set_pos(ui->screen_setting_slider_volume, 50, 42);
    lv_obj_set_size(ui->screen_setting_slider_volume, 156, 8);
    lv_slider_set_range(ui->screen_setting_slider_volume, 0, 100);
    lv_slider_set_mode(ui->screen_setting_slider_volume, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_setting_slider_volume, 50, LV_ANIM_OFF);

    //Write style for screen_setting_slider_volume, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_volume, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_volume, lv_color_hex(0x34ffa4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_volume, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_volume, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_setting_slider_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_slider_volume, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_volume, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_volume, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_volume, lv_color_hex(0x34ff98), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_volume, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_volume, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_volume, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_volume, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_volume, lv_color_hex(0x00ed5c), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_volume, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_volume, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_vol_img
    ui->screen_setting_label_vol_img = lv_label_create(ui->screen_setting_cont_volume);
    lv_obj_set_pos(ui->screen_setting_label_vol_img, 9, 7);
    lv_obj_set_size(ui->screen_setting_label_vol_img, 40, 40);
    lv_label_set_text(ui->screen_setting_label_vol_img, "" LV_SYMBOL_VOLUME_MAX " ");
    lv_label_set_long_mode(ui->screen_setting_label_vol_img, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_vol_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_vol_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_vol_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_vol_img, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_vol_img, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_vol_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_vol_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_vol_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_vol_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_vol_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_vol_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_vol_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_vol_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_vol_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_vol_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_vol_percent
    ui->screen_setting_label_vol_percent = lv_label_create(ui->screen_setting_cont_volume);
    lv_obj_set_pos(ui->screen_setting_label_vol_percent, 154, 10);
    lv_obj_set_size(ui->screen_setting_label_vol_percent, 55, 24);
    lv_label_set_text(ui->screen_setting_label_vol_percent, "");
    lv_label_set_long_mode(ui->screen_setting_label_vol_percent, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_vol_percent, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_vol_percent, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_vol_percent, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_vol_percent, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_vol_percent, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_vol_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_cont_bright
    ui->screen_setting_cont_bright = lv_obj_create(ui->screen_setting);
    lv_obj_set_pos(ui->screen_setting_cont_bright, 10, 139);
    lv_obj_set_size(ui->screen_setting_cont_bright, 220, 60);
    lv_obj_set_scrollbar_mode(ui->screen_setting_cont_bright, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_setting_cont_bright, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_cont_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_cont_bright, 22, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_cont_bright, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_cont_bright, lv_color_hex(0xFF8833), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_cont_bright, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_cont_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_cont_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_cont_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_cont_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_cont_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_bright_title
    ui->screen_setting_label_bright_title = lv_label_create(ui->screen_setting_cont_bright);
    lv_obj_set_pos(ui->screen_setting_label_bright_title, 55, 10);
    lv_obj_set_size(ui->screen_setting_label_bright_title, 80, 24);
    lv_label_set_text(ui->screen_setting_label_bright_title, "");
    lv_label_set_long_mode(ui->screen_setting_label_bright_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_bright_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_bright_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_bright_title, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_bright_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_bright_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_bright_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_slider_bright
    ui->screen_setting_slider_bright = lv_slider_create(ui->screen_setting_cont_bright);
    lv_obj_set_pos(ui->screen_setting_slider_bright, 50, 42);
    lv_obj_set_size(ui->screen_setting_slider_bright, 156, 8);
    lv_slider_set_range(ui->screen_setting_slider_bright, 0, 100);
    lv_slider_set_mode(ui->screen_setting_slider_bright, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_setting_slider_bright, 50, LV_ANIM_OFF);

    //Write style for screen_setting_slider_bright, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_bright, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_bright, lv_color_hex(0x34ffa4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_bright, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_bright, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_setting_slider_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_slider_bright, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_bright, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_bright, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_bright, lv_color_hex(0x34ff98), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_bright, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_bright, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_setting_slider_bright, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_setting_slider_bright, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_setting_slider_bright, lv_color_hex(0x00ed5c), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_setting_slider_bright, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_slider_bright, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_bright_img
    ui->screen_setting_label_bright_img = lv_label_create(ui->screen_setting_cont_bright);
    lv_obj_set_pos(ui->screen_setting_label_bright_img, 9, 7);
    lv_obj_set_size(ui->screen_setting_label_bright_img, 40, 40);
    lv_label_set_text(ui->screen_setting_label_bright_img, "" LV_SYMBOL_CHARGE " ");
    lv_label_set_long_mode(ui->screen_setting_label_bright_img, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_bright_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_bright_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_bright_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_bright_img, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_bright_img, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_bright_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_bright_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_bright_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_bright_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_bright_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_bright_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_bright_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_bright_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_bright_img, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_bright_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_setting_label_bright_percent
    ui->screen_setting_label_bright_percent = lv_label_create(ui->screen_setting_cont_bright);
    lv_obj_set_pos(ui->screen_setting_label_bright_percent, 156, 11);
    lv_obj_set_size(ui->screen_setting_label_bright_percent, 55, 24);
    lv_label_set_text(ui->screen_setting_label_bright_percent, "");
    lv_label_set_long_mode(ui->screen_setting_label_bright_percent, LV_LABEL_LONG_WRAP);

    //Write style for screen_setting_label_bright_percent, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_setting_label_bright_percent, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_setting_label_bright_percent, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_setting_label_bright_percent, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_setting_label_bright_percent, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_setting_label_bright_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_setting.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_setting);

    //Init events for screen.
    events_init_screen_setting(ui);
}
