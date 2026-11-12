#include "radio.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include "ESP32_board.h"
#include "ai_vox_engine.h"
#include "music_audio.h"
#include "setting.h"

const RadioInfo radio_infos[] = {
    {"蜻蜓FM-清晨音乐台","http://lhttp.qingting.fm/live/4915/64k.mp3"},
    {"蜻蜓FM-两广之声音乐","http://lhttp.qingting.fm/live/20500149/64k.mp3"},
    {"蜻蜓FM-河北音乐广播","http://lhttp.qingting.fm/live/1649/64k.mp3"},
    {"蜻蜓FM-怀集音乐之声","http://lhttp.qingting.fm/live/4804/64k.mp3"},
    {"央视官方-中国之声","http://ngcdn001.cnr.cn/live/zgzs/index.m3u8"},
    {"上海LoveRadio 103.7 全天华语流行新歌","http://lhttp.qingting.fm/live/273/64k.mp3"},
    {"广东音乐之声 流行金曲","http://lhttp.qingting.fm/live/1260/64k.mp3"},
    {"江苏Play897 热门流行榜单","http://lhttp.qingting.fm/live/4936/64k.mp3"},
    {"江苏经典流行音乐台 怀旧华语","http://lhttp.qingting.fm/live/4938/64k.mp3"},
    {"湖北经典音乐广播 华语抒情流行","http://lhttp.qingting.fm/live/1296/64k.mp3"},
    {"上海经典947 纯轻音乐、纯流行纯器乐","http://lhttp.qingting.fm/live/267/64k.mp3"},
    {"山东经典音乐广播 老歌流行台","http://lhttp.qingting.fm/live/20240/64k.mp3"},
    {"怀旧年代金曲台 80/90华语老歌","http://lhttp.qingting.fm/live/20211686/64k.mp3"},
    {"两广之声音乐台 通俗流行","http://lhttp.qingting.fm/live/20500149/64k.mp3"},
    {"苏州儿童广播 全天儿歌童谣","http://lhttp.qingting.fm/live/2807/64k.mp3"},
    {"大连少儿广播 儿童歌曲+小故事","https://lhttp.qtfm.cn/live/1084/64k.mp3"},
    {"长春少儿生活广播 亲子儿歌","https://lhttp.qtfm.cn/live/4984/64k.mp3"},
};

Radio& Radio::GetInstance() {
    static Radio instance;
    return instance;
}

void Radio::Init() {
      playing_ = false;
      current_index_ = 0;
}

/* 把当前台名/状态/播放键刷到播放页（screen_radio） */
void Radio::ui_refresh() {
    lv_label_set_text(guider_ui.screen_radio_label_radio_name, radio_infos[current_index_].name);
}

/* 启动播放引擎（复用音乐页那套：静态解码任务 + PSRAM 缓冲 + PCM 直写 ES8311） */
bool Radio::ensure_engine() {
    if (music_audio.is_active()) return true;
    // 硬件音量拉满，响度只由解码 PCM 的软件音量决定（与音乐页一致）
    Setting::GetInstance().PreparePlaybackVolume();
    music_audio.set_volume((uint8_t)Setting::GetInstance().GetVolume());
    if (!music_audio.begin()) {
        printf("[radio] audio engine start failed\n");
        return false;
    }
    return true;
}

void Radio::TogglePlay() {
    if (playing_) {
        // 暂停：解码任务停读网络、DMA 排空成静音；恢复时会丢弃积压数据保持实时
        music_audio.set_paused(true);
        playing_ = false;
    } else if (music_audio.is_active() && music_audio.is_stream()) {
        music_audio.set_paused(false);
        playing_ = true;
    } else {
        PlayStation(current_index_);   // 还没起播过（或引擎已退出）：直接开播当前台
        return;
    }
    ui_refresh();
}

void Radio::PlayStation(size_t index) {
    const size_t size = sizeof(radio_infos)/sizeof(radio_infos[0]);
    if (size == 0) return;
    current_index_ = index % size;

    if (WiFi.status() != WL_CONNECTED) {
        playing_ = false;
        ui_refresh();
        show_toast(lv_screen_active(), "WIFI未连接!");
        return;
    }
    if (!ensure_engine()) {
        playing_ = false;
        ui_refresh();
        show_toast(lv_screen_active(), "音频初始化失败");
        return;
    }

    playing_ = true;
    music_audio.play_stream(radio_infos[current_index_].url);
    printf("[radio] play %s -> %s\n", radio_infos[current_index_].name, radio_infos[current_index_].url);
    ui_refresh();
}

void Radio::radio_screen_load(SCREEN_LOAD_STAUS status){
    if (status == LOADED) {
        // 进页面只刷新 UI，不自动起播：等用户点播放键（省流量，也避免和唤醒词抢 I2S）
        ui_refresh();
    } else {
        // 退页面：停流 + 释放解码任务，codec 恢复默认 16k 单声道给 AI_VOX 用
        StopPlay();
        music_audio.end();
        Setting::GetInstance().FlushVolumeToHardware();
        playing_ = false;
    }
}

void Radio::StopPlay() {
    music_audio.set_paused(false);
    music_audio.stop_current(500);   // 等解码任务退出当前流、释放网络与解码器
}

void radio_win_load(SCREEN_LOAD_STAUS status) {
    auto& radio = Radio::GetInstance();
    radio.radio_screen_load(status);
}

/* 供 events_init.c（C 文件）调用的播放页动作 */
void radio_prev(void) {
    const size_t size = sizeof(radio_infos)/sizeof(radio_infos[0]);
    if (size == 0) return;
    auto& radio = Radio::GetInstance();
    radio.PlayStation((radio.GetCurrentIndex() + size - 1) % size);   // 切台即播放
}

void radio_next(void) {
    const size_t size = sizeof(radio_infos)/sizeof(radio_infos[0]);
    if (size == 0) return;
    auto& radio = Radio::GetInstance();
    radio.PlayStation((radio.GetCurrentIndex() + 1) % size);
}

void radio_toggle_play(void) {
    Radio::GetInstance().TogglePlay();
}
