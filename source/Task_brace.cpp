#include "Task_brace.h"
#include "Codizes.h"
#include "Brace.h"
#include "TextFile.h"
#include <assert.h>

#if 0
static void track(
        std::vector<Gold::Brace> &stack,
        Gold::Codizes &filter,
        Slice const &line,
        char const o[2]
) {
        for (size_t idx = 0; idx < line.size; idx++) {
                char const c = codizes(filter, line.at[idx]);
                if (c == o[0]) {
                        Brace_event_open(
                                stack.back(), 
                                idx,
                                stack
                        );
                        continue;
                } if (c == o[1]) {
                        Brace_event_close(
                                stack.back(), 
                                idx, 
                                stack
                        );
                        continue;
                } if (c == ' ') {
                        /* nothing */
                        continue;
                } 
                Brace_event_nonBrace(stack.back());
        }
        codizes(filter, '\n'); 
        Brace_event_endOfLine(
                stack.back(), 
                line.size,
                stack
        );
}

/* move characters from source and braces from stack, into sink */
static void apply(
        std::vector<Gold::Brace> &stack,
        TextFile &sink,
        Slice const &line,
        char const o[2]
) {
        size_t A = 0;
        for (auto &brace : stack) { 
                size_t const B = brace.idx;
                for (size_t idx = A; idx < B; idx++) {
                        Brace_event_applyChar(
                                brace,
                                line.at[idx],
                                sink
                        );
                }
                Brace_event_apply(brace, o, sink);
                A = B + 1;
        }
}

void task_brace(
        Gold &gold, 
        char const o[2], 
        TextFile const &source,
        TextFile &sink
) {
        assert(
                  o[0] == '{' & o[1] == '}'
                | o[0] == '(' & o[1] == ')'
                | o[0] == '[' & o[1] == ']'
                | o[0] == '<' & o[1] == '>'
        );
        Codizes_init(gold.filter);
        for (auto const &line : source) {
                gold.brace_stack.resize(1);
                Brace_set(gold.brace_stack.back(), BRACE::INIT);
                track(gold.brace_stack, gold.filter, line, o);
                apply(gold.brace_stack, sink, line, o);
        }
}

void Brace_action_clone(
        BRACE::States const state,
        size_t const idx,
        std::vector<Gold::Brace> &stack
) {
        stack.push_back(stack.back());
        stack.back().idx = idx;
        stack.back().state = state;
}

void Brace_action_deleteMe(std::vector<Gold::Brace> &stack) {
        if (stack.size() > 1) {
                stack.pop_back();
        } else {
                Brace_set(stack.back(), BRACE::IDLE);
        };
}

void Brace_action_applyChar(char chr, TextFile &copy) {
        copy.append(chr);
}

void Brace_action_applyEndOfLine(TextFile &copy) {
        copy.append('\n');
}
#else
void task_brace(
        Gold &gold, 
        char const o[2], 
        TextFile const &source,
        TextFile &sink
) {
        assert(
                  o[0] == '{' & o[1] == '}'
                | o[0] == '(' & o[1] == ')'
                | o[0] == '[' & o[1] == ']'
                | o[0] == '<' & o[1] == '>'
        );
        Codizes_init(gold.filter);
        for (auto const &line : source) {
                gold.brace_stack.resize(1);
                Brace_set(gold.brace_stack.back(), BRACE::INIT);
                for (size_t idx = 0; idx < line.size; idx++) {
                        char const c = codizes(gold.filter, line.at[idx]);
                        if (c == o[0]) {
                                Brace_event_open(
                                        gold.brace_stack.back(), 
                                        idx,
                                        gold.brace_stack
                                );
                                continue;
                        } if (c == o[1]) {
                                Brace_event_close(
                                        gold.brace_stack.back(), 
                                        idx, 
                                        gold.brace_stack
                                );
                                continue;
                        } if (c == ' ') {
                                /* nothing */
                                continue;
                        } 
                        Brace_event_nonBrace(gold.brace_stack.back());
                }
                codizes(gold.filter, '\n'); 
                Brace_event_endOfLine(
                        gold.brace_stack.back(), 
                        line.size,
                        gold.brace_stack
                );

                size_t A = 0;
                for (auto &brace : gold.brace_stack) { 
                        size_t const B = brace.idx;
                        for (size_t idx = A; idx < B; idx++) {
                                Brace_event_applyChar(
                                        brace,
                                        line[idx],
                                        sink
                                );
                        }
                        Brace_event_apply(brace, o, sink);
                        A = B + 1;
                }
        }
}

void Brace_action_clone(
        BRACE::States const state,
        size_t const idx,
        std::vector<Gold::Brace> &stack
) {
        stack.push_back(stack.back());
        stack.back().idx = idx;
        stack.back().state = state;
}

void Brace_action_deleteMe(std::vector<Gold::Brace> &stack) {
        if (stack.size() > 1) {
                stack.pop_back();
        } else {
                Brace_set(stack.back(), BRACE::IDLE);
        };
}

void Brace_action_applyChar(char chr, TextFile &copy) {
        copy.append(chr);
}

void Brace_action_applyEndOfLine(TextFile &copy) {
        copy.append('\n');
}
#endif
