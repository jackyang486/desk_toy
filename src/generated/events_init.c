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

static void screen_weather_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_weather_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_weather (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_weather, screen_weather_event_handler, LV_EVENT_ALL, ui);
}

static void screen_photo_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_photo_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_photo (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_photo, screen_photo_event_handler, LV_EVENT_ALL, ui);
}

static void screen_wifi_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_wifi_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

static void screen_wifi_connect_button_event_handler (lv_event_t *e)
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

static void screen_wifi_cancel_button_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_wifi_form, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_wifi (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_wifi, screen_wifi_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_wifi_connect_button, screen_wifi_connect_button_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_wifi_cancel_button, screen_wifi_cancel_button_event_handler, LV_EVENT_ALL, ui);
}

static void screen_music_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_music_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

static void screen_music_rnd_img_event_handler (lv_event_t *e)
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

static void screen_music_next_img_event_handler (lv_event_t *e)
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

static void screen_music_pre_img_event_handler (lv_event_t *e)
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

static void screen_music_pause_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_music_pause_img,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_music_playing_img,LV_OBJ_FLAG_HIDDEN);
        music_play();
        break;
    }
    default:
        break;
    }
}

static void screen_music_loop_img_event_handler (lv_event_t *e)
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

static void screen_music_playing_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.screen_music_playing_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.screen_music_pause_img, LV_OBJ_FLAG_HIDDEN);
        music_play();
        break;
    }
    default:
        break;
    }
}

static void screen_music_btn_list_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        if ((lv_obj_get_y(guider_ui.screen_music_player) == 0)) {
            ui_animation(guider_ui.screen_music_player, 1000, 0, lv_obj_get_y(guider_ui.screen_music_player), -285, &lv_anim_path_linear, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
            lv_label_set_text(guider_ui.screen_music_btn_list_label,"" LV_SYMBOL_UP " ");
        } else {
            ui_animation(guider_ui.screen_music_player, 1000, 0, lv_obj_get_y(guider_ui.screen_music_player), 0, &lv_anim_path_linear, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
            lv_label_set_text(guider_ui.screen_music_btn_list_label,"" LV_SYMBOL_DOWN " ");
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_music (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_music, screen_music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_rnd_img, screen_music_rnd_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_next_img, screen_music_next_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_pre_img, screen_music_pre_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_pause_img, screen_music_pause_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_loop_img, screen_music_loop_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_playing_img, screen_music_playing_img_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_music_btn_list, screen_music_btn_list_event_handler, LV_EVENT_ALL, ui);
}

static void screen_robot_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {

        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_robot_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_robot (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_robot, screen_robot_event_handler, LV_EVENT_ALL, ui);
}

static void screen_setting_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_setting_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_setting (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_setting, screen_setting_event_handler, LV_EVENT_ALL, ui);
}

static void screen_radio_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_radio_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_radio (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_radio, screen_radio_event_handler, LV_EVENT_ALL, ui);
}

static void screen_mouse_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_mouse_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_mouse (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_mouse, screen_mouse_event_handler, LV_EVENT_ALL, ui);
}

static void screen_anim_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    case LV_EVENT_SCREEN_UNLOAD_START:
    {
        screen_load_execute(lv_screen_active(),UNLOAD);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_load_scr_animation(&guider_ui, &guider_ui.screen_home, guider_ui.screen_home_del, &guider_ui.screen_anim_del, setup_scr_screen_home, LV_SCR_LOAD_ANIM_OVER_RIGHT, 100, 200, true, false);
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

void events_init_screen_anim (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_anim, screen_anim_event_handler, LV_EVENT_ALL, ui);
}

static void screen_home_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        screen_load_execute(lv_screen_active(),LOADED);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_lottie_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_anim, guider_ui.screen_anim_del, &guider_ui.screen_home_del, setup_scr_screen_anim, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_mouse_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_mouse, guider_ui.screen_mouse_del, &guider_ui.screen_home_del, setup_scr_screen_mouse, LV_SCR_LOAD_ANIM_NONE, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_radio_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_radio, guider_ui.screen_radio_del, &guider_ui.screen_home_del, setup_scr_screen_radio, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_wifi_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_wifi, guider_ui.screen_wifi_del, &guider_ui.screen_home_del, setup_scr_screen_wifi, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_setting_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_setting, guider_ui.screen_setting_del, &guider_ui.screen_home_del, setup_scr_screen_setting, LV_SCR_LOAD_ANIM_NONE, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_chatbot_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_robot, guider_ui.screen_robot_del, &guider_ui.screen_home_del, setup_scr_screen_robot, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_music_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_music, guider_ui.screen_music_del, &guider_ui.screen_home_del, setup_scr_screen_music, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_album_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_photo, guider_ui.screen_photo_del, &guider_ui.screen_home_del, setup_scr_screen_photo, LV_SCR_LOAD_ANIM_OVER_LEFT, 100, 200, true, false);
        break;
    }
    default:
        break;
    }
}

static void screen_home_img_weather_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.screen_weather, guider_ui.screen_weather_del, &guider_ui.screen_home_del, setup_scr_screen_weather, LV_SCR_LOAD_ANIM_OVER_LEFT, 200, 200, true, false);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_home (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_home, screen_home_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_lottie, screen_home_img_lottie_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_mouse, screen_home_img_mouse_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_radio, screen_home_img_radio_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_wifi, screen_home_img_wifi_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_setting, screen_home_img_setting_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_chatbot, screen_home_img_chatbot_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_music, screen_home_img_music_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_album, screen_home_img_album_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_home_img_weather, screen_home_img_weather_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
