cmake_minimum_required(VERSION 3.20)
project(GraphDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-class-memaccess -Wno-unused-result")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

add_subdirectory(graphDB)
add_subdirectory(cli)
add_subdirectory(benchmark)

find_package(GTest)
if (GTEST_FOUND)
    add_subdirectory(test)
else ()
    message(STATUS "Could not find googletest; testing not possible")
endif ()
