// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

TEST_CASE ("ParFile_assign_1", "[par]")
{
    irbis::ParFile par;
    par.assign (L"IBIS");
    CHECK (par.xrf == L"IBIS");
    const auto dictionary = par.toDictionary();
    CHECK (dictionary.at(1) == L"IBIS");
}

TEST_CASE("ParFile_parse_1", "[par]")
{
    auto path = irbis::IO::combinePath (whereDatai(), L"ibis.par");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    const auto lines = irbis::Text::readAnsiLines (path);
    irbis::ParFile par;
    par.parse (lines);
    CHECK (par.mst == L".\\datai\\ibis\\");
}