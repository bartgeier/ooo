#ifndef BRACE_TYPES_H
#define BRACE_TYPES_H

#include <cstddef>
#include <vector>
#include <string>

namespace BRACE {
        enum States {    /* visible because unittests */
                INIT,
                IDLE,
                FIRST,
                NOT_FIRST,
                LAST,
                NOT_LAST,
                TERMINATOR
        };
};

struct Brace {
        BRACE::States state;
        size_t idx;
};


void Brace_reset(Brace &m); 
void Brace_set(Brace &m, BRACE::States const state);
void Brace_set(Brace &m, BRACE::States const state, size_t idx);
void Brace_event_open(Brace &m, size_t idx, std::vector<Brace> &stack);
void Brace_event_close(Brace &m, size_t idx, std::vector<Brace> &stack);
void Brace_event_nonBrace(Brace &m);
void Brace_event_endOfLine(Brace &m, size_t idx, std::vector<Brace> &stack);
void Brace_event_applyChar(Brace &m, char chr, std::string &copy);
void Brace_event_apply(Brace &m, char const type[2], std::string &copy);

void Brace_action_clone(
        BRACE::States const state,
        size_t const idx,
        std::vector<Brace> &stack
);
void Brace_action_deleteMe(std::vector<Brace> &stack);
void Brace_action_applyChar(char chr, std::string &copy);
void Brace_action_applyEndOfLine(std::string &copy);


#endif
