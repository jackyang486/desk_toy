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



void setup_scr_photo_win(lv_ui *ui)
{
    //Write codes photo_win
    ui->photo_win = lv_obj_create(NULL);
    lv_obj_set_size(ui->photo_win, 240, 320);
    lv_obj_set_scrollbar_mode(ui->photo_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for photo_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->photo_win, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes photo_win_view
    ui->photo_win_view = lv_img_create(ui->photo_win);
    lv_img_set_pivot(ui->photo_win_view, 50,50);
    lv_img_set_angle(ui->photo_win_view, 0);
    lv_obj_set_pos(ui->photo_win_view, 0, 0);
    lv_obj_set_size(ui->photo_win_view, 240, 320);
    lv_obj_add_flag(ui->photo_win_view, LV_OBJ_FLAG_CLICKABLE);

    //Write style for photo_win_view, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->photo_win_view, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->photo_win_view, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->photo_win_view, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->photo_win_view, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes photo_win_left_arrow
    ui->photo_win_left_arrow = lv_btn_create(ui->photo_win);
    ui->photo_win_left_arrow_label = lv_label_create(ui->photo_win_left_arrow);
    lv_label_set_text(ui->photo_win_left_arrow_label, "" LV_SYMBOL_LEFT "");
    lv_label_set_long_mode(ui->photo_win_left_arrow_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->photo_win_left_arrow_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->photo_win_left_arrow, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->photo_win_left_arrow_label, LV_PCT(100));
    lv_obj_set_pos(ui->photo_win_left_arrow, 4, 132);
    lv_obj_set_size(ui->photo_win_left_arrow, 50, 30);

    //Write style for photo_win_left_arrow, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->photo_win_left_arrow, 106, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->photo_win_left_arrow, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->photo_win_left_arrow, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->photo_win_left_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->photo_win_left_arrow, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->photo_win_left_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->photo_win_left_arrow, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->photo_win_left_arrow, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->photo_win_left_arrow, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->photo_win_left_arrow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes photo_win_right_arrow
    ui->photo_win_right_arrow = lv_btn_create(ui->photo_win);
    ui->photo_win_right_arrow_label = lv_label_create(ui->photo_win_right_arrow);
    lv_label_set_text(ui->photo_win_right_arrow_label, "" LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->photo_win_right_arrow_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->photo_win_right_arrow_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->photo_win_right_arrow, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->photo_win_right_arrow_label, LV_PCT(100));
    lv_obj_set_pos(ui->photo_win_right_arrow, 184, 132);
    lv_obj_set_size(ui->photo_win_right_arrow, 50, 30);

    //Write style for photo_win_right_arrow, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->photo_win_right_arrow, 107, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->photo_win_right_arrow, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->photo_win_right_arrow, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->photo_win_right_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->photo_win_right_arrow, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->photo_win_right_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->photo_win_right_arrow, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->photo_win_right_arrow, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->photo_win_right_arrow, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->photo_win_right_arrow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of photo_win.


    //Update current screen layout.
    lv_obj_update_layout(ui->photo_win);

    //Init events for screen.
    events_init_photo_win(ui);
}
