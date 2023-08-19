#ifndef BRACE_FUNCTIONS_H
#define BRACE_FUNCTIONS_H

#include "Brace_types.h"
#include "Gold.h"

void Brace_reset(Brace &m); 
void Brace_set(Brace &m, BRACE::States const state, size_t idx);
void Brace_event_open(Brace &m, size_t idx, BraceStack &stack);
void Brace_event_close(Brace &m, size_t idx, BraceStack &stack);
void Brace_event_endOfLine(Brace &m, size_t idx, BraceStack &stack);
void Brace_event_applyChar(Brace &m, char chr, std::string &copy);
void Brace_event_apply(Brace &m, char const type[2], std::string &copy);

void Brace_action_clone(BRACE::States const state, size_t const idx, BraceStack &stack);
void Brace_action_deleteMe(BraceStack &stack);

void Brace_action_applyChar(char chr, std::string &copy);
void Brace_action_applyEndOfLine(std::string &copy);

#endif
