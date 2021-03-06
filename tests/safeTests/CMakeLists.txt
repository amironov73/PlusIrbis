###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# offline safe tests
project(safeTests)

set(CppFiles
    src/AlphabetTableTest.cpp
    src/AuthorTest.cpp
    src/BookInfoTest.cpp
    src/ByteNavigatorTest.cpp
    src/ChunkedBufferTest.cpp
    src/ChunkedDataTest.cpp
    src/CodesTest.cpp
    src/ConnectionTest.cpp
    src/DateTest.cpp
    src/DirectAccessTest.cpp
    src/DirectoryTest.cpp
    src/EanTest.cpp
    src/EmbeddedFieldTest.cpp
    src/EncodingTest.cpp
    src/ExemplarTest.cpp
    src/FastBufferTest.cpp
    src/FastPImplTest.cpp
    src/FileTest.cpp
    src/FileSpecificationTest.cpp
    src/FormatTest.cpp
    src/FoundLineTest.cpp
    src/FrugalTest.cpp
    src/GblTest.cpp
    src/IlfTest.cpp
    src/IniTest.cpp
    src/IOTest.cpp
    src/IsbnTest.cpp
    src/Iso2709Test.cpp
    src/JoinedDataTest.cpp
    src/main.cpp
    src/MarcRecordTest.cpp
    src/MaybeTest.cpp
    src/MemoryPoolTest.cpp
    src/MenuTest.cpp
    src/NotNullTest.cpp
    src/NumberTextTest.cpp
    src/OptFileTest.cpp
    src/OptionalTest.cpp
    src/ParFileTest.cpp
    src/PointerGuardTest.cpp
    src/PostingTest.cpp
    src/ProcessInfoTest.cpp
    src/RangeTest.cpp
    src/ReaderTest.cpp
    src/RecordFieldTest.cpp
    src/ResultTest.cpp
    src/RetryTest.cpp
    src/SearchTest.cpp
    src/SpanTest.cpp
    src/SubFieldTest.cpp
    src/TermInfoTest.cpp
    src/TextNavigatorTest.cpp
    src/TextTest.cpp
    src/TitleTest.cpp
    src/TreeFileTest.cpp
    src/UpcTest.cpp
    src/UserInfoTest.cpp
    src/UtilsTest.cpp
    src/VersionTest.cpp
    src/VisitTest.cpp
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

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
