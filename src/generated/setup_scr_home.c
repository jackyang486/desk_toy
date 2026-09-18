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



void setup_scr_home(lv_ui *ui)
{
    //Write codes home
    ui->home = lv_obj_create(NULL);
    lv_obj_set_size(ui->home, 240, 320);
    lv_obj_set_scrollbar_mode(ui->home, LV_SCROLLBAR_MODE_OFF);

    //Write style for home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home, lv_color_hex(0xd9f9e3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_photo
    ui->home_img_photo = lv_img_create(ui->home);
    lv_img_set_src(ui->home_img_photo, &_photo_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_photo, 50,50);
    lv_img_set_angle(ui->home_img_photo, 0);
    lv_obj_set_pos(ui->home_img_photo, 10, 10);
    lv_obj_set_size(ui->home_img_photo, 50, 50);
    lv_obj_add_flag(ui->home_img_photo, LV_OBJ_FLAG_CLICKABLE);

    //Write style for home_img_photo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_photo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_photo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_photo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_photo, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_music
    ui->home_img_music = lv_img_create(ui->home);
    lv_obj_add_flag(ui->home_img_music, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->home_img_music, &_music_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_music, 50,50);
    lv_img_set_angle(ui->home_img_music, 0);
    lv_obj_set_pos(ui->home_img_music, 89, 10);
    lv_obj_set_size(ui->home_img_music, 50, 50);

    //Write style for home_img_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_music, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_wifi
    ui->home_img_wifi = lv_img_create(ui->home);
    lv_obj_add_flag(ui->home_img_wifi, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->home_img_wifi, &_wifi_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_wifi, 50,50);
    lv_img_set_angle(ui->home_img_wifi, 0);
    lv_obj_set_pos(ui->home_img_wifi, 163, 10);
    lv_obj_set_size(ui->home_img_wifi, 50, 50);

    //Write style for home_img_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_wifi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_wifi, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_robot
    ui->home_img_robot = lv_img_create(ui->home);
    lv_obj_add_flag(ui->home_img_robot, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->home_img_robot, &_robot_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_robot, 50,50);
    lv_img_set_angle(ui->home_img_robot, 0);
    lv_obj_set_pos(ui->home_img_robot, 10, 65);
    lv_obj_set_size(ui->home_img_robot, 50, 50);

    //Write style for home_img_robot, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_robot, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_robot, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_robot, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_robot, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_setting
    ui->home_img_setting = lv_img_create(ui->home);
    lv_obj_add_flag(ui->home_img_setting, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->home_img_setting, &_setup_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_setting, 50,50);
    lv_img_set_angle(ui->home_img_setting, 0);
    lv_obj_set_pos(ui->home_img_setting, 89, 65);
    lv_obj_set_size(ui->home_img_setting, 50, 50);

    //Write style for home_img_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_setting, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_setting, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_radio
    ui->home_img_radio = lv_img_create(ui->home);
    lv_obj_add_flag(ui->home_img_radio, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->home_img_radio, &_radio_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_radio, 50,50);
    lv_img_set_angle(ui->home_img_radio, 0);
    lv_obj_set_pos(ui->home_img_radio, 163, 65);
    lv_obj_set_size(ui->home_img_radio, 50, 50);

    //Write style for home_img_radio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_radio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_radio, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_radio, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_radio, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_img_online
    ui->home_img_online = lv_img_create(ui->home);
    lv_obj_add_flag(ui->home_img_online, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->home_img_online, &_photo_img2_alpha_50x50);
    lv_img_set_pivot(ui->home_img_online, 50,50);
    lv_img_set_angle(ui->home_img_online, 0);
    lv_obj_set_pos(ui->home_img_online, 10, 122);
    lv_obj_set_size(ui->home_img_online, 50, 50);

    //Write style for home_img_online, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->home_img_online, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->home_img_online, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_img_online, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->home_img_online, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of home.


    //Update current screen layout.
    lv_obj_update_layout(ui->home);

    //Init events for screen.
    events_init_home(ui);
}
