cmake_minimum_required (VERSION 2.6)


# Whether or not to build tests 
option(test "Build all tests." OFF)


# Build with warnings
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++14 -pthread")


################################
# Normal Libraries & Executables
################################
include_directories("src")
add_subdirectory("src")


################################
# Testing
################################
if (test)
    # Find gtest library
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Unit tests
    add_executable(runUnitTests test/testlexer/testlexer.cpp)
    target_link_libraries(runUnitTests gtest gtest_main)
    target_link_libraries(runUnitTests kaleidoscope_lexer)

    add_test(GetTokenTest runUnitTests)
endif()
