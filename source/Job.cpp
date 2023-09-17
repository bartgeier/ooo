#include "Job.h"
#include "Task_brace.h"
#include <cstddef>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <assert.h>


int Job_main(Gold &gold) {
        task_brace(gold, "{}", gold.pong, gold.ping);
//  int task_brace(Gold &gold, char const o[2], std::string sink, TextFile &source)
        // std::cout << gold.txt.file;
        std::cout << gold.ping;
        //https://www.learncpp.com/cpp-tutorial/overloading-the-io-operators/
        for (auto const &line : gold.ping) {
                std::cout << line.start << ' ' << line.end << '\n';
        }
        std::cout << gold.pong;
        return 0;
}

