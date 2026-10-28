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



void setup_scr_screen_game(lv_ui *ui)
{
    //Write codes screen_game
    ui->screen_game = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_game, 240, 320);
    lv_obj_set_scrollbar_mode(ui->screen_game, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_game, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_game, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_game_list_game
    ui->screen_game_list_game = lv_list_create(ui->screen_game);
    lv_obj_set_pos(ui->screen_game_list_game, 0, 35);
    lv_obj_set_size(ui->screen_game_list_game, 240, 285);
    lv_obj_set_scrollbar_mode(ui->screen_game_list_game, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_screen_game_list_game_main_main_default
    static lv_style_t style_screen_game_list_game_main_main_default;
    ui_init_style(&style_screen_game_list_game_main_main_default);

    lv_style_set_pad_top(&style_screen_game_list_game_main_main_default, 1);
    lv_style_set_pad_left(&style_screen_game_list_game_main_main_default, 1);
    lv_style_set_pad_right(&style_screen_game_list_game_main_main_default, 1);
    lv_style_set_pad_bottom(&style_screen_game_list_game_main_main_default, 1);
    lv_style_set_bg_opa(&style_screen_game_list_game_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_game_list_game_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_game_list_game_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_game_list_game_main_main_default, 0);
    lv_style_set_radius(&style_screen_game_list_game_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_game_list_game_main_main_default, 0);
    lv_obj_add_style(ui->screen_game_list_game, &style_screen_game_list_game_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_game_list_game_main_scrollbar_default
    static lv_style_t style_screen_game_list_game_main_scrollbar_default;
    ui_init_style(&style_screen_game_list_game_main_scrollbar_default);

    lv_style_set_radius(&style_screen_game_list_game_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_game_list_game_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_game_list_game_main_scrollbar_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_game_list_game_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_game_list_game, &style_screen_game_list_game_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_game_list_game_extra_btns_main_default
    static lv_style_t style_screen_game_list_game_extra_btns_main_default;
    ui_init_style(&style_screen_game_list_game_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_game_list_game_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_game_list_game_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_game_list_game_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_game_list_game_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_game_list_game_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_game_list_game_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_game_list_game_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_game_list_game_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_game_list_game_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_game_list_game_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_game_list_game_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_game_list_game_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_game_list_game_extra_texts_main_default
    static lv_style_t style_screen_game_list_game_extra_texts_main_default;
    ui_init_style(&style_screen_game_list_game_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_game_list_game_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_game_list_game_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_game_list_game_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_game_list_game_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_game_list_game_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_game_list_game_extra_texts_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_game_list_game_extra_texts_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_game_list_game_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_game_list_game_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_game_list_game_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_game_list_game_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_game_list_game_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_game_list_game_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //The custom code of screen_game.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_game);

    //Init events for screen.
    events_init_screen_game(ui);
}
