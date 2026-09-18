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



void setup_scr_music(lv_ui *ui)
{
    //Write codes music
    ui->music = lv_obj_create(NULL);
    lv_obj_set_size(ui->music, 240, 320);
    lv_obj_set_scrollbar_mode(ui->music, LV_SCROLLBAR_MODE_OFF);

    //Write style for music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music, lv_color_hex(0xba9c9c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_player
    ui->music_player = lv_obj_create(ui->music);
    lv_obj_set_pos(ui->music_player, 0, 0);
    lv_obj_set_size(ui->music_player, 240, 320);
    lv_obj_set_scrollbar_mode(ui->music_player, LV_SCROLLBAR_MODE_OFF);

    //Write style for music_player, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_player, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_player, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_player, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_player, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_playing_img
    ui->music_playing_img = lv_imgbtn_create(ui->music_player);
    lv_obj_add_flag(ui->music_playing_img, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->music_playing_img, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_play_alpha_40x40, NULL);
    ui->music_playing_img_label = lv_label_create(ui->music_playing_img);
    lv_label_set_text(ui->music_playing_img_label, "");
    lv_label_set_long_mode(ui->music_playing_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_playing_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_playing_img, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_playing_img, 108, 235);
    lv_obj_set_size(ui->music_playing_img, 40, 40);
    lv_obj_add_flag(ui->music_playing_img, LV_OBJ_FLAG_HIDDEN);

    //Write style for music_playing_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_playing_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_playing_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_playing_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_playing_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_playing_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_playing_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_playing_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_playing_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_playing_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_playing_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_playing_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_playing_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_playing_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_playing_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_playing_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_playing_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_playing_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_playing_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_playing_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_playing_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_playing_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_playing_img, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_playing_img, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_playing_img, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_loop_img
    ui->music_loop_img = lv_img_create(ui->music_player);
    lv_obj_add_flag(ui->music_loop_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->music_loop_img, &_btn_loop_alpha_20x20);
    lv_img_set_pivot(ui->music_loop_img, 50,50);
    lv_img_set_angle(ui->music_loop_img, 0);
    lv_obj_set_pos(ui->music_loop_img, 210, 256);
    lv_obj_set_size(ui->music_loop_img, 20, 20);

    //Write style for music_loop_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->music_loop_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->music_loop_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_loop_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_loop_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_rnd_img
    ui->music_rnd_img = lv_img_create(ui->music_player);
    lv_obj_add_flag(ui->music_rnd_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->music_rnd_img, &_btn_rnd_alpha_20x20);
    lv_img_set_pivot(ui->music_rnd_img, 50,50);
    lv_img_set_angle(ui->music_rnd_img, 0);
    lv_obj_set_pos(ui->music_rnd_img, 21, 256);
    lv_obj_set_size(ui->music_rnd_img, 20, 20);

    //Write style for music_rnd_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->music_rnd_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->music_rnd_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_rnd_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_rnd_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_next_img
    ui->music_next_img = lv_imgbtn_create(ui->music_player);
    lv_obj_add_flag(ui->music_next_img, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->music_next_img, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_next_alpha_35x35, NULL);
    ui->music_next_img_label = lv_label_create(ui->music_next_img);
    lv_label_set_text(ui->music_next_img_label, "");
    lv_label_set_long_mode(ui->music_next_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_next_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_next_img, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_next_img, 159, 240);
    lv_obj_set_size(ui->music_next_img, 35, 35);

    //Write style for music_next_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_next_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_next_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_next_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_next_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_next_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_next_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_next_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_next_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_next_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_next_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_next_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_next_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_next_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_next_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_next_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_next_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_next_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_next_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_next_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_next_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_next_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_next_img, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_next_img, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_next_img, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_pre_img
    ui->music_pre_img = lv_imgbtn_create(ui->music_player);
    lv_obj_add_flag(ui->music_pre_img, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->music_pre_img, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_prev_alpha_35x35, NULL);
    ui->music_pre_img_label = lv_label_create(ui->music_pre_img);
    lv_label_set_text(ui->music_pre_img_label, "");
    lv_label_set_long_mode(ui->music_pre_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_pre_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_pre_img, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_pre_img, 61, 240);
    lv_obj_set_size(ui->music_pre_img, 35, 35);

    //Write style for music_pre_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_pre_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_pre_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_pre_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_pre_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_pre_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_pre_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_pre_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_pre_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_pre_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_pre_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_pre_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_pre_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_pre_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_pre_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_pre_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_pre_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_pre_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_pre_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_pre_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_pre_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_pre_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_pre_img, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_pre_img, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_pre_img, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_pause_img
    ui->music_pause_img = lv_imgbtn_create(ui->music_player);
    lv_obj_add_flag(ui->music_pause_img, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->music_pause_img, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_pause_alpha_40x40, NULL);
    ui->music_pause_img_label = lv_label_create(ui->music_pause_img);
    lv_label_set_text(ui->music_pause_img_label, "");
    lv_label_set_long_mode(ui->music_pause_img_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_pause_img_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_pause_img, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_pause_img, 108, 234);
    lv_obj_set_size(ui->music_pause_img, 40, 40);

    //Write style for music_pause_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_pause_img, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_pause_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_pause_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_pause_img, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_pause_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_pause_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_pause_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_pause_img, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_pause_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_pause_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_pause_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_pause_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_pause_img, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_pause_img, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_pause_img, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_pause_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_pause_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_pause_img, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_pause_img, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_pause_img, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_pause_img, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_pause_img, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_pause_img, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_pause_img, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_album_img
    ui->music_album_img = lv_img_create(ui->music_player);
    lv_obj_add_flag(ui->music_album_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->music_album_img, &_cover_1_100x100);
    lv_img_set_pivot(ui->music_album_img, 50,50);
    lv_img_set_angle(ui->music_album_img, 0);
    lv_obj_set_pos(ui->music_album_img, 73, 63);
    lv_obj_set_size(ui->music_album_img, 100, 100);

    //Write style for music_album_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->music_album_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->music_album_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_album_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_album_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_top_img
    ui->music_top_img = lv_img_create(ui->music_player);
    lv_obj_add_flag(ui->music_top_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->music_top_img, &_wave_top_240x34);
    lv_img_set_pivot(ui->music_top_img, 50,50);
    lv_img_set_angle(ui->music_top_img, 0);
    lv_obj_set_pos(ui->music_top_img, 0, 0);
    lv_obj_set_size(ui->music_top_img, 240, 34);

    //Write style for music_top_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->music_top_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->music_top_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_top_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_top_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_bottom_img
    ui->music_bottom_img = lv_img_create(ui->music_player);
    lv_obj_add_flag(ui->music_bottom_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->music_bottom_img, &_wave_bottom_240x34);
    lv_img_set_pivot(ui->music_bottom_img, 50,50);
    lv_img_set_angle(ui->music_bottom_img, 0);
    lv_obj_set_pos(ui->music_bottom_img, 0, 285);
    lv_obj_set_size(ui->music_bottom_img, 240, 34);

    //Write style for music_bottom_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->music_bottom_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->music_bottom_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_bottom_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_bottom_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_slider
    ui->music_slider = lv_slider_create(ui->music_player);
    lv_slider_set_range(ui->music_slider, 0, 100);
    lv_slider_set_mode(ui->music_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->music_slider, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->music_slider, 21, 294);
    lv_obj_set_size(ui->music_slider, 200, 2);

    //Write style for music_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_slider, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_slider, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_slider, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_slider, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->music_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_slider, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_slider, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_slider, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_slider, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for music_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->music_slider, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_slider, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_slider, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes music_label_music_name
    ui->music_label_music_name = lv_label_create(ui->music_player);
    lv_label_set_text(ui->music_label_music_name, "");
    lv_label_set_long_mode(ui->music_label_music_name, LV_LABEL_LONG_SCROLL);
    lv_obj_set_pos(ui->music_label_music_name, 10, 10);
    lv_obj_set_size(ui->music_label_music_name, 180, 20);

    //Write style for music_label_music_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_label_music_name, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_label_music_name, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_label_music_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_label_music_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_label_music_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_label_slider_time
    ui->music_label_slider_time = lv_label_create(ui->music_player);
    lv_label_set_text(ui->music_label_slider_time, "00:00/00:00");
    lv_label_set_long_mode(ui->music_label_slider_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->music_label_slider_time, 21, 306);
    lv_obj_set_size(ui->music_label_slider_time, 80, 12);

    //Write style for music_label_slider_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_label_slider_time, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_label_slider_time, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_label_slider_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_label_slider_time, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_imgbtn_list
    ui->music_imgbtn_list = lv_imgbtn_create(ui->music_player);
    lv_obj_add_flag(ui->music_imgbtn_list, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->music_imgbtn_list, LV_IMGBTN_STATE_RELEASED, NULL, &_list_music_alpha_30x30, NULL);
    ui->music_imgbtn_list_label = lv_label_create(ui->music_imgbtn_list);
    lv_label_set_text(ui->music_imgbtn_list_label, "");
    lv_label_set_long_mode(ui->music_imgbtn_list_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->music_imgbtn_list_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->music_imgbtn_list, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->music_imgbtn_list, 204, 9);
    lv_obj_set_size(ui->music_imgbtn_list, 30, 30);

    //Write style for music_imgbtn_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->music_imgbtn_list, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_imgbtn_list, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_imgbtn_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_imgbtn_list, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->music_imgbtn_list, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for music_imgbtn_list, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->music_imgbtn_list, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->music_imgbtn_list, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->music_imgbtn_list, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->music_imgbtn_list, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for music_imgbtn_list, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->music_imgbtn_list, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->music_imgbtn_list, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->music_imgbtn_list, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->music_imgbtn_list, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for music_imgbtn_list, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->music_imgbtn_list, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->music_imgbtn_list, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes music_player_list
    ui->music_player_list = lv_obj_create(ui->music);
    lv_obj_set_pos(ui->music_player_list, 0, 0);
    lv_obj_set_size(ui->music_player_list, 240, 320);
    lv_obj_set_scrollbar_mode(ui->music_player_list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->music_player_list, LV_OBJ_FLAG_HIDDEN);

    //Write style for music_player_list, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_player_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_player_list, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_player_list, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_player_list, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes music_list_music
    ui->music_list_music = lv_list_create(ui->music_player_list);
    ui->music_list_music_item0 = lv_list_add_btn(ui->music_list_music, &_btn_list_pause_alpha_20x20, "save");
    ui->music_list_music_item1 = lv_list_add_btn(ui->music_list_music, &_btn_list_play_alpha_20x20, "save_1");
    lv_obj_set_pos(ui->music_list_music, 0, 21);
    lv_obj_set_size(ui->music_list_music, 240, 299);
    lv_obj_set_scrollbar_mode(ui->music_list_music, LV_SCROLLBAR_MODE_AUTO);

    //Write style state: LV_STATE_DEFAULT for &style_music_list_music_main_main_default
    static lv_style_t style_music_list_music_main_main_default;
    ui_init_style(&style_music_list_music_main_main_default);

    lv_style_set_pad_top(&style_music_list_music_main_main_default, 1);
    lv_style_set_pad_left(&style_music_list_music_main_main_default, 1);
    lv_style_set_pad_right(&style_music_list_music_main_main_default, 1);
    lv_style_set_pad_bottom(&style_music_list_music_main_main_default, 1);
    lv_style_set_bg_opa(&style_music_list_music_main_main_default, 255);
    lv_style_set_bg_color(&style_music_list_music_main_main_default, lv_color_hex(0x343247));
    lv_style_set_bg_grad_dir(&style_music_list_music_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_music_list_music_main_main_default, 0);
    lv_style_set_radius(&style_music_list_music_main_main_default, 1);
    lv_style_set_shadow_width(&style_music_list_music_main_main_default, 0);
    lv_obj_add_style(ui->music_list_music, &style_music_list_music_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_music_list_music_main_scrollbar_default
    static lv_style_t style_music_list_music_main_scrollbar_default;
    ui_init_style(&style_music_list_music_main_scrollbar_default);

    lv_style_set_radius(&style_music_list_music_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_music_list_music_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_music_list_music_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_music_list_music_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->music_list_music, &style_music_list_music_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_music_list_music_extra_btns_main_default
    static lv_style_t style_music_list_music_extra_btns_main_default;
    ui_init_style(&style_music_list_music_extra_btns_main_default);

    lv_style_set_pad_top(&style_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_music_list_music_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_music_list_music_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_music_list_music_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_music_list_music_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_music_list_music_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_music_list_music_extra_btns_main_default, 255);
    lv_style_set_radius(&style_music_list_music_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_music_list_music_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_music_list_music_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_music_list_music_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->music_list_music_item1, &style_music_list_music_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->music_list_music_item0, &style_music_list_music_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_music_list_music_extra_texts_main_default
    static lv_style_t style_music_list_music_extra_texts_main_default;
    ui_init_style(&style_music_list_music_extra_texts_main_default);

    lv_style_set_pad_top(&style_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_music_list_music_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_music_list_music_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_music_list_music_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_music_list_music_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_music_list_music_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_music_list_music_extra_texts_main_default, 255);
    lv_style_set_radius(&style_music_list_music_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_music_list_music_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_music_list_music_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_music_list_music_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_music_list_music_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //Write codes music_label_list_name
    ui->music_label_list_name = lv_label_create(ui->music_player_list);
    lv_label_set_text(ui->music_label_list_name, "");
    lv_label_set_long_mode(ui->music_label_list_name, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->music_label_list_name, 0, 0);
    lv_obj_set_size(ui->music_label_list_name, 240, 20);
    lv_obj_add_flag(ui->music_label_list_name, LV_OBJ_FLAG_CLICKABLE);

    //Write style for music_label_list_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->music_label_list_name, lv_color_hex(0xFFFFFF), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->music_label_list_name, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->music_label_list_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->music_label_list_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->music_label_list_name, 207, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->music_label_list_name, lv_color_hex(0x343247), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->music_label_list_name, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->music_label_list_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of music.


    //Update current screen layout.
    lv_obj_update_layout(ui->music);

    //Init events for screen.
    events_init_music(ui);
}
