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



void setup_scr_screen_mouse(lv_ui *ui)
{
    //Write codes screen_mouse
    ui->screen_mouse = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_mouse, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_mouse, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_mouse, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mouse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mouse_btn_right
    ui->screen_mouse_btn_right = lv_button_create(ui->screen_mouse);
    lv_obj_set_pos(ui->screen_mouse_btn_right, 143, 50);
    lv_obj_set_size(ui->screen_mouse_btn_right, 60, 50);
    ui->screen_mouse_btn_right_label = lv_label_create(ui->screen_mouse_btn_right);
    lv_label_set_text(ui->screen_mouse_btn_right_label, "right");
    lv_label_set_long_mode(ui->screen_mouse_btn_right_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_mouse_btn_right_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_mouse_btn_right, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_mouse_btn_right_label, LV_PCT(100));

    //Write style for screen_mouse_btn_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mouse_btn_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mouse_btn_right, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mouse_btn_right, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_mouse_btn_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mouse_btn_right, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mouse_btn_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mouse_btn_right, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mouse_btn_right, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mouse_btn_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mouse_btn_right, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_mouse_btn_left
    ui->screen_mouse_btn_left = lv_button_create(ui->screen_mouse);
    lv_obj_set_pos(ui->screen_mouse_btn_left, 30, 50);
    lv_obj_set_size(ui->screen_mouse_btn_left, 60, 50);
    ui->screen_mouse_btn_left_label = lv_label_create(ui->screen_mouse_btn_left);
    lv_label_set_text(ui->screen_mouse_btn_left_label, "left");
    lv_label_set_long_mode(ui->screen_mouse_btn_left_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_mouse_btn_left_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_mouse_btn_left, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_mouse_btn_left_label, LV_PCT(100));

    //Write style for screen_mouse_btn_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_mouse_btn_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_mouse_btn_left, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_mouse_btn_left, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_mouse_btn_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_mouse_btn_left, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_mouse_btn_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_mouse_btn_left, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_mouse_btn_left, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_mouse_btn_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_mouse_btn_left, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_mouse.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_mouse);

    //Init events for screen.
    events_init_screen_mouse(ui);
}
