#include "Job.h"
#include "Gold.h"

std::vector<Gold> golds;

//main::job::task::process


int main(int argc, char* argv[]) {
        bool error = (argc < 2);
        Gold gold;
        golds.push_back(gold);
        error = golds[0].txt.load(argv[1], error);
        
        return Job_main(golds[0]);
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
