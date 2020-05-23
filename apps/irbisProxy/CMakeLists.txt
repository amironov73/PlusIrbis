###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# proxy application
project(irbisProxy)

set(CppFiles
        src/main.cpp
        )

add_executable(${PROJECT_NAME}
        ${CppFiles}
        )

target_link_libraries(${PROJECT_NAME} irbis)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
endif(MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
