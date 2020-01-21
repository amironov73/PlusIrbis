project(hello)

FILE(GLOB CppFiles
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
)

target_link_libraries(${PROJECT_NAME} irbis)
