add_library(textfile
        TextFile.h
        TextFile.cpp
        Brace.h
        Brace.cpp
        BraceStack.cpp
)

add_executable(mainApp main.cpp)

target_link_libraries(mainApp
        textfile
)
