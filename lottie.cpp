#include "lottie.h"

#include "ESP32_board.h"
#include "src/generated/gui_guider.h"
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

void show_current_lottie()
{
    if (lottie_files.empty() || lottie_object == nullptr || lottie_buffer == nullptr) {
        return;
    }

    char path[256];
    const int written = snprintf(path, sizeof(path), "%s/%s",
                                 kLottieFilePath, lottie_files[current_lottie].c_str());
    if (written < 0 || static_cast<size_t>(written) >= sizeof(path)) {
        Serial.println("[Lottie] Animation path is too long");
        return;
    }

    Serial.printf("[Lottie] Loading %u/%u: %s\n",
                  static_cast<unsigned>(current_lottie + 1),
                  static_cast<unsigned>(lottie_files.size()),
                  lottie_files[current_lottie].c_str());
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
        Serial.println("[Lottie] Click received, but no JSON files were found");
        return;
    }

    current_lottie = (current_lottie + 1) % lottie_files.size();
    show_current_lottie();
}

}

void lottie_page_init(lv_ui *ui)
{
    lottie_object = lv_lottie_create(ui->screen_anim);
    ui->screen_anim_rlottie_1 = lottie_object;
    lv_obj_set_size(lottie_object, kLottieWidth, kLottieHeight);
    lv_obj_set_pos(lottie_object, 0, 30);
    lv_obj_set_style_bg_opa(lottie_object, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(lottie_object, LV_OBJ_FLAG_EVENT_BUBBLE);

    lottie_buffer = static_cast<uint8_t *>(lv_malloc(kLottieWidth * kLottieHeight * 4));
    if (lottie_buffer == nullptr) {
        Serial.println("[Lottie] Failed to allocate render buffer");
        return;
    }

    lv_lottie_set_buffer(lottie_object, kLottieWidth, kLottieHeight, lottie_buffer);
    lv_obj_add_event_cb(lottie_object, lottie_buffer_delete_cb, LV_EVENT_DELETE, lottie_buffer);

    lv_obj_add_flag(ui->screen_anim, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->screen_anim, lottie_click_cb, LV_EVENT_CLICKED, nullptr);
}

void lottie_page_load(SCREEN_LOAD_STAUS status)
{
    if (status == UNLOAD) {
        lottie_files.clear();
        current_lottie = 0;
        return;
    }

    lottie_files.clear();
    board.scanDir(kLottieScanPath, &lottie_files, JSON_TYPE);
    current_lottie = 0;
    if (lottie_files.empty()) {
        Serial.println("[Lottie] No .json animations found in /lottie");
        return;
    }

    Serial.printf("[Lottie] Found %u JSON file(s)\n",
                  static_cast<unsigned>(lottie_files.size()));
    show_current_lottie();
}
