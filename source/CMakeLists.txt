add_library(textfile
        Gold.h
        Job.h
        Job.cpp
        TextFile.h
        TextFile.cpp
        Brace_types.h
        Brace_functions.cpp
        BraceStack.cpp
        Brace_pass.h
        Brace_pass.cpp
        TrackSwitch.h
        TrackSwitch.cpp
)

add_executable(mainApp main.cpp)

target_link_libraries(mainApp
        textfile
)
