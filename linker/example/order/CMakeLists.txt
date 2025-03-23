cmake_minimum_required(VERSION 3.24)
project(MyProject LANGUAGES CXX)

add_library(mul STATIC mul.cpp)
add_library(add STATIC add.cpp)
add_library(pow STATIC pow.cpp)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE add mul pow)
# /usr/bin/ld: libpow.a(pow.cpp.o): in function `pow(int, int)':
# pow.cpp:(.text+0x33): undefined reference to `mul(int, int)'


## Solution 1:
#target_link_libraries(main PRIVATE pow mul add)

## Solution 2:
#target_link_libraries(main PRIVATE "$<LINK_GROUP:RESCAN,add,mul,pow>")
