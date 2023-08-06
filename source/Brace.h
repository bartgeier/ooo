#ifndef BRACE_H
#define BRACE_H

#include <cstddef>
#include <vector>

namespace BRACE {
        enum States {    /* visible because unittests */
                OUTSIDE,
                INSIDE,
                TERMINATOR
        };
};

struct Brace {
        BRACE::States state;
        size_t chr_idx;
        size_t curlyIdx;
};

class BraceStack {
        std::vector<Brace> v;
public:
        void push();
        void pop();
        Brace& operator[](size_t i);
        size_t size();
        void reset();
        Brace& last();
        std::vector<Brace>::iterator begin();
        std::vector<Brace>::iterator end();
};


void Brace_reset(Brace &m); 
void Brace_event_open(Brace &m, size_t idx, BraceStack &stack);
void Brace_event_close(Brace &m, size_t idx);
void Brace_event_endOfLine(Brace &m, size_t idx);
void Brace_event_applyChar(Brace &m, char chr);
void Brace_event_apply(Brace &m, char const type[2]);

void Brace_action_open(BraceStack &stack);
void Brace_action_close();
void Brace_action_complete();

void Brace_action_applyChar(char chr);
void Brace_action_applyInside(char const brace_char, size_t const chr_idx);
void Brace_action_applyOutside(char const brace_char, size_t const chr_idx);
void Brace_action_applyEndOfLine();

#endif
