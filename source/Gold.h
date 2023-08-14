#ifndef GOLD_H
#define GOLD_H

#include <string>
#include "TextFile.h"
#include "Brace_types.h"

struct Gold {
        TextFile txt;
        BraceStack brace_stack;
        std::string copy;
};

#endif
