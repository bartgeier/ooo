#include "Brace.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>
#include <ranges>
#include "TextFile.h"
#include "BraceStack.h"

struct Gold {
        TextFile txt;
        BraceStack brace_stack;
        std::string copy;
} gold;

void Brace_action_open() {
         gold.brace_stack.push();
}

void Brace_action_close() {
         gold.brace_stack.push();
}

void Brace_action_complete() {
         gold.brace_stack.pop();
}

void Brace_action_endOfLine() {

}

void Brace_action_move(char chr) {
        gold.copy += chr;
}

void Brace_action_applyInside(Brace &brace) {
        if (gold.txt.last_character_in_line(brace.txt_idx)) {
                gold.copy += "{";
        } else {
                gold.copy += "{\n";
        }
}

void Brace_action_applyOutside(Brace &brace) {
        if (gold.txt.first_character_in_line(brace.txt_idx)) {
                gold.copy += "}";
        } else {
                gold.copy += "\n}";
        }
}

void Brace_action_applyEndOfLine(Brace &brace) {
        gold.copy += "\n";
}

int main(int argc, char* argv[]) {
        bool error = (argc < 2);
        error = gold.txt.load(argv[1], error);

        for (auto const &line : gold.txt) {
                gold.brace_stack.reset();
                for (size_t chr_idx = line.start; chr_idx < line.end; chr_idx++) {
                        switch (gold.txt[chr_idx]) {
                        case '{':
                                Brace_event_open(gold.brace_stack.last(), chr_idx);
                                break;
                        case '}':
                                Brace_event_close(gold.brace_stack.last(), chr_idx);
                                break;
                        default:
                                /* nothing */
                                break;
                        }
                }
                Brace_event_endOfLine(gold.brace_stack.last(), line.end);

                size_t A = line.start;
                for (auto &brace : gold.brace_stack) { 
                        /* move character into copy */
                        size_t const B = brace.txt_idx;
                        for (size_t chr_idx = A; chr_idx < B; chr_idx++) {
                                Brace_event_move(brace, gold.txt[chr_idx]);
                        }
                        Brace_event_apply(brace);
                        A = B + 1;
                }
        }

        std::cout << gold.txt.file;
        for (auto const &line : gold.txt) {
                std::cout << line.start << ' ' << line.end << '\n';
        }
        std::cout << gold.copy;
        return 0;
}

#if(0)
if (__cplusplus == 202101L) std::cout << "C++23";
else if (__cplusplus == 202002L) std::cout << "C++20";
else if (__cplusplus == 201703L) std::cout << "C++17";
else if (__cplusplus == 201402L) std::cout << "C++14";
else if (__cplusplus == 201103L) std::cout << "C++11";
else if (__cplusplus == 199711L) std::cout << "C++98";
else std::cout << "pre-standard C++." << __cplusplus;

std::cout << "\n";
for (int i: std::ranges::iota_view{1, 10}) {
        std::cout << i << std::endl;
}

#endif
