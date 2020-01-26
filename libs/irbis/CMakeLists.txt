cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

project(irbis)

if(MSVC)
    add_compile_options(/UTF-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

add_definitions(-DPLUSIRBIS_LIBRARY)

FILE(GLOB HeaderFiles
    ${PROJECT_SOURCE_DIR}/include/*.h
)

FILE(GLOB CppFiles
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME}
    ${HeaderFiles}
    ${CppFiles}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
install(FILES ${PROJECT_SOURCE_DIR}/include/irbis.h DESTINATION ${ARTIFACTS})
