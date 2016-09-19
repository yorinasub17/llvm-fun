cmake_minimum_required (VERSION 2.6)


# Whether or not to build tests 
option(test "Build all tests." OFF)


# Build with warnings
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++14 -pthread")


#######################
# Required dependencies
#######################
set(CMAKE_PREFIX_PATH "/usr/lib/llvm-3.8")
set(LLVM_CMAKE_DIR "/usr/lib/llvm-3.8/share/llvm/cmake")
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
# target_link_libraries(simple-tool ${llvm_libs})


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
    add_executable(runUnitTests test/main.cpp test/testlexer/testlexer.cpp test/testparser/testparser.cpp)
    target_link_libraries(runUnitTests gtest gtest_main)
    target_link_libraries(runUnitTests kaleidoscope_lexer)
    target_link_libraries(runUnitTests kaleidoscope_parser)
    target_link_libraries(runUnitTests ${llvm_libs})

    add_test(GetTokenTest runUnitTests)
    add_test(ParserTest runUnitTests)
endif()
