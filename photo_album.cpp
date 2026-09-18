#include "photo_album.h"
#include "src/generated/gui_guider.h"
#include "ESP32_board.h"

static lv_timer_t *carousel_timer = NULL; 
Photo_album photo_album;

//图片显示
void Photo_album::show_image()
{
    if(image_files.size() == 0) return;
    if(current_img < 0) current_img = image_files.size() - 1;
    if(current_img >= image_files.size()) current_img = 0;
    lv_img_set_src(guider_ui.photo_win_view, image_files.at(current_img).c_str());
}
void Photo_album::scan_image(){
     board.scanDir("S:\photo",&image_files,IMG_TYPE);
     show_image();
}
//页面加载
void photo_win_loaded(SCREEN_LOAD_STAUS status){
    if(status == LOADED){
        photo_album.scan_image();  
    }else{
        if(carousel_timer != NULL)
        {
            lv_timer_del(carousel_timer);
            carousel_timer = NULL;
        }
        std::vector<String> imageFiles = photo_album.getImageFiles();
        imageFiles.clear();
        imageFiles.shrink_to_fit(); 
    }
}
//上一张
void Photo_album::img_left_btn(){
    if(carousel_timer != NULL)
    {
        lv_timer_del(carousel_timer);
        carousel_timer = NULL;
    }
    current_img--;
    show_image();
}
//下一张
void Photo_album::img_right_btn(){
    if(carousel_timer != NULL)
    {
        lv_timer_del(carousel_timer);
        carousel_timer = NULL;
    }
    current_img++;
    show_image();
}
void lv_timer_img_carousel(lv_timer_t *timer){
    lv_img_set_src(guider_ui.photo_win_view, photo_album.getNextImagePath().c_str());
}
//轮播
void Photo_album::image_loop(){
    if(carousel_timer == NULL){
        carousel_timer = lv_timer_create(lv_timer_img_carousel, 2000, NULL);
    }else{
        lv_timer_del(carousel_timer);
        carousel_timer = NULL;
    }
}
//图片轮播任务
void image_carousel(){
    photo_album.image_loop();
}
void click_right_btn(){
    photo_album.img_left_btn();
}
void click_left_btn(){
    photo_album.img_right_btn();
}
