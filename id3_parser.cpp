#include "id3_parser.h"

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_heap_caps.h"

namespace id3 {
namespace {

constexpr uint32_t kTextFrameMax  = 1024;   // 文本帧最多读这么多（歌名/歌手/专辑足够了）
constexpr uint32_t kLyricFrameMax = 8192;   // SYLT/USLT/TXXX 帧最多读这么多

// 歌词帧缓冲：原来每解析一次都 malloc/free 最多 8KB 内部 RAM，反复进出音乐页会在
// 内部堆里留下小洞。改成 PSRAM 上一块常驻复用（解析串行执行，无并发问题）。
// PSRAM 不可用时退回内部 RAM 一次性分配。
uint8_t *lyric_frame_buf() {
    static uint8_t *buf = nullptr;
    if (buf == nullptr) {
        buf = (uint8_t *)heap_caps_malloc(kLyricFrameMax, MALLOC_CAP_SPIRAM);
        if (buf == nullptr) buf = (uint8_t *)malloc(kLyricFrameMax);
    }
    return buf;
}

uint32_t synchsafe32(const uint8_t *p) {
    return ((uint32_t)p[0] << 21) | ((uint32_t)p[1] << 14) | ((uint32_t)p[2] << 7) | (uint32_t)p[3];
}

uint32_t be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

// 一个 Unicode 码点编码成 UTF-8 追加到 out
void append_utf8(uint32_t cp, String &out) {
    char b[4];
    if (cp < 0x80) {
        b[0] = (char)cp;
        out.concat(b, 1);
    } else if (cp < 0x800) {
        b[0] = (char)(0xC0 | (cp >> 6));
        b[1] = (char)(0x80 | (cp & 0x3F));
        out.concat(b, 2);
    } else if (cp < 0x10000) {
        b[0] = (char)(0xE0 | (cp >> 12));
        b[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        b[2] = (char)(0x80 | (cp & 0x3F));
        out.concat(b, 3);
    } else {
        b[0] = (char)(0xF0 | (cp >> 18));
        b[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
        b[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
        b[3] = (char)(0x80 | (cp & 0x3F));
        out.concat(b, 4);
    }
}

// UTF-16 -> UTF-8（处理 BOM 与代理对，遇到 0x0000 结束）
void utf16_to_utf8(const uint8_t *p, uint32_t len, bool little_endian, String &out) {
    uint32_t i = 0;
    while (i + 1 < len) {
        uint32_t u = little_endian ? (uint32_t)(p[i] | (p[i + 1] << 8))
                                   : (uint32_t)((p[i] << 8) | p[i + 1]);
        i += 2;
        if (u == 0) break;
        if (u >= 0xD800 && u <= 0xDBFF && i + 1 < len) {   // 高代理
            uint32_t u2 = little_endian ? (uint32_t)(p[i] | (p[i + 1] << 8))
                                        : (uint32_t)((p[i] << 8) | p[i + 1]);
            if (u2 >= 0xDC00 && u2 <= 0xDFFF) {            // 低代理
                u = 0x10000 + ((u - 0xD800) << 10) + (u2 - 0xDC00);
                i += 2;
            }
        }
        append_utf8(u, out);
    }
}

// 按 ID3 的编码字节把一段字符串转成 UTF-8 追加到 out
//   0 = ISO-8859-1, 1 = UTF-16(带 BOM), 2 = UTF-16BE, 3 = UTF-8
void decode_string(uint8_t enc, const uint8_t *p, uint32_t len, String &out) {
    if (p == nullptr || len == 0) return;
    switch (enc) {
        case 1: {
            bool little_endian = true;   // 没有 BOM 时按小端处理（Windows 生成的标签基本都带 BOM）
            if (len >= 2 && p[0] == 0xFF && p[1] == 0xFE) {
                p += 2;
                len -= 2;
            } else if (len >= 2 && p[0] == 0xFE && p[1] == 0xFF) {
                little_endian = false;
                p += 2;
                len -= 2;
            }
            utf16_to_utf8(p, len, little_endian, out);
            break;
        }
        case 2:
            utf16_to_utf8(p, len, false, out);
            break;
        case 3: {
            uint32_t n = 0;
            while (n < len && p[n] != 0) n++;
            out.concat((const char *)p, n);
            break;
        }
        default:
            for (uint32_t i = 0; i < len && p[i] != 0; i++) {
                if (p[i] < 0x80) out += (char)p[i];
                else append_utf8(p[i], out);   // 0x80~0xFF 映射到 U+0080~U+00FF
            }
            break;
    }
}

// 从 p[i] 开始读一个以 0 结尾的字符串（UTF-16 按 2 字节对齐）。
// 返回文本结束位置（不含终止符），i 前进到终止符之后。
uint32_t scan_cstr(const uint8_t *p, uint32_t len, uint32_t &i, uint8_t enc) {
    if (enc == 1 || enc == 2) {
        while (i + 1 < len) {
            if (p[i] == 0 && p[i + 1] == 0) {
                uint32_t end = i;
                i += 2;
                return end;
            }
            i += 2;
        }
        i = len;
        return len;
    }
    while (i < len && p[i] != 0) i++;
    uint32_t end = i;
    if (i < len) i++;
    return end;
}

uint32_t read_body(File &file, uint32_t pos, uint32_t len, uint8_t *buf, uint32_t cap) {
    if (len > cap) len = cap;
    if (!file.seek(pos)) return 0;
    return (uint32_t)file.read(buf, len);
}

// SYLT：enc(1) lang(3) timestamp_format(1) content_type(1) descriptor\0 (文本 + 4 字节时间戳)*
// timestamp_format: 1 = MPEG 帧号（需要码率才能换算），2 = 毫秒
void parse_sylt(const uint8_t *d, uint32_t n, Tag &tag) {
    if (n < 6) return;
    const uint8_t enc = d[0];
    // d[4] 是时间戳格式：1 = MPEG 帧号，2 = 毫秒。两种都按原始值存，
    // 只有 2 能直接当毫秒用（绝大多数工具写的都是 2）。
    uint32_t i = 6;
    scan_cstr(d, n, i, enc);   // 内容描述符

    while (i < n) {
        const uint32_t start = i;
        const uint32_t end = scan_cstr(d, n, i, enc);
        if (i + 4 > n) break;  // 没有时间戳，说明后面都是填充
        const uint32_t ts = be32(d + i);
        i += 4;
        if (end <= start) continue;   // 空行

        String text;
        decode_string(enc, d + start, end - start, text);
        text.trim();
        if (text.isEmpty()) continue;

        LyricLine line;
        // time_fmt == 2 是毫秒；time_fmt == 1 是 MPEG 帧号（没有码率信息，只能按原值存）
        line.time_ms = ts;
        line.text = text;
        tag.lyrics.push_back(line);
    }
}

// USLT：enc(1) lang(3) descriptor\0 歌词正文
void parse_uslt(const uint8_t *d, uint32_t n, Tag &tag) {
    if (n < 5) return;
    const uint8_t enc = d[0];
    uint32_t i = 4;
    scan_cstr(d, n, i, enc);
    if (i >= n) return;

    String text;
    decode_string(enc, d + i, n - i, text);
    text.trim();
    if (!text.isEmpty() && tag.plain_lyrics.isEmpty()) tag.plain_lyrics = text;
}

// TXXX：enc(1) 描述\0 值。有些文件把整段歌词塞在 "LYRICS" 这个 TXXX 里。
void parse_txxx(const uint8_t *d, uint32_t n, Tag &tag) {
    if (n < 2) return;
    const uint8_t enc = d[0];
    uint32_t i = 1;
    const uint32_t start = i;
    const uint32_t end = scan_cstr(d, n, i, enc);
    if (i >= n) return;

    String desc;
    decode_string(enc, d + start, end - start, desc);
    desc.trim();
    desc.toUpperCase();

    if (desc.indexOf("LYRIC") < 0) return;

    String value;
    decode_string(enc, d + i, n - i, value);
    value.trim();
    if (!value.isEmpty() && tag.plain_lyrics.isEmpty()) tag.plain_lyrics = value;
}

// APIC：enc(1) mime\0 type(1) 描述\0 图片数据
// PIC (v2.2)：enc(1) format(3) type(1) 描述\0 图片数据
void parse_apic(File &file, uint32_t data_pos, uint32_t fsize, bool v22, Tag &tag) {
    uint8_t buf[256];
    const uint32_t n = read_body(file, data_pos, fsize, buf, sizeof(buf));
    if (n < 4) return;

    const uint8_t enc = buf[0];
    uint32_t i = 1;

    if (v22) {
        if (i + 3 > n) return;
        // "JPG" / "PNG" 之类的 3 字符格式
        snprintf(tag.cover.mime, sizeof(tag.cover.mime), "image/%s",
                 (buf[i] == 'J' || buf[i] == 'j') ? "jpeg" : "png");
        i += 3;
    } else {
        const uint32_t start = i;
        while (i < n && buf[i] != 0) i++;
        uint32_t mlen = i - start;
        if (mlen >= sizeof(tag.cover.mime)) mlen = sizeof(tag.cover.mime) - 1;
        memcpy(tag.cover.mime, buf + start, mlen);
        tag.cover.mime[mlen] = 0;
        if (i >= n) return;
        i++;   // 跳过 mime 的结束符
    }

    if (i >= n) return;
    tag.cover.type = buf[i++];

    scan_cstr(buf, n, i, enc);   // 跳过图片描述
    if (i >= n || i >= fsize) return;   // 描述超出缓冲（极少见）或没有图片数据

    tag.cover.offset = data_pos + i;
    tag.cover.length = fsize - i;
}

// LRC 单个时间戳解析："mm:ss.xx" / "mm:ss.xxx" / "mm:ss"，成功返回毫秒
bool lrc_time(const String &s, uint32_t &ms) {
    const int colon = s.indexOf(':');
    if (colon <= 0 || colon == (int)s.length() - 1) return false;
    const int dot = s.indexOf('.', colon);
    const int sec_end = (dot > colon) ? dot : s.length();
    if (dot == (int)s.length() - 1) return false;
    // 冒号前后必须是纯数字（区分 [ti:彩虹] 这类元数据 tag）
    for (int i = 0; i < colon; i++) if (!isdigit((unsigned char)s[i])) return false;
    for (int i = colon + 1; i < sec_end; i++) if (!isdigit((unsigned char)s[i])) return false;
    if (dot > 0) for (int i = dot + 1; i < (int)s.length(); i++) if (!isdigit((unsigned char)s[i])) return false;

    const long m   = s.substring(0, colon).toInt();
    const long sec = s.substring(colon + 1, sec_end).toInt();
    long frac_ms = 0;
    if (dot > 0) {
        const String f = s.substring(dot + 1);
        const long v = f.toInt();
        if (f.length() == 1)      frac_ms = v * 100;  // .x → 十分秒
        else if (f.length() == 2) frac_ms = v * 10;   // .xx → 百分秒
        else                      frac_ms = v;        // .xxx 及以上 → 毫秒
    }
    ms = (uint32_t)(m * 60000L + sec * 1000L + frac_ms);
    return true;
}

// 把 LRC 文本转成同步歌词列表。
// TXXX "LYRICS" / USLT 帧里常见这种形式（Windows 播放器能识别的歌词就是它），
// 而 SYLT 是另一种"正文+独立时间戳"的二进制格式，由 parse_sylt 处理。
// 行格式：可带多个时间戳前缀 [mm:ss.xx][mm:ss.xx]歌词；[ti:]/[ar:]/[by:] 等元数据行跳过。
void parse_lrc(const String &lrc, Tag &tag) {
    long offset_ms = 0;   // [offset:+500] 整体平移（毫秒），有则应用
    int p = lrc.indexOf("[offset:");
    if (p >= 0) {
        const int e = lrc.indexOf(']', p);
        if (e > p) offset_ms = lrc.substring(p + 8, e).toInt();
    }

    int pos = 0;
    const int len = lrc.length();
    while (pos < len) {
        int eol = lrc.indexOf('\n', pos);
        if (eol < 0) eol = len;

        uint32_t stamp[8];
        int ns = 0;
        int scan = pos;
        bool skip_line = false;
        while (scan < eol && lrc.charAt(scan) == '[') {
            const int close = lrc.indexOf(']', scan);
            if (close < 0 || close >= eol) { skip_line = true; break; }
            uint32_t ms;
            if (lrc_time(lrc.substring(scan + 1, close), ms)) {
                if (ns < (int)(sizeof(stamp) / sizeof(stamp[0]))) stamp[ns++] = ms;
                scan = close + 1;
            } else {
                skip_line = true;   // [ti:...] 等元数据行
                break;
            }
        }
        pos = eol + 1;
        if (skip_line || ns == 0) continue;

        String text = lrc.substring(scan, eol);
        text.trim();
        if (!text.isEmpty() && text[text.length() - 1] == '\r') text.remove(text.length() - 1);
        text.trim();
        if (text.isEmpty()) continue;

        for (int k = 0; k < ns; k++) {
            LyricLine line;
            const long t = (long)stamp[k] + offset_ms;
            line.time_ms = (t > 0) ? (uint32_t)t : 0;   // offset 为负时防下溢
            line.text = text;
            tag.lyrics.push_back(line);
        }
    }
}

}  // namespace

void Tag::clear() {
    valid = false;
    version = 0;
    title = "";
    artist = "";
    album = "";
    cover.offset = 0;
    cover.length = 0;
    cover.type = 0;
    cover.mime[0] = 0;
    lyrics.clear();
    plain_lyrics = "";
}

int Tag::line_at(uint32_t time_ms) const {
    int lo = 0;
    int hi = (int)lyrics.size() - 1;
    int res = -1;
    while (lo <= hi) {
        const int mid = (lo + hi) / 2;
        if (lyrics[mid].time_ms <= time_ms) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

bool parse(File &file, Tag &tag) {
    tag.clear();
    if (!file) return false;
    if (!file.seek(0)) return false;

    uint8_t hdr[10];
    if (file.read(hdr, 10) != 10) return false;
    if (memcmp(hdr, "ID3", 3) != 0) return false;

    const uint8_t major = hdr[3];
    if (major < 2 || major > 4) return false;

    const uint32_t tag_size = synchsafe32(hdr + 6);
    if (tag_size < 10) return false;
    const uint32_t tag_end = 10 + tag_size;

    const uint8_t id_len = (major == 2) ? 3 : 4;
    const uint8_t fh_len = (major == 2) ? 6 : 10;

    tag.valid = true;
    tag.version = major;

    uint32_t pos = 10;
    // 扩展头
    if (major >= 3 && (hdr[5] & 0x40)) {
        uint8_t ext[4];
        if (file.seek(10) && file.read(ext, 4) == 4) {
            const uint32_t ext_size = (major == 4) ? synchsafe32(ext) : (be32(ext) + 4);
            if (ext_size >= 6 && (10 + ext_size) < tag_end) pos = 10 + ext_size;
        }
    }

    // 注意：标签级"去同步"(hdr[5] & 0x80) 未处理，这类文件极少见，帧数据里插入的 0x00 会让偏移失真。
    while (pos + fh_len <= tag_end) {
        uint8_t fh[10];
        if (!file.seek(pos)) break;
        if (file.read(fh, fh_len) != fh_len) break;
        if (fh[0] == 0) break;   // 进入 padding

        bool id_ok = true;
        for (uint8_t k = 0; k < id_len; k++) {
            const char c = (char)fh[k];
            if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
                id_ok = false;
                break;
            }
        }
        if (!id_ok) break;

        uint32_t fsize = 0;
        uint8_t fmt_flags = 0;
        if (major == 2) {
            fsize = ((uint32_t)fh[3] << 16) | ((uint32_t)fh[4] << 8) | (uint32_t)fh[5];
        } else {
            if (major == 4) {
                // 规范是 synchsafe；但不少工具写的是普通大端，字节里出现 >=0x80 时按大端解析
                const bool not_synchsafe = (fh[4] & 0x80) || (fh[5] & 0x80) ||
                                           (fh[6] & 0x80) || (fh[7] & 0x80);
                fsize = not_synchsafe ? be32(fh + 4) : synchsafe32(fh + 4);
            } else {
                fsize = be32(fh + 4);
            }
            fmt_flags = fh[9];
        }

        const uint32_t data_pos = pos + fh_len;
        const uint32_t remain = (tag_end > data_pos) ? (tag_end - data_pos) : 0;
        if (fsize == 0 || fsize > remain) break;   // 帧长非法（或已到尾部 padding）

        // 压缩 / 加密的帧解析不了，跳过
        bool skip = false;
        if (major == 3 && (fmt_flags & 0xC0)) skip = true;
        if (major == 4 && (fmt_flags & 0x0C)) skip = true;

        char id[5] = {0};
        if (major == 2) {
            if (!memcmp(fh, "TT2", 3)) strcpy(id, "TIT2");
            else if (!memcmp(fh, "TP1", 3)) strcpy(id, "TPE1");
            else if (!memcmp(fh, "TAL", 3)) strcpy(id, "TALB");
            else if (!memcmp(fh, "SLT", 3)) strcpy(id, "SYLT");
            else if (!memcmp(fh, "ULT", 3)) strcpy(id, "USLT");
            else if (!memcmp(fh, "TXX", 3)) strcpy(id, "TXXX");
            else if (!memcmp(fh, "PIC", 3)) strcpy(id, "APIC");
        } else {
            memcpy(id, fh, 4);
        }

        // v2.4 的 data length indicator：正文前 4 字节是长度
        uint32_t body_pos = data_pos;
        uint32_t body_len = fsize;
        if (major == 4 && (fmt_flags & 0x01) && body_len > 4) {
            body_pos += 4;
            body_len -= 4;
        }

        if (!skip) {
            if (!strcmp(id, "TIT2") || !strcmp(id, "TPE1") || !strcmp(id, "TALB")) {
                uint8_t buf[kTextFrameMax];
                const uint32_t n = read_body(file, body_pos, body_len, buf, sizeof(buf));
                String text;
                if (n > 1) decode_string(buf[0], buf + 1, n - 1, text);
                text.trim();
                if (!strcmp(id, "TIT2")) tag.title = text;
                else if (!strcmp(id, "TPE1")) tag.artist = text;
                else tag.album = text;
            } else if (!strcmp(id, "SYLT") || !strcmp(id, "USLT") || !strcmp(id, "TXXX")) {
                if (body_len <= kLyricFrameMax) {
                    uint8_t *buf = lyric_frame_buf();
                    if (buf != nullptr) {
                        const uint32_t n = read_body(file, body_pos, body_len, buf, body_len);
                        if (n > 6) {
                            if (!strcmp(id, "SYLT")) parse_sylt(buf, n, tag);
                            else if (!strcmp(id, "USLT")) parse_uslt(buf, n, tag);
                            else parse_txxx(buf, n, tag);
                        }
                    }
                }
            } else if (!strcmp(id, "APIC") && !tag.has_cover()) {
                parse_apic(file, body_pos, body_len, major == 2, tag);
            }
        }

        pos = data_pos + fsize;
    }

    if (!tag.lyrics.empty()) {
        std::stable_sort(tag.lyrics.begin(), tag.lyrics.end(),
                         [](const LyricLine &a, const LyricLine &b) { return a.time_ms < b.time_ms; });
    }
    // 没有 SYLT 同步歌词时，若 TXXX "LYRICS"/USLT 存的是 LRC 文本（带 [mm:ss.xx] 时间戳，
    // Windows 播放器显示的就是它），现场转成同步歌词，让这类文件也能逐行显示。
    if (tag.lyrics.empty() && tag.plain_lyrics.length() > 8 &&
        tag.plain_lyrics.indexOf('[') >= 0) {
        parse_lrc(tag.plain_lyrics, tag);
        if (!tag.lyrics.empty()) {
            std::stable_sort(tag.lyrics.begin(), tag.lyrics.end(),
                             [](const LyricLine &a, const LyricLine &b) { return a.time_ms < b.time_ms; });
            printf("[id3] LRC->synced: %u 行\n", (unsigned)tag.lyrics.size());
        }
    }
    return true;
}

bool parse_file(fs::FS &fs, const char *path, Tag &tag) {
    tag.clear();
    File f = fs.open(path, FILE_READ);
    if (!f) {
        printf("[id3] open fail: %s\n", path);
        return false;
    }
    const bool ok = parse(f, tag);
    if (!ok) printf("[id3] no ID3v2 header: %s\n", path);
    f.close();
    return ok;
}

size_t read_cover(File &file, const Tag &tag, uint8_t *buf, size_t size) {
    if (!tag.has_cover() || buf == nullptr || size == 0) return 0;
    const size_t want = (tag.cover.length < size) ? tag.cover.length : size;
    if (!file.seek(tag.cover.offset)) return 0;
    return file.read(buf, want);
}

}  // namespace id3
