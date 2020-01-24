cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

project(rqstShrink)

FILE(GLOB CppFiles
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
)

target_link_libraries(${PROJECT_NAME} irbis)

if(IRBIS_MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
#   target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
endif(IRBIS_MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
