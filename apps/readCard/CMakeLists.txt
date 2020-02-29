###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# simple console smartcard reader
project(readCard)

set(CppFiles
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
)

target_link_libraries(${PROJECT_NAME} irbis rfid)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a libwinscard.a)
endif(MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
