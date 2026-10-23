/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *screen_weather;
	bool screen_weather_del;
	lv_obj_t *screen_weather_animimg_astronaut;
	lv_obj_t *screen_weather_cont_second_2;
	lv_obj_t *screen_weather_img_second_1;
	lv_obj_t *screen_weather_img_second_shi;
	lv_obj_t *screen_weather_cont_second_1;
	lv_obj_t *screen_weather_img_second_2;
	lv_obj_t *screen_weather_img_second_ge;
	lv_obj_t *screen_weather_cont_minute_2;
	lv_obj_t *screen_weather_img_minute_2;
	lv_obj_t *screen_weather_img_minute_shi;
	lv_obj_t *screen_weather_cont_minute_1;
	lv_obj_t *screen_weather_img_minute_1;
	lv_obj_t *screen_weather_img_minute_ge;
	lv_obj_t *screen_weather_cont_hour_2;
	lv_obj_t *screen_weather_img_hour_2;
	lv_obj_t *screen_weather_img_hour_shi;
	lv_obj_t *screen_weather_cont_hour_1;
	lv_obj_t *screen_weather_img_hour_1;
	lv_obj_t *screen_weather_img_hour_ge;
	lv_obj_t *screen_weather_img_weather0;
	lv_obj_t *screen_weather_label_qulaity;
	lv_obj_t *screen_weather_label_year_month;
	lv_obj_t *screen_weather_label_week;
	lv_obj_t *screen_weather_img_temperature;
	lv_obj_t *screen_weather_img_humidity;
	lv_obj_t *screen_weather_bar_hum;
	lv_obj_t *screen_weather_bar_temp;
	lv_obj_t *screen_weather_label_hum;
	lv_obj_t *screen_weather_label_temp;
	lv_obj_t *screen_weather_label_weather_banner;
	lv_obj_t *screen_weather_label_city;
	lv_obj_t *screen_weather_wifi_icon;
	lv_obj_t *screen_photo;
	bool screen_photo_del;
	lv_obj_t *screen_photo_view;
	lv_obj_t *screen_photo_right_arrow;
	lv_obj_t *screen_photo_right_arrow_label;
	lv_obj_t *screen_photo_btn_delete;
	lv_obj_t *screen_photo_btn_delete_label;
	lv_obj_t *screen_photo_left_arrow;
	lv_obj_t *screen_photo_left_arrow_label;
	lv_obj_t *screen_photo_btn_loop;
	lv_obj_t *screen_photo_btn_loop_label;
	lv_obj_t *screen_photo_btn_web;
	lv_obj_t *screen_photo_btn_web_label;
	lv_obj_t *screen_wifi;
	bool screen_wifi_del;
	lv_obj_t *screen_wifi_list;
	lv_obj_t *screen_wifi_list_item0;
	lv_obj_t *screen_wifi_form;
	lv_obj_t *screen_wifi_wifi_name_label;
	lv_obj_t *screen_wifi_wifi_pass_label;
	lv_obj_t *screen_wifi_wifi_pass_value;
	lv_obj_t *screen_wifi_connect_button;
	lv_obj_t *screen_wifi_connect_button_label;
	lv_obj_t *screen_wifi_cancel_button;
	lv_obj_t *screen_wifi_cancel_button_label;
	lv_obj_t *screen_wifi_wifi_name_value;
	lv_obj_t *screen_music;
	bool screen_music_del;
	lv_obj_t *screen_music_player_list;
	lv_obj_t *screen_music_list_music;
	lv_obj_t *screen_music_player;
	lv_obj_t *screen_music_rnd_img;
	lv_obj_t *screen_music_next_img;
	lv_obj_t *screen_music_pre_img;
	lv_obj_t *screen_music_pre_img_label;
	lv_obj_t *screen_music_pause_img;
	lv_obj_t *screen_music_pause_img_label;
	lv_obj_t *screen_music_album_img;
	lv_obj_t *screen_music_top_img;
	lv_obj_t *screen_music_bottom_img;
	lv_obj_t *screen_music_slider;
	lv_obj_t *screen_music_label_music_name;
	lv_obj_t *screen_music_label_slider_time;
	lv_obj_t *screen_music_loop_img;
	lv_obj_t *screen_music_playing_img;
	lv_obj_t *screen_music_playing_img_label;
	lv_obj_t *screen_music_btn_list;
	lv_obj_t *screen_music_btn_list_label;
	lv_obj_t *screen_robot;
	bool screen_robot_del;
	lv_obj_t *screen_robot_cont_label;
	lv_obj_t *screen_robot_label_status;
	lv_obj_t *screen_robot_cont_msg;
	lv_obj_t *screen_setting;
	bool screen_setting_del;
	lv_obj_t *screen_setting_cont_light_switch;
	lv_obj_t *screen_setting_label_light_switch_title;
	lv_obj_t *screen_setting_slider_switch;
	lv_obj_t *screen_setting_label_light_switch_img;
	lv_obj_t *screen_setting_label_minute;
	lv_obj_t *screen_setting_cont_volume;
	lv_obj_t *screen_setting_label_vol_title;
	lv_obj_t *screen_setting_slider_volume;
	lv_obj_t *screen_setting_label_vol_img;
	lv_obj_t *screen_setting_label_vol_percent;
	lv_obj_t *screen_setting_cont_bright;
	lv_obj_t *screen_setting_label_bright_title;
	lv_obj_t *screen_setting_slider_bright;
	lv_obj_t *screen_setting_label_bright_img;
	lv_obj_t *screen_setting_label_bright_percent;
	lv_obj_t *screen_radio;
	bool screen_radio_del;
	lv_obj_t *screen_radio_list_radio;
	lv_obj_t *screen_mouse;
	bool screen_mouse_del;
	lv_obj_t *screen_mouse_btn_right;
	lv_obj_t *screen_mouse_btn_right_label;
	lv_obj_t *screen_mouse_btn_left;
	lv_obj_t *screen_mouse_btn_left_label;
	lv_obj_t *screen_anim;
	bool screen_anim_del;
	lv_obj_t *screen_anim_rlottie_1;
	lv_obj_t *screen_home;
	bool screen_home_del;
	lv_obj_t *screen_home_img_lottie;
	lv_obj_t *screen_home_img_mouse;
	lv_obj_t *screen_home_img_radio;
	lv_obj_t *screen_home_img_wifi;
	lv_obj_t *screen_home_img_setting;
	lv_obj_t *screen_home_img_chatbot;
	lv_obj_t *screen_home_img_music;
	lv_obj_t *screen_home_img_album;
	lv_obj_t *screen_home_img_game;
	lv_obj_t *screen_home_img_weather;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_weather(lv_ui *ui);
void setup_scr_screen_photo(lv_ui *ui);
void setup_scr_screen_wifi(lv_ui *ui);
void setup_scr_screen_music(lv_ui *ui);
void setup_scr_screen_robot(lv_ui *ui);
void setup_scr_screen_setting(lv_ui *ui);
void setup_scr_screen_radio(lv_ui *ui);
void setup_scr_screen_mouse(lv_ui *ui);
void setup_scr_screen_anim(lv_ui *ui);
void setup_scr_screen_home(lv_ui *ui);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti0);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti1);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti2);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti3);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti4);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti5);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti6);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti7);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti8);
LV_IMAGE_DECLARE(screen_weather_animimg_astronauti9);
LV_IMAGE_DECLARE(_W_1830_i0_RGB565A8_18x30);
LV_IMAGE_DECLARE(_O_3660_i0_RGB565A8_36x60);
LV_IMAGE_DECLARE(_W_3660_i0_RGB565A8_36x60);
LV_IMAGE_DECLARE(W_1830_i0);
LV_IMAGE_DECLARE(W_1830_i1);
LV_IMAGE_DECLARE(W_1830_i2);
LV_IMAGE_DECLARE(W_1830_i3);
LV_IMAGE_DECLARE(W_1830_i4);
LV_IMAGE_DECLARE(W_1830_i5);
LV_IMAGE_DECLARE(W_1830_i6);
LV_IMAGE_DECLARE(W_1830_i7);
LV_IMAGE_DECLARE(W_1830_i8);
LV_IMAGE_DECLARE(W_1830_i9);
LV_IMAGE_DECLARE(O_3660_i0);
LV_IMAGE_DECLARE(O_3660_i1);
LV_IMAGE_DECLARE(O_3660_i2);
LV_IMAGE_DECLARE(O_3660_i3);
LV_IMAGE_DECLARE(O_3660_i4);
LV_IMAGE_DECLARE(O_3660_i5);
LV_IMAGE_DECLARE(O_3660_i6);
LV_IMAGE_DECLARE(O_3660_i7);
LV_IMAGE_DECLARE(O_3660_i8);
LV_IMAGE_DECLARE(O_3660_i9);
LV_IMAGE_DECLARE(W_3660_i0);
LV_IMAGE_DECLARE(W_3660_i1);
LV_IMAGE_DECLARE(W_3660_i2);
LV_IMAGE_DECLARE(W_3660_i3);
LV_IMAGE_DECLARE(W_3660_i4);
LV_IMAGE_DECLARE(W_3660_i5);
LV_IMAGE_DECLARE(W_3660_i6);
LV_IMAGE_DECLARE(W_3660_i7);
LV_IMAGE_DECLARE(W_3660_i8);
LV_IMAGE_DECLARE(W_3660_i9);
LV_IMAGE_DECLARE(_t0_RGB565A8_60x60);
LV_IMAGE_DECLARE(_NOWIFI_RGB565A8_20x20);
LV_IMAGE_DECLARE(_temperature_RGB565A8_24x24);
LV_IMAGE_DECLARE(_humidity_RGB565A8_24x24);
LV_IMAGE_DECLARE(_wifiEnbale_RGB565A8_20x20);
LV_IMAGE_DECLARE(_btn_rnd_RGB565A8_25x25);
LV_IMAGE_DECLARE(_btn_next_RGB565A8_40x40);
LV_IMAGE_DECLARE(_btn_prev_RGB565A8_40x40);
LV_IMAGE_DECLARE(_btn_pause_RGB565A8_40x40);
LV_IMAGE_DECLARE(_cover_2_RGB565A8_100x100);
LV_IMAGE_DECLARE(_wave_top_RGB565A8_240x34);
LV_IMAGE_DECLARE(_wave_bottom_RGB565A8_240x34);
LV_IMAGE_DECLARE(_btn_loop_RGB565A8_25x25);
LV_IMAGE_DECLARE(_btn_play_RGB565A8_40x40);
LV_IMAGE_DECLARE(Music);
LV_IMAGE_DECLARE(_game_RGB565A8_50x50);

LV_IMG_DECLARE(t0);
LV_IMG_DECLARE(t1);
LV_IMG_DECLARE(t2);
LV_IMG_DECLARE(t3);
LV_IMG_DECLARE(t4);
LV_IMG_DECLARE(t5);
LV_IMG_DECLARE(t6);
LV_IMG_DECLARE(t7);
LV_IMG_DECLARE(t9);
LV_IMG_DECLARE(t11);
LV_IMG_DECLARE(t13);
LV_IMG_DECLARE(t14);
LV_IMG_DECLARE(t15);
LV_IMG_DECLARE(t16);
LV_IMG_DECLARE(t18);
LV_IMG_DECLARE(t19);
LV_IMG_DECLARE(t20);
LV_IMG_DECLARE(t26);
LV_IMG_DECLARE(t29);
LV_IMG_DECLARE(t30);
LV_IMG_DECLARE(t31);
LV_IMG_DECLARE(t53);
LV_IMG_DECLARE(t99);

LV_IMAGE_DECLARE(_confident_240x320_RGB565A8_240x320);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_CuteJumping);
LV_LOT_DECLARE(lottie_happy);
LV_IMAGE_DECLARE(_photo_img2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_mouse_RGB565A8_50x50);
LV_IMAGE_DECLARE(_radio_img2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_wifi_img2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_setup_img2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_robot_img2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_music_img2_RGB565A8_50x50);
LV_IMAGE_DECLARE(_Weather_RGB565A8_50x50);

LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_20)
LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_montserratMedium_8)
LV_FONT_DECLARE(lv_font_montserratMedium_20)


#ifdef __cplusplus
}
#endif
#endif
