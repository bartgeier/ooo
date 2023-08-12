add_library(textfile
        Gold.h
        Job.h
        Job.cpp
        TextFile.h
        TextFile.cpp
        Brace_types.h
        Brace_functions.h
        Brace_functions.cpp
        Brace_pass.h
        Brace_pass.cpp
        BraceStack.cpp
)

add_executable(mainApp main.cpp)

target_link_libraries(mainApp
        textfile
)
