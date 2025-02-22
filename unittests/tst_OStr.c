#include "gtest/gtest.h"
#include <stdlib.h>
#define OSTR_IMPLEMENTAION
#include "OStr.h"


#define MEM_SIZE 1000*1024

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
                .size = (uint32_t)strlen(a),
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

TEST(OStr, OStr_need_1_or_2LF_GOT_ONE) {
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
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 3};
        size_t x = OStr_need_1_or_2LF(&source_str, s);
        EXPECT_EQ(x, (size_t)1);
}

TEST(OStr, OStr_need_1_or_2LF_GOT_NONE) {
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
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 4};
        size_t x = OStr_need_1_or_2LF(&source_str, s);
        EXPECT_EQ(x, (size_t)1);
}

TEST(OStr, OStr_need_1_or_2LF_GOT_TWO) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The\n\n\nquick  ";
        //                  ^     ^
        //                  3     6
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 6};
        size_t x = OStr_need_1_or_2LF(&source_str, s);
        EXPECT_EQ(x, (size_t)2);
}

TEST(OStr, OStr_need_1_or_2LF_GOT_ANOTHER_ONE) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The\n quick  ";
        //                  ^  ^
        //                  3  5
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 5};
        size_t x = OStr_need_1_or_2LF(&source_str, s);
        EXPECT_EQ(x, (size_t)1);
}

TEST(OStr, OStr_need_1_or_2LF_GOT_ANOTHER_ONE_) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The\n \n  quick  ";
        //                  ^      ^
        //                  3      8
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 8};
        size_t x = OStr_need_1_or_2LF(&source_str, s);
        EXPECT_EQ(x, (size_t)2);
}

TEST(OStr, OStr_need_LF_or_space_GOT_LF) {
        char *src = (char*)malloc(MEM_SIZE);
        const char *a = "The\n\n\nquick  ";
        //                  ^     ^
        //                  3     6
        for (size_t i = 0; i < strlen(a); i++) {
                src[i] = a[i];
        } 
        src[strlen(a)] = 0;
        OStr source_str = {
                .capacity = MEM_SIZE,
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 6};
        EXPECT_FALSE(OStr_last_has_LF(&source_str, s));
        char chr = OStr_need_LF_or_space(&source_str, s);
        EXPECT_EQ(chr, (char)'\n');
}

TEST(OStr, OStr_need_LF_or_space_GOT_SPACE) {
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
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 4, .end = 5};
        EXPECT_FALSE(OStr_last_has_LF(&source_str, s));
        char chr = OStr_need_LF_or_space(&source_str, s);
        EXPECT_EQ(chr, (char)' ');
}

TEST(OStr, OStr_need_LF_or_space_GOT_SPACE_) {
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
                .size = (uint32_t)strlen(a),
                .at = src
        };
        Slice s = {.begin = 3, .end = 3};
        EXPECT_FALSE(OStr_last_has_LF(&source_str, s));
        char chr = OStr_need_LF_or_space(&source_str, s);
        EXPECT_EQ(chr, (char)' ');
}

TEST(OStr, OStr_final_truncate) {
        char *src = (char*)malloc(MEM_SIZE);
        char const *a = "Hello World   \n";
        uint32_t a_size = strlen(a);
        char const *b = "Hello World";
        uint32_t b_size = strlen(b);

        for (size_t i = 0; i <= a_size; i++) {
                src[i] = a[i];
        } 
        OStr str = {
                .capacity = MEM_SIZE,
                .size = a_size,
                .at = src
        };
        bool ok = true;
        for (size_t i = 0; i <= str.size; i++) {
                ok &= str.at[i] == a[i];
        }
        EXPECT_TRUE(ok);
        EXPECT_EQ(str.size, a_size);

        OStr_final_truncate(&str);

        EXPECT_EQ(str.size, b_size);
        ok = true;
        for (size_t i = 0; i <= str.size; i++) {
                ok &= str.at[i] == b[i];
        }
        EXPECT_TRUE(ok);


        str.size = 0;
        OStr_final_truncate(&str);
        EXPECT_EQ(str.size, (uint32_t)0);
}
