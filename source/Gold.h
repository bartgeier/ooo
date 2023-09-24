#ifndef GOLD_H
#define GOLD_H

#include <string>
#include "TextFile.h"

namespace CODIZES {
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
        TextFile ping{10000};
        TextFile pong{10000};

        struct Codizes {
                CODIZES::States state;
        } filter;
        struct Brace {
                BRACE::States state;
                size_t idx;
        };
        std::vector<Brace> brace_stack;
};

#endif
