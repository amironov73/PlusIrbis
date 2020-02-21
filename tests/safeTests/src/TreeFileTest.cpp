// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

TEST_CASE("TreeFile_parse_1", "[tree]")
{
    auto path = irbis::IO::combinePath (whereTestData(), L"test1.tre");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    const auto lines = irbis::Text::readAnsiLines (path);
    irbis::TreeFile tree;
    tree.parse (lines);
    CHECK (tree.roots.size() == 4);
    CHECK (tree.roots[0].value == L"1 - First");
    CHECK (tree.roots[0].children.empty());
    CHECK (tree.roots[1].value == L"2 - Second");
    CHECK (tree.roots[1].children.size() == 3);
    CHECK (tree.roots[1].children[0].value == L"2.1 - Second first");
    CHECK (tree.roots[1].children[0].children.empty());
}