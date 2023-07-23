#include "Brace.h"
#include <cstddef>

using namespace BRACE;

static void set(Brace &m, States const state) {
        m.state = state;
}

static void set(Brace &m, States const state, size_t idx) {
        m.state = state;
        m.txt_idx = idx;
}

void Brace_reset(Brace &m) {
        m.state = OUTSIDE;
}

void Brace_event_open(Brace &m, size_t idx) {
        switch (m.state) {
        case OUTSIDE:
                set(m, INSIDE , idx);
                Brace_action_open();
                break;
        case INSIDE:
                set(m, INSIDE, idx);
                Brace_action_open();
                break;
        case TERMINATOR:
                break;
        }
}

void Brace_event_close(Brace &m, size_t idx) {
        switch (m.state) {
        case OUTSIDE:
                set(m, OUTSIDE , idx);
                Brace_action_close();
                break;
        case INSIDE:
                set(m, OUTSIDE, idx);
                Brace_action_complete();
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
                Brace_action_endOfLine();
                break;
        case TERMINATOR:
                break;
        }
}

void Brace_event_move(Brace &m, char chr) {
        switch(m.state) {
        case OUTSIDE:
        case INSIDE:
        case TERMINATOR:
                Brace_action_move(chr);
                break;
        }
}

/* brace_char "{}" or "()" or "<>" or "[]" */
void Brace_event_apply(Brace &m, char const brace_char[2]) {
        switch(m.state) {
        case OUTSIDE:
                Brace_action_applyOutside(m, brace_char[1]);
                break;
        case INSIDE:
                Brace_action_applyInside(m, brace_char[0]);
                break;
        case TERMINATOR:
                Brace_action_applyEndOfLine(m);
                break;
        }
}
