#include "confirm_dialog.h"
#include <string.h>

// 弹窗内部数据结构体
typedef struct {
    confirm_dialog_cb_t cb;
    void *user_data;
} dialog_user_data_t;

static void dialog_btn_event_cb(lv_event_t *e)
{
    uint8_t is_confirm = *(int *)lv_event_get_user_data(e);
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *dialog = lv_obj_get_parent(btn);
    dialog_user_data_t *ud = (dialog_user_data_t *)lv_obj_get_user_data(dialog);

    if(ud && ud->cb)
    {
        ud->cb(is_confirm, ud->user_data);
    }
    confirm_dialog_close(dialog);
}

lv_obj_t * confirm_dialog_create(lv_obj_t *parent,
                                 const char *title,
                                 const char *btn_ok_text,
                                 const char *btn_cancel_text,
                                 confirm_dialog_cb_t cb,
                                 void *user_data)
{

    // 2. 弹窗主体面板
    lv_obj_t *dialog = lv_obj_create(parent);
    lv_obj_set_size(dialog, 200, 120);
    lv_obj_align(dialog, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(dialog, 12, 0);
    lv_obj_set_style_bg_color(dialog, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(dialog,LV_OPA_50,0);
    lv_obj_set_style_shadow_width(dialog, 10, 0);
    lv_obj_set_style_shadow_opa(dialog, LV_OPA_20, 0);
    lv_obj_clear_flag(dialog, LV_OBJ_FLAG_SCROLLABLE);

    // 存储回调
    dialog_user_data_t *ud = (dialog_user_data_t *)lv_malloc(sizeof(dialog_user_data_t));
    if(ud == NULL)
    {
        lv_obj_del(dialog);
        return NULL;
    }
    ud->cb = cb;
    ud->user_data = user_data;
    lv_obj_set_user_data(dialog, ud);

    // 标题label
    lv_obj_t *label_title = lv_label_create(dialog);
    lv_obj_set_pos(label_title,0,0);
    lv_label_set_text(label_title, title);
    lv_obj_set_style_text_font(label_title, &font_puhui_16_4, 0);
    lv_obj_set_style_text_color(label_title, lv_color_white(),0);

    // 提示消息label
    //lv_obj_t *label_msg = lv_label_create(dialog);
    //lv_label_set_text(label_msg, msg);
    //lv_obj_set_style_text_font(label_msg, &lv_font_montserrat_14, 0);
    //lv_obj_set_style_text_color(label_msg, lv_color_hex(0x444444),0);
    //lv_obj_set_style_pad_top(label_msg, 10, 0);
    //lv_obj_set_style_pad_bottom(label_msg, 20, 0);
    //lv_label_set_long_mode(label_msg, LV_LABEL_LONG_WRAP);
    //lv_obj_set_width(label_msg, 200);

    static uint8_t flag_cancel = 0;
    static uint8_t flag_ok     = 1;

    // 确认按钮（红色，用于删除）
    lv_obj_t *btn_ok = lv_btn_create(dialog);
    lv_obj_set_pos(btn_ok,0,60);
    lv_obj_set_size(btn_ok, 60, 36);
    lv_obj_set_style_radius(btn_ok, 8, 0);
    lv_obj_set_style_bg_color(btn_ok, lv_color_hex(0xe53e3e), 0);
    lv_obj_add_event_cb(btn_ok, dialog_btn_event_cb, LV_EVENT_CLICKED, &flag_ok);
    lv_obj_t *label_ok = lv_label_create(btn_ok);
    lv_obj_set_style_text_font(label_ok, &font_puhui_16_4, 0);
    lv_label_set_text(label_ok, btn_ok_text);
    lv_obj_set_style_text_color(label_ok, lv_color_white(),0);
    lv_obj_center(label_ok);

    // 取消按钮
    lv_obj_t *btn_cancel = lv_btn_create(dialog);
    lv_obj_set_pos(btn_cancel,110,60);
    lv_obj_set_size(btn_cancel, 60, 36);
    lv_obj_set_style_radius(btn_cancel, 8, 0);
    lv_obj_add_event_cb(btn_cancel, dialog_btn_event_cb, LV_EVENT_CLICKED, &flag_cancel);
    lv_obj_t *label_cancel = lv_label_create(btn_cancel);
    lv_obj_set_style_text_font(label_cancel, &font_puhui_16_4, 0);
    lv_label_set_text(label_cancel, btn_cancel_text);
    lv_obj_center(label_cancel);

    

    return dialog; // 返回遮罩对象，关闭时删除mask即可
}

void confirm_dialog_close(lv_obj_t *dialog)
{
    if(dialog == NULL) return;
    dialog_user_data_t *ud = (dialog_user_data_t *)lv_obj_get_user_data(dialog);
    if(ud){
        lv_free(ud);
    }
    lv_obj_del(dialog); // 删除遮罩，弹窗整体销毁
}
