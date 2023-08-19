#include "Job.h"
#include "Brace_functions.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <ranges>


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


static char comment(bool &comment, char const chr) {
        if (chr == '\'' or chr == '\"') {
                comment = !comment;
        }
        return comment ? 'x' : chr;
}

int Job_main(Gold &gold) {
        bool inComment = false;
        for (auto const &line : gold.txt) {
                gold.brace_stack.reset();
                for (size_t chr_idx = line.start; chr_idx < line.end; chr_idx++) {
                        switch (comment(inComment, gold.txt[chr_idx])) {
                        case '{':
                                Brace_event_open(
                                        gold.brace_stack.last(), 
                                        chr_idx,
                                        gold.brace_stack
                                );
                                break;
                        case '}':
                                Brace_event_close(
                                        gold.brace_stack.last(), 
                                        chr_idx, 
                                        gold.brace_stack
                                );
                                break;
                        case ' ':
                                /* nothing */
                                break;
                        default:
                                Brace_event_nonBrace(gold.brace_stack.last());
                                break;
                        }
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
                        size_t const B = brace.chr_idx;
                        for (size_t chr_idx = A; chr_idx < B; chr_idx++) {
                                Brace_event_applyChar(
                                        brace,
                                        gold.txt[chr_idx],
                                        gold.copy
                                );
                        }
                        Brace_event_apply(brace, "{}", gold.copy);
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
