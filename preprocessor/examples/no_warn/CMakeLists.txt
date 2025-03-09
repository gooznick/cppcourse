cmake_minimum_required(VERSION 3.15)

project(no_warn)
add_executable(no_warn main.cpp)

# try with and without "SYSTEM"
target_include_directories(no_warn SYSTEM PUBLIC system)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(no_warn PRIVATE -Wall -Wextra -Wpedantic)
elseif (MSVC)
    target_compile_options(no_warn PRIVATE /W4 /WX)
endif()