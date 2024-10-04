#include "gtest/gtest.h"

#define REGEX_TRUNC_NODE_SPACE_IMPLEMENTATION
#include "Regex_truncNodeSpace.h"

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

TEST(Regex_truncNodeSpace_t, first) {
        Regex_truncNodeSpace_t regex;
        Regex_truncNodeSpace_first(&regex, 5);
        EXPECT_EQ(regex.state, TNS_IDLE);
        EXPECT_EQ(regex.column, (uint32_t)5);
}

TEST(Regex_truncNodeSpace_t, tail_space_indentation) {
        char result[100] = {0};
        Regex_truncNodeSpace_t regex;
        Regex_truncNodeSpace_first(&regex, 5);

        char const *s0 = "x\nabc   \n   def";
        //                      ^       ^
        // NodeSpace            5       12
        // column         0  0  3       3
        EXPECT_EQ(s0[4], 'c');
        EXPECT_EQ(s0[12], 'd');
        uint32_t n = 5;
        strncpy(result, s0, n);
        EXPECT_TRUE(strncmp(result, "x\nabc", n) == 0);

        uint32_t const begin = 5;
        uint32_t const end = 12;
        uint32_t const column = fn_column(s0, begin);
        EXPECT_EQ(column, (uint32_t)3); 

        Regex_truncNodeSpace_first(&regex, column);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_truncNodeSpace(&regex, s0[i]);
                result[n] = s0[i];
                n++;
        }
        if (strlen(s0) > end) {
                n -= Regex_truncNodeSpace(&regex, s0[end]);
        } else {
                n -= Regex_truncNodeSpace(&regex, 0);
        }
        char const *s1 = "x\nabc\ndef";
        EXPECT_TRUE(strncmp(result, s1, n) == 0);
}

TEST(Regex_truncNodeSpace_t, indentation_tail_space) {
        char result[100] = {0};
        Regex_truncNodeSpace_t regex;
        Regex_truncNodeSpace_first(&regex, 5);

        char const *s0 = "x\n   abc   \n   def";
        //                ^                   ^
        // NodeSpace      0                   20 

        uint32_t const begin = 0;
        uint32_t const end = strlen(s0);
        uint32_t const column = fn_column(s0, begin);
        EXPECT_EQ(column, (uint32_t)0); 

        uint32_t n = 0;
        Regex_truncNodeSpace_first(&regex, column);
        for (uint32_t i = begin; i < end; i++) {
                n -= Regex_truncNodeSpace(&regex, s0[i]);
                result[n] = s0[i];
                n++;
        }
        if (strlen(s0) > end) {
                n -= Regex_truncNodeSpace(&regex, s0[end]);
        } else {
                n -= Regex_truncNodeSpace(&regex, 0);
        }
        char const *s1 = "x\nabc\ndef";
        EXPECT_TRUE(strncmp(result, s1, n) == 0);
}

