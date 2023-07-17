#include "BraceStack.h"

void BraceStack::push() {
        v.push_back(v.back());
}

void BraceStack::pop() {
        v.pop_back();
}

Brace& BraceStack::operator[](size_t i) {
        return v[i];
}

size_t BraceStack::size() {
        return v.size();
}

void BraceStack::reset() {
        v.resize(1);
        Brace_reset(v[0]);
}

Brace& BraceStack::last() {
        return v.back();
}

std::vector<Brace>::iterator BraceStack::begin() {
        return v.begin();
}

std::vector<Brace>::iterator BraceStack::end() {
        return v.end();
}
