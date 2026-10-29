#include "photo_album.h"
#include "src/generated/gui_guider.h"
#include "src/custom/confirm_dialog.h"
#include "ESP32_board.h"
#include "SD_MMC.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "esp_heap_caps.h"

static const char *basePath = "S:/photo";
static const char *ONLINE_IMG_URL_BASE = "http://192.168.66.68:5000/proxy?w=240&h=320"; //"https://onlineimage-production.up.railway.app/proxy?w=240&h=320";
static lv_timer_t *carousel_timer = NULL;

Photo_album& Photo_album::GetInstance() {
    static Photo_album instance;
    return instance;
}

Photo_album::Photo_album():playLocal(true),nextBtnCheck(false),current_img(0),hasInitial(false){
}

//图片显示
void Photo_album::show_image()
{
    if (!playLocal) {
        display_online_image();
        return;
    }
    if(image_files.empty()) {
        lv_img_set_src(guider_ui.screen_photo_view, NULL);
        return;
    }
    if(current_img >= image_files.size()) current_img = 0;
     // 构建完整路径
    char fullPath[256];
    lv_snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, image_files.at(current_img).c_str());
    lv_img_set_src(guider_ui.screen_photo_view, fullPath);
}
void Photo_album::scan_image(){
     board.scanDir(basePath,&image_files,IMG_TYPE);
     show_image();
}
//页面加载
void photo_win_loaded(SCREEN_LOAD_STAUS status){ 
     auto& photo_album = Photo_album::GetInstance();
    if(status == LOADED){
        if(!photo_album.hasInitial){
            photo_album.hasInitial = true;
            photo_album.scan_image();
            lv_obj_add_event_cb(guider_ui.screen_photo_left_arrow, Photo_album::img_left_btn, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_right_arrow, Photo_album::img_right_btn, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_btn_web, Photo_album::img_play_choose, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_btn_loop, Photo_album::image_loop, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_btn_delete, Photo_album::img_delete, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_view, Photo_album::online_img_click, LV_EVENT_LONG_PRESSED,&photo_album);
            lv_obj_set_style_text_color(lv_obj_get_child(guider_ui.screen_photo_btn_web, 0),
                                        photo_album.playLocal ? lv_color_white() : lv_color_make(0,0,255), 0);
            lv_obj_set_style_text_color(lv_obj_get_child(guider_ui.screen_photo_btn_loop, 0),
                                         carousel_timer == NULL ? lv_color_white() : lv_color_make(0,0,255), 0);
      
        }
    }else{
        photo_album.playLocal = true;
        if(carousel_timer != NULL)
        { 
            lv_timer_del(carousel_timer);
            carousel_timer = NULL;
        }
        photo_album.stop_online();
        if(photo_album.online_buf_a_.jpg_buf != nullptr){
            heap_caps_free(photo_album.online_buf_a_.jpg_buf);
            photo_album.online_buf_a_.jpg_buf = nullptr;
        }
        if(photo_album.online_buf_b_.jpg_buf != nullptr){ 
            heap_caps_free(photo_album.online_buf_b_.jpg_buf);
            photo_album.online_buf_b_.jpg_buf = nullptr;
        }
    }
}
//上一张
void Photo_album::img_left_btn(lv_event_t *e){
     auto& photo_album = Photo_album::GetInstance();
    lv_obj_set_style_text_color(guider_ui.screen_photo_btn_loop_label, lv_color_white(),0);
    photo_album.nextBtnCheck = true;
    if(carousel_timer != NULL){
        lv_timer_del(carousel_timer);
        carousel_timer = NULL;
    }
    if (photo_album.playLocal) {
        if (!photo_album.image_files.empty()) {
            photo_album.current_img = photo_album.current_img == 0
                ? photo_album.image_files.size() - 1
                : photo_album.current_img - 1;
        }
        photo_album.show_image();
    } else {
        photo_album.request_online_image(-1);
    }
}
//下一张
void Photo_album::img_right_btn(lv_event_t *e){
    
    lv_obj_set_style_text_color(guider_ui.screen_photo_btn_loop_label, lv_color_white(),0);
    auto& photo_album = Photo_album::GetInstance();
    if(carousel_timer != NULL){
        lv_timer_del(carousel_timer);
        carousel_timer = NULL;
    }
    photo_album.nextBtnCheck = true;
    if (photo_album.playLocal) {    
        photo_album.current_img++;
        photo_album.show_image();
    } else {
        photo_album.request_online_image(1);
    }
}
void Photo_album::lv_timer_img_carousel(lv_timer_t *timer){
    Photo_album* self = static_cast<Photo_album*>(lv_timer_get_user_data(timer));  //网络图片轮播
    if (self->playLocal) {        //本地图片轮播
      // 构建完整路径
        char fullPath[256];
        lv_snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, self->getNextImagePath().c_str());
        lv_img_set_src(guider_ui.screen_photo_view, fullPath);
    } else {
        if (!self->online_active_ || xSemaphoreTake(self->online_img_sem_, 0) != pdTRUE) return;
        self->display_online_image();
        xSemaphoreGive(self->online_img_show_sem_);
    }
}
//轮播
void Photo_album::image_loop(lv_event_t *e){
     auto& photo_album = Photo_album::GetInstance();
    if(carousel_timer == NULL){
        lv_obj_set_style_text_color(guider_ui.screen_photo_btn_loop_label, lv_color_make(0,0,255),0);
        carousel_timer = lv_timer_create(lv_timer_img_carousel, 5000,&photo_album);
    }else{
        lv_obj_set_style_text_color(guider_ui.screen_photo_btn_loop_label, lv_color_white(),0);
        if(carousel_timer != NULL){  //播放本地图片
            lv_timer_del(carousel_timer);
            carousel_timer = NULL;
        }
    }
}
//播放类型切换
void Photo_album::img_play_choose(lv_event_t *e){
     auto& photo_album = Photo_album::GetInstance();
     if(photo_album.playLocal){     //播放网络图片
        photo_album.playLocal = false;
        photo_album.nextBtnCheck = true;
        photo_album.start_online();
        lv_obj_set_style_text_color(guider_ui.screen_photo_btn_web_label, lv_color_make(0,0,255),0);
     }else{                         //播放本地文件
        photo_album.playLocal = true;
        photo_album.stop_online();
        photo_album.show_image();
        lv_obj_set_style_text_color(guider_ui.screen_photo_btn_web_label, lv_color_white(),0);
     }
}

bool Photo_album::allocate_online_buf(ImgBuf_t& buf) {
    if (buf.jpg_buf != nullptr) return true;
    buf.jpg_buf = static_cast<uint8_t*>(heap_caps_malloc(ONLINE_JPG_BUF_SIZE, MALLOC_CAP_SPIRAM));
    if (buf.jpg_buf == nullptr) buf.jpg_buf = static_cast<uint8_t*>(malloc(ONLINE_JPG_BUF_SIZE));
    if (buf.jpg_buf == nullptr) return false;
    buf.jpg_len = 0;
    buf.valid = false;
    memset(&buf.dsc, 0, sizeof(buf.dsc));
    buf.dsc.header.cf = LV_COLOR_FORMAT_RAW;
    buf.dsc.header.w = ONLINE_IMG_W;
    buf.dsc.header.h = ONLINE_IMG_H;
    return true;
}

void Photo_album::start_online() {
    if (!allocate_online_buf(online_buf_a_) || !allocate_online_buf(online_buf_b_)) {
        show_toast(lv_scr_act(), "内存不足");
        return;
    }
    if (online_img_sem_ == nullptr) online_img_sem_ = xSemaphoreCreateBinary();
    if (online_img_show_sem_ == nullptr) online_img_show_sem_ = xSemaphoreCreateBinary();
    if (online_save_mutex_ == nullptr) online_save_mutex_ = xSemaphoreCreateMutex();
    if (online_img_sem_ == nullptr || online_save_mutex_ == nullptr || online_img_show_sem_  == nullptr) {
        show_toast(lv_scr_act(), "网络图片初始化失败");
        return;
    }
    online_active_ = true;
    xTaskCreatePinnedToCore(
        online_download_task,
        "online_img_task",
        1024 * 8,
        this,
        2 | portPRIVILEGE_BIT,
        &online_task_handle_,
        1
    );
}

void Photo_album::stop_online() {
    online_active_ = false;
    if(online_img_sem_ != nullptr){
        online_img_sem_ = nullptr;
    }
    if(online_img_show_sem_ != nullptr){
        online_img_show_sem_ = nullptr;
    }
    if(online_save_mutex_ != nullptr){
        online_save_mutex_ = nullptr;
    }
}

void Photo_album::request_online_image(int32_t seed_delta) {
    if (!online_active_) start_online();
    online_seed_ += seed_delta;
}

void Photo_album::display_online_image() {
    if (!online_active_) return;
    online_active_idx_ = !online_active_idx_;
    ImgBuf_t& image = (online_active_idx_ == 0) ? online_buf_a_ : online_buf_b_;
    if (!image.valid || image.dsc.data == nullptr) return;
    lv_img_set_src(guider_ui.screen_photo_view, &image.dsc);
    lv_obj_invalidate(guider_ui.screen_photo_view);
}

void Photo_album::online_download_task(void* pvParam) {
    Photo_album* self = static_cast<Photo_album*>(pvParam);
    //WiFiClientSecure wifi_client;
    //wifi_client.setInsecure();
    WiFiClient wifi_client;
    
    while (self->online_active_) {
        ImgBuf_t& work = (self->online_active_idx_ == 0)
            ? self->online_buf_b_ : self->online_buf_a_;
        if (work.jpg_buf == nullptr) continue;
        if(carousel_timer == NULL && !self->nextBtnCheck){
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }
        HTTPClient http;
        String url = String(ONLINE_IMG_URL_BASE) + "&seed=" + String(self->online_seed_);
        if (http.begin(wifi_client, url)) {
            http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
            http.setTimeout(10000);
            http.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
            http.addHeader("Accept-Encoding", "identity");
            const int response = http.GET();
            const int64_t content_length = http.getSize();
            if (response == HTTP_CODE_OK && content_length > 0 &&
                content_length <= ONLINE_JPG_BUF_SIZE) {
                WiFiClient* stream = http.getStreamPtr();
                size_t received = 0;
                while (http.connected() && received < static_cast<size_t>(content_length) &&
                       self->online_active_) {
                    const size_t available = stream->available();
                    if (available > 0) {
                        size_t amount = available;
                        if (amount > static_cast<size_t>(content_length) - received) {
                            amount = static_cast<size_t>(content_length) - received;
                        }
                        received += stream->readBytes(work.jpg_buf + received, amount);
                    } else {
                        vTaskDelay(1);
                    }
                }
                if (received == static_cast<size_t>(content_length) && self->online_active_) {
                    xSemaphoreTake(self->online_save_mutex_, portMAX_DELAY);
                    work.jpg_len = received;
                    work.dsc.header.cf = LV_COLOR_FORMAT_RAW;
                    work.dsc.header.w = ONLINE_IMG_W;
                    work.dsc.header.h = ONLINE_IMG_H;
                    work.dsc.data = work.jpg_buf;
                    work.dsc.data_size = received;
                    work.valid = true;
                    xSemaphoreGive(self->online_save_mutex_);
                    if(carousel_timer != NULL){
                        xSemaphoreGive(self->online_img_sem_);
                    }
                }
            } else {
                Serial.printf("[PhotoAlbum] GET failed: %d\n", response);
            }
            http.end();
        }
        if(self->nextBtnCheck){ //如果不是轮播模式
           self->display_online_image();    
        } 
        self->nextBtnCheck = false;
        if(carousel_timer != NULL){
            xSemaphoreTake(self->online_img_show_sem_, portMAX_DELAY);  //等待图片展示完释放锁
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
     vTaskDelete(NULL);
}

void Photo_album::save_online_image_to_sd() {
    if (online_save_mutex_ == nullptr ||
        xSemaphoreTake(online_save_mutex_, pdMS_TO_TICKS(500)) != pdTRUE) return;
    ImgBuf_t& image = (online_active_idx_ == 0) ? online_buf_a_ : online_buf_b_;
    if (!image.valid || image.jpg_buf == nullptr || image.jpg_len == 0) {
        xSemaphoreGive(online_save_mutex_);
        show_toast(lv_scr_act(), "图片尚未加载");
        return;
    }
    const size_t length = image.jpg_len;
    uint8_t* copy = static_cast<uint8_t*>(heap_caps_malloc(length, MALLOC_CAP_SPIRAM));
    if (copy == nullptr) copy = static_cast<uint8_t*>(malloc(length));
    if (copy != nullptr) memcpy(copy, image.jpg_buf, length);
    xSemaphoreGive(online_save_mutex_);
    if (copy == nullptr) {
        show_toast(lv_scr_act(), "内存不足");
        return;
    }

    if (SD_MMC.cardType() == CARD_NONE) {
        free(copy);
        show_toast(lv_scr_act(), "未检测到SD卡");
        return;
    }
    if (!SD_MMC.exists("/photo")) SD_MMC.mkdir("/photo");
    char path[64];
    int index = 0;
    do {
        snprintf(path, sizeof(path), "/photo/online_%ld_%d.jpg", static_cast<long>(millis()), index++);
    } while (SD_MMC.exists(path));

    File file = SD_MMC.open(path, FILE_WRITE);
    const bool saved = file && file.write(copy, length) == length;
    if (file) file.close();
    free(copy);
    show_toast(lv_scr_act(), saved ? "保存成功" : "保存失败");
}

void Photo_album::online_img_click(lv_event_t* e) {
    Photo_album* self = static_cast<Photo_album*>(lv_event_get_user_data(e));
    if (self != nullptr && !self->playLocal) {
        self->save_online_image_to_sd();
    }
}

// 弹窗回调
void Photo_album::del_dialog_callback(bool is_confirm, void *user_data)
{
    if (!is_confirm) return;

    auto& photo_album = Photo_album::GetInstance();
    if (photo_album.current_img >= photo_album.image_files.size()) return;

    const String image_path = photo_album.image_files.at(photo_album.current_img);
    char fullPath[256];
    lv_snprintf(fullPath, sizeof(fullPath), "%s/%s", "/photo/", image_path.c_str());
    if (!SD_MMC.remove(fullPath)) {
        show_toast(lv_scr_act(),"删除失败");
        return;
    }
    photo_album.image_files.erase(photo_album.image_files.begin() + photo_album.current_img);
    if (photo_album.current_img >= photo_album.image_files.size()) {
        photo_album.current_img = 0;
    }
    photo_album.show_image();
    show_toast(lv_scr_act(),"删除成功");
}
//删除本地图片
void  Photo_album::img_delete(lv_event_t *e){
    auto& photo_album = Photo_album::GetInstance();
    if(photo_album.playLocal){
        confirm_dialog_create(lv_layer_top(),"删除","确认","取消",del_dialog_callback,NULL);
    }else{
        show_toast(lv_scr_act(),"网络图片无法删除");
    }
}
