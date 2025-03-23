cmake_minimum_required(VERSION 3.10)
project(BoostDLLExample)


# Shared library
add_library(gm5 SHARED gm5.cpp)

# Executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE boost_filesystem dl)