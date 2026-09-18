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
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_main;
	lv_obj_t *screen_battery;
	lv_obj_t *screen_wifi_icon;
	lv_obj_t *screen_no_wifi_icon;
	lv_obj_t *screen_ip_address;
	lv_obj_t *screen_img_humidity;
	lv_obj_t *screen_img_temperature;
	lv_obj_t *screen_bar_temp;
	lv_obj_t *screen_bar_hum;
	lv_obj_t *screen_label_temp;
	lv_obj_t *screen_label_hum;
	lv_obj_t *screen_label_qulaity;
	lv_obj_t *screen_img_weather0;
	lv_obj_t *screen_label_weather_banner;
	lv_obj_t *screen_label_city;
	lv_obj_t *screen_label_week;
	lv_obj_t *screen_label_year_month;
	lv_obj_t *screen_img_hour_9;
	lv_obj_t *screen_img_hour_8;
	lv_obj_t *screen_img_hour_7;
	lv_obj_t *screen_img_hour_6;
	lv_obj_t *screen_img_hour_5;
	lv_obj_t *screen_img_hour_4;
	lv_obj_t *screen_img_hour_3;
	lv_obj_t *screen_img_hour_0;
	lv_obj_t *screen_img_minute9;
	lv_obj_t *screen_img_minute_8;
	lv_obj_t *screen_img_minute_7;
	lv_obj_t *screen_img_minute_6;
	lv_obj_t *screen_img_minute_5;
	lv_obj_t *screen_img_minute_4;
	lv_obj_t *screen_img_minute_3;
	lv_obj_t *screen_img_minute_0;
	lv_obj_t *screen_img_second_9;
	lv_obj_t *screen_img_second_8;
	lv_obj_t *screen_img_second_7;
	lv_obj_t *screen_img_second_6;
	lv_obj_t *screen_img_second_5;
	lv_obj_t *screen_img_second_4;
	lv_obj_t *screen_img_second_3;
	lv_obj_t *screen_img_second_0;
	lv_obj_t *screen_cont_second_1;
	lv_obj_t *screen_img_second_1;
	lv_obj_t *screen_img_second_ge;
	lv_obj_t *screen_cont_second_2;
	lv_obj_t *screen_img_second_2;
	lv_obj_t *screen_img_second_shi;
	lv_obj_t *screen_cont_minute_1;
	lv_obj_t *screen_img_minute_1;
	lv_obj_t *screen_img_minute_ge;
	lv_obj_t *screen_cont_minute_2;
	lv_obj_t *screen_img_minute_2;
	lv_obj_t *screen_img_minute_shi;
	lv_obj_t *screen_cont_hour_2;
	lv_obj_t *screen_img_hour_2;
	lv_obj_t *screen_img_hour_shi;
	lv_obj_t *screen_cont_hour_1;
	lv_obj_t *screen_img_hour_1;
	lv_obj_t *screen_img_hour_ge;
	lv_obj_t *screen_animimg_1;
	lv_obj_t *wifi_win;
	bool wifi_win_del;
	lv_obj_t *wifi_win_cont_head;
	lv_obj_t *wifi_win_list;
	lv_obj_t *wifi_win_message_label;
	lv_obj_t *wifi_win_form;
	lv_obj_t *wifi_win_wifi_name_label;
	lv_obj_t *wifi_win_wifi_name_value;
	lv_obj_t *wifi_win_wifi_pass_label;
	lv_obj_t *wifi_win_wifi_pass_value;
	lv_obj_t *wifi_win_connect_button;
	lv_obj_t *wifi_win_connect_button_label;
	lv_obj_t *wifi_win_cancel_button;
	lv_obj_t *wifi_win_cancel_button_label;
	lv_obj_t *wifi_win_list_title;
	lv_obj_t *home;
	bool home_del;
	lv_obj_t *home_img_photo;
	lv_obj_t *home_img_music;
	lv_obj_t *home_img_wifi;
	lv_obj_t *home_img_robot;
	lv_obj_t *home_img_setting;
	lv_obj_t *home_img_radio;
	lv_obj_t *home_img_online;
	lv_obj_t *photo_win;
	bool photo_win_del;
	lv_obj_t *photo_win_view;
	lv_obj_t *photo_win_left_arrow;
	lv_obj_t *photo_win_left_arrow_label;
	lv_obj_t *photo_win_right_arrow;
	lv_obj_t *photo_win_right_arrow_label;
	lv_obj_t *photo_win_msgbox_del;
	lv_obj_t *music;
	bool music_del;
	lv_obj_t *music_player;
	lv_obj_t *music_playing_img;
	lv_obj_t *music_playing_img_label;
	lv_obj_t *music_loop_img;
	lv_obj_t *music_rnd_img;
	lv_obj_t *music_next_img;
	lv_obj_t *music_next_img_label;
	lv_obj_t *music_pre_img;
	lv_obj_t *music_pre_img_label;
	lv_obj_t *music_pause_img;
	lv_obj_t *music_pause_img_label;
	lv_obj_t *music_album_img;
	lv_obj_t *music_top_img;
	lv_obj_t *music_bottom_img;
	lv_obj_t *music_slider;
	lv_obj_t *music_label_music_name;
	lv_obj_t *music_label_slider_time;
	lv_obj_t *music_imgbtn_list;
	lv_obj_t *music_imgbtn_list_label;
	lv_obj_t *music_player_list;
	lv_obj_t *music_list_music;
	lv_obj_t *music_list_music_item0;
	lv_obj_t *music_list_music_item1;
	lv_obj_t *music_label_list_name;
	lv_obj_t *robot_win;
	bool robot_win_del;
	lv_obj_t *robot_win_cont_msg;
	lv_obj_t *robot_win_cont_1;
	lv_obj_t *robot_win_label_status;
	lv_obj_t *setting_win;
	bool setting_win_del;
	lv_obj_t *setting_win_cont_volume;
	lv_obj_t *setting_win_label_vol_title;
	lv_obj_t *setting_win_slider_volume;
	lv_obj_t *setting_win_label_vol_percent;
	lv_obj_t *setting_win_label_vol_img;
	lv_obj_t *setting_win_cont_bright;
	lv_obj_t *setting_win_label_bright_title;
	lv_obj_t *setting_win_slider_bright;
	lv_obj_t *setting_win_label_bright_percent;
	lv_obj_t *setting_win_label_bright_img;
	lv_obj_t *setting_win_cont_light_switch;
	lv_obj_t *setting_win_label_light_switch_title;
	lv_obj_t *setting_win_label_light_switch_img;
	lv_obj_t *setting_win_slider_switch;
	lv_obj_t *setting_win_label_minute;
	lv_obj_t *setting_win_label_setting;
	lv_obj_t *radio_win;
	bool radio_win_del;
	lv_obj_t *radio_win_list_radio;
	lv_obj_t *radio_win_label_1;
	lv_obj_t *online_img_win;
	bool online_img_win_del;
	lv_obj_t *online_img_win_img_online;
	lv_obj_t *g_kb_top_layer;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
void setup_scr_wifi_win(lv_ui *ui);
void setup_scr_home(lv_ui *ui);
void setup_scr_photo_win(lv_ui *ui);
void setup_scr_music(lv_ui *ui);
void setup_scr_robot_win(lv_ui *ui);
void setup_scr_setting_win(lv_ui *ui);
void setup_scr_radio_win(lv_ui *ui);
void setup_scr_online_img_win(lv_ui *ui);
LV_IMG_DECLARE(_wifiEnbale_alpha_20x20);
LV_IMG_DECLARE(_NOWIFI_alpha_20x20);
LV_IMG_DECLARE(_humidity_24x24);
LV_IMG_DECLARE(_temperature_24x24);
LV_IMG_DECLARE(_W_3660_i9_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i8_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i7_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i6_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i5_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i4_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i3_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i0_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i9_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i8_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i7_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i6_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i5_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i4_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i3_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i0_alpha_36x60);
LV_IMG_DECLARE(_W_1830_i9_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i8_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i7_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i6_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i5_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i4_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i3_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i0_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i1_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i0_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i2_alpha_18x30);
LV_IMG_DECLARE(_W_1830_i0_alpha_18x30);
LV_IMG_DECLARE(_O_3660_i1_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i0_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i2_alpha_36x60);
LV_IMG_DECLARE(_O_3660_i0_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i2_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i0_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i1_alpha_36x60);
LV_IMG_DECLARE(_W_3660_i0_alpha_36x60);
#include "extra/widgets/animimg/lv_animimg.h"
LV_IMG_DECLARE(screen_animimg_1i0);
LV_IMG_DECLARE(screen_animimg_1i1);
LV_IMG_DECLARE(screen_animimg_1i2);
LV_IMG_DECLARE(screen_animimg_1i3);
LV_IMG_DECLARE(screen_animimg_1i4);
LV_IMG_DECLARE(screen_animimg_1i5);
LV_IMG_DECLARE(screen_animimg_1i6);
LV_IMG_DECLARE(screen_animimg_1i7);
LV_IMG_DECLARE(screen_animimg_1i8);
LV_IMG_DECLARE(screen_animimg_1i9);
LV_IMG_DECLARE(_photo_img2_alpha_50x50);
LV_IMG_DECLARE(_music_img2_alpha_50x50);
LV_IMG_DECLARE(_wifi_img2_alpha_50x50);
LV_IMG_DECLARE(_robot_img2_alpha_50x50);
LV_IMG_DECLARE(_setup_img2_alpha_50x50);
LV_IMG_DECLARE(_radio_img2_alpha_50x50);
LV_IMG_DECLARE(_photo_img2_alpha_50x50);
LV_IMG_DECLARE(_btn_play_alpha_40x40);
LV_IMG_DECLARE(_btn_loop_alpha_20x20);
LV_IMG_DECLARE(_btn_rnd_alpha_20x20);
LV_IMG_DECLARE(_btn_next_alpha_35x35);
LV_IMG_DECLARE(_btn_prev_alpha_35x35);
LV_IMG_DECLARE(_btn_pause_alpha_40x40);
LV_IMG_DECLARE(_cover_1_100x100);
LV_IMG_DECLARE(_wave_top_240x34);
LV_IMG_DECLARE(_wave_bottom_240x34);
LV_IMG_DECLARE(_list_music_alpha_30x30);
LV_IMG_DECLARE(_btn_list_pause_alpha_20x20);
LV_IMG_DECLARE(_btn_list_play_alpha_20x20);

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


LV_IMG_DECLARE(_angry_240x320_240x320);

LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_20)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_Antonio_Regular_18)
LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_19)
LV_FONT_DECLARE(lv_font_montserratMedium_14)
LV_FONT_DECLARE(lv_font_montserratMedium_8)
LV_FONT_DECLARE(lv_font_montserratMedium_20)


#ifdef __cplusplus
}
#endif
#endif
