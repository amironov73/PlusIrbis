###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# shared library for rfid reader interaction
project(rfid_shared)

include(../common/rfid.cmake)

add_library(${PROJECT_NAME}
    SHARED
    ${CppFiles}
    src/main.cpp
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    -DPLUSIRBIS_LIBRARY
    -DIRBIS_DYNAMIC_LIBRARY
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/../irbis/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    irbis
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
