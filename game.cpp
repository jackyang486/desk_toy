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
#include <cstring>
#include <vector>

static const char *game_directory = "S:/game";
static constexpr char kGameVfsDirectory[] = "/sdcard/game";
static constexpr int kSwipeThreshold = 20;
static constexpr size_t kListItemCount = 20;
static constexpr int kListItemHeight = 35;
static MusicPathList game_files;   // 游戏路径放 PSRAM 池，不再用 vector<String> 切碎内部堆
// scanDirEach 回调：文件名直接拷进 PSRAM 路径池，不产生内部堆碎片
static bool game_path_sink(void *ctx, const char *name) {
    return static_cast<MusicPathList *>(ctx)->push(name);
}
static String requested_game;
static std::atomic<bool> game_active{false};
static bool game_start_pending = false;
static bool game_resume_pending = false;
static size_t list_start_index = 0;
static int touch_start_y = 0;
static bool list_is_touching = false;
static bool list_swipe_registered = false;

extern TFT_eSPI my_lcd;


static constexpr uint16_t kGamepadTcpPort = 3333;
// 多久没收到数据算掉线。不能设太短：手柄完全可能一二十秒不按键（看剧情、菜单里发呆），
// 之前设 10s 就出现了"连上几秒钟就自己断开"。客户端若想长期挂着不按键，
// 建议每 20~30s 重发一次当前按键状态当心跳（重发同一帧不会有副作用）。
static constexpr uint32_t kGamepadClientTimeoutMs = 60000;

static portMUX_TYPE controller_mux = portMUX_INITIALIZER_UNLOCKED;
static uint32_t controller_state = 0xFFFFFFFF;   // 按键位图，bit=0 表示按下

static WiFiServer gamepad_server(kGamepadTcpPort);
static WiFiClient gamepad_client;
static String gamepad_text_line;   // 文本帧（JSON）的行缓冲
static TaskHandle_t gamepad_task_handle = nullptr;   // 不加 volatile：stop() 要把它的地址传给 xTaskCreate
static bool volatile gamepad_task_stop = false;
static bool volatile gamepad_client_connected = false;
static bool gamepad_started = false;

static void gamepad_apply_pressed(uint32_t pressed)
{
    portENTER_CRITICAL(&controller_mux);
    controller_state = 0xFFFFFFFF ^ (pressed & 0x3FF);
    portEXIT_CRITICAL(&controller_mux);
}

static void gamepad_apply_report(const uint8_t *report)
{
    // 小端 2 字节：byte0 = bit0..7，byte1 = bit8..9
    const uint32_t pressed = static_cast<uint32_t>(report[0]) |
                             (static_cast<uint32_t>(report[1]) << 8);
    gamepad_apply_pressed(pressed);
}

/* 文本帧（备用 / 调试）：{"type":"nes_gamepad","bitmap":65}\n
   用串口/网络调试助手手敲一行就能验证链路，不依赖 App 的 hex 编码是否一致。
   必须是 '{' 开头才会走文本分支，否则按 2 字节二进制解析。 */
static bool gamepad_apply_text(const char *line)
{
    const char *value = strstr(line, "bitmap");
    if (value) {
        value = strchr(value, ':');
        if (!value) {
            return false;
        }
        ++value;
    } else {
        value = line;
    }
    while (*value == ' ' || *value == '\t') {
        ++value;
    }

    char *end = nullptr;
    const long pressed = strtol(value, &end, 0);
    if (end == value) {
        return false;
    }
    gamepad_apply_pressed(pressed > 0 ? static_cast<uint32_t>(pressed) : 0);
    return true;
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
    static uint32_t last_logged_state = 0xFFFFFFFF;

    for (;;) {
        if (gamepad_task_stop) {
            break;
        }

        // 每次循环都看一眼 listen，哪怕当前还有客户端连着：
        // 手机断网/WiFi 重连经常留下"半开连接"——老 socket 看着还连着，实际已经不通了。
        // 这时新客户端来了要能顶掉它，否则 App 会一直连不上。
        // accept() 非阻塞：没有待接受的连接时返回的 client 为假
        WiFiClient incoming = gamepad_server.accept();
        if (incoming) {
            if (gamepad_client) {
                printf("[GAME] gamepad client replaced\n");
                gamepad_client.stop();
            }
            incoming.setNoDelay(true);
            incoming.setTimeout(1);
            gamepad_client = incoming;
            gamepad_client_connected = true;
            gamepad_text_line = "";
            last_rx = millis();
            printf("[GAME] gamepad client %s\n", gamepad_client.remoteIP().toString().c_str());
            vTaskDelay(pdMS_TO_TICKS(2));
            continue;
        }

        if (!gamepad_client || !gamepad_client.connected()) {
            if (gamepad_client_connected) {
                printf("[GAME] gamepad client closed by peer\n");
            }
            gamepad_client.stop();
            gamepad_client_connected = false;
            gamepad_text_line = "";
            gamepad_reset_state();
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        // 收到任何字节都算活跃（哪怕不足一整帧），心跳格式不同也不会被误判成掉线
        if (gamepad_client.available() > 0) {
            last_rx = millis();
        }

        // 只认 '{' 开头的 JSON 文本帧。别用"是不是数字"来判断：
        // 二进制帧的低字节可能是 0x30~0x39（例如 Select+Start = 0x30），会被误判成文本。
        const int head = gamepad_client.peek();
        if (head == '{') {
            // 文本帧：按行读取，读满一行再解析
            while (gamepad_client.available() > 0) {
                const int ch = gamepad_client.read();
                if (ch < 0) {
                    break;
                }
                if (ch == '\n' || ch == '\r') {
                    if (gamepad_text_line.length()) {
                        gamepad_apply_text(gamepad_text_line.c_str());
                        gamepad_text_line = "";
                    }
                    break;
                }
                if (gamepad_text_line.length() < 128) {
                    gamepad_text_line += static_cast<char>(ch);
                }
            }
            last_rx = millis();
        } else {
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
        }

        // 按键状态变化打一条日志：串口上一眼就能看出到底收没收着、收到的是什么
        if (controller_state != last_logged_state) {
            last_logged_state = controller_state;
            printf("[GAME] gamepad bitmap 0x%03X\n",
                   static_cast<unsigned>((~controller_state) & 0x3FF));
        }

        if (millis() - last_rx > kGamepadClientTimeoutMs) {
            printf("[GAME] gamepad client idle timeout, closing\n");
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

    requested_game = game_files.at(index);
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
            lv_obj_t *button = lv_list_add_btn(list, nullptr, game_files.at(i));
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
            lv_label_set_text(label, game_files.at(list_start_index + i));
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
    // UNLOAD 必须先处理：否则 WiFi 掉线时直接 return，任务和 server 会一直留着
    if (status == UNLOAD) {
        // 退出游戏界面：停掉 TCP server 和收包任务
        gamepad_stop();
        return;
    }
    if (status != LOADED) {
        return;
    }
    if(WiFi.status() != WL_CONNECTED){
          show_toast(lv_scr_act(), "WIFI未连接!");
          return;
    }
    char text[64];
    snprintf(text, sizeof(text), "%s:%u [%s]",
                 WiFi.localIP().toString().c_str(), static_cast<unsigned>(kGamepadTcpPort));
    lv_label_set_text(page_head.time_label, text);
    gamepad_start();
    //game_process_pending();
    game_files.clear();
    list_start_index = 0;
    board.scanDirEach(game_directory, game_path_sink, &game_files, NES_TYPE);
    lv_obj_clean(guider_ui.screen_game_list_game);
    if (!list_swipe_registered) {
        lv_obj_add_event_cb(guider_ui.screen_game_list_game, game_list_swipe_event_handler, LV_EVENT_ALL, nullptr);
        list_swipe_registered = true;
    }
    fill_game_list();
}