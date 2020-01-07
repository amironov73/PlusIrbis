// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <codecvt>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

static std::locale _cp1251(".1251");
static std::wstring_convert<std::codecvt_utf8<wchar_t>> _utf8;

namespace irbis {

std::string narrow(const String &wide, const std::locale &loc)
{
    std::string result;
    const auto length = wide.length();
    if (!length) {
        return result;
    }

    result.resize(length);
    const auto start = wide.data();
    const auto stop = start + length;
    const auto buffer = const_cast<char*>(result.data());
    std::use_facet<std::ctype<wchar_t>>(loc).narrow(start, stop, '?', buffer);

    return result;
}

String widen(const std::string &str, const std::locale &loc)
{
    String result;
    const auto length = str.length();
    if (!length) {
        return result;
    }

    result.resize(length);
    const auto start = str.data();
    const auto stop = start + length;
    const auto buffer = const_cast<wchar_t *>(result.data());
    std::use_facet<std::ctype<wchar_t>>(loc).widen(start, stop, buffer);

    return result;
}

String new_cp1251_to_unicode(const std::string &text)
{
    return widen(text, _cp1251);
}

std::string new_unicode_to_cp1251(const String &text)
{
    return narrow(text, _cp1251);
}

std::string new_toUtf(const String &text)
{
    std::string result = _utf8.to_bytes(text);
    return result;
}

String new_fromUtf(const std::string &text)
{
    String result = _utf8.from_bytes(text);
    return result;
}

}