#include "Job.h"
#include "Brace_functions.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <ranges>


void Brace_action_clone(BRACE::States const state, size_t const idx, BraceStack &stack) {
         stack.push();
}

void Brace_action_deleteMe(BraceStack &stack) {
         stack.pop();
}

void Brace_action_applyChar(char chr, std::string &copy) {
        copy += chr;
}

void Brace_action_applyEndOfLine(std::string &copy) {
        copy += "\n";
}

int Job_main(Gold &gold) {
        for (auto const &line : gold.txt) {
                gold.brace_stack.reset();
                for (size_t chr_idx = line.start; chr_idx < line.end; chr_idx++) {
                        switch (gold.txt[chr_idx]) {
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
                        default:
                                /* nothing */
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
