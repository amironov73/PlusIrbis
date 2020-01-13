cmake_minimum_required(VERSION 2.8)

project(irbis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add_compile_definitions(PLUSIRBIS_LIBRARY)
add_definitions(-DPLUSIRBIS_LIBRARY -D_CRT_SECURE_NO_WARNINGS)
 
FILE(GLOB HFiles *.h)
FILE(GLOB CppFiles *.cpp)

set(SOURCES ${CppFiles} ${HFiles})

add_library(irbis SHARED ${SOURCES} ${HEADERS} ${UI})
