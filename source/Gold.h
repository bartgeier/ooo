#ifndef GOLD_H
#define GOLD_H

#include <string>
#include "TextFile.h"
#include "Brace_types.h"
#include "TrackSwitch.h"

struct Gold {
        TextFile txt;
        TrackSwitch filter;
        BraceStack brace_stack;
        std::string copy;
};

#endif
