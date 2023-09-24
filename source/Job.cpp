#include "Job.h"
#include "Task_brace.h"
#include "TextFile.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <assert.h>


static void pr(TextFile::Line const &line) {
        printf("%p", line.at);
        std::string s;
        for (size_t i = 0; i < line.size; i++) {
                s += line.at[i];
        }
        std::cout << s;
}


int Job_main(Gold &gold) {
        printf("%p\n", &gold.pong[0]);
        task_brace(gold, "{}", gold.pong, gold.ping);
//  int task_brace(Gold &gold, char const o[2], std::string sink, TextFile &source)
        // std::cout << gold.txt.file;
        std::cout << gold.ping;
        //https://www.learncpp.com/cpp-tutorial/overloading-the-io-operators/
        for (auto const &line : gold.ping) {
                std::string s;
                if (line.at[0] == '\n') {
                        s = "'\\n'";
                } else {
                        s = '\'';
                        s += line.at[0];
                        s += '\'';
                }
                std::cout << line.start << ' ' << line.end << ' ' << line.size << ' ' << s << ' ';
                pr(line);
                std::cout << '\n';
        }
        std::cout << gold.pong;
        for (auto const &line : gold.pong) {
                std::string s;
                s.clear();
                if (line.at[0] == '\n') {
                        s = "'\\n'";
                } else {
                        s = '\'';
                        s += line.at[0];
                        s += '\'';
                }
                std::cout << line.start << ' ' << line.end << ' ' << line.size << ' ' << s << ' ';
                pr(line);
                std::cout << '\n';
        }
        for (size_t i = 0; i < gold.pong.size(); i++) {
                printf("%p %c\n", &gold.pong[i], gold.pong[i]);
        }
        std::cout << gold.pong;
        return 0;
}

