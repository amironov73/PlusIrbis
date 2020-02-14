cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

project(safeTests)

if(MSVC)
    add_compile_options(/UTF-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

FILE(GLOB CppFiles
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

FILE(GLOB HeaderFiles
    ${PROJECT_SOURCE_DIR}/include/*.h
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
    ${HeaderFiles}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} irbis)

if(IRBIS_MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
    #   target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
endif(IRBIS_MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
