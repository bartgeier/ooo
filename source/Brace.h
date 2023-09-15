#ifndef BRACE_H
#define BRACE_H

#include "Gold.h"
#include <vector>

void Brace_reset(Gold::Brace &m); 
void Brace_set(Gold::Brace &m, BRACE::States const state);
void Brace_set(Gold::Brace &m, BRACE::States const state, size_t idx);
void Brace_event_open(Gold::Brace &m, size_t idx, std::vector<Gold::Brace> &stack);
void Brace_event_close(Gold::Brace &m, size_t idx, std::vector<Gold::Brace> &stack);
void Brace_event_nonBrace(Gold::Brace &m);
void Brace_event_endOfLine(Gold::Brace &m, size_t idx, std::vector<Gold::Brace> &stack);
void Brace_event_applyChar(Gold::Brace &m, char chr, std::string &copy);
void Brace_event_apply(Gold::Brace &m, char const type[2], std::string &copy);

void Brace_action_clone(
        BRACE::States const state,
        size_t const idx,
        std::vector<Gold::Brace> &stack
);
void Brace_action_deleteMe(std::vector<Gold::Brace> &stack);
void Brace_action_applyChar(char chr, std::string &copy);
void Brace_action_applyEndOfLine(std::string &copy);

#endif
