#include "Job.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <assert.h>

void Brace_action_clone(BRACE::States const state, size_t const idx, BraceStack &stack) {
         stack.push(state, idx);
}

void Brace_action_deleteMe(BraceStack &stack) {
        if (stack.size() > 1) {
                stack.pop();
        } else {
                Brace_set(stack.last(), BRACE::IDLE);
        };
}

void Brace_action_applyChar(char chr, std::string &copy) {
        copy += chr;
}

void Brace_action_applyEndOfLine(std::string &copy) {
        copy += "\n";
}

/* o has to be: "{}" or "()" or "<>" or "[]" */
int Job_main(Gold &gold, char const o[2]) {
        assert(
                  o[0] == '{' & o[1] == '}'
                | o[0] == '(' & o[1] == ')'
                | o[0] == '[' & o[1] == ']'
                | o[0] == '<' & o[1] == '>'
        );
        TrackSwitch_init(gold.filter);
        for (auto const &line : gold.txt) {
                gold.brace_stack.reset();
                for (size_t idx = line.start; idx < line.end; idx++) {
                        char const c = trackSwitch(gold.filter, gold.txt[idx]);
                        if (c == o[0]) {
                                Brace_event_open(
                                        gold.brace_stack.last(), 
                                        idx,
                                        gold.brace_stack
                                );
                                continue;
                        } if (c == o[1]) {
                                Brace_event_close(
                                        gold.brace_stack.last(), 
                                        idx, 
                                        gold.brace_stack
                                );
                                continue;
                        } if (c == ' ') {
                                /* nothing */
                                continue;
                        } 
                        Brace_event_nonBrace(gold.brace_stack.last());
                }
                Brace_event_endOfLine(
                        gold.brace_stack.last(),
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

        // std::cout << gold.txt.file;
        std::cout << gold.txt;
        //https://www.learncpp.com/cpp-tutorial/overloading-the-io-operators/
        for (auto const &line : gold.txt) {
                std::cout << line.start << ' ' << line.end << '\n';
        }
        std::cout << gold.copy;
        return 0;
}

