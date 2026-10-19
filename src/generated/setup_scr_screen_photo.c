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



void setup_scr_screen_photo(lv_ui *ui)
{
    //Write codes screen_photo
    ui->screen_photo = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_photo, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_photo, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_photo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_photo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_photo_view
    ui->screen_photo_view = lv_image_create(ui->screen_photo);
    lv_obj_set_pos(ui->screen_photo_view, 0, 0);
    lv_obj_set_size(ui->screen_photo_view, 240, 320);
    lv_obj_add_flag(ui->screen_photo_view, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_pivot(ui->screen_photo_view, 50,50);
    lv_image_set_rotation(ui->screen_photo_view, 0);

    //Write style for screen_photo_view, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->screen_photo_view, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->screen_photo_view, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_photo_right_arrow
    ui->screen_photo_right_arrow = lv_button_create(ui->screen_photo);
    lv_obj_set_pos(ui->screen_photo_right_arrow, 192, 285);
    lv_obj_set_size(ui->screen_photo_right_arrow, 45, 35);
    lv_obj_add_flag(ui->screen_photo_right_arrow, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_photo_right_arrow_label = lv_label_create(ui->screen_photo_right_arrow);
    lv_label_set_text(ui->screen_photo_right_arrow_label, "" LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->screen_photo_right_arrow_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_photo_right_arrow_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_photo_right_arrow, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_photo_right_arrow_label, LV_PCT(100));

    //Write style for screen_photo_right_arrow, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_photo_right_arrow, 106, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_photo_right_arrow, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_photo_right_arrow, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_photo_right_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_photo_right_arrow, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_photo_right_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_photo_right_arrow, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_photo_right_arrow, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_photo_right_arrow, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_photo_right_arrow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_photo_btn_delete
    ui->screen_photo_btn_delete = lv_button_create(ui->screen_photo);
    lv_obj_set_pos(ui->screen_photo_btn_delete, 144, 285);
    lv_obj_set_size(ui->screen_photo_btn_delete, 45, 35);
    lv_obj_add_flag(ui->screen_photo_btn_delete, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_photo_btn_delete_label = lv_label_create(ui->screen_photo_btn_delete);
    lv_label_set_text(ui->screen_photo_btn_delete_label, "" LV_SYMBOL_CLOSE "");
    lv_label_set_long_mode(ui->screen_photo_btn_delete_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_photo_btn_delete_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_photo_btn_delete, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_photo_btn_delete_label, LV_PCT(100));

    //Write style for screen_photo_btn_delete, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_photo_btn_delete, 106, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_photo_btn_delete, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_photo_btn_delete, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_photo_btn_delete, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_photo_btn_delete, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_photo_btn_delete, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_photo_btn_delete, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_photo_btn_delete, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_photo_btn_delete, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_photo_btn_delete, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_photo_left_arrow
    ui->screen_photo_left_arrow = lv_button_create(ui->screen_photo);
    lv_obj_set_pos(ui->screen_photo_left_arrow, 0, 285);
    lv_obj_set_size(ui->screen_photo_left_arrow, 45, 35);
    lv_obj_add_flag(ui->screen_photo_left_arrow, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_photo_left_arrow_label = lv_label_create(ui->screen_photo_left_arrow);
    lv_label_set_text(ui->screen_photo_left_arrow_label, "" LV_SYMBOL_LEFT "");
    lv_label_set_long_mode(ui->screen_photo_left_arrow_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_photo_left_arrow_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_photo_left_arrow, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_photo_left_arrow_label, LV_PCT(100));

    //Write style for screen_photo_left_arrow, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_photo_left_arrow, 107, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_photo_left_arrow, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_photo_left_arrow, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_photo_left_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_photo_left_arrow, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_photo_left_arrow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_photo_left_arrow, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_photo_left_arrow, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_photo_left_arrow, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_photo_left_arrow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_photo_btn_loop
    ui->screen_photo_btn_loop = lv_button_create(ui->screen_photo);
    lv_obj_set_pos(ui->screen_photo_btn_loop, 48, 285);
    lv_obj_set_size(ui->screen_photo_btn_loop, 45, 34);
    lv_obj_add_flag(ui->screen_photo_btn_loop, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_photo_btn_loop_label = lv_label_create(ui->screen_photo_btn_loop);
    lv_label_set_text(ui->screen_photo_btn_loop_label, "" LV_SYMBOL_REFRESH " ");
    lv_label_set_long_mode(ui->screen_photo_btn_loop_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_photo_btn_loop_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_photo_btn_loop, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_photo_btn_loop_label, LV_PCT(100));

    //Write style for screen_photo_btn_loop, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_photo_btn_loop, 106, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_photo_btn_loop, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_photo_btn_loop, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_photo_btn_loop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_photo_btn_loop, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_photo_btn_loop, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_photo_btn_loop, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_photo_btn_loop, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_photo_btn_loop, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_photo_btn_loop, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_photo_btn_web
    ui->screen_photo_btn_web = lv_button_create(ui->screen_photo);
    lv_obj_set_pos(ui->screen_photo_btn_web, 96, 285);
    lv_obj_set_size(ui->screen_photo_btn_web, 45, 35);
    lv_obj_add_flag(ui->screen_photo_btn_web, LV_OBJ_FLAG_CLICKABLE);
    ui->screen_photo_btn_web_label = lv_label_create(ui->screen_photo_btn_web);
    lv_label_set_text(ui->screen_photo_btn_web_label, "" LV_SYMBOL_WIFI " ");
    lv_label_set_long_mode(ui->screen_photo_btn_web_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_photo_btn_web_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_photo_btn_web, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_photo_btn_web_label, LV_PCT(100));

    //Write style for screen_photo_btn_web, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_photo_btn_web, 107, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_photo_btn_web, lv_color_hex(0xb1c9de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_photo_btn_web, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_photo_btn_web, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_photo_btn_web, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_photo_btn_web, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_photo_btn_web, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_photo_btn_web, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_photo_btn_web, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_photo_btn_web, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_photo.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_photo);

    //Init events for screen.
    events_init_screen_photo(ui);
}
