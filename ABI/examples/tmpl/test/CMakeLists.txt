cmake_minimum_required(VERSION 3.16)

project(tmpl_shared_test)


# Build the test
add_executable(test_tmpl_api
    tmpl_test.cpp
)

target_link_libraries(test_tmpl_api
    tmpl_shared
)

