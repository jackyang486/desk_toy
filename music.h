#ifndef __MUSIC_H_
#define __MUSIC_H_
#include "Arduino.h"
#include "Audio.h"
#include <vector>
#include "ESP32_board.h"
#include "src/custom/custom.h"



typedef enum {
  PAUSED,
  PLAY
} PLAY_STATUS;

// MusicPathList（PSRAM 路径池）已上移到 ESP32_board.h，photo_album/lottie/game 页共用。

class Music_player {
  private:
    MusicPathList music_files;
    uint16_t current_music;
    int  list_start_index;
    int  touch_start_Y;
    bool playEnable;
    bool playSequence;
    PLAY_STATUS play_status;
   
  public:
    Music_player();
    void playPauseResume();
    void music_loop(SCREEN_LOAD_STAUS playerLoadStatus);
    void music_left_btn(void);
    void music_right_btn(void);
    void play_music(void);
    void  fill_music_list_panel(void);
    // MCP 播歌：后台扫描曲库（不进音乐页、不碰任何 screen_music 对象），供匹配歌名用
    void scan_songs(void);
    // MCP 播歌：后台播放第 idx 首（0-based），成功返回 true。不操作任何 screen_music 的
    // LVGL 对象（screen_music 可能尚未创建）。调用前须先 music_audio.begin()。
    bool play_song_bg(uint16_t idx);
    
    uint16_t getCurrentMusic(){
       return current_music;
    };
    void setCurrentMusic(uint16_t currentMusic){
       current_music = currentMusic;
    };
    bool getPlayEnable(){
       return playEnable;
    };
    bool getPlaySequence(){
       return playSequence;
    };
    void setPlaySequence(bool playFlag){
       playSequence = playFlag;
    };
    int getMusicFilesSize(){
       return  music_files.size();
    };
    const MusicPathList &getMusicPaths() const {
       return  music_files;
    };
    void setListStartIndex(int idx){
         list_start_index = idx;
    }
    int getListStartIndex(){
        return list_start_index;
    }
     void setTouchStartY(int startY){
         touch_start_Y = startY;
    }
    int getTouchStartY(){
        return touch_start_Y;
    }
};
void add_music(const char* path);
void music_player_load(SCREEN_LOAD_STAUS playerLoadStatus);
extern Music_player music_player;   // 全局唯一实例（music.cpp 定义），MCP 播歌跨文件使用
#endif