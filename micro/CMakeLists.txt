cmake_minimum_required(VERSION 2.8.12)
project(micro)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    add_compile_options(/UTF-8 /permissive- /Gy)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

add_executable(micro main.cpp micro.h micro.cpp)
