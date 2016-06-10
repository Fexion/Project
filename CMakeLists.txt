cmake_minimum_required(VERSION 3.2)
project(Huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -std=c++14")



set(SOURCES
    main.cpp
    huffman.cpp)

set(HEADERS
    huffman.h)

add_executable(Huffman ${SOURCES})
