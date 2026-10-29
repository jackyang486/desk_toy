/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"

static screen_load *screen_load_method=NULL;
static screen_load lv_screen_param;
Page_Head  page_head;
// ================= 关键：模拟器兼容 =================
#if defined(_WIN32) || defined(__SIMULATOR__)
// 电脑模拟器运行时，给一个空实现，不报错
void connect_wifi(void){ }
void click_right_btn(void){ }
void click_left_btn(void){ }
void image_carousel(void){ }
void music_prev(void){ }
void music_next(void){ }
void music_play(void){ }
void music_random(void){ }
void music_sequence(void){ }
void main_sreen_unloaded(void){ }
#endif

void screen_load_test(SCREEN_LOAD_STAUS loadStatus){


}

/* 自动关闭定时器回调 */
 void toast_timer_cb(lv_timer_t *timer)
{
    lv_obj_t *toast = (lv_obj_t *)lv_timer_get_user_data(timer);
    lv_obj_del(toast);
    lv_timer_delete(timer);  /* 一次性定时器，用完自删 */
}

/**
 * @brief 显示一个 2 秒后自动消失的弹出提示
 * @param parent   父对象（通常为当前屏幕）
 * @param msg      提示文本
 */
void show_toast(lv_obj_t *parent, const char *msg)
{
    /* ---- 创建 Toast 容器 ---- */
    lv_obj_t *toast = lv_obj_create(parent);
    lv_obj_remove_style_all(toast);              /* 清除默认样式 */
    lv_obj_set_style_bg_color(toast, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(toast, LV_OPA_70, 0);
    lv_obj_set_style_radius(toast, 10,0);
    lv_obj_set_style_pad_left(toast, 5,0);
    lv_obj_set_style_pad_right(toast, 5,0);
    lv_obj_set_style_pad_top(toast, 5,0);
    lv_obj_set_style_pad_bottom(toast, 5,0);
    lv_obj_clear_flag(toast, LV_OBJ_FLAG_SCROLLABLE);

    /* 居中显示 */
    lv_obj_align(toast, LV_ALIGN_CENTER, 0, 0);

    /* ---- 添加文本标签 ---- */
    lv_obj_t *label = lv_label_create(toast);
    lv_obj_set_style_text_font(label, &font_puhui_16_4,  LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_label_set_text(label, msg);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_center(label);

    /* 根据文本自适应大小 */
    lv_obj_update_layout(toast);

    /* ---- 创建 2 秒一次性定时器 ---- */
    lv_timer_t *timer = lv_timer_create(toast_timer_cb, 1000, toast);
    lv_timer_set_repeat_count(timer, 1);         /* 仅触发一次 */
}

void create_head(){
       
    page_head.pageHeadCont = lv_obj_create(lv_layer_top());//Write codes screen_home_label_battery
    lv_obj_set_pos(page_head.pageHeadCont, 0, 0);
    lv_obj_set_size(page_head.pageHeadCont, 240, 30);
    lv_obj_set_scrollbar_mode(page_head.pageHeadCont, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_home_cont_head, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(page_head.pageHeadCont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(page_head.pageHeadCont, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(page_head.pageHeadCont, 40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(page_head.pageHeadCont, lv_color_hex(0x9a8b8b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(page_head.pageHeadCont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(page_head.pageHeadCont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(page_head.pageHeadCont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(page_head.pageHeadCont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(page_head.pageHeadCont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(page_head.pageHeadCont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_label_battery
    page_head.battery_label = lv_label_create(page_head.pageHeadCont);
    lv_obj_set_pos(page_head.battery_label, 200, 10);
    lv_obj_set_size(page_head.battery_label, 40, 20);
    lv_label_set_text(page_head.battery_label, "" LV_SYMBOL_BATTERY_FULL " ");
    lv_label_set_long_mode(page_head.battery_label, LV_LABEL_LONG_WRAP);

    //Write style for screen_home_page_head.battery_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(page_head.battery_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(page_head.battery_label, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(page_head.battery_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(page_head.battery_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(page_head.battery_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_page_head.wifi_img
    page_head.wifi_img = lv_image_create(page_head.pageHeadCont);
    lv_obj_set_pos(page_head.wifi_img, 10, 8);
    lv_obj_set_size(page_head.wifi_img, 20, 20);
    lv_obj_add_flag(page_head.wifi_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(page_head.wifi_img, &no_wifi);
    lv_image_set_pivot(page_head.wifi_img, 50,50);
    lv_image_set_rotation(page_head.wifi_img, 0);

    //Write style for screen_home_page_head.wifi_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(page_head.wifi_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(page_head.wifi_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_home_ page_head.time_label
    page_head.time_label = lv_label_create(page_head.pageHeadCont);
    lv_obj_set_pos( page_head.time_label, 55, 6);
    lv_obj_set_size( page_head.time_label, 130, 20);
    lv_label_set_text( page_head.time_label, "11:25");

    //Write style for screen_home_ page_head.time_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius( page_head.time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color( page_head.time_label, lv_color_hex(0x12548b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(page_head.time_label, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa( page_head.time_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space( page_head.time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align( page_head.time_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa( page_head.time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top( page_head.time_label, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right( page_head.time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom( page_head.time_label, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left( page_head.time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width( page_head.time_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);     

}

void custom_init(lv_ui *ui)
{
     create_head();
}

void screen_load_register(screen_load *screen_load){
       screen_load_method = screen_load;
}
void screen_load_execute(lv_obj_t *obj,SCREEN_LOAD_STAUS loadStatus){

  if(obj == guider_ui.screen_home){
        screen_load_method->main_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_wifi){
        screen_load_method->wifi_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_photo){
        screen_load_method->photo_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_music){
        screen_load_method->music_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_robot){   
        screen_load_method->robot_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_setting){
        screen_load_method->setting_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_radio){
        screen_load_method->radio_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_mouse){
        screen_load_method->fly_mouse_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_weather){
        screen_load_method->weather_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_anim){
        screen_load_method->lottie_screen_load(loadStatus);
    }else if(obj == guider_ui.screen_game){
        screen_load_method->game_screen_load(loadStatus);
    }

}