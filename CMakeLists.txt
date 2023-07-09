cmake_minimum_required(VERSION 3.14)

project(ooo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CXXFLAGS  "-fopenmp -Wall -std=c++2a -MMD -I${slate_dir}/include -DSLATE_NO_CUDA")
# set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
# set(CMAKE_VERBOSE_MAKEFILE ON)

FILE(GENERATE OUTPUT ${CMAKE_BINARY_DIR}/testrunner
     CONTENT 
"#!/usr/bin/bash
cd unitTests
./testrunner
cd .."
     FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
)

function(print)
        foreach(var ${ARGN})
                message("${var} = ${${var}}")
        endforeach()
endfunction()

function(print_env)
        foreach(var ${ARGN})
                message("${var} = $ENV{${var}}")
        endforeach()
endfunction()

print(PROJECT_NAME)
print_env(PATH)

include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.12.1 
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
        fetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()
add_subdirectory(unitTests)

add_subdirectory(source)

# add_library(library library.ccp)
# add_executable(mainApp main.cpp)
# target_link_libraries(mainApp source)

# target_link_libraries(testRunner library)
# add_test(testRunner unitTests)
