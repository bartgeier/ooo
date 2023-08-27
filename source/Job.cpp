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
        task_brace(gold, "{}");

        // std::cout << gold.txt.file;
        std::cout << gold.txt;
        //https://www.learncpp.com/cpp-tutorial/overloading-the-io-operators/
        for (auto const &line : gold.txt) {
                std::cout << line.start << ' ' << line.end << '\n';
        }
        std::cout << gold.copy;
        return 0;
}

