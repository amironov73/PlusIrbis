// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("sameChar_1", "[utils]")
{
    CHECK       (irbis::sameChar (L'a', L'A'));
    CHECK       (irbis::sameChar (L'a', L'a'));
    CHECK_FALSE (irbis::sameChar (L'1', L'2'));
    CHECK       (irbis::sameChar (L'1', L'1'));
}

TEST_CASE("sameString_1", "[utils]")
{
    CHECK       (irbis::sameString (L"Hello", L"HELLO"));
    CHECK       (irbis::sameString (L"Hello", L"Hello"));
    CHECK       (irbis::sameString (L"Hello", L"hello"));
    CHECK_FALSE (irbis::sameString (L"123",   L"321"));
    CHECK_FALSE (irbis::sameString (L"123",   L"1234"));
    CHECK       (irbis::sameString (L"123",   L"123"));
}

TEST_CASE("toLower_1", "[utils]")
{
    irbis::String text (L"Hello");
    CHECK (irbis::toLower (text) == L"hello");
    CHECK (irbis::toLower (text) == L"hello");
}

TEST_CASE("toLower_2", "[utils]")
{
    std::string text ("Hello");
    CHECK (irbis::toLower (text) == "hello");
    CHECK (irbis::toLower (text) == "hello");
}

TEST_CASE("toUpper_1", "[utils]")
{
    irbis::String text (L"Hello");
    CHECK (irbis::toUpper (text) == L"HELLO");
    CHECK (irbis::toUpper (text) == L"HELLO");
}

TEST_CASE("toUpper_2", "[utils]")
{
    std::string text ("Hello");
    CHECK (irbis::toUpper (text) == "HELLO");
    CHECK (irbis::toUpper (text) == "HELLO");
}

TEST_CASE("iif_1", "[utils]")
{
    const std::string s1 = "Hello", s2 = "World";
    const auto s3 = irbis::iif (s1, s2);
    CHECK (s1 == s3);
}

TEST_CASE("iif_2", "[utils]")
{
    const std::string s1, s2 = "World";
    const auto s3 = irbis::iif (s1, s2);
    CHECK (s2 == s3);
}

TEST_CASE("iif_3", "[utils]")
{
    const std::string s1 = "Hello", s2 = "World", s3 = "Again";
    const auto s4 = irbis::iif (s1, s2, s3);
    CHECK (s1 == s4);
}

TEST_CASE("iif_4", "[utils]")
{
    const std::string s1, s2 = "World", s3 = "Again";
    const auto s4 = irbis::iif (s1, s2, s3);
    CHECK (s2 == s4);
}

TEST_CASE("iif_5", "[utils]")
{
    const std::string s1, s2, s3 = "Again";
    const auto s4 = irbis::iif (s1, s2, s3);
    CHECK (s3 == s4);
}

TEST_CASE("iif_6", "[utils]")
{
    const irbis::String s1 = L"Hello", s2 = L"World";
    const auto s3 = irbis::iif (s1, s2);
    CHECK (s1 == s3);
}

TEST_CASE("iif_7", "[utils]")
{
    const irbis::String s1, s2 = L"World";
    const auto s3 = irbis::iif (s1, s2);
    CHECK (s2 == s3);
}

TEST_CASE("iif_8", "[utils]")
{
    const irbis::String s1 = L"Hello", s2 = L"World", s3 = L"Again";
    const auto s4 = irbis::iif (s1, s2, s3);
    CHECK (s1 == s4);
}

TEST_CASE("iif_9", "[utils]")
{
    const irbis::String s1, s2 = L"World", s3 = L"Again";
    const auto s4 = irbis::iif (s1, s2, s3);
    CHECK (s2 == s4);
}

TEST_CASE("iif_10", "[utils]")
{
    const irbis::String s1, s2, s3 = L"Again";
    const auto s4 = irbis::iif (s1, s2, s3);
    CHECK (s3 == s4);
}

TEST_CASE("safeAt_1", "[utils]")
{
    irbis::StringList v { L"1", L"3", L"5" };
    const auto i = irbis::safeAt (v, 1);
    CHECK (i == L"3");
}

TEST_CASE("safeAt_2", "[utils]")
{
    irbis::StringList v { L"1", L"3", L"5" };
    const auto i = irbis::safeAt (v, 11);
    CHECK (i == irbis::String());
}

TEST_CASE("safeAt_3", "[utils]")
{
    std::vector<std::string> v { "1", "3", "5" };
    const auto i = irbis::safeAt (v, 1);
    CHECK (i == "3");
}

TEST_CASE("safeAt_4", "[utils]")
{
    std::vector<std::string> v { "1", "3", "5" };
    const auto i = irbis::safeAt (v, 11);
    CHECK (i.empty());
}

TEST_CASE("maxSplit_1", "[utils]")
{
    irbis::String text (L"Hello,world,again");
    const auto a = irbis::maxSplit (text, ',', 2);
    irbis::StringList ::size_type expected = 2;
    CHECK (expected == a.size());
    CHECK (a[0] == L"Hello");
    CHECK (a[1] == L"world,again");
}

TEST_CASE("maxSplit_2", "[utils]")
{
    irbis::String text (L"Hello,world,again");
    const auto a = irbis::maxSplit (text, ',', 3);
    irbis::StringList ::size_type expected = 3;
    CHECK (expected == a.size());
    CHECK (a[0] == L"Hello");
    CHECK (a[1] == L"world");
    CHECK (a[2] == L"again");
}

TEST_CASE("contains_1", "[utils]")
{
    irbis::String s1 (L"Hello");
    irbis::String s2 (L"ell");
    CHECK       (irbis::contains (s1, s2));
    CHECK_FALSE (irbis::contains (s2, s1));
}

TEST_CASE("contains_2", "[utils]")
{
    irbis::String s (L"Hello");
    CHECK       (irbis::contains (s, L'H'));
    CHECK       (irbis::contains (s, L'e'));
    CHECK       (irbis::contains (s, L'l'));
    CHECK       (irbis::contains (s, L'o'));
    CHECK_FALSE (irbis::contains (s, L'1'));
}

TEST_CASE("replace_1", "[utils]")
{
    std::string s ("Hello, world");
    CHECK (irbis::replace (s, "Hello", "Goodbye") == "Goodbye, world");
    CHECK (irbis::replace (s, "Aloha", "Goodbye") == "Hello, world");
}

TEST_CASE("replace_2", "[utils]")
{
    irbis::String s (L"Hello, world");
    CHECK (irbis::replace (s, L"Hello", L"Goodbye") == L"Goodbye, world");
    CHECK (irbis::replace (s, L"Aloha", L"Goodbye") == L"Hello, world");
}

TEST_CASE("trimStart_1", "[utils]")
{
    CHECK (irbis::trimStart (irbis::String (L""))   == irbis::String(L""));
    CHECK (irbis::trimStart (irbis::String (L"1"))  == irbis::String(L"1"));
    CHECK (irbis::trimStart (irbis::String (L" 1")) == irbis::String(L"1"));
    CHECK (irbis::trimStart (irbis::String (L"1 ")) == irbis::String(L"1 "));
    CHECK (irbis::trimStart (irbis::String (L" 1 ")) == irbis::String(L"1 "));
    CHECK (irbis::trimStart (irbis::String (L" "))  == irbis::String(L""));
}

TEST_CASE("trimEnd_1", "[utils]")
{
    CHECK (irbis::trimEnd (irbis::String (L""))   == irbis::String(L""));
    CHECK (irbis::trimEnd (irbis::String (L"1"))  == irbis::String(L"1"));
    CHECK (irbis::trimEnd (irbis::String (L" 1")) == irbis::String(L" 1"));
    CHECK (irbis::trimEnd (irbis::String (L"1 ")) == irbis::String(L"1"));
    CHECK (irbis::trimEnd (irbis::String (L" 1 ")) == irbis::String(L" 1"));
    CHECK (irbis::trimEnd (irbis::String (L" "))  == irbis::String(L""));
}

TEST_CASE("trim_1", "[utils]")
{
    CHECK (irbis::trim (irbis::String (L""))   == irbis::String(L""));
    CHECK (irbis::trim (irbis::String (L"1"))  == irbis::String(L"1"));
    CHECK (irbis::trim (irbis::String (L" 1")) == irbis::String(L"1"));
    CHECK (irbis::trim (irbis::String (L"1 ")) == irbis::String(L"1"));
    CHECK (irbis::trim (irbis::String (L" 1 ")) == irbis::String(L"1"));
    CHECK (irbis::trim (irbis::String (L" "))  == irbis::String(L""));
}

TEST_CASE("describeError_1", "[utils]")
{
    for (auto i = -200000; i < 200000; i++) {
        const auto s = irbis::describeError (i);
        CHECK_FALSE (s.empty());
    }
}

TEST_CASE("fastParse32_1", "[utils]")
{
    CHECK (irbis::fastParse32 (irbis::String (L""))        == 0);
    CHECK (irbis::fastParse32 (irbis::String (L"0"))       == 0);
    CHECK (irbis::fastParse32 (irbis::String (L"1"))       == 1);
    CHECK (irbis::fastParse32 (irbis::String (L"12"))      == 12);
    CHECK (irbis::fastParse32 (irbis::String (L"123"))     == 123);
    CHECK (irbis::fastParse32 (irbis::String (L"1234"))    == 1234);
    CHECK (irbis::fastParse32 (irbis::String (L"12345"))   == 12345);
    CHECK (irbis::fastParse32 (irbis::String (L"123456"))  == 123456);
    CHECK (irbis::fastParse32 (irbis::String (L"1234567")) == 1234567);
}

TEST_CASE("fastParse32_2", "[utils]")
{
    CHECK (irbis::fastParse32 (irbis::CharSpan (""))        == 0);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("0"))       == 0);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("1"))       == 1);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("12"))      == 12);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("123"))     == 123);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("1234"))    == 1234);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("12345"))   == 12345);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("123456"))  == 123456);
    CHECK (irbis::fastParse32 (irbis::CharSpan ("1234567")) == 1234567);
}

TEST_CASE("fastParse32_3", "[utils]")
{
    CHECK (irbis::fastParse32 (irbis::WideSpan (L""))        == 0);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"0"))       == 0);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"1"))       == 1);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"12"))      == 12);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"123"))     == 123);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"1234"))    == 1234);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"12345"))   == 12345);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"123456"))  == 123456);
    CHECK (irbis::fastParse32 (irbis::WideSpan (L"1234567")) == 1234567);
}

TEST_CASE("fastParse32_4", "[utils]")
{
    CHECK (irbis::fastParse32 (L"")        == 0);
    CHECK (irbis::fastParse32 (L"0")       == 0);
    CHECK (irbis::fastParse32 (L"1")       == 1);
    CHECK (irbis::fastParse32 (L"12")      == 12);
    CHECK (irbis::fastParse32 (L"123")     == 123);
    CHECK (irbis::fastParse32 (L"1234")    == 1234);
    CHECK (irbis::fastParse32 (L"12345")   == 12345);
    CHECK (irbis::fastParse32 (L"123456")  == 123456);
    CHECK (irbis::fastParse32 (L"1234567") == 1234567);
}

TEST_CASE("fastParse32_5", "[utils]")
{
    const irbis::Char text[10] { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };
    CHECK (irbis::fastParse32 (text, 0)  == 0);
    CHECK (irbis::fastParse32 (text, 1)  == 0);
    CHECK (irbis::fastParse32 (text, 2)  == 1);
    CHECK (irbis::fastParse32 (text, 3)  == 12);
    CHECK (irbis::fastParse32 (text, 4)  == 123);
    CHECK (irbis::fastParse32 (text, 5)  == 1234);
    CHECK (irbis::fastParse32 (text, 6)  == 12345);
    CHECK (irbis::fastParse32 (text, 7)  == 123456);
    CHECK (irbis::fastParse32 (text, 8)  == 1234567);
    CHECK (irbis::fastParse32 (text, 9)  == 12345678);
    CHECK (irbis::fastParse32 (text, 10) == 123456789);
}

TEST_CASE("fastParse32_6", "[utils]")
{
    CHECK (irbis::fastParse32 (std::string (""))        == 0);
    CHECK (irbis::fastParse32 (std::string ("0"))       == 0);
    CHECK (irbis::fastParse32 (std::string ("1"))       == 1);
    CHECK (irbis::fastParse32 (std::string ("12"))      == 12);
    CHECK (irbis::fastParse32 (std::string ("123"))     == 123);
    CHECK (irbis::fastParse32 (std::string ("1234"))    == 1234);
    CHECK (irbis::fastParse32 (std::string ("12345"))   == 12345);
    CHECK (irbis::fastParse32 (std::string ("123456"))  == 123456);
    CHECK (irbis::fastParse32 (std::string ("1234567")) == 1234567);
}

TEST_CASE("fastParse32_7", "[utils]")
{
    CHECK (irbis::fastParse32 ("")        == 0);
    CHECK (irbis::fastParse32 ("0")       == 0);
    CHECK (irbis::fastParse32 ("1")       == 1);
    CHECK (irbis::fastParse32 ("12")      == 12);
    CHECK (irbis::fastParse32 ("123")     == 123);
    CHECK (irbis::fastParse32 ("1234")    == 1234);
    CHECK (irbis::fastParse32 ("12345")   == 12345);
    CHECK (irbis::fastParse32 ("123456")  == 123456);
    CHECK (irbis::fastParse32 ("1234567") == 1234567);
}

TEST_CASE("fastParse32_8", "[utils]")
{
    const char text[10] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    CHECK (irbis::fastParse32 (text, 0)  == 0);
    CHECK (irbis::fastParse32 (text, 1)  == 0);
    CHECK (irbis::fastParse32 (text, 2)  == 1);
    CHECK (irbis::fastParse32 (text, 3)  == 12);
    CHECK (irbis::fastParse32 (text, 4)  == 123);
    CHECK (irbis::fastParse32 (text, 5)  == 1234);
    CHECK (irbis::fastParse32 (text, 6)  == 12345);
    CHECK (irbis::fastParse32 (text, 7)  == 123456);
    CHECK (irbis::fastParse32 (text, 8)  == 1234567);
    CHECK (irbis::fastParse32 (text, 9)  == 12345678);
    CHECK (irbis::fastParse32 (text, 10) == 123456789);
}

TEST_CASE("fastParseUnsigned32_1", "[utils]")
{
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L""))        == 0u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"0"))       == 0u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"1"))       == 1u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"12"))      == 12u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"123"))     == 123u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"1234"))    == 1234u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"12345"))   == 12345u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"123456"))  == 123456u);
    CHECK (irbis::fastParseUnsigned32 (irbis::String (L"1234567")) == 1234567u);
}

TEST_CASE("fastParseUnsigned32_2", "[utils]")
{
    CHECK (irbis::fastParseUnsigned32 (L"")        == 0u);
    CHECK (irbis::fastParseUnsigned32 (L"0")       == 0u);
    CHECK (irbis::fastParseUnsigned32 (L"1")       == 1u);
    CHECK (irbis::fastParseUnsigned32 (L"12")      == 12u);
    CHECK (irbis::fastParseUnsigned32 (L"123")     == 123u);
    CHECK (irbis::fastParseUnsigned32 (L"1234")    == 1234u);
    CHECK (irbis::fastParseUnsigned32 (L"12345")   == 12345u);
    CHECK (irbis::fastParseUnsigned32 (L"123456")  == 123456u);
    CHECK (irbis::fastParseUnsigned32 (L"1234567") == 1234567u);
}

TEST_CASE("fastParseUnsigned32_3", "[utils]")
{
    const irbis::Char text[10] { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };
    CHECK (irbis::fastParseUnsigned32 (text, 0)  == 0u);
    CHECK (irbis::fastParseUnsigned32 (text, 1)  == 0u);
    CHECK (irbis::fastParseUnsigned32 (text, 2)  == 1u);
    CHECK (irbis::fastParseUnsigned32 (text, 3)  == 12u);
    CHECK (irbis::fastParseUnsigned32 (text, 4)  == 123u);
    CHECK (irbis::fastParseUnsigned32 (text, 5)  == 1234u);
    CHECK (irbis::fastParseUnsigned32 (text, 6)  == 12345u);
    CHECK (irbis::fastParseUnsigned32 (text, 7)  == 123456u);
    CHECK (irbis::fastParseUnsigned32 (text, 8)  == 1234567u);
    CHECK (irbis::fastParseUnsigned32 (text, 9)  == 12345678u);
    CHECK (irbis::fastParseUnsigned32 (text, 10) == 123456789u);
}

TEST_CASE("fastParseUnsigned32_4", "[utils]")
{
    CHECK (irbis::fastParseUnsigned32 (std::string (""))        == 0u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("0"))       == 0u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("1"))       == 1u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("12"))      == 12u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("123"))     == 123u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("1234"))    == 1234u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("12345"))   == 12345u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("123456"))  == 123456u);
    CHECK (irbis::fastParseUnsigned32 (std::string ("1234567")) == 1234567u);
}

TEST_CASE("fastParseUnsigned32_5", "[utils]")
{
    const char text[10] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    CHECK (irbis::fastParseUnsigned32 (text, 0)  == 0u);
    CHECK (irbis::fastParseUnsigned32 (text, 1)  == 0u);
    CHECK (irbis::fastParseUnsigned32 (text, 2)  == 1u);
    CHECK (irbis::fastParseUnsigned32 (text, 3)  == 12u);
    CHECK (irbis::fastParseUnsigned32 (text, 4)  == 123u);
    CHECK (irbis::fastParseUnsigned32 (text, 5)  == 1234u);
    CHECK (irbis::fastParseUnsigned32 (text, 6)  == 12345u);
    CHECK (irbis::fastParseUnsigned32 (text, 7)  == 123456u);
    CHECK (irbis::fastParseUnsigned32 (text, 8)  == 1234567u);
    CHECK (irbis::fastParseUnsigned32 (text, 9)  == 12345678u);
    CHECK (irbis::fastParseUnsigned32 (text, 10) == 123456789u);
}

