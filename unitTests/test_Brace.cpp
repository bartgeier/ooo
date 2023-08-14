#include "gtest/gtest.h"
#include <cstddef>
#include "Brace_functions.h"
#include "Brace_types.h"

using namespace BRACE;


static struct Flag {
        States state;
        BraceStack *stack;
        size_t idx;
        bool open;
        bool close;
        bool complete;
        char move;
        struct s1 {
                size_t idx;
                char brace_char;
        } applyInside;
        struct s2 {
                size_t idx;
                char brace_char;
        } applyOutside;
        bool applyEndofLine;
} flag;

/* stubs */
void Brace_action_open(BRACE::States const state, size_t const idx, BraceStack &stack) {
        flag.state = state;
        flag.idx = idx;
        flag.stack = &stack;
        flag.open = true;
}

void Brace_action_close(BraceStack &stack) {
        flag.close = true;
}

void Brace_action_complete(BraceStack &stack) {
        flag.complete = true;
}

void Brace_action_applyChar(char chr, std::string &copy) {
        flag.move = chr;
}

void Brace_action_applyInside(char const brace_char, size_t const chr_idx, Gold &gold) {
        flag.applyInside.brace_char = brace_char;
        flag.applyInside.idx = chr_idx;
}

void Brace_action_applyOutside(char const brace_char, size_t const chr_idx, Gold &gold) {
        flag.applyOutside.brace_char = brace_char;
        flag.applyOutside.idx = chr_idx;
}

void Brace_action_applyEndOfLine(Gold &gold) {
        flag.applyEndofLine = true;
}


TEST(Brace, reset) {
        Brace x;
        x.state = INSIDE;
        Brace_reset(x);
        EXPECT_EQ(x.state, OUTSIDE);

        x.state = OUTSIDE;
        Brace_reset(x);
        EXPECT_EQ(x.state, OUTSIDE);

        x.state = TERMINATOR;
        Brace_reset(x);
        EXPECT_EQ(x.state, OUTSIDE);
}

TEST(Brace, open) {
        BraceStack stack;
        Brace x;
        x.state = INIT;
        x.chr_idx = 99; 
        flag.open = false;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_FALSE(flag.open);

        x.state = IDLE ;
        x.chr_idx = 99; 
        flag.open = false;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_FALSE(flag.open);

        x.state = FIRST;
        x.chr_idx = 99; 
        flag.open = false;
        flag.state = INIT;
        flag.idx = 3333;
        flag.stack = nullptr;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_TRUE(flag.open);
        EXPECT_EQ(flag.state, LAST);
        EXPECT_EQ(flag.idx, 5);
        EXPECT_EQ(flag.stack, &stack);

        x.state = NOT_FIRST;
        x.chr_idx = 99; 
        flag.open = false;
        flag.state = INIT;
        flag.idx = 3333;
        flag.stack = nullptr;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_TRUE(flag.open);
        EXPECT_EQ(flag.state, LAST);
        EXPECT_EQ(flag.idx, 5);
        EXPECT_EQ(flag.stack, &stack);

        x.state = LAST;
        x.chr_idx = 99; 
        flag.open = false;
        flag.state = INIT;
        flag.idx = 3333;
        flag.stack = nullptr;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_TRUE(flag.open);
        EXPECT_EQ(flag.state, LAST);
        EXPECT_EQ(flag.idx, 5);
        EXPECT_EQ(flag.stack, &stack);
}

TEST(Brace, close) {
        BraceStack stack;
        Brace x;
        flag.close = false;
        x.state = OUTSIDE;
        x.chr_idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_TRUE(flag.close);

        flag.complete = false;
        x.state = INSIDE;
        x.chr_idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_TRUE(flag.complete);
}

TEST(Brace, endOfLine) {
        Brace x;
        x.state = OUTSIDE;
        x.chr_idx = 99; 
        Brace_event_endOfLine(x, 5);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.chr_idx, 5);

        x.state = INSIDE;
        x.chr_idx = 999; 
        Brace_event_endOfLine(x, 7);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.chr_idx, 7);
}

TEST(Brace, move) {
        std::string copy;
        Brace x;
        x.state = INSIDE;
        flag.move = '0';
        Brace_event_applyChar(x, 'X', copy);
        EXPECT_EQ(x.state, INSIDE);
        EXPECT_EQ(flag.move, 'X');

        x.state = OUTSIDE;
        flag.move = '0';
        Brace_event_applyChar(x, 'Y', copy);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(flag.move, 'Y');

        x.state = TERMINATOR;
        flag.move = '0';
        Brace_event_applyChar(x, 'Z', copy);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(flag.move, 'Z');
}

TEST(Brace, apply) {
        Gold gold;
        Brace x;
        x.state = INSIDE;
        x.chr_idx = 1;
        flag.applyInside.idx = 0;
        flag.applyInside.brace_char = '0';
        Brace_event_apply(x, "{}", gold);
        EXPECT_EQ(x.state, INSIDE);
        EXPECT_EQ(flag.applyInside.idx, 1);
        EXPECT_EQ(flag.applyInside.brace_char, '{');

        x.state = OUTSIDE;
        x.chr_idx = 2;
        flag.applyOutside.idx = 0;
        flag.applyInside.brace_char = '0';
        Brace_event_apply(x, "()", gold);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(flag.applyOutside.idx, 2);
        EXPECT_EQ(flag.applyOutside.brace_char, ')');

        x.state = TERMINATOR;
        x.chr_idx = 3;
        flag.applyEndofLine = false;
        flag.applyInside.brace_char = '0';
        Brace_event_apply(x, "{}", gold);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_TRUE(flag.applyEndofLine);
}
