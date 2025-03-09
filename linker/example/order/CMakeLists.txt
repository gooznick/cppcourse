cmake_minimum_required(VERSION 3.24)
project(MyProject LANGUAGES CXX)

add_library(mul STATIC mul.cpp)
add_library(add STATIC add.cpp)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE add mul )

#target_link_libraries(main PRIVATE "$<LINK_GROUP:RESCAN,add,mul>")
