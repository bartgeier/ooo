#include "gtest/gtest.h"

#define REGEX_TRUNC_COMMENT_SPACE_IMPLEMENTATION
#include "Regex_truncCommentSpace.h"

/* \n is the last character in a row */
/* returns the column idx            */
static uint32_t fn_column(char const *str, uint32_t idx) {
        uint32_t i = idx;
        while (i > 0) {
                i--;
                if (str[i] == '\n') {
                        break;
                }
        }
        uint32_t result = (i > 0) ? idx - i - 1 : idx;
        return result;
}


TEST(Regex_truncCommentSpace, first) {
        Regex_truncCommentSpace_t regex;
        Regex_truncCommentSpace_first(&regex, 5);
        EXPECT_EQ(regex.state, TCS_IDLE);
        EXPECT_EQ(regex.column, (uint32_t)5);
}

TEST(Regex_truncCommentSpace, trunc_0) {
        char result[100] = {0};
        Regex_truncCommentSpace_t regex;
        char const *s0 = "xx\n   /* n\nabc   \n   def */";
        uint32_t const begin = 6;
        uint32_t const end = 30;
        EXPECT_EQ(s0[begin], '/');
        EXPECT_EQ(s0[end - 1], '/');
        uint32_t const column_idx = fn_column(s0, begin);
        EXPECT_EQ(column_idx, (uint32_t)3);
        Regex_truncCommentSpace_first(&regex, column_idx);
        strncpy(result, s0, begin);
        result[begin] = 0;
        uint32_t n = begin;
        for(uint32_t i = begin; i < end; i++) {
                n -= Regex_truncCommentSpace(&regex, s0[i]);
                result[n++] = s0[i];
        }
        result[end] = 0;
        char const *s1 = "xx\n   /* n\nabc\ndef */";
        EXPECT_TRUE(strncmp(result, s1, n) == 0);
        EXPECT_EQ(n, (uint32_t)24);
}

TEST(Regex_truncCommentSpace, trunc_1) {
        char result[100] = {0};
        Regex_truncCommentSpace_t regex;
        char const *s0 = "/* n\n abc   \n   def */";
        uint32_t const begin = 0;
        uint32_t const end = 22;
        EXPECT_EQ(s0[begin], '/');
        EXPECT_EQ(s0[end - 1], '/');
        uint32_t const column_idx = fn_column(s0, begin);
        EXPECT_EQ(column_idx, (uint32_t)0);
        Regex_truncCommentSpace_first(&regex, column_idx);
        result[begin] = 0;
        uint32_t n = begin;
        for(uint32_t i = begin; i < end; i++) {
                n -= Regex_truncCommentSpace(&regex, s0[i]);
                result[n++] = s0[i];
        }
        result[end] = 0;
        char const *s1 = "/* n\n abc\n   def */";
        EXPECT_TRUE(strncmp(result, s1, n) == 0);
        EXPECT_EQ(n, (uint32_t)19);
}

TEST(Regex_truncCommentSpace, trunc_2) {
        char result[100] = {0};
        Regex_truncCommentSpace_t regex;
        char const *s0 = " /* abc   \n      def */";
        uint32_t const begin = 1;
        uint32_t const end = 23;
        EXPECT_EQ(s0[begin], '/');
        EXPECT_EQ(s0[end - 1], '/');
        uint32_t const column_idx = fn_column(s0, begin);
        EXPECT_EQ(column_idx, (uint32_t)1);
        Regex_truncCommentSpace_first(&regex, column_idx);
        strncpy(result, s0, begin);
        result[begin] = 0;
        uint32_t n = begin;
        for(uint32_t i = begin; i < end; i++) {
                n -= Regex_truncCommentSpace(&regex, s0[i]);
                result[n++] = s0[i];
        }
        result[end] = 0;
        char const *s1 = " /* abc\n     def */";
        EXPECT_TRUE(strncmp(result, s1, n) == 0);
        EXPECT_EQ(n, (uint32_t)19);
}
