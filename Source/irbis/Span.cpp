// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cctype>
#include <cwctype>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

bool sameString(CSpan first, CSpan second)
{
    auto p1 = first.cbegin(), p2 = second.cbegin(), e1 = first.cend();
    while (p1 < e1) {
        if (::towupper(*p1) != ::toupper(*p2)) {
            return false;
        }
        ++p1; ++p2;
    }

    return true;
}

bool sameString(WSpan first, WSpan second)
{
    auto p1 = first.cbegin(), p2 = second.cbegin(), e1 = first.cend();
    while (p1 < e1) {
        if (::towupper(*p1) != ::toupper(*p2)) {
            return false;
        }
        ++p1; ++p2;
    }

    return true;
}

bool startsWith(CSpan text, CSpan prefix)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    auto p2 = prefix.cbegin(), e2 = prefix.cend();

    while (p2 < e2) {
        if (p1 >= e1 || *p1 != *p2) {
            return false;
        }
        ++p1; ++p2;
    }

    return true;
}

bool startsWith(WSpan text, WSpan prefix)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    auto p2 = prefix.cbegin(), e2 = prefix.cend();

    while (p2 < e2) {
        if (p1 >= e1 || *p1 != *p2) {
            return false;
        }
        ++p1; ++p2;
    }

    return true;
}

bool endsWith(CSpan text, CSpan suffix)
{
    // TODO implement
    return false;
}

bool endsWith(WSpan text, WSpan suffix)
{
    // TODO implement
    return false;
}

int compare(CSpan first, CSpan second)
{
    // TODO implement
    return -1;
}

int compare(WSpan first, WSpan second)
{
    // TODO implement
    return -1;
}

CSpan trimStart(CSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    while (p1 < e1) {
        if (!std::isspace(*p1)) {
            break;
        }
        ++p1;
    }
    return CSpan (const_cast<char*>(p1), e1 - p1);
}

WSpan trimStart(WSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    while (p1 < e1) {
        if (!std::iswspace(*p1)) {
            break;
        }
        ++p1;
    }
    return WSpan (const_cast<Char*>(p1), e1 - p1);
}

CSpan trimEnd(CSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend() - 1;
    while (p1 < e1) {
        if (!std::isspace(*e1)) {
            break;
        }
        --e1;
    }
    return CSpan (const_cast<char*>(p1), e1 - p1 + 1);
}

WSpan trimEnd(WSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend() - 1;
    while (p1 < e1) {
        if (!std::iswspace(*e1)) {
            break;
        }
        --e1;
    }
    return WSpan (const_cast<Char*>(p1), e1 - p1 + 1);
}

CSpan trim(CSpan text)
{
    return trimStart(trimEnd(text));
}

WSpan trim(WSpan text)
{
    return trimStart(trimEnd(text));
}

std::vector<CSpan> split(CSpan text, char c)
{
    std::vector<CSpan> result;
    // TODO implement
    return result;
}

std::vector<WSpan> split(WSpan text, Char c)
{
    std::vector<WSpan> result;
    // TODO implement
    return result;
}

std::vector<CSpan> split(CSpan text, char c, int nelem)
{
    std::vector<CSpan> result;
    // TODO implement
    return result;
}

std::vector<WSpan> split(WSpan text, Char c, int nelem)
{
    std::vector<WSpan> result;
    // TODO implement
    return result;
}

CSpan toupper(CSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::toupper(*p));
    }
    return text;
}

WSpan toupper(WSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::towupper(*p));
    }
    return text;
}

CSpan tolower(CSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::tolower(*p));
    }
    return text;
}

WSpan tolower(WSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::towlower(*p));
    }
    return text;
}

}
