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
    lv_obj_t *toast = (lv_obj_t *)timer->user_data;
    lv_obj_del(toast);
    lv_timer_del(timer);  /* 一次性定时器，用完自删 */
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
    lv_obj_set_style_radius(toast, 10, 0);
    lv_obj_set_style_pad_all(toast, 10, 0);
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

void custom_init(lv_ui *ui)
{
    lv_label_set_text(guider_ui.screen_label_week, ""); 
    lv_label_set_text(guider_ui.screen_label_year_month, ""); 
}

void screen_load_register(screen_load *screen_load){
       screen_load_method = screen_load;
}
void screen_load_execute(lv_obj_t *obj,SCREEN_LOAD_STAUS loadStatus){
    if(obj == guider_ui.screen){
        screen_load_method->main_screen_load(loadStatus);
    }else if(obj == guider_ui.wifi_win){
        screen_load_method->wifi_screen_load(loadStatus);
    }else if(obj == guider_ui.photo_win){
        screen_load_method->photo_screen_load(loadStatus);
    }else if(obj == guider_ui.music){
        screen_load_method->music_screen_load(loadStatus);
    }else if(obj == guider_ui.robot_win){
        screen_load_method->robot_screen_load(loadStatus);
    }else if(obj == guider_ui.setting_win){
        screen_load_method->setting_screen_load(loadStatus);
    }else if(obj == guider_ui.radio_win){
        screen_load_method->radio_screen_load(loadStatus);
    }else if(obj == guider_ui.online_img_win){
        screen_load_method->img_screen_load(loadStatus);
    }


}