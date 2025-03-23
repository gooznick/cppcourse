cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)


add_library(mul SHARED mul.cpp)
add_library(add SHARED add.cpp)
add_library(pow SHARED pow.cpp)

# CMAKE_INSTALL_RPATH
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

#set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
#set(CMAKE_SKIP_RPATH TRUE)
#set(CMAKE_SKIP_BUILD_RPATH TRUE)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE pow mul add  )

set_target_properties(main PROPERTIES
  INSTALL_RPATH "$ORIGIN"
  SKIP_BUILD_RPATH ON
)

# Install targets
install(TARGETS main add mul pow
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin)