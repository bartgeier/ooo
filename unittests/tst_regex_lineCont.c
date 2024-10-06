#include "gtest/gtest.h"

#define REGEX_LINE_CONT_IMPLEMENTATION
#include "Regex_lineCont.h"

TEST(Regex_lineCont, first) {
        Regex_lineCont_t regex;
        Regex_lineCont_first(&regex, 'a');
        EXPECT_EQ(regex.state, LCO_CHAR);
        Regex_lineCont_first(&regex, ' ');
        EXPECT_EQ(regex.state, LCO_SPACE);
}

TEST(Regex_lineCont, fn_0) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin\\\n(x);";
        uint32_t const begin = 0;
        uint32_t const end = 8;
        EXPECT_EQ(s0[begin], 's');
        EXPECT_EQ(s0[end], ';');
        uint32_t n = 0;
        Regex_lineCont_first(&regex, 0);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin(x)";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        // printf("fn_0 result: %s\n", result);
}

TEST(Regex_lineCont, fn_1) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin \\\n(x);";
        uint32_t const begin = 0;
        uint32_t const end = 9;
        EXPECT_EQ(s0[begin], 's');
        EXPECT_EQ(s0[end], ';');
        uint32_t n = 0;
        Regex_lineCont_first(&regex, 0);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin \\\n(x)";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        // printf("fn_1 result: %s\n", result);
}

TEST(Regex_lineCont, fn_2) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin\\\n(x);";
        uint32_t const begin = 3;
        uint32_t const end = 5;
        EXPECT_EQ(s0[begin], '\\');
        EXPECT_EQ(s0[end], '(');
        strncpy(result, s0, begin);
        uint32_t n = begin;
        Regex_lineCont_first(&regex, s0[begin - 1]);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        // printf("fn_2 result: %s\n", result);
}

TEST(Regex_lineCont, fn_3) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin\\\n\\\n(x);";
        uint32_t const begin = 3;
        uint32_t const end = 10;
        EXPECT_EQ(s0[begin], '\\');
        EXPECT_EQ(s0[end], ';');
        strncpy(result, s0, begin);
        uint32_t n = begin;
        Regex_lineCont_first(&regex, s0[begin - 1]);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin(x)";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        // printf("fn_3 result: %s\n", result);
}

TEST(Regex_lineCont, fn_4) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin\\\n (x);";
        uint32_t const begin = 3;
        uint32_t const end = 9;
        EXPECT_EQ(s0[begin], '\\');
        EXPECT_EQ(s0[end], ';');
        strncpy(result, s0, begin);
        uint32_t n = begin;
        Regex_lineCont_first(&regex, s0[begin - 1]);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin\\\n (x)";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        // printf("fn_4 result: %s\n", result);
}

TEST(Regex_lineCont, fn_5) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin\\\n\n(x);";
        uint32_t const begin = 3;
        uint32_t const end = 9;
        EXPECT_EQ(s0[begin], '\\');
        EXPECT_EQ(s0[end], ';');
        strncpy(result, s0, begin);
        uint32_t n = begin;
        Regex_lineCont_first(&regex, s0[begin - 1]);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin\\\n\n(x)";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        // printf("fn_5 result: %s\n", result);
}

TEST(Regex_lineCont, fn_6) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = "sin\\\n";
        uint32_t const begin = 3;
        uint32_t const end = 5;
        EXPECT_EQ(s0[begin], '\\');
        EXPECT_EQ(s0[end], 0);
        strncpy(result, s0, begin);
        uint32_t n = begin;
        Regex_lineCont_first(&regex, s0[begin - 1]);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = "sin\\\n";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        //printf("fn_6 result: %s\n", result);
}

TEST(Regex_lineCont, fn_7) {
        Regex_lineCont_t regex;
        char result[100] = {0};
        char const *s0 = " \\\nsin(x);";
        uint32_t const begin = 1;
        uint32_t const end = 3;
        EXPECT_EQ(s0[begin], '\\');
        EXPECT_EQ(s0[end], 's');
        strncpy(result, s0, begin);
        uint32_t n = begin;
        Regex_lineCont_first(&regex, s0[begin - 1]);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_lineCont(&regex, s0[i]);
                result[n++] = s0[i];
        }
        if (strlen(s0) > end) {
                n -= Regex_lineCont(&regex, s0[end]);
        } else {
                n -= Regex_lineCont(&regex, 0);
        }
        result[n] = 0;
        char const *s1 = " \\\n";
        EXPECT_EQ(strlen(s1), n);
        EXPECT_TRUE(strncmp(s1, result, n) == 0);
        //printf("fn_7 result: %s\n", result);
}
