#include "gtest/gtest.h"
#include <cstddef>
#include "Brace_functions.h"
#include "Brace_types.h"

using namespace BRACE;


static struct Flag {
        States state;
        size_t idx;
        BraceStack *stack;
} flag;

/* stubs */
void Brace_action_clone(BRACE::States const state, size_t const idx, BraceStack &stack) {
        stack.push(state, idx);
}

void Brace_action_deleteMe(BraceStack &stack) {
        stack.pop();
}

void Brace_action_applyChar(char chr, std::string &copy) {
        copy += chr;
}

void Brace_action_applyEndOfLine(std::string &copy) {
        copy += '\n';
}


TEST(Brace, reset) {
        Brace x;
        x.state = FIRST;
        Brace_reset(x);
        EXPECT_EQ(x.state, INIT);
        x.state = TERMINATOR;
        Brace_reset(x);
        EXPECT_EQ(x.state, INIT);
}

TEST(Brace, set) {
        Brace x;
        x.state = FIRST;
        Brace_set(x, NOT_LAST, 42);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.chr_idx, 42);
}

TEST(Brace, open) {
        BraceStack stack;
        Brace x;
        stack.reset();
        x.state = INIT;
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack.reset();
        x.state = IDLE ;
        x.chr_idx = 99; 
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack.reset();
        x.state = FIRST;
        x.chr_idx = 99; 
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.last().state, LAST);
        EXPECT_EQ(stack.last().chr_idx, 5);

        stack.reset();
        EXPECT_EQ(stack.size(), 1);
        x.state = NOT_FIRST;
        x.chr_idx = 99; 
        Brace_event_open(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_EQ(stack.last().state, LAST);
        EXPECT_EQ(stack.last().chr_idx, 5);

        stack.reset();
        x.state = LAST;
        x.chr_idx = 99; 
        Brace_event_open(x, 6, stack);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_EQ(stack.last().state, LAST);
        EXPECT_EQ(stack.last().chr_idx, 6);

        stack.reset();
        x.state = NOT_LAST;
        x.chr_idx = 99; 
        Brace_event_open(x, 7, stack);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_EQ(stack.last().state, LAST);
        EXPECT_EQ(stack.last().chr_idx, 7);
}

TEST(Brace, close) {
        BraceStack stack;
        Brace x;
        stack.reset();
        EXPECT_EQ(stack.size(), 1);
        x.state = INIT;
        x.chr_idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack.reset();
        x.state = IDLE;
        x.chr_idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack.reset();
        x.state = FIRST;
        x.chr_idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.last().state, NOT_FIRST);
        EXPECT_EQ(stack.last().chr_idx, 5);

        stack.reset();
        x.state = NOT_FIRST;
        x.chr_idx = 99; 
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.chr_idx, 99);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.last().state, NOT_FIRST);
        EXPECT_EQ(stack.last().chr_idx, 5);


        stack.reset();
        EXPECT_EQ(stack.size(), 1);
        x.state = LAST;
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(stack.size(), 0);

        stack.reset();
        x.state = NOT_LAST;
        Brace_event_close(x, 5, stack);
        EXPECT_EQ(stack.size(), 0);
}

TEST(Brace, endOfLine) {
        BraceStack stack;
        Brace x;
        stack.reset();
        x.state = INIT;
        x.chr_idx = 99; 
        Brace_event_endOfLine(x, 5, stack);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.chr_idx, 5);
        EXPECT_EQ(stack.size(), 1);

        stack.reset();
        x.state = IDLE;
        x.chr_idx = 999; 
        Brace_event_endOfLine(x, 7, stack);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(x.chr_idx, 7);
        EXPECT_EQ(stack.size(), 1);

        stack.reset();
        x.state = FIRST;
        x.chr_idx = 1000; 
        Brace_event_endOfLine(x, 13, stack);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(x.chr_idx, 1000);
        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack.last().state, TERMINATOR);
        EXPECT_EQ(stack.last().chr_idx, 13);

        stack.reset();
        x.state = NOT_FIRST;
        x.chr_idx = 2000; 
        Brace_event_endOfLine(x, 11, stack);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(x.chr_idx, 2000);
        EXPECT_EQ(stack.last().state, TERMINATOR);
        EXPECT_EQ(stack.last().chr_idx, 11);
        
        stack.reset();
        x.state = LAST;
        x.chr_idx = 3000; 
        Brace_event_endOfLine(x, 71, stack);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(x.chr_idx, 3000);
        EXPECT_EQ(stack.last().state, TERMINATOR);
        EXPECT_EQ(stack.last().chr_idx, 71);

        stack.reset();
        x.state = NOT_LAST;
        x.chr_idx = 81; 
        Brace_event_endOfLine(x, 3, stack);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(x.chr_idx, 81);
        EXPECT_EQ(stack.last().state, TERMINATOR);
        EXPECT_EQ(stack.last().chr_idx, 3);
}

TEST(Brace, applyChar) {
        std::string copy;
        Brace x;
        copy.clear();
        x.state = FIRST;
        Brace_event_applyChar(x, 'X', copy);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(copy, std::string("X"));

        copy.clear();
        x.state = NOT_FIRST;
        Brace_event_applyChar(x, 'Y', copy);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(copy, std::string("Y"));

        copy.clear();
        x.state = LAST;
        Brace_event_applyChar(x, 'Z', copy);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(copy, std::string("Z"));

        copy.clear();
        x.state = NOT_LAST;
        Brace_event_applyChar(x, 'A', copy);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(copy, std::string("A"));

        copy.clear();
        x.state = TERMINATOR;
        Brace_event_applyChar(x, 'T', copy);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(copy, std::string("T"));
}

TEST(Brace, apply) {
        std::string copy;
        Brace x;
        copy.clear();
        x.state = FIRST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, FIRST);
        EXPECT_EQ(copy, std::string("}"));

        copy.clear();
        x.state = NOT_FIRST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, NOT_FIRST);
        EXPECT_EQ(copy, std::string("\n}"));

        copy.clear();
        x.state = LAST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, LAST);
        EXPECT_EQ(copy, std::string("{"));

        copy.clear();
        x.state = NOT_LAST;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, NOT_LAST);
        EXPECT_EQ(copy, std::string("{\n"));

        copy.clear();
        x.state = TERMINATOR;
        Brace_event_apply(x, "{}", copy);
        EXPECT_EQ(x.state, TERMINATOR);
        EXPECT_EQ(copy, std::string("\n"));
}
