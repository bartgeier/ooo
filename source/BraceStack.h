#ifndef BRACESTACK_H
#define BRACESTACK_H

#include "Brace.h"
#include <vector>

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
