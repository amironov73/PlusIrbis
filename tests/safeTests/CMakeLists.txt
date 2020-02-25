###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# offline safe tests
project(safeTests)

set(CppFiles
    src/AlphabetTableTest.cpp
    src/ByteNavigatorTest.cpp
    src/DateTest.cpp
    src/EanTest.cpp
    src/EncodingTest.cpp
    src/ExemplarTest.cpp
    src/FileSpecificationTest.cpp
    src/FormatTest.cpp
    src/IOTest.cpp
    src/IsbnTest.cpp
    src/Iso2709Test.cpp
    src/main.cpp
    src/MarcRecordTest.cpp
    src/OptFileTest.cpp
    src/ParFileTest.cpp
    src/PointerGuardTest.cpp
    src/RecordFieldTest.cpp
    src/ResultTest.cpp
    src/SearchTest.cpp
    src/SpanTest.cpp
    src/SubFieldTest.cpp
    src/TermInfoTest.cpp
    src/TermPostingTest.cpp
    src/TextNavigatorTest.cpp
    src/TextTest.cpp
    src/TreeFileTest.cpp
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
