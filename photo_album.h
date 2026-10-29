#ifndef __PHOTO_ALBUM_H_
#define __PHOTO_ALBUM_H_
#include "Arduino.h"
#include <vector>
#include "src/custom/custom.h"
#define ONLINE_IMG_W 240
#define ONLINE_IMG_H 320
#define ONLINE_JPG_BUF_SIZE (100 * 1024)

typedef struct {
  uint8_t* jpg_buf;
  size_t jpg_len;
  bool valid;
  lv_img_dsc_t dsc;
} ImgBuf_t;

#define MAX_IMAGES 50        // 最大支持 50 张图
class Photo_album {
  private:
    std::vector<String> image_files;
    uint8_t current_img;
    uint8_t online_active_idx_ = 0;
    bool online_active_ = false;
    volatile int32_t online_seed_ = 1;
    TaskHandle_t online_task_handle_ = nullptr;
    SemaphoreHandle_t online_img_sem_ = nullptr;
    SemaphoreHandle_t online_img_show_sem_ = nullptr;
    SemaphoreHandle_t online_save_mutex_ = nullptr;
    static void online_download_task(void* pvParam);
    static void lv_timer_img_carousel(lv_timer_t *timer);
    bool allocate_online_buf(ImgBuf_t& buf);
    void display_online_image();
    void save_online_image_to_sd();
  public:
    Photo_album();
    ~Photo_album() = default;
    Photo_album(const Photo_album&) = delete;
    Photo_album& operator=(const Photo_album&) = delete;
    bool hasInitial;
    bool playLocal;
    bool nextBtnCheck;
    ImgBuf_t online_buf_a_{};
    ImgBuf_t online_buf_b_{};
    static Photo_album& GetInstance();
    static void image_loop(lv_event_t *e);
    static void img_left_btn(lv_event_t *e);
    static void img_right_btn(lv_event_t *e);
    static void img_play_choose(lv_event_t *e);
    static void img_delete(lv_event_t *e);
    static void del_dialog_callback(bool is_confirm, void *user_data);
    static void online_img_click(lv_event_t* e);
    void request_online_image(int32_t seed_delta);
    void scan_image(void);
    void show_image(void);
    void start_online();
    void stop_online();
    void pushImage(const char* path){
      image_files.push_back(path);
    }
    uint8_t getCurrentImg(){
       return current_img;
    };
    void setCurrentImg(uint8_t currentImg){
       current_img = currentImg;
    };
    int getImageFilesSize(){
       return  image_files.size();
    };
    std::vector<String> getImageFiles(){
      return image_files;
    }
    String getNextImagePath(){
        current_img++;
        if(current_img >= image_files.size())current_img = 0;
       return image_files.at(current_img);
    };
};
void add_image(const char* path);
void photo_win_loaded(SCREEN_LOAD_STAUS status);
#endif