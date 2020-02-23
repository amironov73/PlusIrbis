// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Date_constructor_1", "[date]")
{
    const irbis::Date date (2020, 1, 13);
    CHECK (date.date.tm_year == 120);
    CHECK (date.date.tm_mon == 0);
    CHECK (date.date.tm_mday == 13);
    CHECK (date.text == L"20200113");
}

TEST_CASE("Date_constructor_2", "[date]")
{
    const irbis::Date date (L"20200113");
    CHECK (date.date.tm_year == 120);
    CHECK (date.date.tm_mon == 0);
    CHECK (date.date.tm_mday == 13);
    CHECK (date.text == L"20200113");
}

TEST_CASE ("Date_constructor_3", "[date]")
{
    struct tm t { 0 };
    t.tm_year = 120;
    t.tm_mon = 0;
    t.tm_mday = 13;
    const irbis::Date date (&t);
    CHECK (date.date.tm_year == 120);
    CHECK (date.date.tm_mon == 0);
    CHECK (date.date.tm_mday == 13);
    CHECK (date.text == L"20200113");
}

TEST_CASE("Date_convert_1", "[date]")
{
    struct tm t { 0 };
    t.tm_year = 120;
    t.tm_mon = 0;
    t.tm_mday = 13;
    CHECK (irbis::Date::convert (&t) == L"20200113");
}

TEST_CASE("Date_parse_1", "[date]")
{
    struct tm t { 0 };
    irbis::String s (L"20200113");
    CHECK (irbis::Date::parse (s, &t));
    CHECK (t.tm_year == 120);
    CHECK (t.tm_mon == 0);
    CHECK (t.tm_mday == 13);
}

TEST_CASE("Date_today_1", "[date]")
{
    const irbis::Date date = irbis::Date::today();
    CHECK (date.text.size() == 8);
}
