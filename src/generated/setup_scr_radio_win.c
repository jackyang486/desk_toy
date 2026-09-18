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



void setup_scr_radio_win(lv_ui *ui)
{
    //Write codes radio_win
    ui->radio_win = lv_obj_create(NULL);
    lv_obj_set_size(ui->radio_win, 240, 320);
    lv_obj_set_scrollbar_mode(ui->radio_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for radio_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->radio_win, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->radio_win, lv_color_hex(0xF5D9A8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->radio_win, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes radio_win_list_radio
    ui->radio_win_list_radio = lv_list_create(ui->radio_win);
    lv_obj_set_pos(ui->radio_win_list_radio, 3, 25);
    lv_obj_set_size(ui->radio_win_list_radio, 234, 292);
    lv_obj_set_scrollbar_mode(ui->radio_win_list_radio, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_radio_win_list_radio_main_main_default
    static lv_style_t style_radio_win_list_radio_main_main_default;
    ui_init_style(&style_radio_win_list_radio_main_main_default);

    lv_style_set_pad_top(&style_radio_win_list_radio_main_main_default, 5);
    lv_style_set_pad_left(&style_radio_win_list_radio_main_main_default, 5);
    lv_style_set_pad_right(&style_radio_win_list_radio_main_main_default, 5);
    lv_style_set_pad_bottom(&style_radio_win_list_radio_main_main_default, 5);
    lv_style_set_bg_opa(&style_radio_win_list_radio_main_main_default, 255);
    lv_style_set_bg_color(&style_radio_win_list_radio_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_radio_win_list_radio_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_radio_win_list_radio_main_main_default, 0);
    lv_style_set_radius(&style_radio_win_list_radio_main_main_default, 3);
    lv_style_set_shadow_width(&style_radio_win_list_radio_main_main_default, 0);
    lv_obj_add_style(ui->radio_win_list_radio, &style_radio_win_list_radio_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_radio_win_list_radio_main_scrollbar_default
    static lv_style_t style_radio_win_list_radio_main_scrollbar_default;
    ui_init_style(&style_radio_win_list_radio_main_scrollbar_default);

    lv_style_set_radius(&style_radio_win_list_radio_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_radio_win_list_radio_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_radio_win_list_radio_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_radio_win_list_radio_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->radio_win_list_radio, &style_radio_win_list_radio_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_radio_win_list_radio_extra_btns_main_default
    static lv_style_t style_radio_win_list_radio_extra_btns_main_default;
    ui_init_style(&style_radio_win_list_radio_extra_btns_main_default);

    lv_style_set_pad_top(&style_radio_win_list_radio_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_radio_win_list_radio_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_radio_win_list_radio_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_radio_win_list_radio_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_radio_win_list_radio_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_radio_win_list_radio_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_radio_win_list_radio_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_radio_win_list_radio_extra_btns_main_default, 255);
    lv_style_set_radius(&style_radio_win_list_radio_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_radio_win_list_radio_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_radio_win_list_radio_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_radio_win_list_radio_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_radio_win_list_radio_extra_texts_main_default
    static lv_style_t style_radio_win_list_radio_extra_texts_main_default;
    ui_init_style(&style_radio_win_list_radio_extra_texts_main_default);

    lv_style_set_pad_top(&style_radio_win_list_radio_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_radio_win_list_radio_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_radio_win_list_radio_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_radio_win_list_radio_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_radio_win_list_radio_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_radio_win_list_radio_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_radio_win_list_radio_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_radio_win_list_radio_extra_texts_main_default, 255);
    lv_style_set_radius(&style_radio_win_list_radio_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_radio_win_list_radio_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_radio_win_list_radio_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_radio_win_list_radio_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_radio_win_list_radio_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes radio_win_label_1
    ui->radio_win_label_1 = lv_label_create(ui->radio_win);
    lv_label_set_text(ui->radio_win_label_1, "");
    lv_label_set_long_mode(ui->radio_win_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->radio_win_label_1, 0, 0);
    lv_obj_set_size(ui->radio_win_label_1, 240, 20);

    //Write style for radio_win_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->radio_win_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->radio_win_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->radio_win_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->radio_win_label_1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->radio_win_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of radio_win.


    //Update current screen layout.
    lv_obj_update_layout(ui->radio_win);

    //Init events for screen.
    events_init_radio_win(ui);
}
