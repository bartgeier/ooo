add_library(textfile
        TextFile.h
        TextFile.cpp
        Brace.h
        Brace.cpp
)

add_executable(mainApp main.cpp)

target_link_libraries(mainApp
        textfile
)
