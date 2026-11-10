#include "music_tag.h"
#include "SD_MMC.h"
#include "esp_heap_caps.h"
#include "src/generated/gui_guider.h"
#include <JPEGDEC.h>
#include <new>

// JPEGDEC 的回调是 C 函数指针，没法带 this；用静态指针指向当前正在解码的实例。
// 封面解码全程在 LVGL 线程，不会并发，单指针足够。
static MusicTag *s_decoding = nullptr;

MusicTag::MusicTag()
    : tag_(nullptr),
      decoded_(nullptr),
      dec_w_(0),
      dec_h_(0),
      pixels_(nullptr),
      jpeg_(nullptr) {
    lv_memzero(&dsc_, sizeof(dsc_));
}

MusicTag::~MusicTag() {
    unload();
    release_cover(nullptr);
}

bool MusicTag::load(const char *path) {
    if (path == nullptr || path[0] == '\0') return false;
    // 每首歌重新 new：上一首的 vector/String 缓冲随 delete 整体归还堆
    unload();
    tag_ = new (std::nothrow) id3::Tag();
    if (tag_ == nullptr) return false;
    if (!id3::parse_file(SD_MMC, path, *tag_) || !tag_->valid) {
        return false;
    }
    if (!tag_->title.isEmpty()) {
        display_name_ = tag_->artist.isEmpty() ? tag_->title : (tag_->artist + " - " + tag_->title);
    }
    return true;
}

void MusicTag::unload() {
    if (tag_ != nullptr) {
        delete tag_;
        tag_ = nullptr;
    }
    display_name_ = "";
}

const char *MusicTag::lyric_text(int idx) const {
    if (tag_ == nullptr || idx < 0 || idx >= (int)tag_->lyrics.size()) return "";
    return tag_->lyrics[idx].text.c_str();
}

const String &MusicTag::title() const {
    static const String kEmpty;
    return tag_ != nullptr ? tag_->title : kEmpty;
}

const String &MusicTag::artist() const {
    static const String kEmpty;
    return tag_ != nullptr ? tag_->artist : kEmpty;
}

void MusicTag::dump() const {
    if (tag_ == nullptr) {
        printf("[id3] no tag\n");
        return;
    }
    printf("[id3] title=%s artist=%s cover=%lu@%lu lyrics=%u plain=%u\n",
           tag_->title.c_str(), tag_->artist.c_str(),
           (unsigned long)tag_->cover.length, (unsigned long)tag_->cover.offset,
           (unsigned)tag_->lyrics.size(), (unsigned)tag_->plain_lyrics.length());
    if (!tag_->lyrics.empty()) {
        printf("[id3] lyric first=%ums last=%ums \"%s\"\n",
               (unsigned)tag_->lyrics.front().time_ms,
               (unsigned)tag_->lyrics.back().time_ms,
               tag_->lyrics.front().text.c_str());
    } else {
        printf("[id3] 没解析到同步歌词（若 plain>0 说明只有无时间轴歌词）\n");
    }
}

void MusicTag::free_decoded() {
    if (decoded_ != nullptr) {
        free(decoded_);
        decoded_ = nullptr;
    }
    dec_w_ = 0;
    dec_h_ = 0;
}

int MusicTag::cover_draw_cb(JPEGDRAW *draw) {
    return s_decoding != nullptr ? s_decoding->cover_draw(draw) : 0;
}

// JPEGDEC 按 MCU 块回调，把每块拷进缩小后的整图缓冲
int MusicTag::cover_draw(JPEGDRAW *draw) {
    if (decoded_ == nullptr || draw == nullptr || draw->pPixels == nullptr) return 0;
    for (int row = 0; row < draw->iHeight; row++) {
        const int dy = draw->y + row;
        if (dy < 0 || dy >= dec_h_) continue;

        const uint16_t *src = draw->pPixels + (int32_t)row * draw->iWidth;
        int dx = draw->x;
        int off = 0;
        int cw = draw->iWidth;
        if (dx < 0) {
            off = -dx;
            cw += dx;
            dx = 0;
        }
        if (dx + cw > dec_w_) cw = dec_w_ - dx;
        if (cw <= 0) continue;

        memcpy(&decoded_[(int32_t)dy * dec_w_ + dx], src + off, (size_t)cw * 2);
    }
    return 1;
}

void MusicTag::reset_cover(lv_obj_t *img) const {
    if (img != nullptr) {
        lv_image_set_src(img, &_cover_2_RGB565A8_180x180);
        lv_image_set_pivot(img, 50, 50);
        lv_image_set_scale(img, LV_SCALE_NONE);
    }
}

void MusicTag::release_cover(lv_obj_t *img) {
    reset_cover(img);
    free_decoded();
    if (pixels_ != nullptr) {
        free(pixels_);
        pixels_ = nullptr;
        lv_memzero(&dsc_, sizeof(dsc_));
    }
    if (jpeg_ != nullptr) {
        jpeg_->~JPEGDEC();   // PSRAM 上 placement new 出来的对象
        free(jpeg_);
        jpeg_ = nullptr;
    }
}

bool MusicTag::show_cover(lv_obj_t *img, const char *path) {
    if (img == nullptr || tag_ == nullptr || !tag_->has_cover()) return false;
    if (path == nullptr || path[0] == '\0') return false;

    // 1) 把 APIC 的图片数据整段读出来（只读这一段，不读整个文件）
    const uint32_t jpg_len_expect = tag_->cover.length;
    uint8_t *jpg = (uint8_t *)heap_caps_malloc(jpg_len_expect, MALLOC_CAP_SPIRAM);
    if (jpg == nullptr) jpg = (uint8_t *)malloc(jpg_len_expect);
    if (jpg == nullptr) {
        printf("[cover] alloc %lu fail\n", (unsigned long)jpg_len_expect);
        return false;
    }

    File f = SD_MMC.open(path);
    const size_t jpg_len = f ? id3::read_cover(f, *tag_, jpg, jpg_len_expect) : 0;
    if (f) f.close();
    if (jpg_len != jpg_len_expect) {
        printf("[cover] read %u != %u（文件可能被截断/损坏）\n",
               (unsigned)jpg_len, (unsigned)jpg_len_expect);
        free(jpg);
        return false;
    }

    // 2) JPEGDEC 上下文约 16KB，优先放 PSRAM，别占内部 DRAM
    if (jpeg_ == nullptr) {
        void *mem = heap_caps_malloc(sizeof(JPEGDEC), MALLOC_CAP_SPIRAM);
        if (mem == nullptr) mem = malloc(sizeof(JPEGDEC));
        if (mem == nullptr) {
            printf("[cover] jpeg ctx alloc fail\n");
            free(jpg);
            return false;
        }
        jpeg_ = new (mem) JPEGDEC();
    }

    s_decoding = this;
    if (!jpeg_->openRAM(jpg, (int)jpg_len, cover_draw_cb)) {
        printf("[cover] openRAM fail (%u bytes)\n", (unsigned)jpg_len);
        s_decoding = nullptr;
        free(jpg);
        return false;
    }

    const int w = jpeg_->getWidth();
    const int h = jpeg_->getHeight();
    if (w <= 0 || h <= 0) {
        jpeg_->close();
        s_decoding = nullptr;
        free(jpg);
        return false;
    }

    // 3) 选一个"缩完仍不小于 180x180"的最大缩小比例
    int scale = 1;
    while (scale < 8 && (w / (2 * scale)) >= kCoverSize && (h / (2 * scale)) >= kCoverSize) {
        scale *= 2;
    }
    jpeg_->setPixelType(RGB565_LITTLE_ENDIAN);   // 和 LVGL 的 RGB565 字节序一致
    int options = 0;
    if (scale == 8) options |= JPEG_SCALE_EIGHTH;
    else if (scale == 4) options |= JPEG_SCALE_QUARTER;
    else if (scale == 2) options |= JPEG_SCALE_HALF;

    free_decoded();
    dec_w_ = (int16_t)((w + scale - 1) / scale);
    dec_h_ = (int16_t)((h + scale - 1) / scale);
    const size_t dec_bytes = (size_t)dec_w_ * dec_h_ * 2;
    decoded_ = (uint16_t *)heap_caps_malloc(dec_bytes, MALLOC_CAP_SPIRAM);
    if (decoded_ == nullptr) decoded_ = (uint16_t *)malloc(dec_bytes);
    if (decoded_ == nullptr) {
        printf("[cover] decode buf alloc %u fail\n", (unsigned)dec_bytes);
        dec_w_ = 0;
        dec_h_ = 0;
        jpeg_->close();
        s_decoding = nullptr;
        free(jpg);
        return false;
    }
    memset(decoded_, 0, dec_bytes);   // 边缘 MCU 可能不满，先清零避免采样到脏数据

    const int decoded_ok = jpeg_->decode(0, 0, options);
    jpeg_->close();
    s_decoding = nullptr;
    free(jpg);
    if (!decoded_ok) {
        printf("[cover] jpeg decode fail\n");
        free_decoded();
        return false;
    }

    if (pixels_ == nullptr) {
        const size_t px_bytes = (size_t)kCoverSize * kCoverSize * 2;
        pixels_ = (uint16_t *)heap_caps_malloc(px_bytes, MALLOC_CAP_SPIRAM);
        if (pixels_ == nullptr) pixels_ = (uint16_t *)malloc(px_bytes);
    }
    if (pixels_ == nullptr) {
        free_decoded();
        return false;
    }

    // 4) 最近邻采样到 180x180（缩略图本身已经很小，画质够用）
    for (int y = 0; y < kCoverSize; y++) {
        const int sy = (int)((int32_t)y * dec_h_ / kCoverSize);
        for (int x = 0; x < kCoverSize; x++) {
            const int sx = (int)((int32_t)x * dec_w_ / kCoverSize);
            pixels_[y * kCoverSize + x] = decoded_[(int32_t)sy * dec_w_ + sx];
        }
    }
    free_decoded();

    lv_memzero(&dsc_, sizeof(dsc_));
    dsc_.header.magic = LV_IMAGE_HEADER_MAGIC;
    dsc_.header.cf = LV_COLOR_FORMAT_RGB565;
    dsc_.header.w = kCoverSize;
    dsc_.header.h = kCoverSize;
    dsc_.header.stride = kCoverSize * 2;
    dsc_.data_size = (uint32_t)kCoverSize * kCoverSize * 2;
    dsc_.data = (const uint8_t *)pixels_;

    lv_obj_set_size(img, kCoverSize, kCoverSize);
    lv_image_set_scale(img, LV_SCALE_NONE);   // 源就是 180x180，不需要缩放
    lv_image_set_src(img, &dsc_);
    return true;
}
