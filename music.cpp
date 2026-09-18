#include "music.h"
#include "SPI.h"
#include "FS.h"
#include "esp_err.h"
#include "esp_check.h"
#include "src/generated/gui_guider.h"
#include "src/generated/events_init.h"
#include "SD_MMC.h"
#include "ESP32_board.h"
#include "ai_vox_engine.h"
#include <random>
#include <chrono>
#include "Ticker.h"
#define I2S_MCK   4
#define I2S_BCK   5
#define I2S_DOUT  8
#define I2S_WS    7
#define MAX_COVER_SIZE 1024*5
uint8_t coverBuffer[MAX_COVER_SIZE];

#define SPECTRUM_BARS 25   // 显示柱数量
#define SPECTRUM_SRC_BANDS 15 // evt_spectrum 提供的源频段数（库内 NUM_BANDS）
#define SPECTRUM_MAX_HEIGHT 60
#define SPECTRUM_BAR_WIDTH 7
#define SPECTRUM_BAR_GAP 1
#define SPECTRUM_PADDING 20
#define SPECTRUM_RIGHT_PADDING 30
#define SPECTRUM_START_Y 230
static lv_obj_t *spectrum_bars[SPECTRUM_BARS];
static lv_obj_t *spectrum_peaks[SPECTRUM_BARS];
static uint8_t spectrum_heights[SPECTRUM_BARS];
static uint8_t spectrum_target_heights[SPECTRUM_BARS];
static uint8_t spectrum_peak_heights[SPECTRUM_BARS];
static bool spectrum_init = false;
void check_list_item(uint16_t idx, bool check);
#define SWIPE_THRESHOLD 20
#define LIST_VISIBLE_ITEMS 20 
static bool list_is_touching = false;
static bool list_swipe_registered = false;
// [v4.0.0 适配] 新版 ESP32-audioI2S 移除了 getVUlevel()/audio_eof_mp3()/audio_id3image()/audio_id3lyrics()，
// 统一改由 audio_info_callback() 上抛事件。回调在音频任务（Core 1）里执行，这里用 volatile 全局变量
// 暂存数据/标志，再交给 LVGL 定时器（主线程）统一处理，保证线程安全。
static volatile bool     g_eof_pending = false;   // 播放结束，待切下一曲
static volatile bool     g_cover_pending = false; // 发现封面，待读取+显示
static volatile uint32_t g_cover_pos = 0;
static volatile uint32_t g_cover_len = 0;
static char              g_cur_file_path[128] = {0}; // 当前播放文件路径（相对 SD_MMC 挂载点）

void on_audio_info(Audio::msg_t msg) {
    switch (msg.e) {
        case Audio::evt_spectrum:
            // vec1 = display_vec（15 个对数分贝频段，已内置 attack/release/hold 平滑），各 0~255
            // 线性插值映射到 25 根柱
            if (msg.vec1.size() >= SPECTRUM_SRC_BANDS) {
                for (int i = 0; i < SPECTRUM_BARS; i++) {
                    float pos  = (float)i * (SPECTRUM_SRC_BANDS - 1) / (SPECTRUM_BARS - 1);
                    int   idx0 = (int)pos;
                    int   idx1 = (idx0 + 1 < SPECTRUM_SRC_BANDS) ? idx0 + 1 : idx0;
                    float frac = pos - idx0;
                    float val  = msg.vec1[idx0] * (1.0f - frac) + msg.vec1[idx1] * frac;
                    spectrum_target_heights[i] = (uint8_t)(val * SPECTRUM_MAX_HEIGHT / 255)+1;
                }
            }
            break;
        case Audio::evt_eof:
            g_eof_pending = true;   // 交给主线程切下一曲
            break;
        case Audio::evt_image:
            // vec1 = [pos, len, pos, len, ...]，指向当前音频文件内的 APIC 数据
            if (msg.vec1.size() >= 2) {
                g_cover_pos = msg.vec1[0];
                g_cover_len = msg.vec1[1];
                g_cover_pending = true;   // 交给主线程读文件+显示
            }
            break;
        case Audio::evt_lyrics:
            // 同步歌词 SYLT/SLT：库按时间戳逐行上抛，msg.msg 是当前行文本
            if (msg.msg) printf("lyrics: %s\n", msg.msg);
            break;
        case Audio::evt_id3data:
            // 非同步歌词 USLT 也通过 ID3 数据事件上抛，避免依赖固定的 "Lyrics:" 前缀。
            if (msg.msg) {
                printf("id3: %s\n", msg.msg);
            }
            break;
        default:
            break;
    }
}

void init_spectrum(void) {
    if (spectrum_init) return;
    int total_width = SPECTRUM_BARS * (SPECTRUM_BAR_WIDTH + SPECTRUM_BAR_GAP) - SPECTRUM_BAR_GAP;
    int start_x = (240 - total_width - SPECTRUM_RIGHT_PADDING);
    int start_y = SPECTRUM_START_Y;
    for (int i = 0; i < SPECTRUM_BARS; i++) {
        spectrum_bars[i] = lv_obj_create(guider_ui.music_player);
        lv_obj_set_size(spectrum_bars[i], SPECTRUM_BAR_WIDTH, 0);
        lv_obj_set_pos(spectrum_bars[i], start_x + i * (SPECTRUM_BAR_WIDTH + SPECTRUM_BAR_GAP), start_y);
        spectrum_peaks[i] = lv_obj_create(guider_ui.music_player);
        lv_obj_set_size(spectrum_peaks[i], SPECTRUM_BAR_WIDTH, 2);
        lv_obj_set_pos(spectrum_peaks[i], start_x + i * (SPECTRUM_BAR_WIDTH + SPECTRUM_BAR_GAP), start_y - 2);
        lv_obj_set_style_bg_color(spectrum_peaks[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(spectrum_peaks[i], 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(spectrum_peaks[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(spectrum_bars[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(spectrum_bars[i], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        spectrum_heights[i] = 0;
        spectrum_target_heights[i] = 0;
        spectrum_peak_heights[i] = 0;
    }
    spectrum_init = true;
}

void deinit_spectrum(void) {
    if (!spectrum_init) return;
    for (int i = 0; i < SPECTRUM_BARS; i++) {
        if (spectrum_bars[i]) {
            lv_obj_del(spectrum_bars[i]);
            spectrum_bars[i] = NULL;
        }
        if (spectrum_peaks[i]) {
            lv_obj_del(spectrum_peaks[i]);
            spectrum_peaks[i] = NULL;
        }
    }
    spectrum_init = false;
}




int getRandomInt(int min, int max);
void update_spectrum(void) {
    if (!spectrum_init || !audio || !audio->isRunning()) {
        for (int i = 0; i < SPECTRUM_BARS; i++) {
            if (spectrum_heights[i] > 0) {
                spectrum_heights[i] -= (spectrum_heights[i] > 4) ? 4 : 1;
                lv_obj_set_height(spectrum_bars[i], spectrum_heights[i]);
                lv_obj_set_y(spectrum_bars[i], SPECTRUM_START_Y - spectrum_heights[i]);
            }
            if (spectrum_peak_heights[i] > spectrum_heights[i]) spectrum_peak_heights[i]--;
            if (spectrum_peak_heights[i] < spectrum_heights[i]) spectrum_peak_heights[i] = spectrum_heights[i];
            if (spectrum_peaks[i]) {
                lv_obj_set_y(spectrum_peaks[i], SPECTRUM_START_Y - spectrum_peak_heights[i] - 2);
                lv_obj_set_style_bg_opa(spectrum_peaks[i], spectrum_peak_heights[i] > 0 ? 255 : 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
        return;
    }
    // spectrum_target_heights[] 已由 evt_spectrum 回调填充（真实 FFT 频段），此处只做爬升/回落平滑
    for (int i = 0; i < SPECTRUM_BARS; i++) {
        int diff = (int)spectrum_target_heights[i] - (int)spectrum_heights[i];
        if (diff > 0) {
            spectrum_heights[i] += (diff > 6) ? 6 : diff;
        } else if (diff < 0) {
            int drop_amount = (-diff > 5) ? 5 : -diff;
            spectrum_heights[i] -= drop_amount;
            if (spectrum_heights[i] > SPECTRUM_MAX_HEIGHT) {
                spectrum_heights[i] = 0;
            }
        }
        if (spectrum_bars[i]) {
            if (spectrum_heights[i] > spectrum_peak_heights[i]) {
                spectrum_peak_heights[i] = spectrum_heights[i];
            } else if (spectrum_peak_heights[i] > spectrum_heights[i]) {
                spectrum_peak_heights[i]--;
            }
            lv_obj_set_height(spectrum_bars[i], spectrum_heights[i]);
            lv_obj_set_y(spectrum_bars[i], SPECTRUM_START_Y - spectrum_heights[i]);
            uint8_t phase = (uint8_t)((uint16_t)i * 255 / (SPECTRUM_BARS - 1));
            static const uint8_t spectrum_palette[][3] = {
                {255, 82, 48},   // coral
                {255, 137, 35},  // orange
                {255, 190, 45},  // amber
                {245, 78, 77},   // rose
                {214, 63, 126}   // warm pink
            };
            uint8_t palette_pos = phase == 255 ? 3 : (uint8_t)((uint16_t)phase * 4 / 255);
            uint8_t palette_frac = phase == 255 ? 255 : (uint8_t)((uint16_t)(phase % 64) * 255 / 64);
            uint8_t red = spectrum_palette[palette_pos][0]
                + (int16_t)(spectrum_palette[palette_pos + 1][0] - spectrum_palette[palette_pos][0]) * palette_frac / 255;
            uint8_t green = spectrum_palette[palette_pos][1]
                + (int16_t)(spectrum_palette[palette_pos + 1][1] - spectrum_palette[palette_pos][1]) * palette_frac / 255;
            uint8_t blue = spectrum_palette[palette_pos][2]
                + (int16_t)(spectrum_palette[palette_pos + 1][2] - spectrum_palette[palette_pos][2]) * palette_frac / 255;
            uint8_t boost = spectrum_heights[i] * 25 / SPECTRUM_MAX_HEIGHT;
            uint8_t bright_red = red > 255 - boost ? 255 : red + boost;
            uint8_t bright_green = green > 255 - boost ? 255 : green + boost;
            uint8_t bright_blue = blue > 255 - boost ? 255 : blue + boost;
            lv_obj_set_style_bg_color(spectrum_bars[i], lv_color_make(bright_red, bright_green, bright_blue), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(spectrum_bars[i], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            if (spectrum_peaks[i]) {
                lv_obj_set_y(spectrum_peaks[i], SPECTRUM_START_Y - spectrum_peak_heights[i] - 2);
                lv_obj_set_style_bg_color(spectrum_peaks[i], lv_color_make(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(spectrum_peaks[i], spectrum_peak_heights[i] > 0 ? 255 : 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    }
}

Music_player music_player;
static lv_anim_t album_anim;
uint32_t total_sec = 1;
static bool music_screen_load = false;
static lv_timer_t *music_timer = NULL;  // 静态，只在本文件可见

//音乐播放上一曲
void music_prev(){
    music_player.music_left_btn();
}
//音乐播放下一曲
void music_next(){
    music_player.music_right_btn();
}
//播放/暂停
void music_play(){
    music_player.playPauseResume();
}
void music_player_load(SCREEN_LOAD_STAUS playerLoadStatus){
    music_player.music_loop(playerLoadStatus);     
}
//播放音乐
void Music_player::play_music()
{
    if(music_files.size() == 0) return;
    total_sec = 1;
    if(current_music < 0) current_music = music_files.size() - 1;
    if(current_music >= music_files.size()) current_music = 0;
    std::pair<String, String> musicFile = music_files.at(current_music);
    lv_obj_clear_flag(guider_ui.music_pause_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(guider_ui.music_playing_img, LV_OBJ_FLAG_HIDDEN);
    //设置歌手和歌曲名称
    String filePath = musicFile.first;
    // 记录当前文件路径，供 evt_image 封面事件在主线程重开文件读取 APIC 数据
    strncpy(g_cur_file_path, filePath.c_str(), sizeof(g_cur_file_path) - 1);
    g_cur_file_path[sizeof(g_cur_file_path) - 1] = '\0';
    String songName = musicFile.second;
    lv_label_set_text(guider_ui.music_label_music_name,songName.c_str()); 
    lv_slider_set_value(guider_ui.music_slider, 0, LV_ANIM_OFF); //切歌时，滑块归0
    check_list_item(current_music,true);
    audio->stopSong();
    audio->connecttoFS(SD_MMC, filePath.c_str());
}
// 生成 [min, max] 范围内的随机整数（闭区间）
int getRandomInt(int min, int max) {
    // 使用静态变量确保引擎和分布只初始化一次，提高效率
    static std::mt19937 engine(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

// 播放结束后切歌逻辑（在 LVGL 主线程调用，会操作 UI）
static void handle_eof_next() {
    if (music_player.getPlaySequence()) {
        music_player.music_right_btn();
    } else {
        int rndNum = getRandomInt(0, music_player.getMusicFilesSize() - 1);
        music_player.setCurrentMusic(rndNum);
        music_player.music_right_btn();
    }
}

// 读取并显示专辑封面（在 LVGL 主线程调用）
static void process_cover_pending() {
    g_cover_pending = false;
    if (g_cover_len == 0 || g_cover_len > MAX_COVER_SIZE) return;

    File f = SD_MMC.open(g_cur_file_path);
    if (!f) return;
    f.seek(g_cover_pos);
    size_t n = f.read(coverBuffer, g_cover_len);
    f.close();
    if (n != g_cover_len) return;

    // 跳过 APIC 头，定位 JPEG SOI (0xFF 0xD8)
    size_t jpegStart = 0;
    for (size_t i = 11; i + 1 < n; i++) {
        if (coverBuffer[i] == 0xFF && coverBuffer[i + 1] == 0xD8) {
            jpegStart = i;
            break;
        }
    }
    if (jpegStart == 0) return; // 没找到 JPEG 头

    lv_img_set_src(guider_ui.music_album_img, &coverBuffer[jpegStart]);
    lv_obj_invalidate(guider_ui.music_album_img);
}


//音乐随机播放
void music_random(){
    music_player.setPlaySequence(false);
    int rndNum = getRandomInt(0,music_player.getMusicFilesSize()-1);
    music_player.setCurrentMusic(rndNum);
    music_player.music_right_btn(); 
}
//音乐顺序播放
void music_sequence(){
    music_player.setPlaySequence(true);
}
//暂停/播放
void Music_player::playPauseResume(){
    audio->pauseResume();
}

//上一曲
void Music_player::music_left_btn(){
    current_music--;
    play_music();
}
//下一曲
void Music_player::music_right_btn(){
    current_music++;
    play_music();
}
void music_back_task(void *pvParam){
  Music_player *p = (Music_player*)pvParam;
  while (p->getPlayEnable())
  {
    audio->loop();
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}
// LVGL 定时器：刷新播放时间（安全，不会崩溃）
static void lv_timer_refresh_time(lv_timer_t *timer)
{
    // 只有播放中才刷新
    if (audio && audio->isRunning())
    {
       if(total_sec == 1){
         total_sec = audio->getAudioFileDuration();
         if(total_sec == 0)
         total_sec = 1;
         lv_slider_set_range(guider_ui.music_slider, 0, total_sec);
       }
        uint32_t curr = audio->getAudioCurrentTime();
        char buf[32];
        sprintf(buf, "%02d:%02d / %02d:%02d", 
            curr / 60, curr % 60, 
            total_sec / 60, total_sec % 60
        );
        lv_label_set_text(guider_ui.music_label_slider_time, buf);
        lv_slider_set_value(guider_ui.music_slider, curr, LV_ANIM_OFF);
    }
    update_spectrum();

    // 处理音频任务上抛的跨线程事件（EOF 切歌 / 封面显示）
    if (g_eof_pending) {
        g_eof_pending = false;
        handle_eof_next();
    }
    if (g_cover_pending) {
        process_cover_pending();
    }
}

void change_list_other_item_style(uint16_t idx) {
    lv_obj_t *list = guider_ui.music_list_music;
    if (!list) return;
    for (int i = 0; i < LIST_VISIBLE_ITEMS; i++) {
        lv_obj_t *otherItem = lv_obj_get_child(list, i);
        lv_obj_t *icon = lv_obj_get_child(otherItem, 0);
        int real_idx = (int)(uintptr_t)lv_obj_get_user_data(otherItem);
        if (real_idx == (int)idx) continue;
        lv_obj_set_style_bg_color(otherItem, lv_color_hex(0x343247), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_img_set_src(icon, &_btn_list_play_alpha_20x20);
    }
}

void music_list_item_click(lv_event_t *e) {
    int idx = (int)(uintptr_t)lv_event_get_user_data(e);
    music_player.setCurrentMusic(idx);
    music_player.play_music();
}

void check_list_item(uint16_t idx,bool check) {
    lv_obj_t *list = guider_ui.music_list_music;
    if (!list) return;
    int listStartIndex = music_player.getListStartIndex();
    if(idx < listStartIndex || idx >= (listStartIndex+LIST_VISIBLE_ITEMS)){  //如果当前歌曲索引大于list的开始和结束索引，则重新刷新播放列表
        return;
    }
    lv_obj_t *selectItem = NULL;
    for (int i = 0; i < LIST_VISIBLE_ITEMS; i++) {
        lv_obj_t *item = lv_obj_get_child(list, i);
        int real_idx = (int)(uintptr_t)lv_obj_get_user_data(item);
        if (real_idx == (int)idx) {
            selectItem = item;
            break;
        }
    } 
    if (selectItem) {
        lv_obj_set_style_bg_color(selectItem, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_t *icon = lv_obj_get_child(selectItem, 0);
        lv_img_set_src(icon, &_btn_list_pause_alpha_20x20);
    }
    if(check){
        change_list_other_item_style(idx);
    }
}



void music_list_swipe_event_handler(lv_event_t *e);


//加载音乐列表
void  Music_player::fill_music_list_panel(void) {
    lv_obj_t *list = guider_ui.music_list_music;
    if (!list) return;
    if(!list_swipe_registered){
        lv_obj_clean(list);
    }
    int total = music_files.size();
    for (int i = list_start_index; i < (list_start_index+LIST_VISIBLE_ITEMS); i++) {
            const char* songName = music_files.at(i).second.c_str();
            lv_obj_t *btn = NULL;
            if(list_swipe_registered){  //已加载过了
                btn = lv_obj_get_child(list, (i-list_start_index));
                lv_obj_t *icon = lv_obj_get_child(btn,0);
                lv_obj_t *label = lv_obj_get_child(btn,1);
                lv_label_set_text(label,songName);
                if(music_player.getCurrentMusic() == i){   //如果是播放中的歌曲，则改变背景色和按钮状态
                    lv_obj_set_style_bg_color(btn, lv_color_hex(0xc6a2a2), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(icon, &_btn_list_pause_alpha_20x20);
                }else{
                    lv_obj_set_style_bg_color(btn, lv_color_hex(0x343247), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_img_set_src(icon, &_btn_list_play_alpha_20x20);
                }
            }else{
                btn = lv_list_add_btn(list, &_btn_list_play_alpha_20x20, songName);
                lv_obj_set_style_bg_color(btn, lv_color_hex(0x343247), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_t *label = lv_obj_get_child(btn, 1);
                if (label) {
                        lv_obj_set_style_text_font(label, &font_puhui_16_4, LV_PART_MAIN | LV_STATE_DEFAULT);
                        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
            lv_obj_set_user_data(btn, (void*)(uintptr_t)i);
            lv_obj_add_event_cb(btn, music_list_item_click, LV_EVENT_LONG_PRESSED, (void*)(uintptr_t)i);
    }
}

void music_list_swipe_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SCROLL_BEGIN) {
        lv_indev_t *indev = lv_indev_get_act();
        if (indev && lv_event_get_param(e) == NULL) {
            lv_point_t point;
            lv_indev_get_point(indev, &point);
            if(!list_is_touching){
                music_player.setTouchStartY(point.y);
                list_is_touching = true;
            }
        }
    } else if (code == LV_EVENT_SCROLL_END) {
        if (!list_is_touching) return;
        list_is_touching = false;
        lv_indev_t *indev = lv_indev_get_act();
        if (indev && lv_event_get_param(e) == NULL) {
            lv_point_t point;
            lv_indev_get_point(indev, &point);
            int16_t delta_y = music_player.getTouchStartY() - point.y;
            if (abs(delta_y) > 20) {          //移动Y轴距离大于20个像素
                int steps = delta_y / SWIPE_THRESHOLD;
                if (steps != 0) {
                    int list_start_index = music_player.getListStartIndex();
                    int total = music_player.getMusicFilesSize();
                    list_start_index += steps;
                    if(list_start_index<0){
                        list_start_index = 0;
                    }
                    if ((list_start_index + LIST_VISIBLE_ITEMS) > total) {
                        list_start_index = total - LIST_VISIBLE_ITEMS;
                        if (list_start_index < 0) list_start_index = 0;
                    }
                    music_player.setListStartIndex(list_start_index);
                    music_player.fill_music_list_panel();
                }
            }
        }
    }
}

//循环播放
void Music_player::music_loop(SCREEN_LOAD_STAUS playerLoadStatus){
    if(playerLoadStatus == LOADED){
        std::vector<String> musicFileList; 
        board.scanDir("S:\music",&musicFileList,MP3_TYPE);
        for(int i=0; i<musicFileList.size(); i++){
            String filePath = musicFileList.at(i);
            filePath = filePath.substring(2);
            size_t pos = filePath.lastIndexOf('/');
            String songName = filePath.substring(pos+1,filePath.length());
            music_files.push_back({filePath,songName});
        }
        char music_title[20];
        sprintf(music_title,"歌曲列表(%d)",music_player.getMusicFilesSize());
        lv_label_set_text(guider_ui.music_label_list_name,music_title);
        lv_obj_set_style_text_font(guider_ui.music_label_list_name, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT); 
        fill_music_list_panel(); 
        lv_obj_add_event_cb(guider_ui.music_list_music, music_list_swipe_event_handler, LV_EVENT_ALL, NULL);
        list_swipe_registered = true;
        lv_obj_set_style_text_font(guider_ui.music_label_music_name, &font_puhui_16_4, LV_PART_MAIN|LV_STATE_DEFAULT); 
        init_spectrum();
        if (music_timer == NULL) {
            music_timer = lv_timer_create(lv_timer_refresh_time, 100, NULL);
        }
            playEnable = true;
            // 让渡前先停掉 AI_VOX(I2S_NUM_1)，避免两套 I2S 同时使能驱动同一组 codec 引脚
            if (auto dev = board.getAudioDeviceEs8311()) {
                dev->disable_i2s();
            }
            // 每次进入页面都创建全新的 Audio 对象，退出时 delete 销毁
            // 析构函数会 stopSong + disable/del I2S_NUM_0 通道 + stopAudioTask，确保状态不累积
            audio = new Audio();
            audio->setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_MCK);
            audio->setTone(0, 0, 0);                   // 触发计算 0dB 平坦响应系数，避免 IIR coeffs 全 0 导致静音
            audio->settings.IIR_FILTER = false;        // 绕过 biquad IIR（旧 esp-dsp 与新版库系数约定不兼容造成静音）
            audio->settings.SPECTRUM = true;           // 使能频谱分析（evt_spectrum，新版库默认 false）
            audio->setVolume(21);                      // v4.0.0 软件音量默认 0（静音），设为满量程 0dB 直通
            audio->audio_info_callback = on_audio_info; // 注册事件回调，接收频谱/EOF/封面/歌词数据
            const BaseType_t xCoreID = 1;
            xTaskCreatePinnedToCore(
                        music_back_task,
                        "music_back_task",
                        1024*20,
                        this,
                        2 | portPRIVILEGE_BIT,
                        NULL,
                        xCoreID
            );
            vTaskDelay(pdMS_TO_TICKS(200));
            play_music();
    }else{
        playEnable = false;
        audio->stopSong();
        // 先删除 LVGL 定时器（内部访问 audio->），再销毁 Audio 对象
        if (music_timer != NULL) {
            lv_timer_del(music_timer);
            music_timer = NULL;
        }
        deinit_spectrum();
        // 销毁 Audio 对象：析构函数 stopSong + disable/del I2S_NUM_0 通道 + stopAudioTask + 释放互斥锁
        delete audio;
        audio = nullptr;
        // 把 codec 的 I2S 引脚让渡回 AI_VOX(I2S_NUM_1)，保证退出后通知音/语音可用
        if (auto dev = board.getAudioDeviceEs8311()) {
            dev->reconfigure_gpio();
        }
        music_files.clear();
        music_files.shrink_to_fit();
        lv_obj_clean(guider_ui.music_list_music);
        list_swipe_registered = false;
    }
}
