#include "gtest/gtest.h"
#include <stdlib.h>
#define OSTR_IMPLEMENTAION
#include "OStr.h"


#define MEM_SIZE 1000*1024

TEST(OStr, NewLine_with_LineFeed_rn) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\r\nIs there something.\r\nHello Sailor! Where are you coming frome.\r\nOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        snk[0] = 0;
        OStr sink_str = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };
        char kind_of_linefeed = OStr_set_NewLine_with_LineFeed(&sink_str, &source_str);
        EXPECT_EQ(kind_of_linefeed, 'R');
        EXPECT_EQ(source_str.size, (size_t)0);
        EXPECT_EQ(source_str.at[0], (char)0);
        EXPECT_EQ(sink_str.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink_str.size);
        bool ok = true;
        for (size_t i = 0; i < strlen(b); i++) {
                ok &= sink_str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);

        OStr_replace_LineFeed(&source_str, &sink_str, kind_of_linefeed);
        EXPECT_EQ(source_str.size, (size_t)113);
        EXPECT_EQ(sink_str.size, (size_t)0);
        EXPECT_EQ(sink_str.at[0], (char)0);
        EXPECT_EQ(strlen(a), source_str.size);
        for (size_t i = 0; i < strlen(a); i++) {
                ok &= source_str.at[i] == a[i];
        }
        EXPECT_TRUE(ok);
}

TEST(OStr, NewLine_with_LineFeed_r) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\rIs there something.\rHello Sailor! Where are you coming frome.\rOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        snk[0] = 0;
        OStr sink_str = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };
        char kind_of_linefeed = OStr_set_NewLine_with_LineFeed(&sink_str, &source_str);
        EXPECT_EQ(kind_of_linefeed, 'r');
        EXPECT_EQ(source_str.size, (size_t)0);
        EXPECT_EQ(source_str.at[0], (char)0);
        EXPECT_EQ(sink_str.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink_str.size);
        bool ok = true;
        for (size_t i = 0; i < strlen(b); i++) {
                ok &= sink_str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);

        OStr_replace_LineFeed(&source_str, &sink_str, kind_of_linefeed);
        EXPECT_EQ(source_str.size, (size_t)110);
        EXPECT_EQ(sink_str.size, (size_t)0);
        EXPECT_EQ(sink_str.at[0], (char)0);
        EXPECT_EQ(strlen(a), source_str.size);
        for (size_t i = 0; i < strlen(a); i++) {
                ok &= source_str.at[i] == a[i];
        }
        EXPECT_TRUE(ok);
}

TEST(OStr, NewLine_with_LineFeed_nr) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\n\rIs there something.\n\rHello Sailor! Where are you coming frome.\n\rOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        snk[0] = 0;
        OStr sink_str = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };
        char kind_of_linefeed = OStr_set_NewLine_with_LineFeed(&sink_str, &source_str);
        EXPECT_EQ(kind_of_linefeed, 'N');
        EXPECT_EQ(source_str.size, (size_t)0);
        EXPECT_EQ(source_str.at[0], (char)0);
        EXPECT_EQ(sink_str.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink_str.size);
        bool ok = true;
        for (size_t i = 0; i < strlen(b); i++) {
                ok &= sink_str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);

        OStr_replace_LineFeed(&source_str, &sink_str, kind_of_linefeed);
        EXPECT_EQ(source_str.size, (size_t)113);
        EXPECT_EQ(sink_str.size, (size_t)0);
        EXPECT_EQ(sink_str.at[0], (char)0);
        EXPECT_EQ(strlen(a), source_str.size);
        for (size_t i = 0; i < strlen(a); i++) {
                ok &= source_str.at[i] == a[i];
        }
        EXPECT_TRUE(ok);
}

TEST(OStr, NewLine_with_LineFeed_n) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        const char *b = "The quick brown fox jumps over the lazy dog.\nIs there something.\nHello Sailor! Where are you coming frome.\nOk!";
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        snk[0] = 0;
        OStr sink_str = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };
        char kind_of_linefeed = OStr_set_NewLine_with_LineFeed(&sink_str, &source_str);
        EXPECT_EQ(kind_of_linefeed, 'n');
        EXPECT_EQ(source_str.size, (size_t)0);
        EXPECT_EQ(source_str.at[0], (char)0);
        EXPECT_EQ(sink_str.size, (size_t)110);
        EXPECT_EQ(strlen(b), sink_str.size);
        bool ok = true;
        for (size_t i = 0; i < strlen(b); i++) {
                ok &= sink_str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);

        OStr_replace_LineFeed(&source_str, &sink_str, kind_of_linefeed);
        EXPECT_EQ(source_str.size, (size_t)110);
        EXPECT_EQ(sink_str.size, (size_t)0);
        EXPECT_EQ(sink_str.at[0], (char)0);
        EXPECT_EQ(strlen(a), source_str.size);
        for (size_t i = 0; i < strlen(a); i++) {
                ok &= source_str.at[i] == a[i];
        }
        EXPECT_TRUE(ok);
}

TEST(OStr, replace_tabs_with_one_space) {
        char *src = (char*)malloc(MEM_SIZE);
        char *snk = (char*)malloc(MEM_SIZE);
        const char *a = "\tThe quick\t\tbrown\tfox\t\t\t\tjumps\tover the\tlazy\tdog.\t\t";
        const char *b = " The quick brown fox jumps over the lazy dog. ";
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        snk[0] = 0;
        OStr sink_str = {
                .capacity = MEM_SIZE,
                .size = 0,
                .at = snk
        };
        OStr_replace_tabs_with_one_space(&sink_str, &source_str);
        EXPECT_EQ(source_str.size, (size_t)0);
        EXPECT_EQ(source_str.at[0], (char)0);
        EXPECT_EQ(strlen(b), sink_str.size);
        bool ok = true;
        for (size_t i = 0; i < sink_str.size; i++) {
                ok &= sink_str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);
}

TEST(OStr, append) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The quick brown fox jumps over the lazy dog.";
        const char *b = "The quick brown fox jumps over the lazy dog.   ";
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        EXPECT_EQ(source_str.size, strlen(a));
        OStr_append_spaces(&source_str, 3);
        EXPECT_EQ(source_str.size, strlen(b));
        bool ok = true;
        for (size_t i = 0; i < source_str.size; i++) {
                ok &= source_str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);
}

TEST(OStr, at_least_1_not_3__zero) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "Thequick  ";
        //                  ^
        //                 3 3
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        size_t x = OStr_at_least_1_not_3(&source_str, 3, 3, ' ');
        EXPECT_EQ(x, (size_t)1);
}

TEST(OStr, at_least_1_not_3__one) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The quick  ";
        //                  ^^
        //                  34
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        size_t x = OStr_at_least_1_not_3(&source_str, 3, 4, ' ');
        EXPECT_EQ(x, (size_t)1);
}

TEST(OStr, at_least_1_not_3__two) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The  quick  ";
        //                  ^ ^
        //                  3 5
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        size_t x = OStr_at_least_1_not_3(&source_str, 3, 5, ' ');
        EXPECT_EQ(x, (size_t)2);
}

TEST(OStr, at_least_1_not_3__three) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The   quick  ";
        //                  ^  ^
        //                  3  6
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        size_t x = OStr_at_least_1_not_3(&source_str, 3, 6, ' ');
        EXPECT_EQ(x, (size_t)2);
}

TEST(OStr, at_least_1__zero) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "Thequick  ";
        //                  ^ 
        //                 3 3
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        size_t x = OStr_at_least_1(&source_str, 3, 3, ' ');
        EXPECT_EQ(x, (size_t)1);
}

TEST(OStr, at_least_1__one) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The   quick  ";
        //                  ^  ^
        //                  3  6
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = strlen(a),
                .at = src
        };
        size_t x = OStr_at_least_1(&source_str, 3, 6, ' ');
        EXPECT_EQ(x, (size_t)3);
}
