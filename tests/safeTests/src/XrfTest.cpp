// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_private.h"
#include "safeTests.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("XrfFile64_readRecord_1", "[xrf]")
{
    auto path = irbis::IO::combinePath (whereIbis(), L"ibis.xrf");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    irbis::XrfFile64 xrf (path, irbis::ReadOnly);
    const auto record = xrf.readRecord (1);
    CHECK (record.offset == 22951100ull);
    CHECK (record.status == irbis::RecordStatus::None);
}

TEST_CASE("XrfFile64_create_1", "[xrf]")
{
    auto path = irbis::IO::combinePath (whereTemp(), L"ibis.xrf");
    irbis::IO::convertSlashes (path);
    if (irbis::IO::fileExist (path)) {
        irbis::IO::deleteFile (path);
    }
    irbis::IO::createFile (path);
    irbis::XrfFile64 xrf (path, irbis::DirectAccessMode::Exclusive);
    irbis::XrfRecord64 record;
    record.offset = 22951100ull;
    xrf.writeRecord(1, record);
    xrf.close();
}