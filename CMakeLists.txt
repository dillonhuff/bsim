cmake_minimum_required(VERSION 2.6)

project(bsim)

INCLUDE_DIRECTORIES(./src/)

SET(EXTRA_CXX_COMPILE_FLAGS "-std=c++11 -I./src -I./test -I/opt/local/include -O2 -Werror -Wall -pedantic -fno-strict-aliasing")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(./src/)

SET(TEST_FILES ./test/bitvector_tests.cpp
	       ./test/dynamic_bitvector_tests.cpp
               ./test/quad_value_bv_tests.cpp
               ./test/static_quad_value_bv_tests.cpp)

add_executable(all-tests ${TEST_FILES})
add_executable(quad-value-tests ./test/quad_value_bv_tests.cpp ./test/test_qv_main.cpp)
