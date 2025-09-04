cmake_minimum_required(VERSION 3.25)
project(UTF8Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources
set(SOURCES
    UTF8.cpp
    test.cpp
)

add_executable(utf8_test ${SOURCES})

# Include current directory for UTF8.h
target_include_directories(utf8_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Compiler and linker flags for Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(utf8_test PRIVATE
        -fsanitize=address,undefined
        -fno-sanitize-recover=all
        -fno-exceptions
        -Wno-deprecated-declarations  # suppress deprecated warnings only
    )
    target_link_options(utf8_test PRIVATE
        -fsanitize=address,undefined
    )
endif()
