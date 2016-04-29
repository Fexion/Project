cmake_minimum_required(VERSION 3.4)

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast -march=native" )
add_executable(main Huffman.cpp)
