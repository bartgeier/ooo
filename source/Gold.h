#ifndef GOLD_H
#define GOLD_H

#include <string>
#include "TextFile.h"
#include "Brace_types.h"

namespace TRACKSWITCH {
        enum States {    /* visible because unittests */
                INIT,
                SLASH,
                CHAR,
                STRING,
                LINE_COMMENT,
                BLOCK_COMMENT,
                ASTERISK,
                ESC_CHAR,
                ESC_STRING,
        };
};

struct Gold {
        TextFile txt;
        struct TrackSwitch {
                TRACKSWITCH::States state;
        } filter;
        BraceStack brace_stack;
        std::string copy;
};

#endif
