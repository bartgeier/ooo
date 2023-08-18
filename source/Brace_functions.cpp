#include "Brace_functions.h"
#include <cstddef>
#include <assert.h>

using namespace BRACE;

static void set(Brace &m, States const state) {
        m.state = state;

}
static void set(Brace &m, States const state, size_t idx) {
        m.state = state;
        m.chr_idx = idx;
}

void Brace_reset(Brace &m) {
        m.state = OUTSIDE;
}

void Brace_event_open(Brace &m, size_t idx, BraceStack &stack) {
        switch (m.state) {
        case INIT:
        case IDLE:
                set(m, LAST , idx);
                break;
        case FIRST:
        case NOT_FIRST:
                Brace_action_clone(LAST, idx, stack);
                break;
        case LAST:
                Brace_action_clone(LAST, idx, stack);
                set(m, NOT_LAST);
                break;
        case NOT_LAST:
                Brace_action_clone(LAST, idx, stack);
                break;
        case TERMINATOR:
                assert(false);
                break;
        }
}

void Brace_event_close(Brace &m, size_t idx, BraceStack &stack) {
        switch (m.state) {
        case INIT:
                set(m, FIRST, idx);
                break;
        case IDLE:
                set(m, NOT_FIRST, idx);
                break;
        case FIRST:
        case NOT_FIRST:
                Brace_action_clone(NOT_FIRST, idx, stack);
                break;
        case LAST:
        case NOT_LAST:
                Brace_action_deleteMe(stack);
                break;
        case TERMINATOR:
                assert(false);
                break;
        }
}

void Brace_event_endOfLine(Brace &m, size_t idx, BraceStack &stack) {
        switch(m.state) {
        case INIT:
        case IDLE:
                set(m, TERMINATOR, idx);
                break;
        case FIRST:
        case NOT_FIRST:
        case LAST:
        case NOT_LAST:
                Brace_action_clone(TERMINATOR, idx, stack);
                break;
        case TERMINATOR:
                assert(false);
                break;
        }
}

void Brace_event_applyChar(Brace &m, char chr, std::string &copy) {
        switch(m.state) {
        case INIT:
        case IDLE:
                assert(false);
                break;
        case FIRST:
        case NOT_FIRST:
        case LAST:
        case NOT_LAST:
        case TERMINATOR:
                Brace_action_applyChar(chr, copy);
                break;
        }
}

/* brace_char "{}" or "()" or "<>" or "[]" */
void Brace_event_apply(Brace &m, char const brace_char[2], std::string &copy) {
        switch(m.state) {
        case INIT:
                break;
        case IDLE:
                break;
        case FIRST:
                break;
        case NOT_FIRST:
                break;
        case LAST:
                break;
        case NOT_LAST:
                break;
        case TERMINATOR:
                Brace_action_applyEndOfLine(copy);
                break;
        case OUTSIDE:
                Brace_action_applyOutside(brace_char[1], m.chr_idx, copy);
                break;
        case INSIDE:
                Brace_action_applyInside(brace_char[0], m.chr_idx, copy);
                break;
        }
}
