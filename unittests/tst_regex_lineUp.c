#include "gtest/gtest.h"
#define REGEX_LINE_UP_IMPLEMENTATION
#include "Regex_lineUp.h"

TEST(regex_lineUp, first) {
        Regex_lineUp_t regex;
        Regex_lineUp_first(&regex, 0);
        EXPECT_EQ(regex.column, (uint32_t)0);
        EXPECT_EQ(regex.state, RLU_LF);
}

TEST(regex_lineUp, lineUp) {
        Regex_lineUp_t regex;
        Regex_lineUp_first(&regex, 0);
        uint32_t idx = 0;

        EXPECT_FALSE(Regex_lineUp(&regex, 'd', idx++));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.column, (uint32_t)1);

        EXPECT_FALSE(Regex_lineUp(&regex, '\n', idx++));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.column, (uint32_t)0);

        EXPECT_FALSE(Regex_lineUp(&regex, '\\', idx++));
        EXPECT_EQ(regex.state, RLU_FIRST);
        EXPECT_EQ(regex.column, (uint32_t)1);

        EXPECT_FALSE(Regex_lineUp(&regex, 'g', idx++));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.column, (uint32_t)2);

        EXPECT_FALSE(Regex_lineUp(&regex, '\\', idx++));
        EXPECT_EQ(regex.state, RLU_FIRST);
        EXPECT_EQ(regex.column, (uint32_t)3);

        EXPECT_EQ(idx, (uint32_t)5);
        EXPECT_FALSE(Regex_lineUp(&regex, '\n', idx++));
        EXPECT_EQ(regex.state, RLU_LINE_CONT);
        EXPECT_EQ(regex.begin, (uint32_t)2);
        EXPECT_EQ(regex.max_col, (uint32_t)2);
        EXPECT_EQ(regex.column, (uint32_t)0);

        EXPECT_FALSE(Regex_lineUp(&regex, 'h', idx++));
        EXPECT_EQ(regex.state, RLU_LINE_CONT);
        EXPECT_EQ(regex.column, (uint32_t)1);

        EXPECT_FALSE(Regex_lineUp(&regex, 't', idx++));
        EXPECT_EQ(regex.state, RLU_LINE_CONT);
        EXPECT_EQ(regex.column, (uint32_t)2);

        EXPECT_EQ(idx, (uint32_t)8);
        EXPECT_TRUE(Regex_lineUp(&regex, '\n', idx++));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)2);
        EXPECT_EQ(regex.end, (uint32_t)9);
        EXPECT_EQ(regex.max_col, (uint32_t)2);
        EXPECT_EQ(regex.column, (uint32_t)0);

        EXPECT_FALSE(Regex_lineUp(&regex, '\\', idx++));
        EXPECT_EQ(regex.state, RLU_FIRST);
        EXPECT_EQ(regex.column, (uint32_t)1);
        
        EXPECT_EQ(idx, (uint32_t)10);
        EXPECT_FALSE(Regex_lineUp(&regex, '\n', idx++));
        EXPECT_EQ(regex.state, RLU_LINE_CONT);
        EXPECT_EQ(regex.begin, (uint32_t)9);
        EXPECT_EQ(regex.max_col, (uint32_t)0);
        EXPECT_EQ(regex.column, (uint32_t)0);

        EXPECT_EQ(idx, (uint32_t)11);
        EXPECT_TRUE(Regex_lineUp(&regex, '\n', idx++));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)9);
        EXPECT_EQ(regex.end, (uint32_t)12);
        EXPECT_EQ(regex.max_col, (uint32_t)0);
        EXPECT_EQ(regex.column, (uint32_t)0);
}

TEST(regex_lineUp, lineUp_end_of_string) {
        Regex_lineUp_t regex;
        Regex_lineUp_first(&regex, 0);
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)0);
        EXPECT_EQ(regex.end, (uint32_t)0);
        EXPECT_EQ(regex.max_col, (uint32_t)0);
        EXPECT_EQ(regex.column, (uint32_t)0);

        uint32_t idx = 3;
        regex.state = RLU_LF;
        regex.column = 11;
        EXPECT_FALSE(Regex_lineUp(&regex, 0, idx));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)0);
        EXPECT_EQ(regex.end, (uint32_t)0);
        EXPECT_EQ(regex.max_col, (uint32_t)0);
        EXPECT_EQ(regex.column, (uint32_t)0);

        regex.state = RLU_FIRST;
        regex.column = 11;
        EXPECT_FALSE(Regex_lineUp(&regex, 0, idx));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)0);
        EXPECT_EQ(regex.end, (uint32_t)0);
        EXPECT_EQ(regex.max_col, (uint32_t)0);
        EXPECT_EQ(regex.column, (uint32_t)0);

        regex.state = RLU_LINE_CONT;
        regex.column = 11;
        EXPECT_TRUE(Regex_lineUp(&regex, 0, idx));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)0);
        EXPECT_EQ(regex.end, (uint32_t)4);
        EXPECT_EQ(regex.max_col, (uint32_t)0);
        EXPECT_EQ(regex.column, (uint32_t)0);
        
        regex.state = RLU_BACK_SLASH;
        regex.column = 11;
        EXPECT_TRUE(Regex_lineUp(&regex, 0, idx));
        EXPECT_EQ(regex.state, RLU_LF);
        EXPECT_EQ(regex.begin, (uint32_t)0);
        EXPECT_EQ(regex.end, (uint32_t)4);
        EXPECT_EQ(regex.max_col, (uint32_t)10);
        EXPECT_EQ(regex.column, (uint32_t)0);
}

