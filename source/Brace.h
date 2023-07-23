#ifndef BRACE_H
#define BRACE_H

#include <cstddef>

namespace BRACE {
        enum States {
                OUTSIDE,
                INSIDE,
                TERMINATOR
        };
        struct Brace {
                States state;
                size_t txt_idx;
                size_t curlyIdx;
        };
};

using namespace BRACE;

void Brace_reset(Brace &m); 
void Brace_event_open(Brace &m, size_t idx);
void Brace_event_close(Brace &m, size_t idx);
void Brace_event_endOfLine(Brace &m, size_t idx);
void Brace_event_move(Brace &m, char chr);
void Brace_event_apply(Brace &m, char const type[2]);

void Brace_action_open();
void Brace_action_close();
void Brace_action_complete();
void Brace_action_endOfLine();

void Brace_action_move(char chr);
void Brace_action_applyInside(Brace &brace, char const brace_char);
void Brace_action_applyOutside(Brace &brace, char const brace_char);
void Brace_action_applyEndOfLine(Brace &brace);

#endif
