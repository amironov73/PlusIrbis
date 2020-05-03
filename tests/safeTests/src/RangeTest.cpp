// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

#include <algorithm>

TEST_CASE("makeRange_1", "[range]")
{
    std::vector<int> v;
    auto range = irbis::makeRange (v);
    CHECK (range.empty());
    CHECK_FALSE (range.moveNext());
}

TEST_CASE("makeRange_2", "[range]")
{
    std::vector<int> v { 1, 2, 3 };
    auto range = irbis::makeRange (v);
    CHECK_FALSE (range.empty());
    CHECK (*range.current() == 1);
    CHECK (range.moveNext());
    CHECK (*range.current() == 2);
    CHECK (range.moveNext());
    CHECK (*range.current() == 3);
    CHECK_FALSE (range.moveNext());
    CHECK_FALSE (range.moveNext());
}

TEST_CASE("makeRange_3", "[range]")
{
    std::vector<int> v { 1, 2, 3 };
    auto range = irbis::makeRange (v);
    const auto c = std::count (std::begin (range), std::end (range), 1);
    CHECK (c == 1);
}

TEST_CASE("Range_reset_1", "[range]")
{
    std::vector<int> v { 1, 2, 3 };
    auto range = irbis::makeRange (v);
    CHECK_FALSE (range.empty());
    CHECK (*range.current() == 1);
    CHECK (range.moveNext());
    range.reset();
    CHECK (*range.current() == 1);
}

TEST_CASE("Range_value_1", "[range]")
{
    std::vector<int> v { 1, 2, 3 };
    auto range = irbis::makeRange (v);
    CHECK (range.value() == 1);
    CHECK (range.moveNext());
    CHECK (range.value() == 2);
    CHECK (range.moveNext());
    CHECK (range.value() == 3);
    CHECK_FALSE (range.moveNext());
}
