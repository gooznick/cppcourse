cmake_minimum_required(VERSION 3.10)
project(abi_structs_demo)

add_executable(abi_structs_demo abi_structs_demo.cpp)

# Enable MSVC struct layout report
if(MSVC)
    set_source_files_properties(abi_structs_demo.cpp PROPERTIES COMPILE_FLAGS "/d1reportSingleClassLayoutFlagsPadded")
endif()

# Enable warning flags for GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(abi_structs_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
