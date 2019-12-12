cmake_minimum_required(VERSION 3.5)

project(irbis)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_definitions(PLUSIRBIS_LIBRARY)
 
FILE(GLOB HFiles *.h)
FILE(GLOB CppFiles *.cpp)

set(SOURCES ${CppFiles} ${HFiles})

add_library(PlusIrbis SHARED ${SOURCES} ${HEADERS} ${UI})
