/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../generated/gui_guider.h"
 LV_FONT_DECLARE(font_puhui_16_4);
typedef enum {
    UNLOAD,
    LOADED
} SCREEN_LOAD_STAUS;
typedef enum {
    PRESSED,
    RELEASED,
} PRESS_STATUS;

typedef void (*screen_load_callback_t)(SCREEN_LOAD_STAUS loadStatus);

typedef struct {
    screen_load_callback_t main_screen_load;
    screen_load_callback_t wifi_screen_load;
    screen_load_callback_t photo_screen_load;
    screen_load_callback_t robot_screen_load;
    screen_load_callback_t fly_mouse_screen_load;
    screen_load_callback_t music_screen_load;
    screen_load_callback_t setting_screen_load;
    screen_load_callback_t radio_screen_load;
    screen_load_callback_t lottie_screen_load;
    screen_load_callback_t weather_screen_load;
    screen_load_callback_t game_screen_load;
}screen_load;

typedef struct {
    lv_obj_t  *wifi_img;
    lv_obj_t  *time_label;
    lv_obj_t  *battery_label;
    lv_obj_t  *pageHeadCont;
}Page_Head;

extern Page_Head  page_head;

void screen_load_register(screen_load *screen_load);
void screen_load_execute(lv_obj_t *obj,SCREEN_LOAD_STAUS loadStatus);

void connect_wifi(void);
void custom_init(lv_ui *ui);
void click_right_btn(void);
void click_left_btn(void);
void image_carousel(void);
void music_prev(void);
void music_next(void);
void music_play(void);
void music_random(void);
void music_sequence(void);
void music_play_init(void);
void lv_demo_music_resume(void);
void lv_demo_music_pause(void);
void weather_play_init(void);
void show_toast(lv_obj_t *parent, const char *msg);
void photo_album_open_local(void);
void photo_album_open_online(void);
#ifdef __cplusplus
}


#endif
#endif /* EVENT_CB_H_ */
