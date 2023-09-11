add_library(textfile
        Gold.h
        TextFile.h
        TextFile.cpp
        Job.h
        Job.cpp
        Task_brace.h
        Task_brace.cpp
        Brace_functions.h
        Brace_functions.cpp
        TrackSwitch.h
        TrackSwitch.cpp
)

add_executable(mainApp main.cpp)

target_link_libraries(mainApp
        textfile
)
