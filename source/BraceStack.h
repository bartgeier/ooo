#ifndef BRACESTACK_H
#define BRACESTACK_H

#include "Brace.h"
#include <vector>

struct BraceStack {
        std::vector<Brace> v;
        void push() {
                v.push_back(v.back());
        }
        void pop() {
                v.pop_back();
        }
        Brace& operator[](size_t i) {
                return v[i];
        }
        size_t size() {
                return v.size();
        }
        void reset() {
                v.resize(1);
                Brace_reset(v[0]);
        }
        Brace& last() {
                return v.back();
        }
        auto begin() {
                return v.begin();
        }
        auto end() {
                return v.end();
        }
};



#endif
