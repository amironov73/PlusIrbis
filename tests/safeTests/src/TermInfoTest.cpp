// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("TermInfo_constructor_1", "[term]")
{
    irbis::TermInfo term;
    CHECK (term.count == 0);
    CHECK (term.text.empty());
}

TEST_CASE("TermInfo_parse_1", "[term]")
{
    irbis::StringList lines { L"1#First", L"", L"2#Second", L"3#", L"" };
    const auto terms = irbis::TermInfo::parse (lines);
    REQUIRE (terms.size() == 3);
    CHECK (terms[0].count == 1);
    CHECK (terms[0].text == L"First");
    CHECK (terms[1].count == 2);
    CHECK (terms[1].text == L"Second");
    CHECK (terms[2].count == 3);
    CHECK (terms[2].text.empty());
}

TEST_CASE("TermInfo_toString_1", "[term]")
{
    irbis::TermInfo term;
    CHECK (term.toString() == L"0#");

    term.count = 123;
    term.text = L"Hello";
    CHECK (term.toString() == L"123#Hello");
}