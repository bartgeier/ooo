#ifndef BRACE_TYPES_H
#define BRACE_TYPES_H

#include <cstddef>
#include <vector>

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
        size_t chr_idx;
};

class BraceStack {
        std::vector<Brace> v;
public:
        void push();
        void push(BRACE::States const state, size_t const idx);
        void pop();
        Brace& operator[](size_t i);
        size_t size();
        void reset();
        Brace& last();
        std::vector<Brace>::iterator begin();
        std::vector<Brace>::iterator end();
};



#endif
