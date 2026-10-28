#include "ESP32_board.h"
#include "src/MY_FT6336U.h"

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <NimBLEDevice.h>
#include <driver/gpio.h>
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include "game_nofrendo.h"

#include <algorithm>
#include <cstring>

extern "C" {
#include <event.h>
#include <gui.h>
#include <log.h>
#include <memguard.h>
#include <nes/nes.h>
#include <nes/nesinput.h>
#include <nofconfig.h>
#include <nofrendo.h>
#include <osd.h>
}

extern TFT_eSPI my_lcd;
extern FT6336U ft6336u;

namespace {

constexpr uint32_t kNofrendoSampleRate = 16000;
constexpr uint32_t kExitHoldMs = 1000;
constexpr char kConfigPath[] = "/sdcard/nofrendo.cfg";
constexpr char kGamepadServiceUuid[] = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
constexpr char kGamepadInputUuid[] = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
char kExitPrompt[] = "Release to exit";

uint16_t nes_palette[256] = {};
uint16_t lcd_line[320] = {};
uint8_t *frame_storage = nullptr;
bitmap_t *frame_bitmap = nullptr;
bool video_initialized = false;
bool memguard_tracking_ready = false;
esp_timer_handle_t nes_timer = nullptr;
void (*audio_callback)(void *buffer, int length) = nullptr;
constexpr size_t kAudioSamplesPerFrame = kNofrendoSampleRate / NES_REFRESH_RATE;
int16_t audio_frame[kAudioSamplesPerFrame + 1] = {};
int audio_sample_remainder = 0;
portMUX_TYPE controller_mux = portMUX_INITIALIZER_UNLOCKED;
uint32_t controller_state = 0xFFFFFFFF;
bool gamepad_started = false;

class GamepadServerCallbacks : public NimBLEServerCallbacks {
    void onDisconnect(NimBLEServer *, NimBLEConnInfo &, int) override
    {
        portENTER_CRITICAL(&controller_mux);
        controller_state = 0xFFFFFFFF;
        portEXIT_CRITICAL(&controller_mux);
        NimBLEDevice::startAdvertising();
    }
};

class GamepadInputCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *characteristic, NimBLEConnInfo &) override
    {
        const NimBLEAttValue report = characteristic->getValue();
        if (report.size() != 2) {
            return;
        }

        const uint32_t pressed = static_cast<uint8_t>(report[0]) |
            (static_cast<uint32_t>(static_cast<uint8_t>(report[1])) << 8);
        portENTER_CRITICAL(&controller_mux);
        controller_state = 0xFFFFFFFF ^ (pressed & 0x3FF);
        portEXIT_CRITICAL(&controller_mux);
    }
};

GamepadServerCallbacks gamepad_server_callbacks;
GamepadInputCallbacks gamepad_input_callbacks;

uint32_t read_controller(void)
{
    portENTER_CRITICAL(&controller_mux);
    const uint32_t state = controller_state;
    portEXIT_CRITICAL(&controller_mux);
    return state;
}

void initialize_controller(void)
{
    if (gamepad_started) {
        return;
    }

    if (!NimBLEDevice::init("Desk Toy NES Gamepad")) {
        Serial.println("[GAME] NimBLE initialization failed");
        return;
    }

    NimBLEServer *server = NimBLEDevice::createServer();
    if (!server) {
        return;
    }
    server->setCallbacks(&gamepad_server_callbacks, false);

    NimBLEService *service = server->createService(kGamepadServiceUuid);
    if (!service) {
        return;
    }
    NimBLECharacteristic *input = service->createCharacteristic(
        kGamepadInputUuid, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR, 2);
    if (!input) {
        return;
    }
    input->setCallbacks(&gamepad_input_callbacks);
    if (!server->start()) {
        return;
    }

    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();
    advertising->enableScanResponse(true);
    advertising->addServiceUUID(kGamepadServiceUuid);
    advertising->setName("Desk Toy NES Gamepad");
    gamepad_started = advertising->start();
    Serial.printf("[GAME] BLE gamepad %s\n", gamepad_started ? "ready" : "failed");
}

void nes_timer_callback(void *)
{
    ++nofrendo_ticks;
}

int video_driver_init(int width, int height)
{
    if (width != NES_SCREEN_WIDTH || height != NES_SCREEN_HEIGHT) {
        return -1;
    }

    frame_storage = static_cast<uint8_t *>(heap_caps_malloc(
        static_cast<size_t>(width) * height, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (!frame_storage) {
        return -1;
    }

    frame_bitmap = bmp_createhw(frame_storage, width, height, width);
    if (!frame_bitmap) {
        heap_caps_free(frame_storage);
        frame_storage = nullptr;
        return -1;
    }
    video_initialized = true;
    return 0;
}

void video_driver_shutdown(void)
{
    if (frame_bitmap) {
        bmp_destroy(&frame_bitmap);
    }
    heap_caps_free(frame_storage);
    frame_storage = nullptr;
    video_initialized = false;
}

int video_set_mode(int width, int height)
{
    return width == NES_SCREEN_WIDTH && height == NES_SCREEN_HEIGHT ? 0 : -1;
}

void video_set_palette(rgb_t *palette)
{
    for (size_t i = 0; i < 256; ++i) {
        nes_palette[i] = static_cast<uint16_t>(
            ((palette[i].r & 0xF8) << 8) |
            ((palette[i].g & 0xFC) << 3) |
            ((palette[i].b & 0xF8) >> 3));
    }
}

void video_clear(uint8 color)
{
    (void)color;
    my_lcd.fillScreen(TFT_BLACK);
}

bitmap_t *video_lock_write(void)
{
    return frame_bitmap;
}

void video_free_write(int, rect_t *)
{
}

void play_audio_frame(void);

void video_write_frame(bitmap_t *bitmap)
{
    if (!bitmap || !bitmap->line[0]) {
        return;
    }

    const int display_width = std::min(static_cast<int>(my_lcd.width()),
                                       static_cast<int>(NES_SCREEN_WIDTH));
    const int source_x = (NES_SCREEN_WIDTH - display_width) / 2;
    const int display_x = (my_lcd.width() - display_width) / 2;
    const int display_y = (my_lcd.height() - NES_SCREEN_HEIGHT) / 2;

    my_lcd.startWrite();
    my_lcd.setAddrWindow(display_x, display_y, display_width, NES_SCREEN_HEIGHT);
    for (int y = 0; y < NES_SCREEN_HEIGHT; ++y) {
        const uint8_t *source = bitmap->line[y] + source_x;
        for (int x = 0; x < display_width; ++x) {
            lcd_line[x] = nes_palette[source[x]];
        }
        my_lcd.pushColors(lcd_line, display_width, true);
    }
    my_lcd.endWrite();
}

void video_blit(bitmap_t *bitmap, int, rect_t *)
{
    video_write_frame(bitmap);
}

viddriver_t nofrendo_video_driver = {
    "Desk Toy TFT_eSPI",
    video_driver_init,
    video_driver_shutdown,
    video_set_mode,
    video_set_palette,
    video_clear,
    video_lock_write,
    video_free_write,
    video_blit,
    false,
};

void initialize_audio(void)
{
    if (auto device = board.getAudioDeviceEs8311()) {
        device->OpenOutput(kNofrendoSampleRate);
    }
}

void shutdown_audio(void)
{
    audio_callback = nullptr;
    audio_sample_remainder = 0;
}

void play_audio_frame(void)
{
    if (!audio_callback) {
        return;
    }

    auto device = board.getAudioDeviceEs8311();
    if (!device) {
        return;
    }

    const int accumulated_samples = kNofrendoSampleRate + audio_sample_remainder;
    const int samples = accumulated_samples / NES_REFRESH_RATE;
    audio_sample_remainder = accumulated_samples % NES_REFRESH_RATE;
    audio_callback(audio_frame, samples);
    for (int i = 0; i < samples; ++i) {
        audio_frame[i] >>= 2;
    }
    device->Write(audio_frame, static_cast<size_t>(samples));
}

void controller_init(void)
{
    initialize_controller();
}

uint32_t controller_read_input(void)
{
    return read_controller();
}

int log_print(const char *message)
{
    return printf("%s", message);
}

void wait_for_touch_release(void)
{
    while (ft6336u.scan().touch_count != 0) {
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

} // namespace

extern "C" void *mem_alloc(int size, bool prefer_fast_memory)
{
    if (size <= 0) {
        return nullptr;
    }
    if (!prefer_fast_memory && memguard_tracking_ready) {
        static char allocator_file[] = "game_nofrendo.cpp";
        return _my_malloc(size, allocator_file, __LINE__);
    }
    if (prefer_fast_memory) {
        return heap_caps_malloc(static_cast<size_t>(size), MALLOC_CAP_8BIT);
    }
    return heap_caps_malloc_prefer(static_cast<size_t>(size), MALLOC_CAP_SPIRAM, MALLOC_CAP_8BIT);
}

extern "C" void osd_setsound(void (*playfunc)(void *, int))
{
    audio_callback = playfunc;
}

extern "C" void osd_audio_frame(void)
{
    play_audio_frame();
}

extern "C" void osd_getsoundinfo(sndinfo_t *info)
{
    info->sample_rate = kNofrendoSampleRate;
    info->bps = 16;
}

extern "C" int osd_init(void)
{
    nofrendo_log_chain_logfunc(log_print);
    initialize_audio();
    controller_init();
    return 0;
}

extern "C" void osd_shutdown(void)
{
    shutdown_audio();
    if (nes_timer) {
        esp_timer_stop(nes_timer);
        esp_timer_delete(nes_timer);
        nes_timer = nullptr;
    }
}

extern "C" void osd_getvideoinfo(vidinfo_t *info)
{
    info->default_width = NES_SCREEN_WIDTH;
    info->default_height = NES_SCREEN_HEIGHT;
    info->driver = &nofrendo_video_driver;
}

extern "C" int osd_installtimer(int frequency, void *, int, void *, int)
{
    if (frequency <= 0) {
        return -1;
    }
    esp_timer_create_args_t timer_args = {};
    timer_args.callback = nes_timer_callback;
    timer_args.dispatch_method = ESP_TIMER_TASK;
    timer_args.name = "nes";
    if (esp_timer_create(&timer_args, &nes_timer) != ESP_OK) {
        nes_timer = nullptr;
        return -1;
    }
    const uint64_t period_us = (1000000ULL + static_cast<uint32_t>(frequency / 2)) /
        static_cast<uint32_t>(frequency);
    if (esp_timer_start_periodic(nes_timer, period_us) != ESP_OK) {
        if (nes_timer) {
            esp_timer_delete(nes_timer);
            nes_timer = nullptr;
        }
        return -1;
    }
    return 0;
}

extern "C" void osd_getinput(void)
{
    static uint32_t previous_state = 0xFFFFFFFF;
    static bool touch_active = false;
    static bool exit_pending = false;
    static uint32_t touch_started_at = 0;
    static const int input_events[10] = {
        event_joypad1_up, event_joypad1_down, event_joypad1_left, event_joypad1_right,
        event_joypad1_select, event_joypad1_start, event_joypad1_a, event_joypad1_b,
        event_none, event_none,
    };

    uint32_t state = controller_read_input();
    uint32_t changed = state ^ previous_state;
    previous_state = state;
    for (size_t bit = 0; bit < 10; ++bit) {
        if ((changed & (1u << bit)) == 0) {
            continue;
        }
        event_t handler = event_get(input_events[bit]);
        if (handler) {
            handler((state & (1u << bit)) ? INP_STATE_BREAK : INP_STATE_MAKE);
        }
    }

    const FT6336U_TouchPointType touch = ft6336u.scan();
    if (touch.touch_count) {
        if (!touch_active) {
            touch_active = true;
            touch_started_at = millis();
            exit_pending = false;
        } else if (!exit_pending && millis() - touch_started_at >= kExitHoldMs) {
            exit_pending = true;
            gui_sendmsg(GUI_YELLOW, kExitPrompt);
        }
    } else {
        if (exit_pending) {
            event_t quit_handler = event_get(event_quit);
            if (quit_handler) {
                quit_handler(INP_STATE_MAKE);
            }
        }
        touch_active = false;
        exit_pending = false;
    }
}

extern "C" void osd_getmouse(int *, int *, int *)
{
}

extern "C" int osd_main(int argc, char *argv[])
{
    if (argc < 1 || !argv || !argv[0]) {
        return -1;
    }
    config.filename = const_cast<char *>(kConfigPath);
    return main_loop(argv[0], system_autodetect);
}

extern "C" void osd_fullname(char *fullname, const char *shortname)
{
    if (!fullname || !shortname) {
        return;
    }
    strncpy(fullname, shortname, PATH_MAX - 1);
    fullname[PATH_MAX - 1] = '\0';
}

extern "C" char *osd_newextension(char *filename, char *extension)
{
    if (!filename || !extension) {
        return filename;
    }
    const size_t length = strlen(filename);
    if (length >= 3 && strlen(extension) >= 4) {
        filename[length - 3] = extension[1];
        filename[length - 2] = extension[2];
        filename[length - 1] = extension[3];
    }
    return filename;
}

extern "C" int osd_makesnapname(char *, int)
{
    return -1;
}

int nofrendo_run_rom(const char *rom_path)
{
    static char allocator_file[] = "game_nofrendo.cpp";
    memguard_tracking_ready = false;
    void *allocator_probe = _my_malloc(1, allocator_file, __LINE__);
    memguard_tracking_ready = true;
    _my_free(&allocator_probe, allocator_file, __LINE__);

    char *arguments[] = {const_cast<char *>(rom_path)};
    const int result = nofrendo_main(1, arguments);
    wait_for_touch_release();

    osd_shutdown();
    config.close();
    gui_shutdown();
    if (video_initialized) {
        vid_shutdown();
    }
    nofrendo_log_shutdown();
    return result;
}