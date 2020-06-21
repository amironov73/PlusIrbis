###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# headers only for easy template programming
project(headersOnly)

set(CppFiles
    src/main.cpp
    src/mmap.cpp
    src/ByteRange.cpp
    src/TextRange.cpp
    src/System.cpp
    src/BusyState.cpp
    src/Network.cpp
    src/SecretString.cpp
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
)

target_include_directories(${PROJECT_NAME} PRIVATE ../../libs/irbis/include)

# no artifacts needed
