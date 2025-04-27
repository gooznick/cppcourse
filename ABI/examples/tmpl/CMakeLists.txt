cmake_minimum_required(VERSION 3.16)
project(tmpl_shared)

# Set hidden visibility by default
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(tmpl_shared SHARED
    src/tmpl_impl.cpp
)

target_include_directories(tmpl_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)



target_compile_definitions(tmpl_shared PRIVATE BUILDING_TMPL_DLL)


# Read version from "version" file
file(READ "${CMAKE_SOURCE_DIR}/version" TMPL_FULL_VERSION_STRING)
string(STRIP "${TMPL_FULL_VERSION_STRING}" TMPL_FULL_VERSION_STRING)
string(REPLACE "." ";" TMPL_VERSION_LIST ${TMPL_FULL_VERSION_STRING})
list(GET TMPL_VERSION_LIST 0 TMPL_MAJOR_VERSION)
list(GET TMPL_VERSION_LIST 1 TMPL_MINOR_VERSION)
list(GET TMPL_VERSION_LIST 2 TMPL_PATCH_VERSION)

# Write version.h file
configure_file(
  ${CMAKE_SOURCE_DIR}/tmpl_version.h.in
  ${CMAKE_BINARY_DIR}/tmpl_version.h
  @ONLY
)

set_target_properties(tmpl_shared PROPERTIES
    OUTPUT_NAME "tmpl"
    VERSION ${TMPL_FULL_VERSION_STRING}
    SOVERSION ${TMPL_MAJOR_VERSION}
)

include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(test)