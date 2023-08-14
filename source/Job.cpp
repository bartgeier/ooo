#include "Job.h"
#include "Brace_functions.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <ranges>


void Brace_action_open(BraceStack &stack) {
         stack.push();
}

void Brace_action_close(BraceStack &stack) {
         stack.push();
}

void Brace_action_complete(BraceStack &stack) {
         stack.pop();
}

void Brace_action_applyChar(char chr, std::string &copy) {
        copy += chr;
}

void Brace_action_applyInside(char const brace_char, size_t const chr_idx, Gold &gold) {
        gold.copy += brace_char; // '{', '(', '<', '['
        if (!gold.txt.last_character_in_line(chr_idx)) {
                gold.copy += "\n";
        }
}

void Brace_action_applyOutside(char const brace_char, size_t const chr_idx, Gold &gold) {
        if (!gold.txt.first_character_in_line(chr_idx)) {
                gold.copy += "\n";
        }
        gold.copy += brace_char; // '}', ')', '>', ']'
}

void Brace_action_applyEndOfLine(Gold &gold) {
        gold.copy += "\n";
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
                Brace_event_endOfLine(gold.brace_stack.last(), line.end);

                size_t A = line.start;
                for (auto &brace : gold.brace_stack) { 
                        /* apply characters from txt and Braces from stack, */ 
                        /* and move them into copy */
                        size_t const B = brace.chr_idx;
                        for (size_t chr_idx = A; chr_idx < B; chr_idx++) {
                                Brace_event_applyChar(brace, gold.txt[chr_idx], gold.copy);
                        }
                        Brace_event_apply(brace, "{}", gold);
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
