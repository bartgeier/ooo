#include "gtest/gtest.h"
#include <cstddef>
#include "Brace.h"

using namespace BRACE;


static struct Flag {
        bool open;
        bool close;
        bool complete;
        bool endOfLine;
        char move;
        size_t applyInside;
        size_t applyOutside;
        size_t applyEndofLine;
} flag;

/* stubs */
void Brace_action_open() {
        flag.open = true;
}

void Brace_action_close() {
        flag.close = true;
}

void Brace_action_complete() {
        flag.complete = true;
}

void Brace_action_endOfLine() {
        flag.endOfLine = true;
}

void Brace_action_move(char chr) {
        flag.move = chr;
}

void Brace_action_applyInside(Brace &brace) {
        flag.applyInside = brace.txt_idx;
}

void Brace_action_applyOutside(Brace &brace) {
        flag.applyOutside = brace.txt_idx;
}

void Brace_action_applyEndOfLine(Brace &brace) {
        flag.applyEndofLine = brace.txt_idx;
}


TEST(Brace, reset) {
        BRACE::Brace x;
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
        BRACE::Brace x;
        flag.open = false;
        x.state = OUTSIDE;
        x.txt_idx = 99; 
        Brace_event_open(x, 5);
        EXPECT_EQ(x.state, INSIDE);
        EXPECT_EQ(x.txt_idx, 5);
        EXPECT_TRUE(flag.open);

        flag.open = false;
        x.state = INSIDE;
        x.txt_idx = 99; 
        Brace_event_open(x, 5);
        EXPECT_EQ(x.state, INSIDE);
        EXPECT_EQ(x.txt_idx, 5);
        EXPECT_TRUE(flag.open);
}

TEST(Brace, close) {
        BRACE::Brace x;
        flag.close = false;
        x.state = OUTSIDE;
        x.txt_idx = 99; 
        Brace_event_close(x, 5);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(x.txt_idx, 5);
        EXPECT_TRUE(flag.close);

        flag.complete = false;
        x.state = INSIDE;
        x.txt_idx = 99; 
        Brace_event_close(x, 5);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(x.txt_idx, 5);
        EXPECT_TRUE(flag.complete);
}

TEST(Brace, endOfLine) {
        BRACE::Brace x;
        flag.endOfLine = false;
        x.state = OUTSIDE;
        x.txt_idx = 99; 
        Brace_event_endOfLine(x, 5);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.txt_idx, 5);
        EXPECT_TRUE(flag.endOfLine);

        flag.endOfLine = false;
        x.state = INSIDE;
        x.txt_idx = 99; 
        Brace_event_endOfLine(x, 7);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.txt_idx, 7);
        EXPECT_TRUE(flag.endOfLine);
}

TEST(Brace, move) {
        BRACE::Brace x;
        x.state = INSIDE;
        flag.move = '0';
        Brace_event_move(x, 'X');
        EXPECT_EQ(x.state, INSIDE);
        EXPECT_EQ(flag.move, 'X');

        x.state = OUTSIDE;
        flag.move = '0';
        Brace_event_move(x, 'Y');
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(flag.move, 'Y');

        x.state = TERMINATOR;
        flag.move = '0';
        Brace_event_move(x, 'Z');
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(flag.move, 'Z');
}

TEST(Brace, apply) {
        BRACE::Brace x;
        x.state = INSIDE;
        x.txt_idx = 1;
        flag.applyInside = 0;
        Brace_event_apply(x);
        EXPECT_EQ(x.state, INSIDE);
        EXPECT_EQ(flag.applyInside, 1);

        x.state = OUTSIDE;
        x.txt_idx = 2;
        flag.applyOutside = 0;
        Brace_event_apply(x);
        EXPECT_EQ(x.state, OUTSIDE);
        EXPECT_EQ(flag.applyOutside, 2);

        x.state = TERMINATOR;
        x.txt_idx = 3;
        flag.applyEndofLine = 0;
        Brace_event_apply(x);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(flag.applyEndofLine, 3);
}