#ifndef BRACE_TYPES_H
#define BRACE_TYPES_H

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



#endif
