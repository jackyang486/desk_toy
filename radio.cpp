#include "radio.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "ESP32_board.h"
#include "ai_vox_engine.h"

#define I2S_MCK   4
#define I2S_BCK   5
#define I2S_DOUT  8
#define I2S_WS    7
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
      playing_ = true;
      current_index_ = 0;
}


void Radio::FillListUI() {
    lv_obj_t* list = guider_ui.screen_radio_list_radio;
    if (!list) return;
    lv_obj_clean(list);
    int size = sizeof(radio_infos)/sizeof(radio_infos[0]);
    for (size_t i = 0; i < size; i++) {
        lv_obj_t* btn = lv_list_add_btn(list, &_btn_play_RGB565A8_40x40, radio_infos[i].name);
        lv_obj_t* icon = lv_obj_get_child(btn, 0);
        if (icon) lv_image_set_scale(icon, LV_SCALE_NONE / 2);
        lv_obj_t* label = lv_obj_get_child(btn, 1);
        if (label) {
            lv_obj_set_style_text_font(label, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(label, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        lv_obj_set_user_data(btn, (void*)i);
        lv_obj_add_event_cb(btn, ListItemClickHandler, LV_EVENT_CLICKED, (void*)i);
    }
}

void Radio::ListItemClickHandler(lv_event_t* e) {
    int idx = (int)(uintptr_t)lv_event_get_user_data(e);
    if (idx < 0) return;
    Radio::GetInstance().PlayStation((size_t)idx);
}

void Radio::PlayStation(size_t index) {
    int size = sizeof(radio_infos)/sizeof(radio_infos[0]);
    lv_obj_t* list = guider_ui.screen_radio_list_radio;
    if(index == current_index_){
        StopPlay();
        return;
    }
    for (int i = 0; i < size; i++) {
        lv_obj_t *item = lv_obj_get_child(list, i);
        int real_idx = (int)lv_obj_get_user_data(item);
        if (real_idx == (int)current_index_) {
            lv_obj_set_style_bg_color(item, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_t *icon = lv_obj_get_child(item, 0);
            lv_img_set_src(icon, &_btn_play_RGB565A8_40x40);
        }
        if (real_idx == (int)index) {
            lv_obj_set_style_bg_color(item, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_t *icon = lv_obj_get_child(item, 0);
            lv_img_set_src(icon, &_btn_pause_RGB565A8_40x40);
        }
    } 
    StopPlay();
    current_index_ = index;
    lv_label_set_text(guider_ui.screen_radio_label_1, radio_infos[index].name);
    if (audio) audio->connecttohost(radio_infos[index].url);
    
}


 void Radio::radio_screen_load(SCREEN_LOAD_STAUS status){
    if (status == LOADED) {
        playing_ = true; 
        lv_label_set_text(guider_ui.screen_radio_label_1,"list");
        lv_obj_set_style_text_font(guider_ui.screen_radio_label_1, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT);
        FillListUI();
        if (auto dev = board.getAudioDeviceEs8311()) {
                dev->disable_i2s();
        }
        // 每次进入页面都创建全新的 Audio 对象，退出时 delete 销毁
        audio = new Audio();
        audio->setTone(0, 0, 0);                   // 触发计算 0dB 平坦响应系数，避免 IIR coeffs 全 0 导致静音
        audio->settings.IIR_FILTER = false;        // 绕过 biquad IIR（旧 esp-dsp 与新版库系数约定不兼容造成静音）
        audio->settings.SPECTRUM = true;           // 使能频谱分析（evt_spectrum，新版库默认 false）
        audio->setVolume(21);           
        audio->setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_MCK);
            // 创建独立任务持续调用 audio->loop() 处理网络音频流
        xTaskCreatePinnedToCore([](void *arg){
                Radio* self = static_cast<Radio*>(arg);
                while (self->playing_) {
                    audio->loop();
                    vTaskDelay(1);
                }
                vTaskDelete(NULL);
        },"radio_audio_task",1024 * 20,this,2 | portPRIVILEGE_BIT,NULL,1);
    } else {
        StopPlay();
        playing_ = false; 
        // 销毁 Audio 对象：析构函数会 stopSong + disable/del I2S_NUM_0 通道 + stopAudioTask
        delete audio;
        audio = nullptr;
        // 把 codec 的 I2S 引脚让渡回 AI_VOX(I2S_NUM_1)，保证退出后通知音/语音可用
        if (auto dev = board.getAudioDeviceEs8311()) {
            dev->reconfigure_gpio();
        }
        if (guider_ui.screen_radio_list_radio) {
            lv_obj_clean(guider_ui.screen_radio_list_radio);
        }
    }
 }

void Radio::StopPlay() {
    vTaskDelay(pdMS_TO_TICKS(50));  // 等待任务结束
    if (audio) audio->stopSong();
}

void radio_win_load(SCREEN_LOAD_STAUS status) {
    auto& radio = Radio::GetInstance();
    radio.radio_screen_load(status);
}
