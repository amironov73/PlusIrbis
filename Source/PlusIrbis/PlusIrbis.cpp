// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// PlusIrbis.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

bool sameChar(wchar_t first, wchar_t second)
{
    return towupper(first) == towupper(second);
}

bool sameString(const std::wstring &first, const std::wstring &second)
{
    return std::equal(std::begin(first), std::end(first),
        std::begin(second), std::end(second),
        [] (wchar_t a, wchar_t b) { return towupper(a) == towupper(b); });
}

bool contains(const std::wstring &text, const std::wstring &fragment)
{
    return text.find(fragment) != std::string::npos;
}

int fastParse32(const std::wstring &text)
{
    int result = 0;
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;

}

int fastParse32(const wchar_t *text)
{
    int result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

int fastParse32(const wchar_t *text, int length)
{
    int result = 0;
    while (length > 0) {
        result = result * 10 + *text - '0';
        text++;
        length--;
    }

    return result;
}

NAMESPACE_IRBIS_END
