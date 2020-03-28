// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("NumberText_constructor_1", "[number]")
{
    irbis::NumberText number;
    CHECK (number.empty());
}

TEST_CASE("NumberText_append_1", "[number]")
{
    irbis::NumberText number;
    number.append (L"prefix", 123);
    CHECK (number.size() == 1);
    CHECK (number.lastIndex() == 0);
    const auto &front = number.front();
    CHECK (front.havePrefix());
    CHECK (front.haveValue);
    const auto &back = number.back();
    CHECK (front.havePrefix());
    CHECK (front.haveValue);
}

//TEST_CASE("NumberText_append_2", "[number]")
//{
//    irbis::NumberText number;
//    irbis::String prefix (L"prefix");
//    number.append (std::move (prefix), 123);
//    CHECK (number.size() == 1);
//    CHECK (number.lastIndex() == 0);
//}

TEST_CASE("NumberText_append_3", "[number]")
{
    irbis::NumberText number;
    number.append (L"first", 123);
    number.append (L"second", 321);
    CHECK (number.size() == 2);
    CHECK (number.lastIndex() == 1);
}

TEST_CASE("NumberText_increment_1", "[number]")
{
    irbis::NumberText number;
    number.append (L"prefix", 123);
    number.increment();
    CHECK (number.chunks.back().value == 124);
}

TEST_CASE("NumberText_parse_1", "[number]")
{
    auto number = irbis::NumberText::parse (L"");
    REQUIRE (number.empty());
}

TEST_CASE("NumberText_parse_2", "[number]")
{
    auto number = irbis::NumberText::parse (L"prefix123");
    REQUIRE (number.size() == 1);
    const auto &chunk = number.front();
    CHECK (chunk.prefix == L"prefix");
    CHECK (chunk.havePrefix());
    CHECK (chunk.value == 123);
    CHECK (chunk.haveValue);
}

TEST_CASE("NumberText_parse_3", "[number]")
{
    auto number = irbis::NumberText::parse (L"123suffix");
    REQUIRE (number.size() == 2);
    auto &chunk = number.front();
    CHECK_FALSE (chunk.havePrefix());
    CHECK (chunk.value == 123);
    CHECK (chunk.haveValue);
    chunk = number.back();
    CHECK (chunk.prefix == L"suffix");
    CHECK_FALSE (chunk.haveValue);
}

TEST_CASE("NumberText_parse_4", "[number]")
{
    auto number = irbis::NumberText::parse (L"123");
    REQUIRE (number.size() == 1);
    auto &chunk = number.front();
    CHECK_FALSE (chunk.havePrefix());
    CHECK (chunk.value == 123);
    CHECK (chunk.haveValue);
}

TEST_CASE("NumberText_parse_5", "[number]")
{
    auto number = irbis::NumberText::parse (L"suffix");
    REQUIRE (number.size() == 1);
    auto &chunk = number.front();
    CHECK (chunk.havePrefix());
    CHECK_FALSE (chunk.haveValue);
    CHECK (chunk.prefix == L"suffix");
}

TEST_CASE("NumberText_toString_1", "[number]")
{
    irbis::NumberText number;
    CHECK (number.toString().empty());
    number.append (L"prefix", 123);
    CHECK (number.toString() == L"prefix123");
}

TEST_CASE("NumberText_toString_2", "[number]")
{
    irbis::NumberText number;
    number.append (123);
    CHECK (number.toString() == L"123");
    number.append (456);
    CHECK (number.toString() == L"123456");
    number.back().prefix = L"|";
    CHECK (number.toString() == L"123|456");
}
