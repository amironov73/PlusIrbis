###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# offline safe tests
project(safeTests)

set(CppFiles
    src/ByteNavigatorTest.cpp
    src/DateTest.cpp
    src/EanTest.cpp
    src/EncodingTest.cpp
    src/ExemplarTest.cpp
    src/FormatTest.cpp
    src/IOTest.cpp
    src/IsbnTest.cpp
    src/main.cpp
    src/MarcRecordTest.cpp
    src/RecordFieldTest.cpp
    src/ResultTest.cpp
    src/SpanTest.cpp
    src/SubFieldTest.cpp
    src/TextNavigatorTest.cpp
    src/TextTest.cpp
    src/UpcTest.cpp
    src/UtilsTest.cpp
    src/XrfTest.cpp
)

set(HeaderFiles
    include/safeTests.h
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
    ${HeaderFiles}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
)

target_link_libraries(${PROJECT_NAME} irbis)

if(IRBIS_MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
endif(IRBIS_MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
