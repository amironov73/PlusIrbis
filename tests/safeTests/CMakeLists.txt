cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

project(safeTests)

FILE(GLOB CppFiles
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
)

target_link_libraries(${PROJECT_NAME} irbis)

install(TARGETS safeTests DESTINATION ${ARTIFACTS})
