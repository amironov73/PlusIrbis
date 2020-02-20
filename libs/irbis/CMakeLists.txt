###########################################################
# PlusIrbis project
# Alexey Mironov, 2018-2020
###########################################################

# network client library
project(irbis)

add_definitions(-DPLUSIRBIS_LIBRARY)

set(HeaderFiles
    include/irbis_private.h
)

set(CppFiles
    src/AlphabetTable.cpp
    src/ByteNavigator.cpp
    src/ChunkedBuffer.cpp
    src/ClientQuery.cpp
    src/ClientSocket.cpp
    src/Connection.cpp
    src/ConnectionAdmin.cpp
    src/ConnectionBase.cpp
    src/ConnectionContext.cpp
    src/ConnectionFactory.cpp
    src/ConnectionFull.cpp
    src/ConnectionLite.cpp
    src/ConnectionPhantom.cpp
    src/ConnectionSearch.cpp
    src/DatabaseInfo.cpp
    src/Date.cpp
    src/DirectAccess.cpp
    src/Ean.cpp
    src/EmbeddedField.cpp
    src/Encoding.cp1251.cpp
    src/Encoding.cp866.cpp
    src/Encoding.cpp
    src/Encoding.koi8r.cpp
    src/Encoding.utf8.cpp
    src/Exemplar.cpp
    src/File.cpp
    src/FileSpecification.cpp
    src/GblResult.cpp
    src/IlfFile.cpp
    src/IniFile.cpp
    src/IO.cpp
    src/irbis.cpp
    src/Isbn.cpp
    src/Iso2709.cpp
    src/Lite.cpp
    src/MarcRecord.cpp
    src/MemoryChunk.cpp
    src/MenuEntry.cpp
    src/MenuFile.cpp
    src/MstControlRecord64.cpp
    src/MstDictionaryEntry64.cpp
    src/MstFile64.cpp
    src/MstRecord64.cpp
    src/MstRecordLeader64.cpp
    src/NumberChunk.cpp
    src/OptFile.cpp
    src/ParFile.cpp
    src/Phantom.cpp
    src/ProcessInfo.cpp
    src/RawRecord.cpp
    src/RecordField.cpp
    src/RecordSerializer.cpp
    src/Search.cpp
    src/ServerResponse.cpp
    src/ServerStat.cpp
    src/Span.cpp
    src/SubField.cpp
    src/Tcp4Socket.cpp
    src/TermInfo.cpp
    src/TermPosting.cpp
    src/Text.cpp
    src/TextNavigator.cpp
    src/TreeFile.cpp
    src/TreeNode.cpp
    src/Upc.cpp
    src/UserInfo.cpp
    src/Version.cpp
    src/XrfFile64.cpp
    src/XrfRecord64.cpp
)

add_library(${PROJECT_NAME}
    ${HeaderFiles}
    ${CppFiles}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})
