// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// PlusIrbis.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include <iterator>

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

const std::string& iif(const std::string& s1, const std::string &s2)
{
    if (!s1.empty())
    {
        return s1;
    }

    return s2;
}

const std::wstring& iif(const std::wstring& s1, const std::wstring &s2)
{
    if (!s1.empty())
    {
        return s1;
    }

    return s2;
}

const std::string& iif(const std::string& s1, const std::string &s2, const std::string &s3)
{
    if (!s1.empty())
    {
        return s1;
    }

    if (!s2.empty())
    {
        return s2;
    }

    return s3;
}

const std::wstring& iif(const std::wstring& s1, const std::wstring &s2, const std::wstring &s3)
{
    if (!s1.empty())
    {
        return s1;
    }

    if (!s2.empty())
    {
        return s2;
    }

    return s3;
}

std::wstring safeAt(const std::vector<std::wstring> &list, int index)
{
    if ((index < 0) || (index >= list.size()))
    {
        return std::wstring();
    }

    return list.at(index);
}

std::vector<std::wstring> maxSplit(const std::wstring &text, wchar_t separator, int count)
{
    std::vector<std::wstring> result;

    size_t position = 0, length = text.length();
    while ((count > 1) && (position < length)) {
        size_t index = text.find(separator, position);
        if (index != std::wstring::npos) {
            result.push_back(text.substr(position, index - position));
            position = index + 1;
        } else {
            result.push_back(text.substr(position));
            break;
        }
        count--;
    }

    if (position < length) {
        result.push_back(text.substr(position));
    }

    return result;

}

NAMESPACE_IRBIS_END
