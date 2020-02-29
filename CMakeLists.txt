###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

option(WITH_TESTS    "Build with tests"             ON)
option(WITH_COVERAGE "Build with code coverage"     OFF)
option(WITH_APPS     "Build with apps"              ON)
option(WITH_EXAMPLES "Build with examples"          ON)
option(WITH_DYNAMIC  "Build with dynamic libraries" OFF)

# top level project
project(PlusIrbis)

# force C++11
set(CMAKE_CXX_STANDARD 11)

# compiler-dependent definitions
if(MSVC)
    add_compile_options(/UTF-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    # g++ or clang++
    add_compile_options("--std=c++11") # for old CMake
    if (WITH_COVERAGE)
      add_compile_options("--coverage")
      add_link_options("--coverage")
    endif()
    if (MINGW)
        add_definitions(-DIRBIS_MINGW)
    endif()
endif()

# headers of irbis and third-party libraries
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/third)

# where artifacts will be gathered
set(ARTIFACTS ${PROJECT_SOURCE_DIR}/artifacts)

# installation settings
install(FILES ${PROJECT_SOURCE_DIR}/include/irbis.h DESTINATION ${ARTIFACTS})
install(FILES ${PROJECT_SOURCE_DIR}/include/rfid.h DESTINATION ${ARTIFACTS})

# subdirectories
add_subdirectory(libs)
if (WITH_TESTS)
    add_subdirectory(tests)
endif()
if (WITH_APPS)
    add_subdirectory(apps)
endif()
