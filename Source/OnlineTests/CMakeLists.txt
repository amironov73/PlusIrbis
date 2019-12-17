cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

project(OnlineTests)

FILE(GLOB HFiles *.h)
FILE(GLOB CppFiles *.cpp)

set(SOURCES ${CppFiles} ${HFiles})

add_library(PlusIrbis SHARED IMPORTED)
set_target_properties(PlusIrbis PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/../irbis/cmake-build-debug/libPlusIrbis.dylib )
target_link_libraries(PlusIrbis)

add_executable(OnlineTests ${SOURCES} ${HEADERS})
