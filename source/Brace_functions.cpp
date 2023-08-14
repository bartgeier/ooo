#include "Brace_functions.h"
#include <cstddef>

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
        case OUTSIDE:
                set(m, INSIDE , idx);
                Brace_action_open(stack);
                break;
        case INSIDE:
                set(m, INSIDE, idx);
                Brace_action_open(stack);
                break;
        case TERMINATOR:
                break;
        }
}

void Brace_event_close(Brace &m, size_t idx, BraceStack &stack) {
        switch (m.state) {
        case OUTSIDE:
                set(m, OUTSIDE , idx);
                Brace_action_close(stack);
                break;
        case INSIDE:
                set(m, OUTSIDE, idx);
                Brace_action_complete(stack);
                break;
        case TERMINATOR:
                break;
        }
}

void Brace_event_endOfLine(Brace &m, size_t idx) {
        switch(m.state) {
        case OUTSIDE:
        case INSIDE:
                set(m, TERMINATOR, idx);
                break;
        case TERMINATOR:
                break;
        }
}

void Brace_event_applyChar(Brace &m, char chr, std::string &copy) {
        switch(m.state) {
        case OUTSIDE:
        case INSIDE:
        case TERMINATOR:
                Brace_action_applyChar(chr, copy);
                break;
        }
}

/* brace_char "{}" or "()" or "<>" or "[]" */
void Brace_event_apply(Brace &m, char const brace_char[2], Gold &gold) {
        switch(m.state) {
        case OUTSIDE:
                Brace_action_applyOutside(brace_char[1], m.chr_idx, gold);
                break;
        case INSIDE:
                Brace_action_applyInside(brace_char[0], m.chr_idx, gold);
                break;
        case TERMINATOR:
                Brace_action_applyEndOfLine(gold);
                break;
        }
}
