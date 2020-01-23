#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("sameChar_1", "[utils]")
{
    CHECK(irbis::sameChar(L'a', L'A'));
    CHECK(irbis::sameChar(L'a', L'a'));
    CHECK_FALSE(irbis::sameChar(L'1', L'2'));
    CHECK(irbis::sameChar(L'1', L'1'));
}

TEST_CASE("sameString_1", "[utils]")
{
    CHECK(irbis::sameString(L"Hello", L"HELLO"));
    CHECK(irbis::sameString(L"Hello", L"Hello"));
    CHECK(irbis::sameString(L"Hello", L"hello"));
    CHECK_FALSE(irbis::sameString(L"123", L"321"));
    CHECK_FALSE(irbis::sameString(L"123", L"1234"));
    CHECK(irbis::sameString(L"123", L"123"));
}

TEST_CASE("toLower_1", "[utils]")
{
    irbis::String text(L"Hello");
    CHECK(irbis::toLower(text) == L"hello");
    CHECK(irbis::toLower(text) == L"hello");
}

TEST_CASE("toUpper_1", "[utils]")
{
    irbis::String text(L"Hello");
    CHECK(irbis::toUpper(text) == L"HELLO");
    CHECK(irbis::toUpper(text) == L"HELLO");
}

TEST_CASE("iif_1", "[utils]")
{
    const std::string s1 = "Hello", s2 = "World";
    const auto s3 = irbis::iif(s1, s2);
    CHECK(s1 == s3);
}

TEST_CASE("iif_2", "[utils]")
{
    const std::string s1, s2 = "World";
    const auto s3 = irbis::iif(s1, s2);
    CHECK(s2 == s3);
}

TEST_CASE("iif_3", "[utils]")
{
    const std::string s1 = "Hello", s2 = "World", s3 = "Again";
    const auto s4 = irbis::iif(s1, s2, s3);
    CHECK(s1 == s4);
}

TEST_CASE("iif4_1", "[utils]")
{
    const std::string s1, s2 = "World", s3 = "Again";
    const auto s4 = irbis::iif(s1, s2, s3);
    CHECK(s2 == s4);
}

TEST_CASE("iif_5", "[utils]")
{
    const std::string s1, s2, s3 = "Again";
    const auto s4 = irbis::iif(s1, s2, s3);
    CHECK(s3 == s4);
}

TEST_CASE("safeAt_1", "[utils]")
{
    irbis::StringList v{ L"1", L"3", L"5" };
    const auto i = irbis::safeAt(v, 1);
    CHECK(i == L"3");
}

TEST_CASE("safeAt_2", "[utils]")
{
    irbis::StringList v{L"1", L"3", L"5"};
    const auto i = irbis::safeAt(v, 11);
    CHECK(i == irbis::String());
}

TEST_CASE("maxSplit_1", "[utils]")
{
    irbis::String text(L"Hello,world,again");
    const auto a = irbis::maxSplit(text, ',', 2);
    irbis::StringList ::size_type expected = 2;
    CHECK(expected == a.size());
    CHECK(a[0] == L"Hello");
    CHECK(a[1] == L"world,again");
}

TEST_CASE("maxSplit_2", "[utils]")
{
    irbis::String text(L"Hello,world,again");
    const auto a = irbis::maxSplit(text, ',', 3);
    irbis::StringList ::size_type expected = 3;
    CHECK(expected == a.size());
    CHECK(a[0] == L"Hello");
    CHECK(a[1] == L"world");
    CHECK(a[2] == L"again");
}
