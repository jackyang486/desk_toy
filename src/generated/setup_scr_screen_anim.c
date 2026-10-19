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



void setup_scr_screen_anim(lv_ui *ui)
{
    //Write codes screen_anim
    ui->screen_anim = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_anim, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_anim, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_anim, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_anim, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_anim_rlottie_1
    //ui->screen_anim_rlottie_1 = lv_rlottie_create_from_raw(ui->screen_anim, 240, 240, (const void *)lottie_CuteJumping);
    //lv_obj_set_pos(ui->screen_anim_rlottie_1, 0, 30);
    //lv_obj_set_size(ui->screen_anim_rlottie_1, 240, 240);

    //Write style for screen_anim_rlottie_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_anim_rlottie_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_anim.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_anim);

    //Init events for screen.
    events_init_screen_anim(ui);
}
