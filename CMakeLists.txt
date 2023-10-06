cmake_minimum_required(VERSION 3.0.0)
project(wordhunt VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

add_executable(wordhunt main.cpp find.cpp trie.hpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
