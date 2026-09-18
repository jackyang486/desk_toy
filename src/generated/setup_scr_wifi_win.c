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



void setup_scr_wifi_win(lv_ui *ui)
{
    //Write codes wifi_win
    ui->wifi_win = lv_obj_create(NULL);
    lv_obj_set_size(ui->wifi_win, 240, 320);
    lv_obj_set_scrollbar_mode(ui->wifi_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for wifi_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_win, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win, lv_color_hex(0xf0f595), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_list
    ui->wifi_win_list = lv_list_create(ui->wifi_win);
    lv_obj_set_pos(ui->wifi_win_list, 0, 20);
    lv_obj_set_size(ui->wifi_win_list, 240, 300);
    lv_obj_set_scrollbar_mode(ui->wifi_win_list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->wifi_win_list, LV_OBJ_FLAG_CLICKABLE);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_win_list_main_main_default
    static lv_style_t style_wifi_win_list_main_main_default;
    ui_init_style(&style_wifi_win_list_main_main_default);

    lv_style_set_pad_top(&style_wifi_win_list_main_main_default, 0);
    lv_style_set_pad_left(&style_wifi_win_list_main_main_default, 0);
    lv_style_set_pad_right(&style_wifi_win_list_main_main_default, 0);
    lv_style_set_pad_bottom(&style_wifi_win_list_main_main_default, 0);
    lv_style_set_bg_opa(&style_wifi_win_list_main_main_default, 255);
    lv_style_set_bg_color(&style_wifi_win_list_main_main_default, lv_color_hex(0xf0f595));
    lv_style_set_bg_grad_dir(&style_wifi_win_list_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_wifi_win_list_main_main_default, 1);
    lv_style_set_border_opa(&style_wifi_win_list_main_main_default, 255);
    lv_style_set_border_color(&style_wifi_win_list_main_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_wifi_win_list_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_wifi_win_list_main_main_default, 3);
    lv_style_set_shadow_width(&style_wifi_win_list_main_main_default, 0);
    lv_obj_add_style(ui->wifi_win_list, &style_wifi_win_list_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_win_list_main_scrollbar_default
    static lv_style_t style_wifi_win_list_main_scrollbar_default;
    ui_init_style(&style_wifi_win_list_main_scrollbar_default);

    lv_style_set_radius(&style_wifi_win_list_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_wifi_win_list_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_wifi_win_list_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_wifi_win_list_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->wifi_win_list, &style_wifi_win_list_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_win_list_extra_btns_main_default
    static lv_style_t style_wifi_win_list_extra_btns_main_default;
    ui_init_style(&style_wifi_win_list_extra_btns_main_default);

    lv_style_set_pad_top(&style_wifi_win_list_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_wifi_win_list_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_wifi_win_list_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_wifi_win_list_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_wifi_win_list_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_wifi_win_list_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_wifi_win_list_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_wifi_win_list_extra_btns_main_default, 255);
    lv_style_set_radius(&style_wifi_win_list_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_wifi_win_list_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_wifi_win_list_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_wifi_win_list_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_wifi_win_list_extra_texts_main_default
    static lv_style_t style_wifi_win_list_extra_texts_main_default;
    ui_init_style(&style_wifi_win_list_extra_texts_main_default);

    lv_style_set_pad_top(&style_wifi_win_list_extra_texts_main_default, 6);
    lv_style_set_pad_left(&style_wifi_win_list_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_wifi_win_list_extra_texts_main_default, 0);
    lv_style_set_pad_bottom(&style_wifi_win_list_extra_texts_main_default, 0);
    lv_style_set_border_width(&style_wifi_win_list_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_wifi_win_list_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_wifi_win_list_extra_texts_main_default, &lv_font_Antonio_Regular_18);
    lv_style_set_text_opa(&style_wifi_win_list_extra_texts_main_default, 255);
    lv_style_set_radius(&style_wifi_win_list_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_wifi_win_list_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_wifi_win_list_extra_texts_main_default, 0);

    //Write codes wifi_win_message_label
    ui->wifi_win_message_label = lv_label_create(ui->wifi_win);
    lv_label_set_text(ui->wifi_win_message_label, "");
    lv_label_set_long_mode(ui->wifi_win_message_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->wifi_win_message_label, 2, 295);
    lv_obj_set_size(ui->wifi_win_message_label, 200, 20);

    //Write style for wifi_win_message_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_win_message_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_message_label, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_message_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_win_message_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_message_label, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_message_label, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_message_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_form
    ui->wifi_win_form = lv_obj_create(ui->wifi_win);
    lv_obj_set_pos(ui->wifi_win_form, 10, 100);
    lv_obj_set_size(ui->wifi_win_form, 220, 100);
    lv_obj_set_scrollbar_mode(ui->wifi_win_form, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->wifi_win_form, LV_OBJ_FLAG_HIDDEN);

    //Write style for wifi_win_form, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_win_form, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->wifi_win_form, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->wifi_win_form, lv_color_hex(0xb7b9c1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->wifi_win_form, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_form, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_form, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win_form, lv_color_hex(0xdbdce0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win_form, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_win_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_form, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_wifi_name_label
    ui->wifi_win_wifi_name_label = lv_label_create(ui->wifi_win_form);
    lv_label_set_text(ui->wifi_win_wifi_name_label, "wifi名称：");
    lv_label_set_long_mode(ui->wifi_win_wifi_name_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->wifi_win_wifi_name_label, 5, 5);
    lv_obj_set_size(ui->wifi_win_wifi_name_label, 78, 25);

    //Write style for wifi_win_wifi_name_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_win_wifi_name_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_wifi_name_label, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_wifi_name_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_wifi_name_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_wifi_name_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_wifi_name_value
    ui->wifi_win_wifi_name_value = lv_textarea_create(ui->wifi_win_form);
    lv_textarea_set_text(ui->wifi_win_wifi_name_value, "");
    lv_textarea_set_placeholder_text(ui->wifi_win_wifi_name_value, "");
    lv_textarea_set_password_bullet(ui->wifi_win_wifi_name_value, "*");
    lv_textarea_set_password_mode(ui->wifi_win_wifi_name_value, false);
    lv_textarea_set_one_line(ui->wifi_win_wifi_name_value, true);
    lv_textarea_set_accepted_chars(ui->wifi_win_wifi_name_value, "");
    lv_textarea_set_max_length(ui->wifi_win_wifi_name_value, 32);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->wifi_win_wifi_name_value, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->wifi_win_wifi_name_value, 88, 2);
    lv_obj_set_size(ui->wifi_win_wifi_name_value, 125, 25);

    //Write style for wifi_win_wifi_name_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->wifi_win_wifi_name_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_wifi_name_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_wifi_name_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_win_wifi_name_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_wifi_name_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_wifi_name_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_win_wifi_name_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->wifi_win_wifi_name_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->wifi_win_wifi_name_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->wifi_win_wifi_name_value, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_wifi_name_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_wifi_name_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_wifi_name_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_wifi_name_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_wifi_name_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for wifi_win_wifi_name_value, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_win_wifi_name_value, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win_wifi_name_value, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win_wifi_name_value, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_wifi_name_value, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes wifi_win_wifi_pass_label
    ui->wifi_win_wifi_pass_label = lv_label_create(ui->wifi_win_form);
    lv_label_set_text(ui->wifi_win_wifi_pass_label, "wifi密码：");
    lv_label_set_long_mode(ui->wifi_win_wifi_pass_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->wifi_win_wifi_pass_label, 5, 32);
    lv_obj_set_size(ui->wifi_win_wifi_pass_label, 78, 25);

    //Write style for wifi_win_wifi_pass_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_win_wifi_pass_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_wifi_pass_label, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_wifi_pass_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_wifi_pass_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_wifi_pass_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_wifi_pass_value
    ui->wifi_win_wifi_pass_value = lv_textarea_create(ui->wifi_win_form);
    lv_textarea_set_text(ui->wifi_win_wifi_pass_value, "");
    lv_textarea_set_placeholder_text(ui->wifi_win_wifi_pass_value, "");
    lv_textarea_set_password_bullet(ui->wifi_win_wifi_pass_value, "*");
    lv_textarea_set_password_mode(ui->wifi_win_wifi_pass_value, true);
    lv_textarea_set_one_line(ui->wifi_win_wifi_pass_value, true);
    lv_textarea_set_accepted_chars(ui->wifi_win_wifi_pass_value, "");
    lv_textarea_set_max_length(ui->wifi_win_wifi_pass_value, 32);
#if LV_USE_KEYBOARD != 0 || LV_USE_ZH_KEYBOARD != 0
    lv_obj_add_event_cb(ui->wifi_win_wifi_pass_value, ta_event_cb, LV_EVENT_ALL, ui->g_kb_top_layer);
#endif
    lv_obj_set_pos(ui->wifi_win_wifi_pass_value, 88, 29);
    lv_obj_set_size(ui->wifi_win_wifi_pass_value, 125, 25);

    //Write style for wifi_win_wifi_pass_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->wifi_win_wifi_pass_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_wifi_pass_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_wifi_pass_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_win_wifi_pass_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_wifi_pass_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_wifi_pass_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_win_wifi_pass_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->wifi_win_wifi_pass_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->wifi_win_wifi_pass_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->wifi_win_wifi_pass_value, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_wifi_pass_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_wifi_pass_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_wifi_pass_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_wifi_pass_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_wifi_pass_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for wifi_win_wifi_pass_value, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_win_wifi_pass_value, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win_wifi_pass_value, lv_color_hex(0x2195f6), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win_wifi_pass_value, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_wifi_pass_value, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write codes wifi_win_connect_button
    ui->wifi_win_connect_button = lv_btn_create(ui->wifi_win_form);
    ui->wifi_win_connect_button_label = lv_label_create(ui->wifi_win_connect_button);
    lv_label_set_text(ui->wifi_win_connect_button_label, "连接");
    lv_label_set_long_mode(ui->wifi_win_connect_button_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->wifi_win_connect_button_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->wifi_win_connect_button, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->wifi_win_connect_button_label, LV_PCT(100));
    lv_obj_set_pos(ui->wifi_win_connect_button, 21, 68);
    lv_obj_set_size(ui->wifi_win_connect_button, 60, 25);
    lv_obj_add_flag(ui->wifi_win_connect_button, LV_OBJ_FLAG_CLICKABLE);

    //Write style for wifi_win_connect_button, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_win_connect_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win_connect_button, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win_connect_button, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_win_connect_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_connect_button, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_connect_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_win_connect_button, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_connect_button, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_connect_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_connect_button, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_cancel_button
    ui->wifi_win_cancel_button = lv_btn_create(ui->wifi_win_form);
    ui->wifi_win_cancel_button_label = lv_label_create(ui->wifi_win_cancel_button);
    lv_label_set_text(ui->wifi_win_cancel_button_label, "取消");
    lv_label_set_long_mode(ui->wifi_win_cancel_button_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->wifi_win_cancel_button_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->wifi_win_cancel_button, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->wifi_win_cancel_button_label, LV_PCT(100));
    lv_obj_set_pos(ui->wifi_win_cancel_button, 131, 68);
    lv_obj_set_size(ui->wifi_win_cancel_button, 60, 25);

    //Write style for wifi_win_cancel_button, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_win_cancel_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win_cancel_button, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win_cancel_button, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_win_cancel_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_cancel_button, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_cancel_button, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_win_cancel_button, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_cancel_button, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_cancel_button, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_cancel_button, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes wifi_win_list_title
    ui->wifi_win_list_title = lv_label_create(ui->wifi_win);
    lv_label_set_text(ui->wifi_win_list_title, "" LV_SYMBOL_LIST "  wifi list");
    lv_label_set_long_mode(ui->wifi_win_list_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->wifi_win_list_title, 0, 0);
    lv_obj_set_size(ui->wifi_win_list_title, 240, 20);

    //Write style for wifi_win_list_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_win_list_title, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_win_list_title, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_win_list_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_win_list_title, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_win_list_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_win_list_title, lv_color_hex(0xdbd9d9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_win_list_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_win_list_title, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_win_list_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of wifi_win.


    //Update current screen layout.
    lv_obj_update_layout(ui->wifi_win);

    //Init events for screen.
    events_init_wifi_win(ui);
}
