// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("Iso2709_readRecord_1", "[iso]")
{
    auto path = irbis::IO::combinePath (whereTestData(), L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    irbis::Encoding *encoding = irbis::Encoding::ansi();
    REQUIRE (encoding != nullptr);
    irbis::File file (path, L"rb");
    irbis::MarcRecord *record = irbis::Iso2709::readRecord (&file, encoding);
    CHECK (record != nullptr);
    CHECK (record->fields.size() == 16);
    delete (record);
    record = irbis::Iso2709::readRecord (&file, encoding);
    CHECK (record != nullptr);
    CHECK (record->fields.size() == 15);
    delete (record);
}

TEST_CASE("Iso2709_writeRecord_1", "[iso]")
{
    irbis::Encoding *encoding = irbis::Encoding::ansi();
    REQUIRE (encoding != nullptr);
    auto path = irbis::IO::combinePath (whereTemp(), L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    if (irbis::IO::fileExist (path)) {
        irbis::IO::deleteFile (path);
    }
    irbis::IO::createFile (path);
    {
        irbis::File file(path, L"wb");
        irbis::MarcRecord record;
        record.add(200, L"").add(L'a', L"Title");
        record.add(300, L"Comment");
        irbis::Iso2709::writeRecord(&file, record, encoding);
    }
    CHECK (irbis::IO::getFileSize(path) == 70);
}