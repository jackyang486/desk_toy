#ifndef __MUSIC_TAG_H_
#define __MUSIC_TAG_H_

#include <Arduino.h>
#include <lvgl.h>
#include <JPEGDEC.h>   // JPEGDRAW 是 typedef，无法前向声明，只能直接包含
#include "id3_parser.h"

// 音乐标签对象：ID3v2 解析结果的持有者 + 专辑封面 JPEG 解码器。
//
// 之前这部分代码散落在 music.cpp 里（一堆 g_cover_xxx 全局变量 + static 函数），
// 现在收敛成一个对象，music.cpp 只保留 UI 逻辑。
//
// 生命周期（重要）：进音乐页 load()，退音乐页 unload()。
// 不能只 clear()：vector.clear() 不还元素数组容量，String 赋 "" 不还缓冲，
// 退出后仍会常驻 2~6KB 且正好散在主区空闲洞里，阻碍空闲块合并。
class MusicTag {
public:
    static const int kCoverSize = 180;   // 与 screen_music_album_img 的尺寸保持一致

    MusicTag();
    ~MusicTag();

    // 解析指定文件的 ID3 标签（路径相对 SD_MMC 挂载点）
    bool load(const char *path);
    // 释放标签对象与所有封面缓冲
    void unload();

    bool valid() const {
        return tag_ != nullptr && tag_->valid;
    }
    bool has_lyrics() const {
        return tag_ != nullptr && !tag_->lyrics.empty();
    }
    bool has_cover() const {
        return tag_ != nullptr && tag_->has_cover();
    }

    // ---- 歌词 ----
    int  lyric_count() const {
        return tag_ != nullptr ? (int)tag_->lyrics.size() : 0;
    }
    // idx 越界返回空串
    const char *lyric_text(int idx) const;
    // 返回 time_ms 时刻应显示的歌词行下标；早于第一行返回 -1
    int  line_at(uint32_t time_ms) const {
        return tag_ != nullptr ? tag_->line_at(time_ms) : -1;
    }

    // ---- 文字信息 ----
    const String &title() const;
    const String &artist() const;
    // "歌手 - 歌名"；没有歌手时只有歌名；都没有时返回空串
    const String &display_name() const {
        return display_name_;
    }
    void dump() const;   // 打印解析结果，便于排查

    // ---- 封面 ----
    // 读取 APIC 数据 → JPEGDEC 缩小解码 → 最近邻采样成 180x180 RGB565 显示到 img
    bool show_cover(lv_obj_t *img, const char *path);
    // 恢复默认封面资源（不改动已分配的解码缓冲）
    void reset_cover(lv_obj_t *img) const;
    // 退出播放页：恢复默认封面并释放解码缓冲 / JPEG 上下文
    void release_cover(lv_obj_t *img);

private:
    int  cover_draw(JPEGDRAW *draw);        // JPEGDEC 逐 MCU 回调的成员实现
    static int cover_draw_cb(JPEGDRAW *draw);   // 静态跳板（JPEGDEC 只接受 C 函数指针）
    void free_decoded();

    id3::Tag *tag_;
    String    display_name_;
    uint16_t *decoded_;    // 缩小解码后的整图（RGB565）
    int16_t   dec_w_;
    int16_t   dec_h_;
    uint16_t *pixels_;     // 180x180 RGB565，直接作为 LVGL 图片源
    lv_image_dsc_t dsc_;
    JPEGDEC  *jpeg_;       // 约 16KB，按需分配，优先放 PSRAM
};

#endif  // __MUSIC_TAG_H_
