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



void setup_scr_screen_music(lv_ui *ui)
{
    //Write codes screen_music
    ui->screen_music = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_music, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_music, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_player_list
    ui->screen_music_player_list = lv_obj_create(ui->screen_music);
    lv_obj_set_pos(ui->screen_music_player_list, 240, 0);
    lv_obj_set_size(ui->screen_music_player_list, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_music_player_list, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music_player_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_player_list, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_player_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_player_list, lv_color_hex(0xd1e8f9), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_player_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_list_music
    ui->screen_music_list_music = lv_list_create(ui->screen_music_player_list);
    lv_obj_set_pos(ui->screen_music_list_music, 0, 35);
    lv_obj_set_size(ui->screen_music_list_music, 240, 285);
    lv_obj_set_scrollbar_mode(ui->screen_music_list_music, LV_SCROLLBAR_MODE_AUTO);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_main_main_default
    static lv_style_t style_screen_music_list_music_main_main_default;
    ui_init_style(&style_screen_music_list_music_main_main_default);

    lv_style_set_pad_top(&style_screen_music_list_music_main_main_default, 1);
    lv_style_set_pad_left(&style_screen_music_list_music_main_main_default, 1);
    lv_style_set_pad_right(&style_screen_music_list_music_main_main_default, 1);
    lv_style_set_pad_bottom(&style_screen_music_list_music_main_main_default, 1);
    lv_style_set_bg_opa(&style_screen_music_list_music_main_main_default, 0);
    lv_style_set_border_width(&style_screen_music_list_music_main_main_default, 0);
    lv_style_set_radius(&style_screen_music_list_music_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_music_list_music_main_main_default, 0);
    lv_obj_add_style(ui->screen_music_list_music, &style_screen_music_list_music_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_main_scrollbar_default
    static lv_style_t style_screen_music_list_music_main_scrollbar_default;
    ui_init_style(&style_screen_music_list_music_main_scrollbar_default);

    lv_style_set_radius(&style_screen_music_list_music_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_music_list_music_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_music_list_music, &style_screen_music_list_music_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_extra_btns_main_default
    static lv_style_t style_screen_music_list_music_extra_btns_main_default;
    ui_init_style(&style_screen_music_list_music_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_music_list_music_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_music_list_music_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_music_list_music_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_music_list_music_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_music_list_music_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_music_list_music_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_music_list_music_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_music_list_music_extra_texts_main_default
    static lv_style_t style_screen_music_list_music_extra_texts_main_default;
    ui_init_style(&style_screen_music_list_music_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_music_list_music_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_music_list_music_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_music_list_music_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_music_list_music_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_music_list_music_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_music_list_music_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_music_list_music_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_music_list_music_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_music_list_music_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_music_list_music_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes screen_music_btn_music
    ui->screen_music_btn_music = lv_button_create(ui->screen_music_player_list);
    lv_obj_set_pos(ui->screen_music_btn_music, 0, 113);
    lv_obj_set_size(ui->screen_music_btn_music, 30, 40);
    ui->screen_music_btn_music_label = lv_label_create(ui->screen_music_btn_music);
    lv_label_set_text(ui->screen_music_btn_music_label, "" LV_SYMBOL_LEFT "");
    lv_label_set_long_mode(ui->screen_music_btn_music_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_music_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_music, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_music_label, LV_PCT(100));

    //Write style for screen_music_btn_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_btn_music, lv_color_hex(0xff5700), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_btn_music, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_music, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_music, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_music, &lv_font_montserratMedium_17, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_music, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_player
    ui->screen_music_player = lv_obj_create(ui->screen_music);
    lv_obj_set_pos(ui->screen_music_player, 0, 0);
    lv_obj_set_size(ui->screen_music_player, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_music_player, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_music_player, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_player, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_player, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_player, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_rnd_img
    ui->screen_music_rnd_img = lv_image_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_rnd_img, 19, 256);
    lv_obj_set_size(ui->screen_music_rnd_img, 25, 25);
    lv_obj_add_flag(ui->screen_music_rnd_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_music_rnd_img, &_btn_rnd_RGB565A8_25x25);
    lv_image_set_pivot(ui->screen_music_rnd_img, 50,50);
    lv_image_set_rotation(ui->screen_music_rnd_img, 0);

    //Write style for screen_music_rnd_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_rnd_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_music_rnd_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_next_img
    ui->screen_music_next_img = lv_image_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_next_img, 153, 240);
    lv_obj_set_size(ui->screen_music_next_img, 40, 40);
    lv_obj_add_flag(ui->screen_music_next_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_music_next_img, &_btn_next_RGB565A8_40x40);
    lv_image_set_pivot(ui->screen_music_next_img, 50,50);
    lv_image_set_rotation(ui->screen_music_next_img, 0);

    //Write style for screen_music_next_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_next_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_music_next_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_pre_img
    ui->screen_music_pre_img = lv_imagebutton_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_pre_img, 55, 240);
    lv_obj_set_size(ui->screen_music_pre_img, 40, 40);
    lv_obj_add_flag(ui->screen_music_pre_img, LV_OBJ_FLAG_CHECKABLE);
    lv_imagebutton_set_src(ui->screen_music_pre_img, LV_IMAGEBUTTON_STATE_RELEASED, &_btn_prev_RGB565A8_40x40, NULL, NULL);
    ui->screen_music_pre_img_label = lv_label_create(ui->screen_music_pre_img);
    lv_label_set_text(ui->screen_music_pre_img_label, "");
    lv_label_set_long_mode(ui->screen_music_pre_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_pre_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_pre_img, 0, LV_STATE_DEFAULT);

    //Write style for screen_music_pre_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_music_pre_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_pre_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_pre_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_pre_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_pre_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_music_pre_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_pre_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_music_pre_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_music_pre_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_music_pre_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_music_pre_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_music_pre_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_music_pre_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_pre_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_music_pre_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_music_pre_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_music_pre_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_music_pre_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_music_pre_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_music_pre_img, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_pre_img, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_music_pre_img, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_music_pause_img
    ui->screen_music_pause_img = lv_imagebutton_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_pause_img, 102, 234);
    lv_obj_set_size(ui->screen_music_pause_img, 40, 40);
    lv_imagebutton_set_src(ui->screen_music_pause_img, LV_IMAGEBUTTON_STATE_RELEASED, &_btn_pause_RGB565A8_40x40, NULL, NULL);
    ui->screen_music_pause_img_label = lv_label_create(ui->screen_music_pause_img);
    lv_label_set_text(ui->screen_music_pause_img_label, "");
    lv_label_set_long_mode(ui->screen_music_pause_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_pause_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_pause_img, 0, LV_STATE_DEFAULT);

    //Write style for screen_music_pause_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_music_pause_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_pause_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_pause_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_pause_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_pause_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_music_pause_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_pause_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_music_pause_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_music_pause_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_music_pause_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_music_pause_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_music_pause_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_music_pause_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_pause_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_music_pause_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_music_pause_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_music_pause_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_music_pause_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_music_pause_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_music_pause_img, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_pause_img, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_music_pause_img, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_music_album_img
    ui->screen_music_album_img = lv_image_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_album_img, 73, 63);
    lv_obj_set_size(ui->screen_music_album_img, 100, 100);
    lv_obj_add_flag(ui->screen_music_album_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_music_album_img, &_cover_2_RGB565A8_100x100);
    lv_image_set_pivot(ui->screen_music_album_img, 50,50);
    lv_image_set_rotation(ui->screen_music_album_img, 0);

    //Write style for screen_music_album_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_album_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_music_album_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_top_img
    ui->screen_music_top_img = lv_image_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_top_img, 0, 3);
    lv_obj_set_size(ui->screen_music_top_img, 240, 34);
    lv_obj_add_flag(ui->screen_music_top_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_music_top_img, &_wave_top_RGB565A8_240x34);
    lv_image_set_pivot(ui->screen_music_top_img, 50,50);
    lv_image_set_rotation(ui->screen_music_top_img, 0);

    //Write style for screen_music_top_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_top_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_music_top_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_bottom_img
    ui->screen_music_bottom_img = lv_image_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_bottom_img, 0, 285);
    lv_obj_set_size(ui->screen_music_bottom_img, 240, 34);
    lv_obj_add_flag(ui->screen_music_bottom_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_music_bottom_img, &_wave_bottom_RGB565A8_240x34);
    lv_image_set_pivot(ui->screen_music_bottom_img, 50,50);
    lv_image_set_rotation(ui->screen_music_bottom_img, 0);

    //Write style for screen_music_bottom_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_bottom_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_music_bottom_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_slider
    ui->screen_music_slider = lv_slider_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_slider, 21, 294);
    lv_obj_set_size(ui->screen_music_slider, 200, 2);
    lv_slider_set_range(ui->screen_music_slider, 0, 100);
    lv_slider_set_mode(ui->screen_music_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_music_slider, 0, LV_ANIM_OFF);

    //Write style for screen_music_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_slider, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_slider, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_slider, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_music_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_music_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_slider, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_slider, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_music_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_slider, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_slider, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_music_label_music_name
    ui->screen_music_label_music_name = lv_label_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_label_music_name, 25, 35);
    lv_obj_set_size(ui->screen_music_label_music_name, 180, 20);
    lv_label_set_text_static(ui->screen_music_label_music_name, "");
    lv_label_set_long_mode(ui->screen_music_label_music_name, LV_LABEL_LONG_SCROLL);

    //Write style for screen_music_label_music_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_label_music_name, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_label_music_name, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_label_music_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_label_music_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_label_slider_time
    ui->screen_music_label_slider_time = lv_label_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_label_slider_time, 21, 306);
    lv_obj_set_size(ui->screen_music_label_slider_time, 80, 12);
    lv_label_set_text(ui->screen_music_label_slider_time, "00:00/00:00");
    lv_label_set_long_mode(ui->screen_music_label_slider_time, LV_LABEL_LONG_WRAP);

    //Write style for screen_music_label_slider_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_label_slider_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_label_slider_time, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_label_slider_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_label_slider_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_loop_img
    ui->screen_music_loop_img = lv_image_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_loop_img, 200, 256);
    lv_obj_set_size(ui->screen_music_loop_img, 25, 25);
    lv_obj_add_flag(ui->screen_music_loop_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_music_loop_img, &_btn_loop_RGB565A8_25x25);
    lv_image_set_pivot(ui->screen_music_loop_img, 50,50);
    lv_image_set_rotation(ui->screen_music_loop_img, 0);

    //Write style for screen_music_loop_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_loop_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_music_loop_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_music_playing_img
    ui->screen_music_playing_img = lv_imagebutton_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_playing_img, 102, 234);
    lv_obj_set_size(ui->screen_music_playing_img, 40, 40);
    lv_obj_add_flag(ui->screen_music_playing_img, LV_OBJ_FLAG_HIDDEN);
    lv_imagebutton_set_src(ui->screen_music_playing_img, LV_IMAGEBUTTON_STATE_RELEASED, &_btn_play_RGB565A8_40x40, NULL, NULL);
    ui->screen_music_playing_img_label = lv_label_create(ui->screen_music_playing_img);
    lv_label_set_text(ui->screen_music_playing_img_label, "");
    lv_label_set_long_mode(ui->screen_music_playing_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_playing_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_playing_img, 0, LV_STATE_DEFAULT);

    //Write style for screen_music_playing_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_music_playing_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_playing_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_playing_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_playing_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_playing_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_music_playing_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_playing_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->screen_music_playing_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_music_playing_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_music_playing_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_music_playing_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_music_playing_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_music_playing_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_playing_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->screen_music_playing_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_music_playing_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_music_playing_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_music_playing_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_music_playing_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_music_playing_img, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->screen_music_playing_img, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->screen_music_playing_img, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes screen_music_btn_list
    ui->screen_music_btn_list = lv_button_create(ui->screen_music_player);
    lv_obj_set_pos(ui->screen_music_btn_list, 207, 113);
    lv_obj_set_size(ui->screen_music_btn_list, 30, 40);
    ui->screen_music_btn_list_label = lv_label_create(ui->screen_music_btn_list);
    lv_label_set_text(ui->screen_music_btn_list_label, "" LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->screen_music_btn_list_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_music_btn_list_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_music_btn_list, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_music_btn_list_label, LV_PCT(100));

    //Write style for screen_music_btn_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_music_btn_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_music_btn_list, lv_color_hex(0xdedcdc), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_music_btn_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_music_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_music_btn_list, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_music_btn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_music_btn_list, lv_color_hex(0x008dff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_music_btn_list, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_music_btn_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_music_btn_list, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_music.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_music);

    //Init events for screen.
    events_init_screen_music(ui);
}
