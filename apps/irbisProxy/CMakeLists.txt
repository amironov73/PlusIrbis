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

#if(MSVC)
#else(MSVC)
#if(MINGW)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -libws2_32")
#else(MINGW)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#endif(MINGW)
#endif(MSVC)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
