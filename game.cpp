#include "game.h"
#include "ESP32_board.h"
#include "src/generated/gui_guider.h"
#include "src/MY_FT6336U.h"
#include "game_nofrendo.h"
#include <Arduino.h>
#include <WiFi.h>
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

/*==================== TCP 手柄 ====================*/
// 手柄输入走 WiFi TCP server：手机/PC 上的客户端连到板子 IP:3333，每帧发 2 字节按键位图。
// 换掉 BLE 的原因：BLE controller+host 常驻要吃掉约 65KB internal DRAM，而且反复
// init/deinit 会把堆永久打碎（几轮后 init() 直接 fault 重启）、btStop() 还会卡死界面。
// WiFi 本工程本来就在用，TCP server 几乎不额外占内存。
//
// 生命周期同原来一样挂在游戏列表页面上：LOADED 起 server + 收包任务，UNLOAD 停掉。
// 收包跑在独立任务里，所以 nofrendo 运行时（ui_task 被游戏阻塞）也照样收得到按键。
//
// 协议（沿用之前 BLE 那 2 字节报告的语义，小端）：
//   byte0 bit0..7 = Up Down Left Right Select Start A B
//   byte1 bit0..1 = 保留（bit8 / bit9）
//   bit = 1 表示按下。服务端取反后写入 controller_state（nofrendo 侧 bit=0 表示按下）。
static constexpr uint16_t kGamepadTcpPort = 3333;
static constexpr uint32_t kGamepadClientTimeoutMs = 10000;   // 多久没数据算掉线

static portMUX_TYPE controller_mux = portMUX_INITIALIZER_UNLOCKED;
static uint32_t controller_state = 0xFFFFFFFF;   // 按键位图，bit=0 表示按下

static WiFiServer gamepad_server(kGamepadTcpPort);
static WiFiClient gamepad_client;
static TaskHandle_t gamepad_task_handle = nullptr;   // 不加 volatile：stop() 要把它的地址传给 xTaskCreate
static bool volatile gamepad_task_stop = false;
static bool volatile gamepad_client_connected = false;
static bool gamepad_started = false;

static void gamepad_apply_report(const uint8_t *report)
{
    const uint32_t pressed = static_cast<uint32_t>(report[0]) |
                             (static_cast<uint32_t>(report[1]) << 8);
    portENTER_CRITICAL(&controller_mux);
    controller_state = 0xFFFFFFFF ^ (pressed & 0x3FF);
    portEXIT_CRITICAL(&controller_mux);
}

static void gamepad_reset_state(void)
{
    portENTER_CRITICAL(&controller_mux);
    controller_state = 0xFFFFFFFF;
    portEXIT_CRITICAL(&controller_mux);
}

static void gamepad_tcp_task(void *)
{
    uint32_t last_rx = millis();

    for (;;) {
        if (gamepad_task_stop) {
            break;
        }

        if (!gamepad_client || !gamepad_client.connected()) {
            if (gamepad_client_connected) {
                printf("[GAME] gamepad client disconnected\n");
            }
            gamepad_client.stop();
            gamepad_client_connected = false;
            gamepad_reset_state();

            // accept() 非阻塞：没有待接受的连接时返回的 client 为假
            gamepad_client = gamepad_server.accept();
            if (gamepad_client) {
                gamepad_client.setNoDelay(true);
                gamepad_client.setTimeout(1);
                gamepad_client_connected = true;
                last_rx = millis();
                printf("[GAME] gamepad client %s\n", gamepad_client.remoteIP().toString().c_str());
            }
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        const int available = gamepad_client.available();
        if (available >= 2) {
            // 客户端发得比我们读得快时只保留最新一帧：手柄输入宁可丢帧也不要延迟。
            uint8_t scratch[64];
            int drop = available - 2;
            while (drop > 0) {
                const size_t want = drop > 64 ? 64 : static_cast<size_t>(drop);
                const int got = gamepad_client.read(scratch, want);
                if (got <= 0) {
                    break;
                }
                drop -= got;
            }
            uint8_t report[2];
            if (gamepad_client.read(report, 2) == 2) {
                gamepad_apply_report(report);
                last_rx = millis();
            }
        }

        if (millis() - last_rx > kGamepadClientTimeoutMs) {
            gamepad_client.stop();   // 超时断开，下一轮重新 accept
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(2));
    }

    gamepad_client.stop();
    gamepad_client_connected = false;
    gamepad_reset_state();
    gamepad_task_handle = nullptr;
    vTaskDelete(nullptr);
}

void gamepad_start(void)
{
    if (gamepad_started || gamepad_task_handle != nullptr) {
        return;
    }

    if (WiFi.status() != WL_CONNECTED) {
        printf("[GAME] WiFi not connected, gamepad server skipped\n");
        return;
    }

    gamepad_task_stop = false;
    gamepad_server.begin();
    gamepad_server.setNoDelay(true);

    // 固定到 core 0：nofrendo 跑在 ui_task（core 1）上且会把那个核吃满，
    // 收包任务必须待在另一个核，否则游戏运行时按键会明显延迟。
    if (xTaskCreatePinnedToCore(gamepad_tcp_task, "gamepad_tcp", 4096, nullptr, 1,
                                &gamepad_task_handle, 0) != pdPASS) {
        gamepad_server.stop();
        printf("[GAME] gamepad task create failed\n");
        return;
    }

    gamepad_started = true;
    printf("[GAME] gamepad TCP server on %s:%u\n",
           WiFi.localIP().toString().c_str(), static_cast<unsigned>(kGamepadTcpPort));
}

void gamepad_stop(void)
{
    gamepad_started = false;

    // 让任务自己退出并负责关 socket、复位按键状态，这里只做有上限的等待
    if (gamepad_task_handle != nullptr) {
        gamepad_task_stop = true;
        for (int i = 0; i < 100 && gamepad_task_handle != nullptr; i++) {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        if (gamepad_task_handle != nullptr) {
            printf("[GAME] gamepad task did not exit in time\n");
        }
    }

    gamepad_server.stop();
    gamepad_client.stop();
    gamepad_client_connected = false;
    gamepad_reset_state();
    printf("[GAME] gamepad TCP server stopped\n");
}

uint32_t gamepad_read_state(void)
{
    portENTER_CRITICAL(&controller_mux);
    const uint32_t state = controller_state;
    portEXIT_CRITICAL(&controller_mux);
    return state;
}

/* ---- 游戏列表页顶部显示 IP:端口 和连接状态 ----
   挂在 lv_layer_top() 上：generated 的 screen_game 可能被删除重建，
   顶层对象不会跟着失效，指针不会被悄悄变成野指针。 */
static lv_obj_t *gamepad_status_label = nullptr;
static uint32_t gamepad_label_last_update = 0;

static void gamepad_show_status_label(bool show)
{
    if (gamepad_status_label == nullptr) {
        gamepad_status_label = lv_label_create(lv_layer_top());
        lv_obj_set_width(gamepad_status_label, lv_pct(100));
        lv_obj_align(gamepad_status_label, LV_ALIGN_TOP_MID, 0, 2);
        lv_obj_set_style_text_align(gamepad_status_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_label_set_long_mode(gamepad_status_label, LV_LABEL_LONG_WRAP);   // 屏窄时折行，不裁掉 IP
        lv_obj_set_style_text_color(gamepad_status_label, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_font(gamepad_status_label, &font_puhui_16_4,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gamepad_status_label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(gamepad_status_label, LV_OPA_80, LV_PART_MAIN);
    }
    if (show) {
        lv_obj_remove_flag(gamepad_status_label, LV_OBJ_FLAG_HIDDEN);
        gamepad_label_last_update = 0;   // 下一帧立刻刷新文本，不用等 500ms
    } else {
        lv_obj_add_flag(gamepad_status_label, LV_OBJ_FLAG_HIDDEN);
    }
}

// 只能在 ui_task 里调用（LVGL 不是线程安全的）
static void gamepad_update_status_label(void)
{
    if (gamepad_status_label == nullptr || lv_obj_has_flag(gamepad_status_label, LV_OBJ_FLAG_HIDDEN)) {
        return;
    }
    if (millis() - gamepad_label_last_update < 500) {
        return;
    }
    gamepad_label_last_update = millis();

    char text[64];
    if (WiFi.status() == WL_CONNECTED) {
        snprintf(text, sizeof(text), "Gamepad %s:%u [%s]",
                 WiFi.localIP().toString().c_str(), static_cast<unsigned>(kGamepadTcpPort),
                 gamepad_client_connected ? "connected" : "waiting");
    } else {
        snprintf(text, sizeof(text), "Gamepad needs WiFi");
    }
    lv_label_set_text(gamepad_status_label, text);
}
/*================== TCP 手柄结束 ==================*/

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
    gamepad_update_status_label();   // 在 ui_task 线程里刷新 LVGL 标签，跨线程调用不安全

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
    if (status == UNLOAD) {
        // 退出游戏界面：停掉 TCP server 和收包任务
        gamepad_stop();
        gamepad_show_status_label(false);
        return;
    }

    if (status != LOADED) {
        return;
    }
    // 进入游戏列表界面即开启手柄服务，这样列表页和进游戏后都能连上手柄
    gamepad_show_status_label(true);
    gamepad_start();
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