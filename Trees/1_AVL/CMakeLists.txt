cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable sanitizers in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -fsanitize=undefined
        -fsanitize=address
        -fno-sanitize-recover=all
    )
    add_link_options(
        -fsanitize=undefined
        -fsanitize=address
    )
endif()

# Your executable
add_executable(SetAVL.h tests.cpp)