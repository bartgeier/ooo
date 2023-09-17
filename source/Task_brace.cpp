#include "Task_brace.h"
#include "Codizes.h"
#include "Brace.h"
#include "TextFile.h"
#include <assert.h>

#if(0)
static void track(
        std::vector<Gold::Brace> &stack,
        Gold::Codizes &filter,
        TextFile::Line const &line,
        TextFile &source,
        char const o[2]
) {
        for (size_t idx = line.start; idx < line.end; idx++) {
                char const c = codizes(filter, source[idx]);
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
                line.end,
                stack
        );
}

/* apply characters from source and braces from stack, */ 
/* and move them into sink */
static void apply(
        std::vector<Gold::Brace> &stack,
        std::string &sink,
        TextFile::Line const &line,
        TextFile &source,
        char const o[2]
) {
        size_t A = line.start;
        for (auto &brace : stack) { 
                size_t const B = brace.idx;
                for (size_t idx = A; idx < B; idx++) {
                        Brace_event_applyChar(
                                brace,
                                source[idx],
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
        std::string &sink, 
        TextFile &source
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
                track(gold.brace_stack, gold.filter, line, source, o);
                apply(gold.brace_stack, sink, line, source, o);
        }
}
#else
void task_brace(
        Gold &gold, 
        char const o[2], 
        std::string &sink, 
        TextFile &source
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
                for (size_t idx = line.start; idx < line.end; idx++) {
                        char const c = codizes(gold.filter, source[idx]);
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
                        line.end,
                        gold.brace_stack
                );

                size_t A = line.start;
                for (auto &brace : gold.brace_stack) { 
                        size_t const B = brace.idx;
                        for (size_t idx = A; idx < B; idx++) {
                                Brace_event_applyChar(
                                        brace,
                                        source[idx],
                                        sink
                                );
                        }
                        Brace_event_apply(brace, o, sink);
                        A = B + 1;
                }
        }
}
#endif

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

void Brace_action_applyChar(char chr, std::string &copy) {
        copy += chr;
}

void Brace_action_applyEndOfLine(std::string &copy) {
        copy += "\n";
}
