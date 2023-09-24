#include "gtest/gtest.h"
#include <cstddef>
#include "Brace.h"
#include <vector>

using namespace BRACE;

static void stack_reset(std::vector<Gold::Brace> &stack) {
        stack.resize(1);
        stack.back().state = BRACE::INIT;
}

/* stubs */
void Brace_action_clone(
        BRACE::States const state,
        size_t const idx,
        std::vector<Gold::Brace> &stack
) {
        stack.push_back(stack.back());
        stack.back().idx = idx;
        stack.back().state = state;
}

void Brace_action_deleteMe(std::vector<Gold::Brace> &stack) {
        stack.pop_back();
}

void Brace_action_applyChar(char chr, TextFile &copy) {
        copy.append(chr);
}

void Brace_action_applyEndOfLine(TextFile &copy) {
        copy.append('\n');
}


TEST(Brace, reset) {
        Gold::Brace x;
        x.state = FIRST;
        Brace_reset(x);
        EXPECT_EQ(x.state, INIT);
        x.state = TERMINATOR;
        Brace_reset(x);
        EXPECT_EQ(x.state, INIT);
}

TEST(Brace, set) {
        Gold::Brace x;
        x.state = FIRST;
        Brace_set(x, NOT_LAST, 42);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.idx, 42);
}

TEST(Brace, open) {
        std::vector<Gold::Brace> stack;
        Gold::Brace x;
        stack_reset(stack);
        x.state = INIT;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack_reset(stack);
        x.state = IDLE ;
        x.idx = 99; 
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack_reset(stack);
        x.state = FIRST;
        x.idx = 99; 
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.idx, 99);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.back().state, LAST);
        EXPECT_EQ(stack.back().idx, 5);

        stack_reset(stack);
        EXPECT_EQ(stack.size(), 1);
        x.state = NOT_FIRST;
        x.idx = 99; 
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.idx, 99);
        EXPECT_EQ(stack.back().state, LAST);
        EXPECT_EQ(stack.back().idx, 5);

        stack_reset(stack);
        x.state = LAST;
        x.idx = 99; 
        Brace_event_open(x, 6, stack);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.idx, 99);
        EXPECT_EQ(stack.back().state, LAST);
        EXPECT_EQ(stack.back().idx, 6);

        stack_reset(stack);
        x.state = NOT_LAST;
        x.idx = 99; 
        Brace_event_open(x, 7, stack);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.idx, 99);
        EXPECT_EQ(stack.back().state, LAST);
        EXPECT_EQ(stack.back().idx, 7);
}

TEST(Brace, close) {
        std::vector<Gold::Brace> stack;
        Gold::Brace x;
        stack_reset(stack);
        EXPECT_EQ(stack.size(), 1);
        x.state = INIT;
        x.idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack_reset(stack);
        x.state = IDLE;
        x.idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack_reset(stack);
        x.state = FIRST;
        x.idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.idx, 99);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.back().state, NOT_FIRST);
        EXPECT_EQ(stack.back().idx, 5);

        stack_reset(stack);
        x.state = NOT_FIRST;
        x.idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.idx, 99);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.back().state, NOT_FIRST);
        EXPECT_EQ(stack.back().idx, 5);


        stack_reset(stack);
        EXPECT_EQ(stack.size(), 1);
        x.state = LAST;
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(stack.size(), 0);

        stack_reset(stack);
        x.state = NOT_LAST;
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(stack.size(), 0);
}

TEST(Brace, endOfLine) {
        std::vector<Gold::Brace> stack;
        Gold::Brace x;
        stack_reset(stack);
        x.state = INIT;
        x.idx = 99; 
        Brace_event_endOfLine(x, 5, stack);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack_reset(stack);
        x.state = IDLE;
        x.idx = 999; 
        Brace_event_endOfLine(x, 7, stack);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.idx, 7);
        EXPECT_EQ(stack.size(), 1);

        stack_reset(stack);
        x.state = FIRST;
        x.idx = 1000; 
        Brace_event_endOfLine(x, 13, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.idx, 1000);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.back().state, TERMINATOR);
        EXPECT_EQ(stack.back().idx, 13);

        stack_reset(stack);
        x.state = NOT_FIRST;
        x.idx = 2000; 
        Brace_event_endOfLine(x, 11, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.idx, 2000);
        EXPECT_EQ(stack.back().state, TERMINATOR);
        EXPECT_EQ(stack.back().idx, 11);
        
        stack_reset(stack);
        x.state = LAST;
        x.idx = 3000; 
        Brace_event_endOfLine(x, 71, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.idx, 3000);
        EXPECT_EQ(stack.back().state, TERMINATOR);
        EXPECT_EQ(stack.back().idx, 71);

        stack_reset(stack);
        x.state = NOT_LAST;
        x.idx = 81; 
        Brace_event_endOfLine(x, 3, stack);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.idx, 81);
        EXPECT_EQ(stack.back().state, TERMINATOR);
        EXPECT_EQ(stack.back().idx, 3);
}

TEST(Brace, applyChar) {
        TextFile  copy(10000);
        Gold::Brace x;
        copy.clear();
        x.state = FIRST;
        Brace_event_applyChar(x, 'X', copy);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(copy[0], 'X');

        copy.clear();
        x.state = NOT_FIRST;
        Brace_event_applyChar(x, 'Y', copy);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(copy[0], 'Y');

        copy.clear();
        x.state = LAST;
        Brace_event_applyChar(x, 'Z', copy);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(copy[0], 'Z');

        copy.clear();
        x.state = NOT_LAST;
        Brace_event_applyChar(x, 'A', copy);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(copy[0], 'A');

        copy.clear();
        x.state = TERMINATOR;
        Brace_event_applyChar(x, 'T', copy);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(copy[0], 'T');
}

TEST(Brace, apply) {
        TextFile copy(10000);
        Gold::Brace x;
        copy.clear();
        x.state = FIRST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(copy[0], '}');

        copy.clear();
        x.state = NOT_FIRST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(copy[0], '\n');

        copy.clear();
        x.state = LAST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(copy[0], '{');

        copy.clear();
        x.state = NOT_LAST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(copy[0], '{');
        EXPECT_EQ(copy[1], '\n');

        copy.clear();
        x.state = TERMINATOR;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(copy[0], '\n');
}
