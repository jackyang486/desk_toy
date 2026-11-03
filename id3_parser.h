#ifndef __ID3_PARSER_H_
#define __ID3_PARSER_H_

#include <Arduino.h>
#include <FS.h>
#include <vector>

// 轻量 ID3v2 标签解析（支持 v2.2 / v2.3 / v2.4）
// 只读标签区，不把整个文件读进内存：
//   - 文本帧（TIT2/TPE1/TALB）直接转成 UTF-8
//   - SYLT 同步歌词解析成 (时间, 文本) 列表
//   - APIC/PIC 封面只记录"图片数据在文件里的绝对偏移 + 长度"，用的时候再流式读取
namespace id3 {

struct LyricLine {
    uint32_t time_ms;   // 该行歌词的起始时间（毫秒）
    String   text;      // UTF-8
};

struct Cover {
    uint32_t offset;    // 图片数据在文件中的绝对偏移（已跳过 APIC/PIC 头）
    uint32_t length;    // 图片数据长度
    uint8_t  type;      // APIC 图片类型，3 = 封面正面
    char     mime[16];  // 例如 "image/jpeg"
};

class Tag {
public:
    Tag() { clear(); }
    void clear();

    bool has_cover() const { return cover.length > 0; }
    bool has_lyrics() const { return !lyrics.empty(); }

    // 返回 time_ms 时刻应显示的歌词行下标；早于第一行返回 -1
    int line_at(uint32_t time_ms) const;

    bool     valid;          // 是否存在合法的 ID3v2 标签
    uint8_t  version;        // 主版本号：2 / 3 / 4
    String   title;          // TIT2
    String   artist;         // TPE1
    String   album;          // TALB
    Cover    cover;          // APIC
    std::vector<LyricLine> lyrics;   // SYLT，已按时间升序
    String   plain_lyrics;   // USLT / TXXX，无时间轴（每行以 '\n' 分隔）
};

// 解析已打开文件中的 ID3v2 标签（内部会 seek，不会关闭文件）
bool parse(File &file, Tag &tag);

// 便捷封装：按路径打开并解析
bool parse_file(fs::FS &fs, const char *path, Tag &tag);

// 把封面图片数据读入 buf，返回实际读取到的字节数（0 表示失败）
size_t read_cover(File &file, const Tag &tag, uint8_t *buf, size_t size);

}  // namespace id3

#endif  // __ID3_PARSER_H_
