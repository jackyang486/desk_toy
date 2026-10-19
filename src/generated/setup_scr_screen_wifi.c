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



void setup_scr_screen_wifi(lv_ui *ui)
{
    //Write codes screen_wifi
    ui->screen_wifi = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_wifi, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_wifi, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_list_title
    ui->screen_wifi_list_title = lv_label_create(ui->screen_wifi);
    lv_obj_set_pos(ui->screen_wifi_list_title, 0, 0);
    lv_obj_set_size(ui->screen_wifi_list_title, 240, 20);
    lv_label_set_text(ui->screen_wifi_list_title, "" LV_SYMBOL_LIST "  wifi list");
    lv_label_set_long_mode(ui->screen_wifi_list_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_wifi_list_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_list_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_list_title, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_list_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_list_title, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_list_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_list_title, lv_color_hex(0xdbd9d9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_list_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_list
    ui->screen_wifi_list = lv_list_create(ui->screen_wifi);
    lv_obj_set_pos(ui->screen_wifi_list, 0, 20);
    lv_obj_set_size(ui->screen_wifi_list, 240, 300);
    lv_obj_set_scrollbar_mode(ui->screen_wifi_list, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_main_main_default
    static lv_style_t style_screen_wifi_list_main_main_default;
    ui_init_style(&style_screen_wifi_list_main_main_default);

    lv_style_set_pad_top(&style_screen_wifi_list_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_wifi_list_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_wifi_list_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_wifi_list_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_wifi_list_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_wifi_list_main_main_default, 0);
    lv_style_set_radius(&style_screen_wifi_list_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_wifi_list_main_main_default, 0);
    lv_obj_add_style(ui->screen_wifi_list, &style_screen_wifi_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_main_scrollbar_default
    static lv_style_t style_screen_wifi_list_main_scrollbar_default;
    ui_init_style(&style_screen_wifi_list_main_scrollbar_default);

    lv_style_set_radius(&style_screen_wifi_list_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_wifi_list_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_wifi_list, &style_screen_wifi_list_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_extra_btns_main_default
    static lv_style_t style_screen_wifi_list_extra_btns_main_default;
    ui_init_style(&style_screen_wifi_list_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_wifi_list_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_wifi_list_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_wifi_list_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_wifi_list_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_wifi_list_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_wifi_list_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_wifi_list_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_wifi_list_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_wifi_list_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_wifi_list_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_wifi_list_extra_texts_main_default
    static lv_style_t style_screen_wifi_list_extra_texts_main_default;
    ui_init_style(&style_screen_wifi_list_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_wifi_list_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_wifi_list_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_wifi_list_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_wifi_list_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_wifi_list_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_wifi_list_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_wifi_list_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_wifi_list_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_wifi_list_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_wifi_list_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_wifi_list_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_wifi_list_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_wifi_list_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes screen_wifi_form
    ui->screen_wifi_form = lv_obj_create(ui->screen_wifi);
    lv_obj_set_pos(ui->screen_wifi_form, 10, 100);
    lv_obj_set_size(ui->screen_wifi_form, 220, 100);
    lv_obj_set_scrollbar_mode(ui->screen_wifi_form, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->screen_wifi_form, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_wifi_form, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_form, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_wifi_form, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_wifi_form, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_wifi_form, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_form, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_form, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_form, lv_color_hex(0xdbdce0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_form, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_wifi_name_label
    ui->screen_wifi_wifi_name_label = lv_label_create(ui->screen_wifi_form);
    lv_obj_set_pos(ui->screen_wifi_wifi_name_label, 5, 5);
    lv_obj_set_size(ui->screen_wifi_wifi_name_label, 78, 25);
    lv_label_set_text(ui->screen_wifi_wifi_name_label, "wifi名称：");
    lv_label_set_long_mode(ui->screen_wifi_wifi_name_label, LV_LABEL_LONG_WRAP);

    //Write style for screen_wifi_wifi_name_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_wifi_name_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_wifi_name_label, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_wifi_name_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_wifi_name_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_wifi_pass_label
    ui->screen_wifi_wifi_pass_label = lv_label_create(ui->screen_wifi_form);
    lv_obj_set_pos(ui->screen_wifi_wifi_pass_label, 5, 32);
    lv_obj_set_size(ui->screen_wifi_wifi_pass_label, 78, 25);
    lv_label_set_text(ui->screen_wifi_wifi_pass_label, "wifi密码：");
    lv_label_set_long_mode(ui->screen_wifi_wifi_pass_label, LV_LABEL_LONG_WRAP);

    //Write style for screen_wifi_wifi_pass_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_wifi_pass_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_wifi_pass_label, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_wifi_pass_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_wifi_pass_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_wifi_pass_value
    ui->screen_wifi_wifi_pass_value = lv_textarea_create(ui->screen_wifi_form);
    lv_obj_set_pos(ui->screen_wifi_wifi_pass_value, 88, 29);
    lv_obj_set_size(ui->screen_wifi_wifi_pass_value, 125, 25);
    lv_textarea_set_text(ui->screen_wifi_wifi_pass_value, "");
    lv_textarea_set_placeholder_text(ui->screen_wifi_wifi_pass_value, "");
    lv_textarea_set_password_bullet(ui->screen_wifi_wifi_pass_value, "*");
    lv_textarea_set_password_mode(ui->screen_wifi_wifi_pass_value, true);
    lv_textarea_set_one_line(ui->screen_wifi_wifi_pass_value, true);
    lv_textarea_set_accepted_chars(ui->screen_wifi_wifi_pass_value, "");
    lv_textarea_set_max_length(ui->screen_wifi_wifi_pass_value, 32);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->screen_wifi_wifi_pass_value, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for screen_wifi_wifi_pass_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_wifi_wifi_pass_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_wifi_pass_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_wifi_pass_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_wifi_pass_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_wifi_pass_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_wifi_pass_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_wifi_pass_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_wifi_wifi_pass_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_wifi_wifi_pass_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_wifi_wifi_pass_value, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_wifi_pass_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_wifi_pass_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_wifi_pass_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_wifi_pass_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_wifi_pass_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_wifi_wifi_pass_value, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_wifi_pass_value, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_wifi_pass_value, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_wifi_pass_value, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_wifi_pass_value, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes screen_wifi_connect_button
    ui->screen_wifi_connect_button = lv_button_create(ui->screen_wifi_form);
    lv_obj_set_pos(ui->screen_wifi_connect_button, 22, 68);
    lv_obj_set_size(ui->screen_wifi_connect_button, 60, 25);
    lv_obj_add_flag(ui->screen_wifi_connect_button, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_wifi_connect_button_label = lv_label_create(ui->screen_wifi_connect_button);
    lv_label_set_text(ui->screen_wifi_connect_button_label, "连接");
    lv_label_set_long_mode(ui->screen_wifi_connect_button_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_wifi_connect_button_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_wifi_connect_button, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_wifi_connect_button_label, LV_PCT(100));

    //Write style for screen_wifi_connect_button, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_connect_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_connect_button, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_connect_button, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_connect_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_connect_button, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_connect_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_connect_button, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_connect_button, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_connect_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_connect_button, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_cancel_button
    ui->screen_wifi_cancel_button = lv_button_create(ui->screen_wifi_form);
    lv_obj_set_pos(ui->screen_wifi_cancel_button, 131, 68);
    lv_obj_set_size(ui->screen_wifi_cancel_button, 60, 25);
    lv_obj_add_flag(ui->screen_wifi_cancel_button, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_wifi_cancel_button_label = lv_label_create(ui->screen_wifi_cancel_button);
    lv_label_set_text(ui->screen_wifi_cancel_button_label, "取消");
    lv_label_set_long_mode(ui->screen_wifi_cancel_button_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_wifi_cancel_button_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_wifi_cancel_button, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_wifi_cancel_button_label, LV_PCT(100));

    //Write style for screen_wifi_cancel_button, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_cancel_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_cancel_button, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_cancel_button, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_cancel_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_cancel_button, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_cancel_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_wifi_cancel_button, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_cancel_button, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_cancel_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_cancel_button, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_wifi_wifi_name_value
    ui->screen_wifi_wifi_name_value = lv_textarea_create(ui->screen_wifi_form);
    lv_obj_set_pos(ui->screen_wifi_wifi_name_value, 88, 2);
    lv_obj_set_size(ui->screen_wifi_wifi_name_value, 125, 25);
    lv_textarea_set_text(ui->screen_wifi_wifi_name_value, "");
    lv_textarea_set_placeholder_text(ui->screen_wifi_wifi_name_value, "");
    lv_textarea_set_password_bullet(ui->screen_wifi_wifi_name_value, "*");
    lv_textarea_set_password_mode(ui->screen_wifi_wifi_name_value, false);
    lv_textarea_set_one_line(ui->screen_wifi_wifi_name_value, true);
    lv_textarea_set_accepted_chars(ui->screen_wifi_wifi_name_value, "");
    lv_textarea_set_max_length(ui->screen_wifi_wifi_name_value, 32);
#if LV_USE_KEYBOARD
    lv_obj_add_event_cb(ui->screen_wifi_wifi_name_value, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif

    //Write style for screen_wifi_wifi_name_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_wifi_wifi_name_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_wifi_wifi_name_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_wifi_wifi_name_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_wifi_wifi_name_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_wifi_wifi_name_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_wifi_wifi_name_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_wifi_wifi_name_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_wifi_wifi_name_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_wifi_wifi_name_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_wifi_wifi_name_value, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_wifi_wifi_name_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_wifi_wifi_name_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_wifi_wifi_name_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_wifi_wifi_name_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_wifi_name_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_wifi_wifi_name_value, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_wifi_wifi_name_value, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_wifi_wifi_name_value, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_wifi_wifi_name_value, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_wifi_wifi_name_value, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //The custom code of screen_wifi.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_wifi);

    //Init events for screen.
    events_init_screen_wifi(ui);
}
