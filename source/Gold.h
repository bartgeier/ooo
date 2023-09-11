#ifndef GOLD_H
#define GOLD_H

#include <string>
#include "TextFile.h"

namespace TRACKSWITCH {
        enum States {
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

namespace BRACE {
        enum States {
                INIT,
                IDLE,
                FIRST,
                NOT_FIRST,
                LAST,
                NOT_LAST,
                TERMINATOR
        };
};

struct Gold {
        TextFile txt;
        std::string copy;

        struct TrackSwitch {
                TRACKSWITCH::States state;
        } filter;
        struct Brace {
                BRACE::States state;
                size_t idx;
        };
        std::vector<Brace> brace_stack;
};

#endif
