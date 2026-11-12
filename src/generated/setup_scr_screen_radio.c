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



void setup_scr_screen_radio(lv_ui *ui)
{
    //Write codes screen_radio
    ui->screen_radio = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_radio, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_radio, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_radio, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_radio, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_radio, lv_color_hex(0x1A1A1A), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_radio, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_img_cover
    ui->screen_radio_img_cover = lv_image_create(ui->screen_radio);
    lv_obj_set_pos(ui->screen_radio_img_cover, 55, 60);
    lv_obj_set_size(ui->screen_radio_img_cover, 130, 130);
    lv_obj_add_flag(ui->screen_radio_img_cover, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_radio_img_cover, &_station_artwork_130_RGB565A8_130x130);
    lv_image_set_pivot(ui->screen_radio_img_cover, 50,50);
    lv_image_set_rotation(ui->screen_radio_img_cover, 0);

    //Write style for screen_radio_img_cover, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_radio_img_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_radio_img_cover, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_cont_control
    ui->screen_radio_cont_control = lv_obj_create(ui->screen_radio);
    lv_obj_set_pos(ui->screen_radio_cont_control, 20, 270);
    lv_obj_set_size(ui->screen_radio_cont_control, 200, 40);
    lv_obj_set_scrollbar_mode(ui->screen_radio_cont_control, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_radio_cont_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_radio_cont_control, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_radio_cont_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_radio_cont_control, lv_color_hex(0x353535), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_radio_cont_control, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_radio_cont_control, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_radio_cont_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_radio_cont_control, lv_color_hex(0x282828), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_radio_cont_control, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_radio_cont_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_radio_cont_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_radio_cont_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_radio_cont_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_radio_cont_control, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_img_play
    ui->screen_radio_img_play = lv_image_create(ui->screen_radio_cont_control);
    lv_obj_set_pos(ui->screen_radio_img_play, 85, 5);
    lv_obj_set_size(ui->screen_radio_img_play, 30, 30);
    lv_obj_add_flag(ui->screen_radio_img_play, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_radio_img_play, &_btn_play_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_radio_img_play, 50,50);
    lv_image_set_rotation(ui->screen_radio_img_play, 0);

    //Write style for screen_radio_img_play, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_radio_img_play, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_radio_img_play, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_img_next
    ui->screen_radio_img_next = lv_image_create(ui->screen_radio_cont_control);
    lv_obj_set_pos(ui->screen_radio_img_next, 160, 5);
    lv_obj_set_size(ui->screen_radio_img_next, 30, 30);
    lv_obj_add_flag(ui->screen_radio_img_next, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_radio_img_next, &_icon_next_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_radio_img_next, 50,50);
    lv_image_set_rotation(ui->screen_radio_img_next, 0);

    //Write style for screen_radio_img_next, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_radio_img_next, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_radio_img_next, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_img_prev
    ui->screen_radio_img_prev = lv_image_create(ui->screen_radio_cont_control);
    lv_obj_set_pos(ui->screen_radio_img_prev, 5, 5);
    lv_obj_set_size(ui->screen_radio_img_prev, 30, 30);
    lv_obj_add_flag(ui->screen_radio_img_prev, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_radio_img_prev, &_icon_prev_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_radio_img_prev, 50,50);
    lv_image_set_rotation(ui->screen_radio_img_prev, 0);

    //Write style for screen_radio_img_prev, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_radio_img_prev, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_radio_img_prev, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_img_pause
    ui->screen_radio_img_pause = lv_image_create(ui->screen_radio_cont_control);
    lv_obj_set_pos(ui->screen_radio_img_pause, 85, 5);
    lv_obj_set_size(ui->screen_radio_img_pause, 30, 30);
    lv_obj_add_flag(ui->screen_radio_img_pause, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->screen_radio_img_pause, &_btn_pause_RGB565A8_30x30);
    lv_image_set_pivot(ui->screen_radio_img_pause, 50,50);
    lv_image_set_rotation(ui->screen_radio_img_pause, 0);

    //Write style for screen_radio_img_pause, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_radio_img_pause, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_radio_img_pause, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_label_title
    ui->screen_radio_label_title = lv_label_create(ui->screen_radio);
    lv_obj_set_pos(ui->screen_radio_label_title, 30, 195);
    lv_obj_set_size(ui->screen_radio_label_title, 180, 40);
    lv_label_set_text(ui->screen_radio_label_title, "经典FM  音乐电台");
    lv_label_set_long_mode(ui->screen_radio_label_title, LV_LABEL_LONG_WRAP);

    //Write style for screen_radio_label_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_radio_label_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_radio_label_title, &lv_font_SourceHanSerifSC_Regular_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_radio_label_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_radio_label_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_radio_label_title, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_radio_label_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_radio_label_radio_name
    ui->screen_radio_label_radio_name = lv_label_create(ui->screen_radio);
    lv_obj_set_pos(ui->screen_radio_label_radio_name, 40, 240);
    lv_obj_set_size(ui->screen_radio_label_radio_name, 160, 30);
    lv_label_set_text(ui->screen_radio_label_radio_name, "");
    lv_label_set_long_mode(ui->screen_radio_label_radio_name, LV_LABEL_LONG_SCROLL_CIRCULAR);

    //Write style for screen_radio_label_radio_name, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_radio_label_radio_name, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_radio_label_radio_name, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_radio_label_radio_name, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_radio_label_radio_name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_radio_label_radio_name, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_radio.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_radio);

    //Init events for screen.
    events_init_screen_radio(ui);
}
