#include "lottie.h"

#include "ESP32_board.h"
#include "src/generated/gui_guider.h"
#include "ai_vox_engine.h"
#include <stdint.h>
#include <stdio.h>
#include <vector>

namespace {

constexpr char kLottieScanPath[] = "S:/lottie";
constexpr char kLottieFilePath[] = "/sdcard/lottie";
constexpr int32_t kLottieWidth = 240;
constexpr int32_t kLottieHeight = 240;
constexpr uint32_t kLottieFps = 30;

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
        return;
    }

    lottie_files.clear();
    board.scanDir(kLottieScanPath, &lottie_files, JSON_TYPE);
    current_lottie = 0;
    if (lottie_files.empty()) {
        printf("[Lottie] No .json animations found in /lottie");
        return;
    }
    show_current_lottie();
}
