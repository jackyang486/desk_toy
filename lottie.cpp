#include "lottie.h"

#include "ESP32_board.h"
#include "src/generated/gui_guider.h"
#include "ai_vox_engine.h"
#include <stdint.h>
#include <stdio.h>
#include <vector>

// LVGL 心跳归属交接标志，定义在 main.cpp
extern volatile bool g_lvgl_external_active;   // true：本文件的任务正在驱动 lv_timer_handler()

namespace {

constexpr char kLottieScanPath[] = "S:/lottie";
constexpr char kLottieFilePath[] = "/sdcard/lottie";
constexpr int32_t kLottieWidth = 240;
constexpr int32_t kLottieHeight = 240;
constexpr uint32_t kLottieFps = 30;

// 专用任务的栈。xTaskCreate 的单位是 word（4 字节），10*1024 = 40KB。
// rlottie 的 JSON 解析与逐帧渲染都很吃栈，40KB 是实测够用的量级；
// 若换用更复杂的动画，可用 uxTaskGetStackHighWaterMark() 的打印值复核后调整。
#define LOTTIE_TASK_STACK_WORDS  (30 * 1024)
#define LOTTIE_TASK_PRIO         2       // 高于 ui_task(1)，保证接管后能抢占
#define LOTTIE_TASK_CORE         1       // 与 ui_task 同核，避免 LVGL 跨核访问

static TaskHandle_t lottie_task_handle = NULL;
static volatile bool lottie_task_stop = false;

std::vector<String> lottie_files;
size_t current_lottie = 0;
lv_obj_t *lottie_object = nullptr;
lv_obj_t *lottie_parent = nullptr;
uint8_t *lottie_buffer = nullptr;

void lottie_buffer_delete_cb(lv_event_t *event)
{
    void *buffer = lv_event_get_user_data(event);
    lv_free(buffer);
    if (buffer == lottie_buffer) {
        lottie_buffer = nullptr;
        lottie_object = nullptr;
        guider_ui.screen_anim_rlottie_1 = nullptr;
    }
}

void destroy_lottie_object()
{
    if (lottie_object != nullptr) {
        lv_obj_delete(lottie_object);
    }
}

bool create_lottie_object()
{
    if (lottie_parent == nullptr) {
        printf("[Lottie] Cannot create animation without a parent screen\n");
        return false;
    }

    lv_obj_t *object = lv_lottie_create(lottie_parent);
    if (object == nullptr) {
        printf("[Lottie] Failed to create animation object\n");
        return false;
    }

    uint8_t *buffer = static_cast<uint8_t *>(lv_malloc(kLottieWidth * kLottieHeight * 4));
    if (buffer == nullptr) {
        printf("[Lottie] Failed to allocate render buffer\n");
        lv_obj_delete(object);
        return false;
    }

    lv_obj_set_size(object, kLottieWidth, kLottieHeight);
    lv_obj_set_pos(object, 0, 30);
    lv_obj_set_style_bg_opa(object, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(object, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_lottie_set_buffer(object, kLottieWidth, kLottieHeight, buffer);
    lv_obj_add_event_cb(object, lottie_buffer_delete_cb, LV_EVENT_DELETE, buffer);

    lottie_object = object;
    lottie_buffer = buffer;
    guider_ui.screen_anim_rlottie_1 = object;
    return true;
}

void show_current_lottie()
{
    if (lottie_files.empty() || lottie_parent == nullptr) {
        return;
    }

    char path[256];
    const int written = snprintf(path, sizeof(path), "%s/%s",
                                 kLottieFilePath, lottie_files[current_lottie].c_str());
    if (written < 0 || static_cast<size_t>(written) >= sizeof(path)) {
        printf("[Lottie] Animation path is too long");
        return;
    }

    destroy_lottie_object();
    if (!create_lottie_object()) {
        return;
    }

    lv_lottie_set_src_file(lottie_object, path);

    lv_anim_t *anim = lv_lottie_get_anim(lottie_object);
    const uint32_t default_duration = lv_anim_get_time(anim);
    if (default_duration > 0 && default_duration <= UINT32_MAX / (60 / kLottieFps)) {
        lv_anim_set_duration(anim, default_duration * (60 / kLottieFps));
    }
}

// lottie 专用任务：接管 LVGL 心跳，直到页面退出。
// rlottie 的解析/渲染都在本任务的 40KB 栈上完成，不占用 ui_task 的栈。
void lottie_task_entry(void *param)
{
    (void)param;
    vTaskDelay(pdMS_TO_TICKS(500));
    // 到这里 LVGL 已归本任务独占：创建动画对象、解析 JSON 都在 40KB 栈上完成
    show_current_lottie();
    while (!lottie_task_stop) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    // 交还心跳：先清标志，ui_task 下一拍恢复；清完之后本任务不再触碰 LVGL
    g_lvgl_external_active = false;
    vTaskDelay(pdMS_TO_TICKS(200));
    lottie_task_handle = NULL;
    vTaskDelete(NULL);
}

bool start_lottie_task()
{
    if (lottie_task_handle != NULL) {
        lottie_task_stop = false;   // 任务还活着，复用它（任务退出时会自己把句柄置 NULL）
        return true;
    }

    // 先声明"外部任务将接管"，ui_task 在下一拍就会停止调用 lv_timer_handler()
    g_lvgl_external_active = true;
    lottie_task_stop = false;
    const BaseType_t ret = xTaskCreatePinnedToCore(
        lottie_task_entry,
        "lottie_task",
        LOTTIE_TASK_STACK_WORDS,
        NULL,
        LOTTIE_TASK_PRIO,
        &lottie_task_handle,
        LOTTIE_TASK_CORE);

    if (ret != pdPASS) {
        printf("[Lottie] Failed to create lottie task\n");
        lottie_task_handle = NULL;
        g_lvgl_external_active = false;   // 交还 ui_task，页面退化为主任务渲染
        return false;
    }
    return true;
}

void stop_lottie_task()
{
    if (lottie_task_handle == NULL) {
        // 任务没起来（创建失败 / 没有动画文件），确保心跳仍归 ui_task
        g_lvgl_external_active = false;
        return;
    }
    // 注意：UNLOAD 事件本身就是在 lottie_task 的 lv_timer_handler() 里触发的，
    // 因此这里不能等待或删除自己，只能置停止标志，由任务在 handler 返回后自行退出。
    lottie_task_stop = true;
}

void lottie_click_cb(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED) {
        return;
    }
    if (lottie_files.empty()) {
        printf("[Lottie] Click received, but no JSON files were found");
        return;
    }
    current_lottie = (current_lottie + 1) % lottie_files.size();
    show_current_lottie();
}

}

void lottie_page_init(lv_ui *ui)
{
    lottie_parent = ui->screen_anim;
    lv_obj_add_flag(ui->screen_anim, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->screen_anim, lottie_click_cb, LV_EVENT_CLICKED, nullptr);
}

void lottie_page_load(SCREEN_LOAD_STAUS status)
{
    if (status == UNLOAD) {
        destroy_lottie_object();
        lottie_files.clear();
        current_lottie = 0;
        // 置停止标志，由任务在当前 lv_timer_handler() 返回后自行退出并交还心跳
        stop_lottie_task();
        return;
    }

    lottie_files.clear();
    board.scanDir(kLottieScanPath, &lottie_files, JSON_TYPE);
    current_lottie = 0;
    if (lottie_files.empty()) {
        printf("[Lottie] No .json animations found in /lottie");
        return;
    }
    // 交给专用任务：它先完成心跳接管，再在 40KB 栈上创建/解析/渲染动画
    start_lottie_task();
}
