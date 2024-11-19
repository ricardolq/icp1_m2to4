cmake_minimum_required(VERSION 3.10)
project(Assignment2 LANGUAGES C CXX)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    enable_testing()
    find_package(GTest REQUIRED)
    include(GoogleTest) # include the GoogleTest CMake module
    include_directories(${GTEST_INCLUDE_DIRS})
    # Link our tests code with what we want to test and the GTest and pthread library
    add_executable(assignment_tests tests/tests.cpp)
    target_link_libraries(assignment_tests ${GTEST_LIBRARIES})
    gtest_discover_tests(assignment_tests) # discovers tests by asking the compiled test executable to enumerate its tests
    add_library(assignment_lib src/assignment.c)
else()
    add_executable(assignment src/assignment.c tests/metadata/data.c)
endif()