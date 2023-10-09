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


static void pr(Slice const &line) {
        printf("%p |", line.at);
        std::string s;
        s.clear();
        for (size_t i = 0; i < line.size; i++) {
                s += line.at[i];
        }
        std::cout << s << "|";
}

int Job_main(Gold &gold) {
        task_brace(gold, "{}", gold.ping, gold.pong);

        std::cout << gold.ping;
        gold.pong.setLineSlice();
        for (auto const &line : gold.pong) {

                std::string s;
                s.clear();
                if (line.at[0] == '\n') {
                        s = "'\\n'    ";
                } else {
                        s = '\'';
                        s += line.at[0];
                        s += '\'';
                        s +=  (line.at[line.size] == '\n') ? " '\\n'" : " '0'";
                }
                std::cout << line.front << ' ' << line.front + line.size << ' ' << line.size << ' ' << s << ' ';
                pr(line);
                std::cout << '\n';
        }
        return 0;
}

