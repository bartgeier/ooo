#include "Task_brace.h"
#include "Codizes.h"
#include "Brace.h"
#include <assert.h>

static void track(
        std::vector<Gold::Brace> &stack,
        std::string &sinke,
        TextFile const &source,
        char const o[2]
) {
        
}

static void apply(
        std::vector<Gold::Brace> &stack,
        std::string &sinke,
        TextFile const &source,
        char const o[2]
) {
        
}

int task_brace(Gold &gold, char const o[2]) {
        assert(
                  o[0] == '{' & o[1] == '}'
                | o[0] == '(' & o[1] == ')'
                | o[0] == '[' & o[1] == ']'
                | o[0] == '<' & o[1] == '>'
        );
        Codizes_init(gold.filter);
        for (auto const &line : gold.txt) {
                gold.brace_stack.resize(1);
                gold.brace_stack.back().state = BRACE::INIT;
                for (size_t idx = line.start; idx < line.end; idx++) {
                        char const c = codizes(gold.filter, gold.txt[idx]);
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
                Brace_event_endOfLine( gold.brace_stack.back(), 
                        line.end,
                        gold.brace_stack
                );

                size_t A = line.start;
                for (auto &brace : gold.brace_stack) { 
                        /* apply characters from txt and Braces from stack, */ 
                        /* and move them into copy */
                        size_t const B = brace.idx;
                        for (size_t idx = A; idx < B; idx++) {
                                Brace_event_applyChar(
                                        brace,
                                        gold.txt[idx],
                                        gold.copy
                                );
                        }
                        Brace_event_apply(brace, o, gold.copy);
                        A = B + 1;
                }
        }
        return 0;
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

void Brace_action_applyChar(char chr, std::string &copy) {
        copy += chr;
}

void Brace_action_applyEndOfLine(std::string &copy) {
        copy += "\n";
}
