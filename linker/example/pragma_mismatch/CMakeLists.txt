cmake_minimum_required(VERSION 3.15)
project(MismatchDemo LANGUAGES CXX)

add_library(mylib STATIC mylib.cpp)
add_executable(myexe myexe.cpp)
target_link_libraries(myexe PRIVATE mylib)
