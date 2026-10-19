#ifndef __CONFIRM_DIALOG_H_
#define __CONFIRM_DIALOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
LV_FONT_DECLARE(font_puhui_16_4)
// 回调函数类型
// is_confirm = true：点击确认；false：点击取消
typedef void (*confirm_dialog_cb_t)(bool is_confirm, void *user_data);

/**
 * @brief 创建通用确认弹窗
 * @param parent 父对象，一般传 lv_layer_top()
 * @param title 标题文字
 * @param msg 提示内容
 * @param btn_ok_text 确认按钮文字（如"删除"）
 * @param btn_cancel_text 取消按钮文字（如"取消"）
 * @param cb 回调函数
 * @param user_data 传给回调的自定义参数
 * @return 弹窗obj
 */
lv_obj_t * confirm_dialog_create(lv_obj_t *parent,
                                 const char *title,
                                 const char *btn_ok_text,
                                 const char *btn_cancel_text,
                                 confirm_dialog_cb_t cb,
                                 void *user_data);

// 手动关闭弹窗
void confirm_dialog_close(lv_obj_t *dialog);

#ifdef __cplusplus
}

#endif

#endif
