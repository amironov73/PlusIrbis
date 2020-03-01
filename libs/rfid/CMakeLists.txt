###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# library for rfid reader interaction
project(rfid)

include(../common/rfid.cmake)

add_library(${PROJECT_NAME}
    STATIC
    ${CppFiles}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    -DPLUSIRBIS_LIBRARY
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
