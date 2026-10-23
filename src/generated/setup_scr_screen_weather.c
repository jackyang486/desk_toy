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



void setup_scr_screen_weather(lv_ui *ui)
{
    //Write codes screen_weather
    ui->screen_weather = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_weather, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_weather, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_animimg_astronaut
    ui->screen_weather_animimg_astronaut = lv_animimg_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_animimg_astronaut, 160, 205);
    lv_obj_set_size(ui->screen_weather_animimg_astronaut, 70, 70);
    lv_animimg_set_src(ui->screen_weather_animimg_astronaut, (const void **) screen_weather_animimg_astronaut_imgs, 10);
    lv_animimg_set_duration(ui->screen_weather_animimg_astronaut, 37*10);
    lv_animimg_set_repeat_count(ui->screen_weather_animimg_astronaut, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(ui->screen_weather_animimg_astronaut);

    //Write codes screen_weather_cont_second_2
    ui->screen_weather_cont_second_2 = lv_obj_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_cont_second_2, 195, 132);
    lv_obj_set_size(ui->screen_weather_cont_second_2, 18, 30);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_second_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather_cont_second_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_second_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_second_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_second_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_second_1
    ui->screen_weather_img_second_1 = lv_image_create(ui->screen_weather_cont_second_2);
    lv_obj_set_pos(ui->screen_weather_img_second_1, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_second_1, 18, 30);
    lv_obj_add_flag(ui->screen_weather_img_second_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_weather_img_second_1, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_second_1, 0);

    //Write style for screen_weather_img_second_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_second_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_second_shi
    ui->screen_weather_img_second_shi = lv_image_create(ui->screen_weather_cont_second_2);
    lv_obj_set_pos(ui->screen_weather_img_second_shi, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_second_shi, 18, 30);
    lv_obj_add_flag(ui->screen_weather_img_second_shi, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_second_shi, &_W_1830_i0_RGB565A8_18x30);
    lv_image_set_pivot(ui->screen_weather_img_second_shi, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_second_shi, 0);

    //Write style for screen_weather_img_second_shi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_second_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_second_shi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_cont_second_1
    ui->screen_weather_cont_second_1 = lv_obj_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_cont_second_1, 215, 132);
    lv_obj_set_size(ui->screen_weather_cont_second_1, 18, 30);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_second_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather_cont_second_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_second_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_second_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_second_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_second_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_second_2
    ui->screen_weather_img_second_2 = lv_image_create(ui->screen_weather_cont_second_1);
    lv_obj_set_pos(ui->screen_weather_img_second_2, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_second_2, 18, 30);
    lv_obj_add_flag(ui->screen_weather_img_second_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_weather_img_second_2, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_second_2, 0);

    //Write style for screen_weather_img_second_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_second_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_second_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_second_ge
    ui->screen_weather_img_second_ge = lv_image_create(ui->screen_weather_cont_second_1);
    lv_obj_set_pos(ui->screen_weather_img_second_ge, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_second_ge, 18, 30);
    lv_obj_add_flag(ui->screen_weather_img_second_ge, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_second_ge, &_W_1830_i0_RGB565A8_18x30);
    lv_image_set_pivot(ui->screen_weather_img_second_ge, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_second_ge, 0);

    //Write style for screen_weather_img_second_ge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_second_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_second_ge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_cont_minute_2
    ui->screen_weather_cont_minute_2 = lv_obj_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_cont_minute_2, 105, 102);
    lv_obj_set_size(ui->screen_weather_cont_minute_2, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_minute_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather_cont_minute_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_minute_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_minute_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_minute_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_minute_2
    ui->screen_weather_img_minute_2 = lv_image_create(ui->screen_weather_cont_minute_2);
    lv_obj_set_pos(ui->screen_weather_img_minute_2, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_minute_2, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_minute_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_weather_img_minute_2, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_minute_2, 0);

    //Write style for screen_weather_img_minute_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_minute_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_minute_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_minute_shi
    ui->screen_weather_img_minute_shi = lv_image_create(ui->screen_weather_cont_minute_2);
    lv_obj_set_pos(ui->screen_weather_img_minute_shi, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_minute_shi, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_minute_shi, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_minute_shi, &_O_3660_i0_RGB565A8_36x60);
    lv_image_set_pivot(ui->screen_weather_img_minute_shi, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_minute_shi, 0);

    //Write style for screen_weather_img_minute_shi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_minute_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_minute_shi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_cont_minute_1
    ui->screen_weather_cont_minute_1 = lv_obj_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_cont_minute_1, 145, 102);
    lv_obj_set_size(ui->screen_weather_cont_minute_1, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_minute_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather_cont_minute_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_minute_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_minute_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_minute_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_minute_1
    ui->screen_weather_img_minute_1 = lv_image_create(ui->screen_weather_cont_minute_1);
    lv_obj_set_pos(ui->screen_weather_img_minute_1, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_minute_1, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_minute_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_weather_img_minute_1, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_minute_1, 0);

    //Write style for screen_weather_img_minute_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_minute_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_minute_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_minute_ge
    ui->screen_weather_img_minute_ge = lv_image_create(ui->screen_weather_cont_minute_1);
    lv_obj_set_pos(ui->screen_weather_img_minute_ge, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_minute_ge, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_minute_ge, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_minute_ge, &_O_3660_i0_RGB565A8_36x60);
    lv_image_set_pivot(ui->screen_weather_img_minute_ge, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_minute_ge, 0);

    //Write style for screen_weather_img_minute_ge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_minute_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_minute_ge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_cont_hour_2
    ui->screen_weather_cont_hour_2 = lv_obj_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_cont_hour_2, 10, 102);
    lv_obj_set_size(ui->screen_weather_cont_hour_2, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_hour_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather_cont_hour_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_hour_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_hour_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_hour_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_hour_2
    ui->screen_weather_img_hour_2 = lv_image_create(ui->screen_weather_cont_hour_2);
    lv_obj_set_pos(ui->screen_weather_img_hour_2, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_hour_2, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_hour_2, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_weather_img_hour_2, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_hour_2, 0);

    //Write style for screen_weather_img_hour_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_hour_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_hour_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_hour_shi
    ui->screen_weather_img_hour_shi = lv_image_create(ui->screen_weather_cont_hour_2);
    lv_obj_set_pos(ui->screen_weather_img_hour_shi, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_hour_shi, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_hour_shi, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_hour_shi, &_W_3660_i0_RGB565A8_36x60);
    lv_image_set_pivot(ui->screen_weather_img_hour_shi, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_hour_shi, 0);

    //Write style for screen_weather_img_hour_shi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_hour_shi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_hour_shi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_cont_hour_1
    ui->screen_weather_cont_hour_1 = lv_obj_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_cont_hour_1, 50, 102);
    lv_obj_set_size(ui->screen_weather_cont_hour_1, 36, 60);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_hour_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_weather_cont_hour_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_hour_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_hour_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_hour_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_hour_1
    ui->screen_weather_img_hour_1 = lv_image_create(ui->screen_weather_cont_hour_1);
    lv_obj_set_pos(ui->screen_weather_img_hour_1, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_hour_1, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_hour_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_weather_img_hour_1, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_hour_1, 0);

    //Write style for screen_weather_img_hour_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_hour_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_hour_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_hour_ge
    ui->screen_weather_img_hour_ge = lv_image_create(ui->screen_weather_cont_hour_1);
    lv_obj_set_pos(ui->screen_weather_img_hour_ge, 0, 0);
    lv_obj_set_size(ui->screen_weather_img_hour_ge, 36, 60);
    lv_obj_add_flag(ui->screen_weather_img_hour_ge, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_hour_ge, &_W_3660_i0_RGB565A8_36x60);
    lv_image_set_pivot(ui->screen_weather_img_hour_ge, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_hour_ge, 0);

    //Write style for screen_weather_img_hour_ge, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_hour_ge, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_hour_ge, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_weather0
    ui->screen_weather_img_weather0 = lv_image_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_img_weather0, 170, 34);
    lv_obj_set_size(ui->screen_weather_img_weather0, 60, 60);
    lv_obj_add_flag(ui->screen_weather_img_weather0, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_weather0, &_t0_RGB565A8_60x60);
    lv_image_set_pivot(ui->screen_weather_img_weather0, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_weather0, 0);

    //Write style for screen_weather_img_weather0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_weather0, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_weather0, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_qulaity
    ui->screen_weather_label_qulaity = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_qulaity, 100, 38);
    lv_obj_set_size(ui->screen_weather_label_qulaity, 56, 24);
    lv_label_set_text(ui->screen_weather_label_qulaity, "");
    lv_label_set_long_mode(ui->screen_weather_label_qulaity, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_qulaity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_qulaity, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_qulaity, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_qulaity, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_qulaity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_qulaity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_qulaity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_label_qulaity, lv_color_hex(0x9CCA7F), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_label_qulaity, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_qulaity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_year_month
    ui->screen_weather_label_year_month = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_year_month, 5, 170);
    lv_obj_set_size(ui->screen_weather_label_year_month, 175, 30);
    lv_label_set_text(ui->screen_weather_label_year_month, "2026年05月20日");
    lv_label_set_long_mode(ui->screen_weather_label_year_month, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_year_month, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_year_month, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_year_month, &lv_font_SourceHanSerifSC_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_year_month, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_year_month, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_year_month, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_year_month, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_week
    ui->screen_weather_label_week = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_week, 182, 170);
    lv_obj_set_size(ui->screen_weather_label_week, 58, 30);
    lv_label_set_text(ui->screen_weather_label_week, "周一二三四五六日");
    lv_label_set_long_mode(ui->screen_weather_label_week, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_week, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_week, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_week, &lv_font_SourceHanSerifSC_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_week, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_week, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_week, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_week, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_temperature
    ui->screen_weather_img_temperature = lv_image_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_img_temperature, 10, 213);
    lv_obj_set_size(ui->screen_weather_img_temperature, 24, 24);
    lv_obj_add_flag(ui->screen_weather_img_temperature, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_temperature, &_temperature_RGB565A8_24x24);
    lv_image_set_pivot(ui->screen_weather_img_temperature, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_temperature, 0);

    //Write style for screen_weather_img_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_temperature, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_temperature, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_img_humidity
    ui->screen_weather_img_humidity = lv_image_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_img_humidity, 10, 243);
    lv_obj_set_size(ui->screen_weather_img_humidity, 24, 24);
    lv_obj_add_flag(ui->screen_weather_img_humidity, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_img_humidity, &_humidity_RGB565A8_24x24);
    lv_image_set_pivot(ui->screen_weather_img_humidity, 50,50);
    lv_image_set_rotation(ui->screen_weather_img_humidity, 0);

    //Write style for screen_weather_img_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_img_humidity, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_img_humidity, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_bar_hum
    ui->screen_weather_bar_hum = lv_slider_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_bar_hum, 40, 252);
    lv_obj_set_size(ui->screen_weather_bar_hum, 52, 6);
    lv_slider_set_range(ui->screen_weather_bar_hum, 0, 100);
    lv_slider_set_mode(ui->screen_weather_bar_hum, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_weather_bar_hum, 25, LV_ANIM_OFF);

    //Write style for screen_weather_bar_hum, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather_bar_hum, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_bar_hum, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_bar_hum, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_bar_hum, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_weather_bar_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_bar_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_weather_bar_hum, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather_bar_hum, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_bar_hum, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_bar_hum, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_bar_hum, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_weather_bar_hum, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather_bar_hum, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_bar_hum, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_bar_hum, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_bar_hum, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_weather_bar_temp
    ui->screen_weather_bar_temp = lv_slider_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_bar_temp, 40, 222);
    lv_obj_set_size(ui->screen_weather_bar_temp, 52, 6);
    lv_slider_set_range(ui->screen_weather_bar_temp, 0, 52);
    lv_slider_set_mode(ui->screen_weather_bar_temp, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_weather_bar_temp, 25, LV_ANIM_OFF);

    //Write style for screen_weather_bar_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather_bar_temp, 116, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_bar_temp, lv_color_hex(0x85e2a4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_bar_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_bar_temp, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_weather_bar_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_bar_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_weather_bar_temp, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather_bar_temp, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_bar_temp, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_bar_temp, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_bar_temp, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_weather_bar_temp, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_weather_bar_temp, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_bar_temp, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_bar_temp, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_bar_temp, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_hum
    ui->screen_weather_label_hum = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_hum, 95, 244);
    lv_obj_set_size(ui->screen_weather_label_hum, 58, 24);
    lv_label_set_text(ui->screen_weather_label_hum, "80%");
    lv_label_set_long_mode(ui->screen_weather_label_hum, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_hum, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_hum, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_hum, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_hum, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_hum, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_hum, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_temp
    ui->screen_weather_label_temp = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_temp, 95, 218);
    lv_obj_set_size(ui->screen_weather_label_temp, 58, 24);
    lv_label_set_text(ui->screen_weather_label_temp, "12℃");
    lv_label_set_long_mode(ui->screen_weather_label_temp, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_temp, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_weather_banner
    ui->screen_weather_label_weather_banner = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_weather_banner, 10, 65);
    lv_obj_set_size(ui->screen_weather_label_weather_banner, 155, 30);
    lv_label_set_text(ui->screen_weather_label_weather_banner, "");
    lv_label_set_long_mode(ui->screen_weather_label_weather_banner, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_weather_banner, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_weather_banner, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_weather_banner, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_weather_banner, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_weather_banner, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_weather_banner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_label_city
    ui->screen_weather_label_city = lv_label_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_label_city, 5, 35);
    lv_obj_set_size(ui->screen_weather_label_city, 94, 30);
    lv_label_set_text(ui->screen_weather_label_city, "");
    lv_label_set_long_mode(ui->screen_weather_label_city, LV_LABEL_LONG_WRAP);

    //Write style for screen_weather_label_city, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_label_city, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_label_city, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_label_city, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_label_city, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_label_city, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_weather_wifi_icon
    ui->screen_weather_wifi_icon = lv_image_create(ui->screen_weather);
    lv_obj_set_pos(ui->screen_weather_wifi_icon, 3, 3);
    lv_obj_set_size(ui->screen_weather_wifi_icon, 20, 20);
    lv_obj_add_flag(ui->screen_weather_wifi_icon, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->screen_weather_wifi_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_weather_wifi_icon, &_wifiEnbale_RGB565A8_20x20);
    lv_image_set_pivot(ui->screen_weather_wifi_icon, 50,50);
    lv_image_set_rotation(ui->screen_weather_wifi_icon, 0);

    //Write style for screen_weather_wifi_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_weather_wifi_icon, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_weather_wifi_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_weather.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_weather);

    //Init events for screen.
    events_init_screen_weather(ui);
}
