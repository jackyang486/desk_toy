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



void setup_scr_robot_win(lv_ui *ui)
{
    //Write codes robot_win
    ui->robot_win = lv_obj_create(NULL);
    lv_obj_set_size(ui->robot_win, 240, 320);
    lv_obj_set_scrollbar_mode(ui->robot_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for robot_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->robot_win, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->robot_win, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->robot_win, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->robot_win,"S:/image/neutral.jpg", LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui->robot_win, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_recolor_opa(ui->robot_win, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes robot_win_cont_msg
    ui->robot_win_cont_msg = lv_obj_create(ui->robot_win);
    lv_obj_set_pos(ui->robot_win_cont_msg, 0, 38);
    lv_obj_set_size(ui->robot_win_cont_msg, 240, 282);
    lv_obj_set_scrollbar_mode(ui->robot_win_cont_msg, LV_SCROLLBAR_MODE_OFF);

    //Write style for robot_win_cont_msg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->robot_win_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes robot_win_cont_1
    ui->robot_win_cont_1 = lv_obj_create(ui->robot_win);
    lv_obj_set_pos(ui->robot_win_cont_1, 0, 0);
    lv_obj_set_size(ui->robot_win_cont_1, 240, 36);
    lv_obj_set_scrollbar_mode(ui->robot_win_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for robot_win_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->robot_win_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes robot_win_label_status
    ui->robot_win_label_status = lv_label_create(ui->robot_win_cont_1);
    lv_label_set_text(ui->robot_win_label_status, "");
    lv_label_set_long_mode(ui->robot_win_label_status, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->robot_win_label_status, 71, 3);
    lv_obj_set_size(ui->robot_win_label_status, 100, 33);

    //Write style for robot_win_label_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->robot_win_label_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->robot_win_label_status, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->robot_win_label_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->robot_win_label_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->robot_win_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of robot_win.


    //Update current screen layout.
    lv_obj_update_layout(ui->robot_win);

    //Init events for screen.
    events_init_robot_win(ui);
}
