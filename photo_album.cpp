#include "photo_album.h"
#include "src/generated/gui_guider.h"
#include "src/custom/confirm_dialog.h"
#include "ESP32_board.h"
#include "SD_MMC.h"
#include "setting.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "esp_heap_caps.h"

static const char *basePath = "S:/photo";
static lv_timer_t *carousel_timer = NULL;

// lv_image_set_src() 对文件路径只保存指针不拷贝，解码发生在之后的绘制阶段，
// 所以路径缓冲必须是静态的：栈上 char[256] 函数返回后就是野指针。
static char lv_img_path[256];

// 只校验 SOI+APPn 标志（FF D8 FF）：原来的 FF D8 FF E0 00 10 "JFIF" 太严，
// 网上大量 Exif（FF D8 FF E1）图片会被误判为非法而丢弃。
static bool is_jpeg(const uint8_t* data, size_t len) {
    if (data == nullptr || len < 3) return false;
    return data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF;
}

Photo_album& Photo_album::GetInstance() {
    static Photo_album instance;
    return instance;
}

Photo_album::Photo_album():playLocal(true),nextBtnCheck(false),current_img(0),hasInitial(false){
}

//展示本地图片显示
void Photo_album::show_image()
{
    if(image_files.empty()) {
        lv_image_set_src(guider_ui.screen_photo_view, NULL);
        return;
    }
    if(image_files.empty()) return;   // 空列表保护
    if(current_img >= image_files.size()) current_img = 0;
     // 构建完整路径（静态缓冲，LVGL 后续绘制阶段还会用到这个指针）
    lv_snprintf(lv_img_path, sizeof(lv_img_path), "%s/%s", basePath, image_files.at(current_img));
    lv_image_set_src(guider_ui.screen_photo_view, lv_img_path);
}
// scanDirEach 回调：文件名直接拷进 PSRAM 路径池，不产生内部堆碎片
static bool photo_path_sink(void *ctx, const char *name) {
    return static_cast<MusicPathList *>(ctx)->push(name);
}
void Photo_album::scan_image(){
     board.scanDirEach(basePath, photo_path_sink, &image_files, IMG_TYPE);
     show_image();
}
//页面加载
void photo_win_loaded(SCREEN_LOAD_STAUS status){ 
     auto& photo_album = Photo_album::GetInstance();
    if(status == LOADED){
        Setting::GetInstance().LoadOnlineImageUrl();
        if(!photo_album.hasInitial){
            photo_album.hasInitial = true;
            photo_album.scan_image();
            lv_obj_add_event_cb(guider_ui.screen_photo_left_arrow, Photo_album::img_left_btn, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_right_arrow, Photo_album::img_right_btn, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_btn_web, Photo_album::img_play_choose, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_btn_loop, Photo_album::image_loop, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_btn_delete, Photo_album::img_delete, LV_EVENT_CLICKED,NULL);
            lv_obj_add_event_cb(guider_ui.screen_photo_view, Photo_album::online_img_click, LV_EVENT_LONG_PRESSED,&photo_album);
        }
        lv_obj_clear_state(guider_ui.screen_photo_btn_web, LV_STATE_CHECKED);
        lv_obj_clear_state(guider_ui.screen_photo_btn_loop, LV_STATE_CHECKED);
    }else{
        photo_album.playLocal = true;
        // 目录列表里几十个 String 占的是内部堆，退出页面就放掉，别一直攥着
        photo_album.release_file_list();
        if(carousel_timer != NULL)
        { 
            lv_timer_del(carousel_timer);
            carousel_timer = NULL;
        }
        photo_album.stop_online();
        // 下载任务可能还卡在 HTTP 里（超时 10s），此时 free 缓冲就是 use-after-free。
        // release_online_buffers() 会先检查任务句柄，任务没退就什么都不做（缓冲留到下次复用）。
        photo_album.release_online_buffers();
    }
}
//上一张
void Photo_album::img_left_btn(lv_event_t *e){
     auto& photo_album = Photo_album::GetInstance();
     lv_obj_clear_state(guider_ui.screen_photo_btn_loop, LV_STATE_CHECKED);
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
    
    lv_obj_clear_state(guider_ui.screen_photo_btn_loop, LV_STATE_CHECKED);
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
        if (self->image_files.empty()) return;   //目录为空时不要走 at()，会 abort
      // 构建完整路径（静态缓冲）
        lv_snprintf(lv_img_path, sizeof(lv_img_path), "%s/%s", basePath, self->getNextImagePath());
        if (lv_img_path[0] == '\0') return;
        lv_image_set_src(guider_ui.screen_photo_view, lv_img_path);
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
        carousel_timer = lv_timer_create(lv_timer_img_carousel, 5000,&photo_album);
    }else{
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
        if(WiFi.status() != WL_CONNECTED){
          show_toast(lv_scr_act(), "WIFI未连接!");
          return;
        }
        photo_album.playLocal = false;
        photo_album.nextBtnCheck = true;
        if (!photo_album.start_online()) {
            // 启动失败（任务未退出/内存不足）：回滚状态，否则 UI 卡在网络模式黑屏
            photo_album.playLocal = true;
            return;
        }
     }else{                         //播放本地文件
        photo_album.playLocal = true;
        photo_album.stop_online();
        photo_album.show_image();
     }
}

bool Photo_album::allocate_online_buf(ImgBuf_t& buf) {
    if (buf.jpg_buf != nullptr) return true;
    // 只许吃 PSRAM，绝不回退 malloc()：内部 SRAM 要留给 SSL 握手等大块需求，
    // 100KB 的图片缓冲占掉内部堆等于谋杀 chat_robot 的 websocket 连接
    buf.jpg_buf = static_cast<uint8_t*>(heap_caps_malloc(ONLINE_JPG_BUF_SIZE, MALLOC_CAP_SPIRAM));
    if (buf.jpg_buf == nullptr) return false;
    buf.jpg_len = 0;
    buf.valid = false;
    memset(&buf.dsc, 0, sizeof(buf.dsc));
    buf.dsc.header.cf = LV_COLOR_FORMAT_RAW;
    buf.dsc.header.w = ONLINE_IMG_W;
    buf.dsc.header.h = ONLINE_IMG_H;
    return true;
}

bool Photo_album::start_online() {
    // 上一个下载任务还没退干净就再起一个，会有两个任务同时写同一对 buffer
    if (online_task_handle_ != nullptr) {
        show_toast(lv_scr_act(), "图片任务未结束");
        return false;
    }
    if (!allocate_online_buf(online_buf_a_) || !allocate_online_buf(online_buf_b_)) {
        show_toast(lv_scr_act(), "内存不足");
        return false;
    }
    if (online_img_sem_ == nullptr) online_img_sem_ = xSemaphoreCreateBinary();
    if (online_img_show_sem_ == nullptr) online_img_show_sem_ = xSemaphoreCreateBinary();
    if (online_img_sem_ == nullptr || online_img_show_sem_  == nullptr) {
        show_toast(lv_scr_act(), "网络图片初始化失败");
        return false;
    }
    online_active_ = true;
    online_display_pending_ = false;

    // 主线程定时器：消费 online_display_pending_，把 lv_image_set_src 拉回 ui_task 执行
    if (online_view_timer_ == nullptr) {
        online_view_timer_ = lv_timer_create(lv_timer_online_view, 30, this);
    }

    const BaseType_t ok = xTaskCreatePinnedToCore(
        online_download_task,
        "online_img_task",
        1024 * 8,
        this,
        2 | portPRIVILEGE_BIT,
        &online_task_handle_,
        1
    );
    if (ok != pdPASS) {
        // 创建失败时句柄内容未定义，必须复位，否则 stop_online 会一直以为任务还在跑
        online_task_handle_ = nullptr;
        online_active_ = false;
        if (online_view_timer_ != nullptr) {
            lv_timer_del(online_view_timer_);
            online_view_timer_ = nullptr;
        }
        show_toast(lv_scr_act(), "下载任务创建失败");
        return false;
    }
    return true;
}

void Photo_album::stop_online() {
    online_active_ = false;
    online_display_pending_ = false;

    if (online_view_timer_ != nullptr) {
        lv_timer_del(online_view_timer_);
        online_view_timer_ = nullptr;
    }

    // 先把可能卡在等待里的下载任务唤醒：它要么在 take(online_img_show_sem_, ...)，
    // 要么在 take(online_img_sem_)。不唤醒就永远出不来（原来直接置 nullptr 就是这个问题）。
    if (online_img_sem_ != nullptr) xSemaphoreGive(online_img_sem_);
    if (online_img_show_sem_ != nullptr) xSemaphoreGive(online_img_show_sem_);

    // 必须等任务真的退出：调用方紧接着可能 free buffer，任务还在写就是 use-after-free。
    // 1.5s 足以覆盖任务在各处 vTaskDelay(50ms) 的切片和 sem 等待，HTTP 卡死的极端情况
    // 交给 release_online_buffers() 兜底（它不会在任务活着时释放）。
    for (int i = 0; i < 150 && online_task_handle_ != nullptr; i++) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    if (online_task_handle_ == nullptr) {
        if (online_img_sem_ != nullptr) {
            vSemaphoreDelete(online_img_sem_);
            online_img_sem_ = nullptr;
        }
        if (online_img_show_sem_ != nullptr) {
            vSemaphoreDelete(online_img_show_sem_);
            online_img_show_sem_ = nullptr;
        }
    } else {
        // 任务还卡在 HTTP 里（超时 10s），这时删信号量会让它踩空指针，只能先留着
        printf("[PhotoAlbum] download task still running, semaphores kept\n");
    }
}

void Photo_album::release_online_buffers() {
    // 下载任务还在写这两块内存时绝对不能释放，否则就是 use-after-free。
    // 留到下次 start_online() 复用（allocate_online_buf 会判断非空直接返回），
    // 或者下次退出页面时任务已经退了再释放。
    if (online_task_handle_ != nullptr) {
        printf("[PhotoAlbum] task alive, buffers kept\n");
        return;
    }
    if (online_buf_a_.jpg_buf != nullptr) {
        heap_caps_free(online_buf_a_.jpg_buf);
        online_buf_a_.jpg_buf = nullptr;
    }
    if (online_buf_b_.jpg_buf != nullptr) {
        heap_caps_free(online_buf_b_.jpg_buf);
        online_buf_b_.jpg_buf = nullptr;
    }
    online_buf_a_.valid = false;
    online_buf_a_.jpg_len = 0;
    online_buf_a_.dsc.data = nullptr;
    online_buf_a_.dsc.data_size = 0;
    online_buf_b_.valid = false;
    online_buf_b_.jpg_len = 0;
    online_buf_b_.dsc.data = nullptr;
    online_buf_b_.dsc.data_size = 0;
}

void Photo_album::request_online_image(int32_t seed_delta) {
    if (!online_active_ && !start_online()) return;
    online_seed_ += seed_delta;
}

// 只能在主线程（ui_task / lv_timer 回调）里调用
void Photo_album::display_online_image() {
    if (!online_active_) return;
    ImgBuf_t& image = (online_active_idx_ == 0) ? online_buf_a_ : online_buf_b_;
    if (!image.valid || image.dsc.data == nullptr) return;   // 无效时不要翻转索引，否则下次会写错 buffer
    online_active_idx_ = !online_active_idx_;
    printf("play online image\n");
    lv_image_set_src(guider_ui.screen_photo_view, &image.dsc);
    lv_obj_invalidate(guider_ui.screen_photo_view);
}

// 下载任务只写 online_display_pending_，由这个主线程定时器消费，
// 避免在后台任务里直接操作 LVGL 对象（竞态 / 花屏 / 偶发崩溃）。
void Photo_album::lv_timer_online_view(lv_timer_t *timer) {
    Photo_album* self = static_cast<Photo_album*>(lv_timer_get_user_data(timer));
    if (self == nullptr || !self->online_active_) return;
    if (!self->online_display_pending_) return;
    self->online_display_pending_ = false;
    self->display_online_image();
}

void Photo_album::online_download_task(void* pvParam) {
    Photo_album* self = static_cast<Photo_album*>(pvParam);
    //WiFiClientSecure wifi_client;
    //wifi_client.setInsecure();
    WiFiClient wifi_client;
    
    while (self->online_active_) {
        ImgBuf_t& work = (self->online_active_idx_ == 0)
            ? self->online_buf_a_ : self->online_buf_b_;
        if (work.jpg_buf == nullptr) {
            vTaskDelay(pdMS_TO_TICKS(200));   // 不能 continue 空转，否则核 1 被打满
            continue;
        }
        if(carousel_timer == NULL && !self->nextBtnCheck){
            // 分段睡眠，便于 stop_online() 尽快回收任务
            for (int i = 0; i < 10 && self->online_active_; i++) {
                vTaskDelay(pdMS_TO_TICKS(50));
            }
            continue;
        }
        work.valid = false;   // 下载前先作废旧状态，失败时不会重复显示上一张
        HTTPClient http;
        String url = Setting::GetInstance().GetOnlineImageUrlBase()+ "&seed=" + String(self->online_seed_);
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
                    if (!is_jpeg(work.jpg_buf, received)) {
                        printf("[PhotoAlbum] not a jpeg (head %02X %02X %02X %02X), skipped\n",
                               work.jpg_buf[0], work.jpg_buf[1], work.jpg_buf[2], work.jpg_buf[3]);
                        work.valid = false;
                    } else {
                        work.jpg_len = received;
                        work.dsc.header.cf = LV_COLOR_FORMAT_RAW;
                        work.dsc.header.w = ONLINE_IMG_W;
                        work.dsc.header.h = ONLINE_IMG_H;
                        work.dsc.data = work.jpg_buf;
                        work.dsc.data_size = received;
                        work.valid = true;
                        // content_length 是 int64_t，用 %d 会读到寄存器填充槽里的垃圾值
                        printf("[PhotoAlbum] image ready: %lld bytes\n",
                               static_cast<long long>(content_length));
                        if(carousel_timer != NULL){
                            xSemaphoreGive(self->online_img_sem_);
                        }
                    }
                }
            } else {
                Serial.printf("[PhotoAlbum] GET failed: %d\n", response);
            }
            http.end();
        }
        if(self->nextBtnCheck){ //如果不是轮播模式
            self->online_display_pending_ = true;
        } 
        self->nextBtnCheck = false;
        if(carousel_timer != NULL){
            xSemaphoreTake(self->online_img_show_sem_, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    // 先清句柄再自删：stop_online() 靠这个句柄判断任务是否已经退干净
    self->online_task_handle_ = nullptr;
    vTaskDelete(NULL);
}

void Photo_album::save_online_image_to_sd() {
    ImgBuf_t& image = (online_active_idx_ == 0) ? online_buf_b_ : online_buf_a_;
    if (!image.valid || image.jpg_buf == nullptr || image.jpg_len == 0) {
        show_toast(lv_scr_act(), "图片尚未加载");
        return;
    }
    const size_t length = image.jpg_len;
    uint8_t* copy = static_cast<uint8_t*>(heap_caps_malloc(length, MALLOC_CAP_SPIRAM));
    if (copy == nullptr) copy = static_cast<uint8_t*>(malloc(length));
    if (copy != nullptr) memcpy(copy, image.jpg_buf, length);
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
    // user_data 是弹窗创建时快照下来的文件名（new String），这里接管所有权
    String *name_ptr = static_cast<String *>(user_data);
    const String name = name_ptr ? *name_ptr : String();
    delete name_ptr;
    if (!is_confirm || name.isEmpty()) return;

    auto& photo_album = Photo_album::GetInstance();
    // 按名字重新定位：弹窗期间可能已经翻页/重新扫描过，用索引会删错文件
    int index = -1;
    for (size_t i = 0; i < photo_album.image_files.size(); i++) {
        if (strcmp(photo_album.image_files.at(i), name.c_str()) == 0) {
            index = static_cast<int>(i);
            break;
        }
    }
    if (index < 0) {
        show_toast(lv_scr_act(),"文件不存在");
        return;
    }

    char fullPath[256];
    lv_snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, name.c_str());
    if (!SD_MMC.remove(fullPath)) {
        show_toast(lv_scr_act(),"删除失败");
        return;
    }
    photo_album.image_files.remove_at(static_cast<size_t>(index));
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
        if (photo_album.image_files.empty()) {
            show_toast(lv_scr_act(),"没有可删除的图片");
            return;
        }
        // 弹窗期间停轮播：否则 current_img 会变化，用户确认时删的就不是眼前这张
        if(carousel_timer != NULL){
            lv_timer_del(carousel_timer);
            carousel_timer = NULL;
        }
        lv_obj_clear_state(guider_ui.screen_photo_btn_loop, LV_STATE_CHECKED);
        String *name = new String(photo_album.image_files.at(photo_album.current_img));
        confirm_dialog_create(lv_layer_top(),"删除","确认","取消",del_dialog_callback,name);
    }else{
        show_toast(lv_scr_act(),"网络图片无法删除");
    }
}
