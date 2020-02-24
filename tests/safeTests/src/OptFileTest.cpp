// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

TEST_CASE("OptFile_parse_1", "[opt]")
{
    auto path = irbis::IO::combinePath (whereTestData(), L"ws32.opt");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    const auto lines = irbis::Text::readAnsiLines (path);
    irbis::OptFile opt;
    opt.parse (lines);
    CHECK (opt.lines.size()   == 13);
    CHECK (opt.lines[0].key   == L"PAZK");
    CHECK (opt.lines[0].value == L"PAZK42");
}
