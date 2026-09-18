/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "../custom/custom.h"

static void screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.screen,LOADED);
        lv_animimg_start(guider_ui.screen_animimg_1);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.screen,UNLOAD);
        break;
    }
    default:
        break;
    }
}

static void screen_main_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_anim_del(guider_ui.screen_animimg_1,NULL);
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.screen_del, setup_scr_home, LV_SCR_LOAD_ANIM_FADE_ON, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen, screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_main, screen_main_event_handler, LV_EVENT_ALL, ui);
}

static void wifi_win_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.wifi_win,LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.wifi_win,UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.wifi_win_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void wifi_win_connect_button_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        connect_wifi();
        break;
    }
    default:
        break;
    }
}

static void wifi_win_cancel_button_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.wifi_win_form, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.wifi_win_list, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_wifi_win (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->wifi_win, wifi_win_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->wifi_win_connect_button, wifi_win_connect_button_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->wifi_win_cancel_button, wifi_win_cancel_button_event_handler, LV_EVENT_ALL, ui);
}

static void home_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.home_del, setup_scr_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void home_img_photo_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.photo_win, guider_ui.photo_win_del, &guider_ui.home_del, setup_scr_photo_win, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_img_music_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.music, guider_ui.music_del, &guider_ui.home_del, setup_scr_music, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_img_wifi_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.wifi_win, guider_ui.wifi_win_del, &guider_ui.home_del, setup_scr_wifi_win, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_img_robot_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.robot_win, guider_ui.robot_win_del, &guider_ui.home_del, setup_scr_robot_win, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_img_setting_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.setting_win, guider_ui.setting_win_del, &guider_ui.home_del, setup_scr_setting_win, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_img_radio_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.radio_win, guider_ui.radio_win_del, &guider_ui.home_del, setup_scr_radio_win, LV_SCR_LOAD_ANIM_OVER_LEFT, 1000, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_img_online_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.online_img_win, guider_ui.online_img_win_del, &guider_ui.home_del, setup_scr_online_img_win, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_home (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->home, home_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_photo, home_img_photo_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_music, home_img_music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_wifi, home_img_wifi_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_robot, home_img_robot_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_setting, home_img_setting_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_radio, home_img_radio_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_img_online, home_img_online_event_handler, LV_EVENT_ALL, ui);
}

static void photo_win_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.photo_win,LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.photo_win,UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.photo_win_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void photo_win_view_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        image_carousel();
        break;
    }
    default:
        break;
    }
}

static void photo_win_left_arrow_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        click_left_btn();
        break;
    }
    default:
        break;
    }
}

static void photo_win_right_arrow_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        click_right_btn();
        break;
    }
    default:
        break;
    }
}

void events_init_photo_win (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->photo_win, photo_win_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->photo_win_view, photo_win_view_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->photo_win_left_arrow, photo_win_left_arrow_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->photo_win_right_arrow, photo_win_right_arrow_event_handler, LV_EVENT_ALL, ui);
}

static void music_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.music,LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.music,UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.music_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void music_playing_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.music_playing_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.music_pause_img, LV_OBJ_FLAG_HIDDEN);
        music_play();
        break;
    }
    default:
        break;
    }
}

static void music_loop_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_sequence();
        break;
    }
    default:
        break;
    }
}

static void music_rnd_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_random();
        break;
    }
    default:
        break;
    }
}

static void music_next_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_next();
        break;
    }
    default:
        break;
    }
}

static void music_pre_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        music_prev();
        break;
    }
    default:
        break;
    }
}

static void music_pause_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.music_pause_img,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.music_playing_img,LV_OBJ_FLAG_HIDDEN);
        music_play();
        break;
    }
    default:
        break;
    }
}

static void music_imgbtn_list_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.music_player,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.music_player_list,LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void music_label_list_name_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.music_player_list,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.music_player,LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_music (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->music, music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_playing_img, music_playing_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_loop_img, music_loop_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_rnd_img, music_rnd_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_next_img, music_next_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_pre_img, music_pre_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_pause_img, music_pause_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_imgbtn_list, music_imgbtn_list_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->music_label_list_name, music_label_list_name_event_handler, LV_EVENT_ALL, ui);
}

static void robot_win_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.robot_win,LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.robot_win,UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.robot_win_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_robot_win (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->robot_win, robot_win_event_handler, LV_EVENT_ALL, ui);
}

static void setting_win_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.setting_win,LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.setting_win,UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.setting_win_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_setting_win (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->setting_win, setting_win_event_handler, LV_EVENT_ALL, ui);
}

static void radio_win_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.radio_win,LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.radio_win,UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.radio_win_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 1000, 200, true, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_radio_win (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->radio_win, radio_win_event_handler, LV_EVENT_ALL, ui);
}

static void online_img_win_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(guider_ui.online_img_win,UNLOAD);
        break;
    }
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(guider_ui.online_img_win,LOADED);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_get_act());
            ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.online_img_win_del, setup_scr_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 200, false, true);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_online_img_win (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->online_img_win, online_img_win_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
