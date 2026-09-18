#ifndef __PHOTO_ALBUM_H_
#define __PHOTO_ALBUM_H_
#include "Arduino.h"
#include <vector>
#include "src/custom/custom.h"

#define MAX_IMAGES 50        // 最大支持 50 张图
class Photo_album {
  private:
    std::vector<String> image_files;
    uint8_t current_img;
  public:
    Photo_album(){
      current_img=0;
    }
    void image_loop(void);
    void img_left_btn(void);
    void img_right_btn(void);
    void add_image(const char* path);
    void scan_image(void);
    void show_image(void);
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