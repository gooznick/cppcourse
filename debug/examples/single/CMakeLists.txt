cmake_minimum_required(VERSION 3.12)

project(Single LANGUAGES CXX)

# CMake: one file — force **no optimization** 
set(src bad.cpp)

if (MSVC)
  set_source_files_properties(
    gm5.cpp PROPERTIES COMPILE_OPTIONS "/Od"
  )
else() # GCC/Clang
  set_source_files_properties(
    gm5.cpp PROPERTIES COMPILE_OPTIONS "-O0"
  )
endif()


add_executable(app main.cpp gm5.cpp)
