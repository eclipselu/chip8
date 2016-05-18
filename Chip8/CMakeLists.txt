cmake_minimum_required(VERSION 3.3)
project(Chip8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    chip8.cpp
    chip8.h
    main.cpp)

add_executable(Chip8 ${SOURCE_FILES})