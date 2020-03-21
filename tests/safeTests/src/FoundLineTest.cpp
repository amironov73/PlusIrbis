// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("FoundLine_constructor_1", "[found]")
{
    irbis::FoundLine found;
    CHECK (found.mfn == 0);
    CHECK (found.description.empty());
}

TEST_CASE("FoundLine_constructor_2", "[found]")
{
    irbis::FoundLine found (123, L"Value");
    CHECK (found.mfn == 123);
    CHECK (found.description == L"Value");
}

TEST_CASE("FoundLine_parse_1", "[found]")
{
    irbis::FoundLine found;
    found.parse (L"123#Some text");
    CHECK (found.mfn == 123);
    CHECK (found.description == L"Some text");
}

TEST_CASE("FoundLine_parse_2", "[found]")
{
    irbis::FoundLine found;
    found.parse (L"123#");
    CHECK (found.mfn == 123);
    CHECK (found.description.empty());
}

TEST_CASE("FoundLine_parse_3", "[found]")
{
    irbis::FoundLine found;
    found.parse (L"123");
    CHECK (found.mfn == 123);
    CHECK (found.description.empty());
}

