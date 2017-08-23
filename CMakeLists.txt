cmake_minimum_required(VERSION 2.6)

project(bsim)

INCLUDE_DIRECTORIES(./src/)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I/opt/local/include -O2 -Werror -Wall")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(./include/)

SET(TEST_FILES ./test/bitvector_tests.cpp)

add_executable(all-tests ${TEST_FILES})
