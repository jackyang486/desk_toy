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



void setup_scr_screen_robot(lv_ui *ui)
{
    //Write codes screen_robot
    ui->screen_robot = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_robot, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_robot, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_robot, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_robot, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->screen_robot, &_confident_240x320_RGB565A8_240x320, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->screen_robot, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->screen_robot, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_robot_cont_label
    ui->screen_robot_cont_label = lv_obj_create(ui->screen_robot);
    lv_obj_set_pos(ui->screen_robot_cont_label, 0, 0);
    lv_obj_set_size(ui->screen_robot_cont_label, 240, 36);
    lv_obj_set_scrollbar_mode(ui->screen_robot_cont_label, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_robot_cont_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_robot_cont_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_robot_label_status
    ui->screen_robot_label_status = lv_label_create(ui->screen_robot_cont_label);
    lv_obj_set_pos(ui->screen_robot_label_status, 71, 3);
    lv_obj_set_size(ui->screen_robot_label_status, 100, 33);
    lv_label_set_text(ui->screen_robot_label_status, "");
    lv_label_set_long_mode(ui->screen_robot_label_status, LV_LABEL_LONG_WRAP);

    //Write style for screen_robot_label_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_robot_label_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_robot_label_status, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_robot_label_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_robot_label_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_robot_label_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_robot_cont_msg
    ui->screen_robot_cont_msg = lv_obj_create(ui->screen_robot);
    lv_obj_set_pos(ui->screen_robot_cont_msg, 0, 38);
    lv_obj_set_size(ui->screen_robot_cont_msg, 240, 282);
    lv_obj_set_scrollbar_mode(ui->screen_robot_cont_msg, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_robot_cont_msg, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_robot_cont_msg, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_robot.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_robot);

    //Init events for screen.
    events_init_screen_robot(ui);
}
