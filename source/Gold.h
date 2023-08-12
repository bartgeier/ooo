#ifndef GOLD_H
#define GOLD_H

#include "Brace_pass.h"
#include <vector>

struct Gold {
        std::vector<Brace_pass> brace_passes;
        TextFile txt;
        BraceStack brace_stack;
        std::string copy;
};

#endif
