cmake_minimum_required(VERSION 3.15)
project(tpl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Make local installation
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix" FORCE)

# - Define shared library
add_library(tpl SHARED src/tpl.cpp)
target_include_directories(tpl PUBLIC include)

# - Set visibility and define `TPL_BUILD` for exports
target_compile_definitions(tpl PRIVATE TPL_BUILD)

# - Apply versioning and symbol export control
set_target_properties(tpl PROPERTIES
    VERSION ${PROJECT_VERSION}              # defined in project()
    SOVERSION 1                             # Creates libtpl.so -> libtpl.so.1
    PUBLIC_HEADER "include/tpl/tpl.h" 
    CXX_VISIBILITY_PRESET hidden            # Hides all symbols except explicitly exported ones
    VISIBILITY_INLINES_HIDDEN ON            # Hides inline functions 
)

# - Apply Linux version script (symbol export control)
if(UNIX)
    target_link_options(tpl PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/src/tpl_version.script")
endif()


# - Installation instructions
install(TARGETS tpl
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/tpl
)

# - TEST EXECUTABLE 
add_executable(test_tpl test/test_tpl.cpp)
target_include_directories(test_tpl PRIVATE include)
target_link_libraries(test_tpl PRIVATE tpl)

# Make the test search for so's in current directory
set_target_properties(test_tpl PROPERTIES INSTALL_RPATH "$ORIGIN")

# - Install test (in lib directory)
install(TARGETS test_tpl RUNTIME DESTINATION lib)

