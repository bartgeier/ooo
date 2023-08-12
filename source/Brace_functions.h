#ifndef BRACE_FUNCTIONS_H
#define BRACE_FUNCTIONS_H

#include "Brace_types.h"
#include "Gold.h"

void Brace_reset(Brace &m); 
void Brace_event_open(Brace &m, size_t idx, BraceStack &stack);
void Brace_event_close(Brace &m, size_t idx, BraceStack &stack);
void Brace_event_endOfLine(Brace &m, size_t idx);
void Brace_event_applyChar(Brace &m, char chr);
void Brace_event_apply(Brace &m, char const type[2]);

void Brace_action_open(BraceStack &stack);
void Brace_action_close(BraceStack &stack);
void Brace_action_complete(BraceStack &stack);

void Brace_action_applyChar(char chr);
void Brace_action_applyInside(char const brace_char, size_t const chr_idx);
void Brace_action_applyOutside(char const brace_char, size_t const chr_idx);
void Brace_action_applyEndOfLine();

#endif
