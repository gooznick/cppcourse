cmake_minimum_required(VERSION 3.16)

project(myproject)
add_executable(my_program regular.cpp special.cpp)

set_source_files_properties(special.cpp PROPERTIES COMPILE_FLAGS "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)