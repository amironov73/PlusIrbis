cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

project(sendChar)

FILE(GLOB CppFiles
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

FILE(GLOB RcFiles
    ${PROJECT_SOURCE_DIR}/res/*.rc
)

add_executable(${PROJECT_NAME} WIN32
    ${CppFiles}
    ${RcFiles}
)

target_link_libraries(${PROJECT_NAME} irbis rfid)

if(IRBIS_MINGW)
    #add_compile_options(-static -static-libgcc -static-libstdc++)
    target_link_libraries(${PROJECT_NAME} libws2_32.a libwinscard.a)
    #set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif(IRBIS_MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
