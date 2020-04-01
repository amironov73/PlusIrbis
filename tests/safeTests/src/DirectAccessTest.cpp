// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_internal.h"
#include "safeTests.h"

TEST_CASE("DirectAccess_readRecord_1", "[directAccess]")
{
    auto systemPath = whereIrbis64();
    irbis::IO::convertSlashes (systemPath);
    auto parPath = irbis::IO::combinePath (whereDatai(), L"ibis.par");
    irbis::IO::convertSlashes (parPath);
    REQUIRE (irbis::IO::fileExist (parPath));
    irbis::DirectAccess64 access (parPath, systemPath);
    REQUIRE (access.mst != nullptr);
    REQUIRE (access.xrf != nullptr);
}
