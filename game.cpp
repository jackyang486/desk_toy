#include "game.h"
#include "ESP32_board.h"
#include "src/generated/gui_guider.h"
#include "src/MY_FT6336U.h"
#include "game_nofrendo.h"
#include <TFT_eSPI.h>
#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstdio>
#include <vector>

static const char *game_directory = "S:/game";
static constexpr char kGameVfsDirectory[] = "/sdcard/game";
static constexpr int kSwipeThreshold = 20;
static constexpr size_t kListItemCount = 20;
static constexpr int kListItemHeight = 35;
static std::vector<String> game_files;
static String requested_game;
static std::atomic<bool> game_active{false};
static bool game_start_pending = false;
static bool game_resume_pending = false;
static size_t list_start_index = 0;
static int touch_start_y = 0;
static bool list_is_touching = false;
static bool list_swipe_registered = false;

extern TFT_eSPI my_lcd;

static void game_list_item_clicked(lv_event_t *event)
{
    if (lv_event_get_code(event) != LV_EVENT_CLICKED || game_active.load()) {
        return;
    }

    lv_obj_t *button = static_cast<lv_obj_t *>(lv_event_get_target(event));
    const size_t index = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(button));
    if (index >= game_files.size()) {
        return;
    }

    requested_game = game_files[index];
    game_start_pending = true;
    game_active.store(true);
}

static void fill_game_list()
{
    lv_obj_t *list = guider_ui.screen_game_list_game;
    const size_t item_count = std::min(kListItemCount, game_files.size());

    if (lv_obj_get_child_count(list) != item_count) {
        lv_obj_clean(list);
        for (size_t i = 0; i < item_count; ++i) {
            lv_obj_t *button = lv_list_add_btn(list, nullptr, game_files[i].c_str());
            if (!button) {
                continue;
            }
            lv_obj_set_height(button, kListItemHeight);
            lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<uintptr_t>(i)));
            lv_obj_add_event_cb(button, game_list_item_clicked, LV_EVENT_CLICKED, nullptr);
            lv_obj_t *label = lv_obj_get_child(button, 0);
            if (label) {
                lv_obj_set_style_text_font(label, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
        return;
    }

    for (size_t i = 0; i < item_count; ++i) {
        lv_obj_t *button = lv_obj_get_child(list, i);
        lv_obj_t *label = lv_obj_get_child(button, 0);
        lv_obj_set_user_data(button, reinterpret_cast<void *>(static_cast<uintptr_t>(list_start_index + i)));
        if (label) {
            lv_obj_set_style_text_font(label, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(label, game_files[list_start_index + i].c_str());
        }
    }
}

static void game_list_swipe_event_handler(lv_event_t *event)
{
    const lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_SCROLL_BEGIN) {
        lv_indev_t *indev = lv_indev_get_act();
        if (indev && !list_is_touching) {
            lv_point_t point;
            lv_indev_get_point(indev, &point);
            touch_start_y = point.y;
            list_is_touching = true;
        }
        return;
    }

    if (code != LV_EVENT_SCROLL_END || !list_is_touching) {
        return;
    }

    list_is_touching = false;
    lv_indev_t *indev = static_cast<lv_indev_t *>(lv_event_get_param(event));
    if (!indev) {
        return;
    }

    lv_point_t point;
    lv_indev_get_point(indev, &point);
    const int delta_y = touch_start_y - point.y;
    if (abs(delta_y) <= kSwipeThreshold) {
        return;
    }

    const int steps = delta_y / kSwipeThreshold;
    const size_t max_start_index = game_files.size() > kListItemCount
        ? game_files.size() - kListItemCount
        : 0;
    int next_index = static_cast<int>(list_start_index) + steps;
    if (next_index < 0) {
        next_index = 0;
    } else if (next_index > static_cast<int>(max_start_index)) {
        next_index = static_cast<int>(max_start_index);
    }
    list_start_index = static_cast<size_t>(next_index);
    fill_game_list();
}

void game_process_pending(void)
{
    if (game_resume_pending) {
        game_resume_pending = false;
        game_active.store(false);
        return;
    }
    if (!game_start_pending) {
        return;
    }
    game_start_pending = false;

    char rom_path[256];
    const int path_length = snprintf(rom_path, sizeof(rom_path), "%s/%s",
                                     kGameVfsDirectory, requested_game.c_str());
    if (path_length < 0 || static_cast<size_t>(path_length) >= sizeof(rom_path)) {
        printf("[GAME] ROM path is too long\n");
    } else {
        printf("[GAME] Starting %s\n", rom_path);
        my_lcd.setRotation(1);
        my_lcd.fillScreen(TFT_BLACK);
        nofrendo_run_rom(rom_path);
        my_lcd.setRotation(0);
    }
    requested_game = "";
    my_lcd.fillScreen(TFT_WHITE);
    lv_obj_invalidate(lv_screen_active());
    game_resume_pending = true;
}

bool game_is_active(void)
{
    return game_active.load();
}

void game_screen_load(SCREEN_LOAD_STAUS status)
{
    if (status != LOADED) {
        return;
    }
    //game_process_pending();
    game_files.clear();
    list_start_index = 0;
    board.scanDir(game_directory, &game_files, NES_TYPE);
    lv_obj_clean(guider_ui.screen_game_list_game);
    if (!list_swipe_registered) {
        lv_obj_add_event_cb(guider_ui.screen_game_list_game, game_list_swipe_event_handler, LV_EVENT_ALL, nullptr);
        list_swipe_registered = true;
    }
    fill_game_list();
}