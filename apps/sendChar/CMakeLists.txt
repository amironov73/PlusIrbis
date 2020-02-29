###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# my own SendChar
project(sendChar)

set(CppFiles
    src/main.cpp
)

set(RcFiles
    res/resources.rc
)

add_executable(${PROJECT_NAME} WIN32
    ${CppFiles}
    ${RcFiles}
)

target_link_libraries(${PROJECT_NAME} irbis rfid)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a libwinscard.a)
endif(MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
