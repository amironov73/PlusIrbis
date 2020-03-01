###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# shared network client library
project(irbis_shared)

include(../common/irbis.cmake)

add_library(${PROJECT_NAME}
    SHARED
    ${HeaderFiles}
    ${CppFiles}
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

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
