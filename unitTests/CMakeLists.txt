add_executable(testrunner 
        gtest-main.cpp 
        test_hello.cpp 
        test_TextFile.cpp
        test_Brace.cpp
)

target_include_directories(testrunner
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../source"
)

target_link_libraries(testrunner
        gtest_main
        gmock_main
        textfile
)
