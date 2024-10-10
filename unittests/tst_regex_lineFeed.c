#include "gtest/gtest.h"
#include "OStr.h"
#define REGEX_LINE_FEED_IMPLEMENATATION
#include "Regex_lineFeed.h"

#define MEM_SIZE 1000*1024

TEST(Regex_lineFeed, lineFeed_rn) {
        char *src = (char*)calloc(MEM_SIZE, 1);
        char *snk = (char*)calloc(MEM_SIZE, 1);
        const char *a = "The quick brown fox jumps over the lazy dog.\r\nIs there something.\r\nHello Sailor! Where are you coming frome.\r\nOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        strcpy(src, a);
        OStr source = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        OStr sink = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };

        Regex_lineFeed_t regex = { .state = RLF_CHAR, .r = 0, .n = 0, .rn = 0, .nr = 0 };
        for (uint32_t i = 0; i < source.size; i++) {
                OptionalChar_t o = Regex_lineFeed(&regex, source.at[i]);
                if (o.ok) {
                        OStr_append_chr(&sink, o.chr);
                }
        }
        char const kind_of_linefeed = Regex_lineFeed_last(&regex);

        EXPECT_EQ(kind_of_linefeed, 'R');
        EXPECT_EQ(sink.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink.size);
        EXPECT_TRUE(strncmp(b, sink.at, sink.size) == 0);
}

TEST(Regex_lineFeed, lineFeed_r) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\rIs there something.\rHello Sailor! Where are you coming frome.\rOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        strcpy(src, a);
        OStr source = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        OStr sink = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };

        Regex_lineFeed_t regex = { .state = RLF_CHAR, .r = 0, .n = 0, .rn = 0, .nr = 0 };
        for (uint32_t i = 0; i < source.size; i++) {
                OptionalChar_t o = Regex_lineFeed(&regex, source.at[i]);
                if (o.ok) {
                        OStr_append_chr(&sink, o.chr);
                }
        }
        char const kind_of_linefeed = Regex_lineFeed_last(&regex);

        EXPECT_EQ(kind_of_linefeed, 'r');
        EXPECT_EQ(sink.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink.size);
        EXPECT_TRUE(strncmp(b, sink.at, sink.size) == 0);
}

TEST(Regex_lineFeed, lineFeed_nr) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\n\rIs there something.\n\rHello Sailor! Where are you coming frome.\n\rOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        strcpy(src, a);
        OStr source = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        OStr sink = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };

        Regex_lineFeed_t regex = { .state = RLF_CHAR, .r = 0, .n = 0, .rn = 0, .nr = 0 };
        for (uint32_t i = 0; i < source.size; i++) {
                OptionalChar_t o = Regex_lineFeed(&regex, source.at[i]);
                if (o.ok) {
                        OStr_append_chr(&sink, o.chr);
                }
        }
        char const kind_of_linefeed = Regex_lineFeed_last(&regex);

        EXPECT_EQ(kind_of_linefeed, 'N');
        EXPECT_EQ(sink.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink.size);
        EXPECT_TRUE(strncmp(b, sink.at, sink.size) == 0);
}

TEST(Regex_lineFeed, lineFeed_n) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        strcpy(src, a);
        OStr source = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        OStr sink = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };

        Regex_lineFeed_t regex = { .state = RLF_CHAR, .r = 0, .n = 0, .rn = 0, .nr = 0 };
        for (uint32_t i = 0; i < source.size; i++) {
                OptionalChar_t o = Regex_lineFeed(&regex, source.at[i]);
                if (o.ok) {
                        OStr_append_chr(&sink, o.chr);
                }
        }
        char const kind_of_linefeed = Regex_lineFeed_last(&regex);

        EXPECT_EQ(kind_of_linefeed, 'n');
        EXPECT_EQ(sink.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink.size);
        EXPECT_TRUE(strncmp(b, sink.at, sink.size) == 0);
}
