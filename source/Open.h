#ifndef BRACE_H
#define BRACE_H

#include <cstddef>

namespace BRACE {
        enum States {
                OUTSIDE,
                INSIDE
        };
        struct Brace {
                States state;
                size_t curlyIdx;
        };
};

using namespace BRACE;

void Brace_event_reset(Brace &m); 
void Brace_event_open(Brace &m, size_t idx);
void Brace_event_close(Brace &m, size_t idx);

void Brace_action_close();



#endif
