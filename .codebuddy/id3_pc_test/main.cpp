#include "id3_parser.h"
#include <cstdio>

int main(int argc, char **argv) {
    for (int k = 1; k < argc; k++) {
        id3::Tag tag;
        bool ok = id3::parse_file(SD_MMC, argv[k], tag);
        printf("== %s\n", argv[k]);
        printf("   valid=%d version=%d title=[%s] artist=[%s] album=[%s]\n",
               (int)tag.valid, (int)tag.version,
               tag.title.c_str(), tag.artist.c_str(), tag.album.c_str());
        printf("   cover=%s off=%u len=%u type=%d\n", tag.cover.mime,
               tag.cover.offset, tag.cover.length, (int)tag.cover.type);
        printf("   lyrics=%u plain=%u\n", (unsigned)tag.lyrics.size(),
               (unsigned)tag.plain_lyrics.length());
        for (size_t i = 0; i < tag.lyrics.size() && i < 5; i++)
            printf("     [%2u] %8u ms  %s\n", (unsigned)i, tag.lyrics[i].time_ms,
                   tag.lyrics[i].text.c_str());
        if (tag.lyrics.size() >= 5)
            printf("     [%2u] %8u ms  %s\n", (unsigned)(tag.lyrics.size() - 1),
                   tag.lyrics[tag.lyrics.size() - 1].time_ms,
                   tag.lyrics[tag.lyrics.size() - 1].text.c_str());
        if (!tag.lyrics.empty()) {
            uint32_t last = tag.lyrics.back().time_ms;
            printf("   line_at(0)=%d line_at(5000)=%d line_at(30000)=%d line_at(last+1)=%d\n",
                   tag.line_at(0), tag.line_at(5000), tag.line_at(30000),
                   tag.line_at(last + 1));
            bool mono = true;
            for (size_t i = 1; i < tag.lyrics.size(); i++)
                if (tag.lyrics[i].time_ms < tag.lyrics[i - 1].time_ms) mono = false;
            printf("   ts monotonic=%d first=%u last=%u\n", (int)mono,
                   tag.lyrics.front().time_ms, last);
        }
        (void)ok;
    }
    return 0;
}
