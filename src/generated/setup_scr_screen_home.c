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



void setup_scr_screen_home(lv_ui *ui)
{
    //Write codes screen_home
    ui->screen_home = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_home, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_home, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_home, lv_color_hex(0xd9f9e3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_lottie
    ui->screen_home_img_lottie = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_lottie, 89, 157);
    lv_obj_set_size(ui->screen_home_img_lottie, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_lottie, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_lottie, &_photo_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_lottie, 50,50);
    lv_image_set_rotation(ui->screen_home_img_lottie, 0);

    //Write style for screen_home_img_lottie, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_lottie, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_lottie, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_mouse
    ui->screen_home_img_mouse = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_mouse, 10, 157);
    lv_obj_set_size(ui->screen_home_img_mouse, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_mouse, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_mouse, &_mouse_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_mouse, 50,50);
    lv_image_set_rotation(ui->screen_home_img_mouse, 0);

    //Write style for screen_home_img_mouse, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_mouse, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_mouse, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_radio
    ui->screen_home_img_radio = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_radio, 163, 100);
    lv_obj_set_size(ui->screen_home_img_radio, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_radio, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_radio, &_radio_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_radio, 50,50);
    lv_image_set_rotation(ui->screen_home_img_radio, 0);

    //Write style for screen_home_img_radio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_radio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_radio, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_wifi
    ui->screen_home_img_wifi = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_wifi, 89, 100);
    lv_obj_set_size(ui->screen_home_img_wifi, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_wifi, &_wifi_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_wifi, 50,50);
    lv_image_set_rotation(ui->screen_home_img_wifi, 0);

    //Write style for screen_home_img_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_setting
    ui->screen_home_img_setting = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_setting, 10, 100);
    lv_obj_set_size(ui->screen_home_img_setting, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_setting, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_setting, &_setup_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_setting, 50,50);
    lv_image_set_rotation(ui->screen_home_img_setting, 0);

    //Write style for screen_home_img_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_chatbot
    ui->screen_home_img_chatbot = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_chatbot, 163, 35);
    lv_obj_set_size(ui->screen_home_img_chatbot, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_chatbot, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_chatbot, &_robot_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_chatbot, 50,50);
    lv_image_set_rotation(ui->screen_home_img_chatbot, 0);

    //Write style for screen_home_img_chatbot, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_chatbot, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_chatbot, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_music
    ui->screen_home_img_music = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_music, 89, 35);
    lv_obj_set_size(ui->screen_home_img_music, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_music, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_music, &_music_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_music, 50,50);
    lv_image_set_rotation(ui->screen_home_img_music, 0);

    //Write style for screen_home_img_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_album
    ui->screen_home_img_album = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_album, 10, 35);
    lv_obj_set_size(ui->screen_home_img_album, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_album, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_album, &_photo_img2_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_album, 50,50);
    lv_image_set_rotation(ui->screen_home_img_album, 0);

    //Write style for screen_home_img_album, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_album, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_album, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_game
    ui->screen_home_img_game = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_game, 163, 157);
    lv_obj_set_size(ui->screen_home_img_game, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_game, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_game, &_game_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_game, 50,50);
    lv_image_set_rotation(ui->screen_home_img_game, 0);

    //Write style for screen_home_img_game, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_game, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_game, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_img_weather
    ui->screen_home_img_weather = lv_image_create(ui->screen_home);
    lv_obj_set_pos(ui->screen_home_img_weather, 10, 214);
    lv_obj_set_size(ui->screen_home_img_weather, 50, 50);
    lv_obj_add_flag(ui->screen_home_img_weather, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_home_img_weather, &_Weather_RGB565A8_50x50);
    lv_image_set_pivot(ui->screen_home_img_weather, 50,50);
    lv_image_set_rotation(ui->screen_home_img_weather, 0);

    //Write style for screen_home_img_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_home_img_weather, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_home_img_weather, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_home.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_home);

    //Init events for screen.
    events_init_screen_home(ui);
}
