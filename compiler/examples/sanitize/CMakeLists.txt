cmake_minimum_required(VERSION 3.15)

# cmake .. -DENABLE_UBSAN=ON
# make

project(MyProject CXX)

# Option to enable UBSan
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(ENABLE_UBSAN)
    message(STATUS "UBSan enabled")
    set(SANITIZER_FLAGS "-fsanitize=undefined -g -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZER_FLAGS}")
endif()

# Define the executable
add_executable(my_program main.cpp)

