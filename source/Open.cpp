#include "Open.h"
#include <cstddef>

using namespace BRACE;

static void set(Open &m, States const state) {
        m.state = state;
}

static void set(Open &m, States const state, size_t idx) {
        m.state = state;
        m.curlyIdx = idx;
}

void Brace_event_reset(Open &m) {
        switch (m.state) {
        case OUTSIDE:
                break;
        default:
                set(m, OUTSIDE);
        }
}

void Brace_event_open(Open &m, size_t idx) {
        switch (m.state) {
        case OUTSIDE:
                set(m, INSIDE , idx);
                break;
        case INSIDE:
                set(m, INSIDE, idx);
                break;
        }
}

void Brace_event_close(Open &m, size_t idx) {
        switch (m.state) {
        case OUTSIDE:
                set(m, OUTSIDE , idx);
                break;
        case INSIDE:
                set(m, OUTSIDE, idx);
                Brace_action_close();
                break;
        }
}

