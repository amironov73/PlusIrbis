###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# headers only for easy template programming
project(headersOnly)

set(CppFiles
    src/main.cpp
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
)

target_include_directories(${PROJECT_NAME} PRIVATE ../../libs/irbis/include)

# no artifacts needed
