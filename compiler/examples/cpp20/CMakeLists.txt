cmake_minimum_required(VERSION 3.16)
project(MyCpp20Project LANGUAGES CXX)

add_executable(my_program main.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_compile_features(my_program PUBLIC cxx_std_20)