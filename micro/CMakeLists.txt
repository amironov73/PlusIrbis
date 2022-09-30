cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(micro)

set(CMAKE_CXX_STANDARD 11)

add_executable(micro main.cpp micro.h sub.cpp field.cpp utils.cpp internal.h record.cpp status.cpp query.cpp connect.cpp response.cpp socket.cpp)
