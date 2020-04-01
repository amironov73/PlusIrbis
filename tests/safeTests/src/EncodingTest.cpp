// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("Encoding_cp1251_1", "[encoding]")
{
    const std::string source = "\xD3\x20\xEF\xEE\xEF\xE0\x20\xE1\xFB\xEB\xE0\x20\xF1\xEE\xE1\xE0\xEA\xE0";
    const irbis::String destination = irbis::cp1251_to_unicode(source);
    CHECK(irbis::String(L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430") == destination);
}

TEST_CASE("Encoding_cp1251_2", "[encoding]")
{
    const irbis::String source = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    const std::string destination = irbis::unicode_to_cp1251(source);
    CHECK(std::string("\xD3\x20\xEF\xEE\xEF\xE0\x20\xE1\xFB\xEB\xE0\x20\xF1\xEE\xE1\xE0\xEA\xE0") == destination);
}

TEST_CASE("Encoding_toUtf_1", "[encoding]")
{
    irbis::Char text[] = L"Hello! \u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    irbis::Byte buffer[1024], *ptr;
    ptr = irbis::toUtf(buffer, text, wcslen(text));
    const std::ptrdiff_t expected = 40;
    const auto actual = static_cast<std::ptrdiff_t>(ptr - buffer);
    CHECK(expected == actual);
    irbis::Byte goodBytes[40] {0x48, 0x65, 0x6C, 0x6C, 0x6F,
        0x21, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
        0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
        0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
        0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
        0xBA, 0xD0, 0xB0 };
    for (int i = 0; i < 40; i++) {
        CHECK(goodBytes[i] == buffer[i]);
    }
}

TEST_CASE("Encoding_toUtf_2", "[encoding]")
{
    const irbis::String text = L"Hello! \u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    irbis::Byte buffer[1024], *ptr;
    ptr = irbis::toUtf(buffer, text);
    const std::ptrdiff_t expected = 40;
    const auto actual = static_cast<std::ptrdiff_t>(ptr - buffer);
    CHECK(expected == actual);
    irbis::Byte goodBytes[40] {0x48, 0x65, 0x6C, 0x6C, 0x6F,
        0x21, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
        0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
        0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
        0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
        0xBA, 0xD0, 0xB0 };
    for (int i = 0; i < 40; i++) {
        CHECK(goodBytes[i] == buffer[i]);
    }
}

TEST_CASE("Encoding_fromUtf_1", "[encoding]")
{
    irbis::Byte text[40] {0x48, 0x65, 0x6C, 0x6C, 0x6F,
        0x21, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
        0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
        0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
        0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
        0xBA, 0xD0, 0xB0 };
    irbis::Char buffer[1024], *ptr;
    ptr = irbis::fromUtf(buffer, text, 40);
    const std::ptrdiff_t expected = 25;
    const auto actual = static_cast<std::ptrdiff_t>(ptr - buffer);
    CHECK(expected == actual);
    irbis::Char goodChars[] = L"Hello! \u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    for (int i = 0; i < 25; i++) {
        CHECK(goodChars[i] == buffer[i]);
    }
}

TEST_CASE("Encoding_fromUtf_2", "[encoding]")
{
    irbis::Byte src[] {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21,
        0x10, 0x20, 0xD0, 0xA3, 0x20, 0xD0, 0xBF, 0xD0,
        0xBE, 0xD0, 0xBF, 0xD0, 0xB0, 0x20, 0xD0, 0xB1,
        0xD1, 0x8B, 0xD0, 0xBB, 0xD0, 0xB0, 0x20, 0xD1,
        0x81, 0xD0, 0xBE, 0xD0, 0xB1, 0xD0, 0xB0, 0xD0,
        0xBA, 0xD0, 0xB0 };
    std::size_t size = sizeof(src);
    irbis::String result;
    const irbis::Byte *end = irbis::fromUtf(src, size, 0x10, result);
    CHECK(irbis::String (L"Hello!") == result);
    end++;
    size--;
    end++;
    size--;
    irbis::fromUtf(end, size, 0x10, result);
    CHECK(irbis::String (L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430") == result);
}
