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



void setup_scr_online_img_win(lv_ui *ui)
{
    //Write codes online_img_win
    ui->online_img_win = lv_obj_create(NULL);
    lv_obj_set_size(ui->online_img_win, 240, 320);
    lv_obj_set_scrollbar_mode(ui->online_img_win, LV_SCROLLBAR_MODE_OFF);

    //Write style for online_img_win, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->online_img_win, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes online_img_win_img_online
    ui->online_img_win_img_online = lv_img_create(ui->online_img_win);
    lv_obj_add_flag(ui->online_img_win_img_online, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_pivot(ui->online_img_win_img_online, 50,50);
    lv_img_set_angle(ui->online_img_win_img_online, 0);
    lv_obj_set_pos(ui->online_img_win_img_online, 0, 0);
    lv_obj_set_size(ui->online_img_win_img_online, 240, 320);

    //Write style for online_img_win_img_online, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->online_img_win_img_online, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->online_img_win_img_online, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->online_img_win_img_online, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->online_img_win_img_online, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of online_img_win.


    //Update current screen layout.
    lv_obj_update_layout(ui->online_img_win);

    //Init events for screen.
    events_init_online_img_win(ui);
}
