cmake_minimum_required(VERSION 3.16)
project(mini_compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mini_compiler
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/compiler.cpp
)
target_include_directories(mini_compiler PRIVATE include)

if (MSVC)
    target_compile_options(mini_compiler PRIVATE /W4)
else()
    target_compile_options(mini_compiler PRIVATE -Wall -Wextra -Wpedantic)
endif()
