cmake_minimum_required(VERSION 3.10)
project(GMockDemo)

# 1. Find GTest ONLY (It often brings GMock along)
find_package(GTest REQUIRED)

add_executable(run_tests main.cpp)

# 2. Check if GTest::gmock target exists
if(TARGET GTest::gmock)
    target_link_libraries(run_tests GTest::gtest_main GTest::gmock)
else()
    # Fallback: Sometimes it's just raw libraries on older systems
    target_link_libraries(run_tests ${GTEST_LIBRARIES} gmock) 
endif()