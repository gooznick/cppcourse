cmake_minimum_required(VERSION 3.10)
project(defs)

add_executable(defs add_one.cpp main.cpp)

# target_compile_definitions(defs PRIVATE 
#     $<$<TARGET_SOURCE_FILE:defs,add_one.cpp>:ADD_ONE_DEBUG>
# )

set_source_files_properties(add_one.cpp PROPERTIES COMPILE_DEFINITIONS "ADD_ONE_DEBUG")
