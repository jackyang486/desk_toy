#include "usb_camera.h"

#include <USB_STREAM.h>
#include <JPEGDEC.h>
#include <JPEGENC.h>
#include <SD_MMC.h>
#include "usb_disk.h"
#include "esp_heap_caps.h"
#include <new>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace {

constexpr uint32_t kBufSize = 64 * 1024;  // 单缓冲大小：480×320 MJPEG 帧一般 15~40KB
constexpr uint16_t kFrameW = 480;         // 摄像头支持的分辨率二选一：480×320（比 1280×720
constexpr uint16_t kFrameH = 320;         //   帧小、解码缓冲省 1.5MB，且高度与输出 320 一致）
constexpr uint16_t kOutW = 240;           // 保存/显示尺寸：竖版 240×320（与屏同尺寸铺满）
constexpr uint16_t kOutH = 320;           //   （两者均为 16 的倍数：4:2:0 MCU 无需补边）
constexpr uint32_t kJpgOutSize = 64 * 1024;

USB_STREAM* s_usb = nullptr;
bool s_inited = false;
bool s_host_started = false;  // OTG 已切到 host 模式后，TinyUSB 设备模式（U盘）不可再用
volatile bool s_cam_connected = false;
volatile bool s_busy = false;  // capture_to_file 进行中（release 时避让）

uint8_t* s_xfer_a = nullptr;    // PSRAM：USB 传输缓冲（双缓冲）
uint8_t* s_xfer_b = nullptr;
uint8_t* s_frame_buf = nullptr; // PSRAM：驱动帧缓冲
uint8_t* s_jpeg_buf = nullptr;  // PSRAM：最新一帧 JPEG 的副本（回调任务写、拍照线程读）
uint8_t* s_rgb_buf = nullptr;   // PSRAM：解码后的整幅 RGB565（640×480×2 = 600KB）
uint8_t* s_out_rgb = nullptr;   // PSRAM：缩放后的 230×320 RGB565
uint8_t* s_jpg_out = nullptr;   // PSRAM：重编码 JPEG 输出缓冲
volatile size_t s_jpeg_len = 0;
volatile int s_dec_w = 0;       // 实际解码尺寸（按摄像头协商结果）
volatile int s_dec_h = 0;
SemaphoreHandle_t s_frame_sem = nullptr;

// USB 任务回调：收到一帧 MJPEG（即 JPEG）就拷进 s_jpeg_buf 并通知拍照线程
void on_frame(uvc_frame* frame, void* arg) {
    if (frame == nullptr || frame->data == nullptr || s_jpeg_buf == nullptr) return;
    const size_t len = frame->data_bytes;
    if (len < 2 || len > kBufSize) return;
    memcpy(s_jpeg_buf, frame->data, len);
    s_jpeg_len = len;
    xSemaphoreGive(s_frame_sem);
}

void on_state(usb_stream_state_t state, void* arg) {
    s_cam_connected = (state == STREAM_CONNECTED);
}

// JPEGDEC 绘制回调：按 MCU 块拼进整幅 RGB565 缓冲
int on_decode_draw(JPEGDRAW* pDraw) {
    if (s_rgb_buf == nullptr) return 0;
    uint16_t* dst = (uint16_t*)s_rgb_buf;
    const int w = s_dec_w;
    for (int j = 0; j < pDraw->iHeight; ++j) {
        memcpy(&dst[(pDraw->y + j) * w + pDraw->x],
               pDraw->pPixels + j * pDraw->iWidth,
               pDraw->iWidth * sizeof(uint16_t));
    }
    return 1;
}

// 中心裁剪 + 最近邻缩放：横版 (s_dec_w×s_dec_h) → 竖版 kOutW×kOutH
// 裁剪宽 = h * (kOutW/kOutH)，保比例只切左右两侧
void crop_and_scale() {
    const int w = s_dec_w, h = s_dec_h;
    const int crop_w = h * kOutW / kOutH;
    const int crop_x = (w - crop_w) / 2;
    const uint16_t* src = (uint16_t*)s_rgb_buf;
    uint16_t* dst = (uint16_t*)s_out_rgb;
    for (int oy = 0; oy < kOutH; ++oy) {
        const int sy = oy * h / kOutH;
        const uint16_t* srow = src + sy * w + crop_x;
        uint16_t* drow = dst + oy * kOutW;
        for (int ox = 0; ox < kOutW; ++ox) {
            drow[ox] = srow[ox * crop_w / kOutW];
        }
    }
}

// RGB565 → JPEG（内存编码，JPEGENC）。返回输出长度，0 = 失败。
// 注意：240×320 正好 16 对齐（4:2:0 MCU 16×16），addMCU 直接引用 s_out_rgb
// 的连续 MCU 数据（stride=480 字节），无需行缓冲补边——省 7.5KB SRAM。
size_t encode_out_rgb() {
    JPEGENC enc;
    // JPEGENCODE 结构含 2KB 文件缓冲，放堆上不占任务栈/静态 SRAM
    void* ctx_mem = heap_caps_malloc(sizeof(JPEGENCODE), MALLOC_CAP_8BIT);
    if (ctx_mem == nullptr) ctx_mem = heap_caps_malloc(sizeof(JPEGENCODE), MALLOC_CAP_SPIRAM);
    if (ctx_mem == nullptr) return 0;
    JPEGENCODE* jpe = new (ctx_mem) JPEGENCODE();
    size_t result = 0;
    if (enc.open(s_jpg_out, kJpgOutSize) == JPEGE_SUCCESS &&
        enc.encodeBegin(jpe, kOutW, kOutH, JPEGE_PIXEL_RGB565,
                        JPEGE_SUBSAMPLE_420, JPEGE_Q_MED) == JPEGE_SUCCESS) {
        const uint16_t* src = (const uint16_t*)s_out_rgb;
        result = (size_t)kJpgOutSize;  // 占位：成功路径由 close() 结果覆盖
        bool ok = true;
        for (int y = 0; y < kOutH && ok; y += 16) {
            for (int x = 0; x < kOutW; x += 16) {
                if (enc.addMCU(jpe, (uint8_t*)(src + y * kOutW + x),
                               kOutW * sizeof(uint16_t)) != JPEGE_SUCCESS) {
                    ok = false;
                    break;
                }
            }
        }
        const int len = ok ? enc.close() : -1;
        result = (len > 0) ? (size_t)len : 0;
    } else {
        enc.close();
    }
    jpe->~JPEGENCODE();
    heap_caps_free(ctx_mem);
    return result;
}
}  // namespace

Usb_camera& Usb_camera::GetInstance() {
    static Usb_camera inst;
    return inst;
}

bool Usb_camera::host_started() {
    return s_host_started;
}

bool Usb_camera::init(std::string& err_msg) {
    if (s_inited) return true;

    // 缓冲全放 PSRAM（usb_host_uvc 组件支持 Frame buffers in PSRAM），
    // 内部堆只剩 ~60KB，放不下这批缓冲
    s_xfer_a = (uint8_t*)heap_caps_malloc(kBufSize, MALLOC_CAP_SPIRAM);
    s_xfer_b = (uint8_t*)heap_caps_malloc(kBufSize, MALLOC_CAP_SPIRAM);
    s_frame_buf = (uint8_t*)heap_caps_malloc(kBufSize, MALLOC_CAP_SPIRAM);
    s_jpeg_buf = (uint8_t*)heap_caps_malloc(kBufSize, MALLOC_CAP_SPIRAM);
    s_rgb_buf = (uint8_t*)heap_caps_malloc((size_t)kFrameW * kFrameH * 2, MALLOC_CAP_SPIRAM);
    s_out_rgb = (uint8_t*)heap_caps_malloc((size_t)kOutW * kOutH * 2, MALLOC_CAP_SPIRAM);
    s_jpg_out = (uint8_t*)heap_caps_malloc(kJpgOutSize, MALLOC_CAP_SPIRAM);
    s_frame_sem = xSemaphoreCreateBinary();
    if (s_xfer_a == nullptr || s_xfer_b == nullptr || s_frame_buf == nullptr ||
        s_jpeg_buf == nullptr || s_rgb_buf == nullptr || s_out_rgb == nullptr ||
        s_jpg_out == nullptr || s_frame_sem == nullptr) {
        err_msg = "内存不足：无法分配摄像头缓冲";
        return false;
    }

    s_usb = new USB_STREAM();
    s_usb->uvcConfiguration(kFrameW, kFrameH,
                            FRAME_INTERVAL_FPS_15, kBufSize,
                            s_xfer_a, s_xfer_b, kBufSize, s_frame_buf);
    s_usb->uvcCamRegisterCb(on_frame, nullptr);
    s_usb->registerState(on_state, nullptr);
    s_usb->start();
    s_host_started = true;  // 从此 OTG 被 host 占用，U 盘模式（TinyUSB device）不可用
    s_usb->connectWait(5000);  // 等摄像头枚举完成（首次插入可能要几秒）

    if (!s_cam_connected) {
        err_msg = "未检测到 USB 摄像头，请确认已插好";
        return false;
    }
    s_usb->uvcCamSuspend(nullptr);  // 待机时不收流，拍照时再 resume
    s_inited = true;
    return true;
}

bool Usb_camera::capture_to_file(const char* path, std::string& err_msg) {
    // RAII 忙标志：函数任意路径退出都会清 s_busy，release() 据此避让拍照中的资源
    struct BusyGuard {
        ~BusyGuard() { s_busy = false; }
    } busy_guard;
    if (s_busy) {
        err_msg = "正在拍照，请稍候";
        return false;
    }
    s_busy = true;
    if (usb_disk_active()) {
        err_msg = "U盘模式运行中，无法使用摄像头";
        return false;
    }
    if (!init(err_msg)) return false;
    if (!s_cam_connected) {
        err_msg = "USB 摄像头未连接";
        return false;
    }

    s_jpeg_len = 0;
    xSemaphoreTake(s_frame_sem, 0);  // 清掉上次残留的信号量

    s_usb->uvcCamResume(nullptr);    // 开始收流
    const bool got = xSemaphoreTake(s_frame_sem, pdMS_TO_TICKS(4000)) == pdTRUE;
    s_usb->uvcCamSuspend(nullptr);   // 无论成败都停流
    if (!got) {
        err_msg = "拍照超时：摄像头无响应";
        return false;
    }

    const size_t len = s_jpeg_len;
    if (len < 4 || s_jpeg_buf[0] != 0xFF || s_jpeg_buf[1] != 0xD8) {
        err_msg = "拍照失败：帧数据不是有效的 JPEG";
        return false;
    }

    // 1) 解码 MJPEG → 整幅 RGB565（PSRAM）
    // JPEGDEC 对象（JPEGIMAGE 结构 ~18KB：Huffman 表 8KB + 像素/MCU 缓冲等）
    // 不能放任务栈，也不做静态常驻（白占 SRAM）——拍照时才从堆分配，用完释放。
    // 优先内部 RAM（解码高频访问快），不够退 PSRAM
    void* dec_mem = heap_caps_malloc(sizeof(JPEGDEC), MALLOC_CAP_8BIT);
    if (dec_mem == nullptr) dec_mem = heap_caps_malloc(sizeof(JPEGDEC), MALLOC_CAP_SPIRAM);
    if (dec_mem == nullptr) {
        err_msg = "拍照失败：内存不足以分配解码器";
        return false;
    }
    JPEGDEC* dec = new (dec_mem) JPEGDEC();
    bool dec_ok = false;
    if (dec->openRAM(s_jpeg_buf, (int)len, on_decode_draw)) {
        s_dec_w = dec->getWidth();
        s_dec_h = dec->getHeight();
        if (s_dec_w > 0 && s_dec_h > 0 && s_dec_w <= (int)kFrameW && s_dec_h <= (int)kFrameH) {
            dec->setPixelType(RGB565_LITTLE_ENDIAN);  // 与 JPEGENC 的 RGB565 输入字节序一致
            dec_ok = (dec->decode(0, 0, 0) == 1);
        }
        dec->close();
    }
    dec->~JPEGDEC();
    heap_caps_free(dec_mem);
    if (!dec_ok) {
        err_msg = "拍照失败：JPEG 解码错误";
        return false;
    }

    // 2) 中心裁剪 + 缩放到 230×320
    crop_and_scale();

    // 3) 重编码 JPEG（4:2:0，内存输出）
    const size_t out_len = encode_out_rgb();
    if (out_len == 0) {
        err_msg = "拍照失败：JPEG 编码错误";
        return false;
    }

    // 4) 写 SD
    File f = SD_MMC.open(path, FILE_WRITE);
    if (!f) {
        err_msg = "无法写入 SD 卡";
        return false;
    }
    const size_t written = f.write(s_jpg_out, out_len);
    f.close();
    if (written != out_len) {
        err_msg = "SD 卡写入不完整";
        return false;
    }
    return true;
}

bool Usb_camera::release() {
    if (s_busy) return false;   // 拍照进行中，不能动 USB/缓冲
    if (!s_host_started) return true;  // 未启动，视为已释放

    if (s_usb != nullptr) {
        s_usb->stop();  // 删除 USB 内部任务、释放驱动资源
        delete s_usb;
        s_usb = nullptr;
    }
    heap_caps_free(s_xfer_a);
    heap_caps_free(s_xfer_b);
    heap_caps_free(s_frame_buf);
    heap_caps_free(s_jpeg_buf);
    heap_caps_free(s_rgb_buf);
    heap_caps_free(s_out_rgb);
    heap_caps_free(s_jpg_out);
    if (s_frame_sem != nullptr) {
        vSemaphoreDelete(s_frame_sem);
        s_frame_sem = nullptr;
    }
    s_xfer_a = s_xfer_b = s_frame_buf = s_jpeg_buf = nullptr;
    s_rgb_buf = s_out_rgb = s_jpg_out = nullptr;
    s_inited = false;
    s_host_started = false;  // OTG 归还：U 盘模式（TinyUSB device）恢复可用
    s_cam_connected = false;
    return true;
}
