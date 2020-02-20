###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# library for rfid reader interaction
project(rfid)

add_definitions(-DPLUSIRBIS_LIBRARY)

set(CppFiles
    src/rfid.cpp
    src/WinSmartCards.cpp
)

add_library(${PROJECT_NAME}
    ${HeaderFiles}
    ${CppFiles}
)

target_link_libraries(${PROJECT_NAME} irbis)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
