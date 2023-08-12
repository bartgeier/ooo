#ifndef BRACE_PASS_H 
#define BRACE_PASS_H

#include <string>
#include "TextFile.h"
#include "Brace_types.h"

struct Brace_pass {
        TextFile txt;
        BraceStack brace_stack;
        std::string copy;
};

void Brace_pass_do();

#endif
