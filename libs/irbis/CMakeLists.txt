project(irbis)

add_definitions(-DPLUSIRBIS_LIBRARY -D_CRT_SECURE_NO_WARNINGS)

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
