cmake_minimum_required(VERSION 3.23)
project(micro)

set(CMAKE_CXX_STANDARD 11)

add_executable(micro main.cpp micro.h sub.cpp field.cpp utils.cpp internal.h record.cpp status.cpp)
