// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cstring>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

// Quick and dirty UTF-8 conversion

// Преобразует UCS-16 в UTF-8
// Возвращает указатель на место после последнего преобразованного символа
Byte* toUtf (Byte *dst, const Char *src, std::size_t length)
{
    while (length > 0) {
        const unsigned int c = *src++;
        if (c < (1u << 7u)) {
            *dst++ = static_cast<Byte>(c);
        }
        else if (c < (1u << 11u)) {
            *dst++ = static_cast<Byte>((c >> 6u) | 0xC0u);
            *dst++ = static_cast<Byte>((c & 0x3Fu) | 0x80u);
        }
        else if (c < (1u << 16u)) { //-V547
            *dst++ = static_cast<Byte>((c >> 12u) | 0xE0u);
            *dst++ = static_cast<Byte>(((c >> 6u) & 0x3Fu) | 0x80u);
            *dst++ = static_cast<Byte>((c & 0x3Fu) | 0x80u);
        }
        else if (c < (1u << 21u)) {
            *dst++ = static_cast<Byte>((c >> 18u) | 0xF0u);
            *dst++ = static_cast<Byte>(((c >> 12u) & 0x3Fu) | 0x80u);
            *dst++ = static_cast<Byte>(((c >> 6u) & 0x3Fu) | 0x80u);
            *dst++ = static_cast<Byte>((c & 0x3Fu) | 0x80u);
        }
        else {
            return nullptr;
        }
        length--;
    }

    return dst;
}

// Подсчитывает число байт, необходимых для размещения в UTF-8.
std::size_t countUtf (const Char *src, std::size_t length)
{
    std::size_t result = 0;

    while (length > 0)
    {
        const unsigned int c = *src++;
        if (c < (1u << 7u))
        {
            result++;
        }
        else if (c < (1u << 11u))
        {
            result++;
            result++;
        }
        else if (c < (1u << 16u)) //-V547
        {
            result += 3;
        }
        else if (c < (1u << 21u))
        {
            result +=4;
        }
        else
        {
            return 0;
        }
        length--;
    }

    return result;
}

// Преобразует UTF-8 в UCS-16
// Возвращает указатель на место после последнего преобразованного символа
Char* fromUtf (Char *dst, const Byte *src, std::size_t length)
{
    const Byte *stop = src + length;

    while (src < stop)
    {
        unsigned int c = *src++;
        if ((c & 0x80u) == 0u)
        {
            // 1-Byte sequence: 000000000xxxxxxx = 0xxxxxxx
        }
        else if ((c & 0xE0u) == 0xC0u)
        {
            // 2-Byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
            c = (c & 0x1Fu) << 6u;
            c |= (*src++ & 0x3Fu);
        }
        else if ((c & 0xF0u) == 0xE0u)
        {
            // 3-Byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
            c = (c & 0x0Fu) << 12u;
            c |= (*src++ & 0x3Fu) << 6u;
            c |= (*src++ & 0x3Fu);
            if (c == 0xFEFFu) { // bom at start
                continue; // skip it
            }
        }
        else if ((c & 0xF8u) == 0xF0u)
        {
            // 4-Byte sequence: 11101110wwwwzzzzyy + 110111yyyyxxxxxx = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
            c = (c & 0x07u) << 18u;
            c |= (*src++ & 0x3Fu) << 12u;
            c |= (*src++ & 0x3Fu) << 6u;
            c |= (*src++ & 0x3Fu);
        }
        *dst++ = static_cast<Char>(c);
    }

    return dst;
}

// Подсчитывает число Char, необходимых для размещения в UCS-16.
std::size_t countUtf (const Byte *src, std::size_t length)
{
    std::size_t result = 0;

    const Byte *stop = src + length;

    while (src < stop)
    {
        unsigned int c = *src++;
        if ((c & 0x80u) == 0u)
        {
        }
        else if ((c & 0xE0u) == 0xC0u)
        {
            src++;
        }
        else if ((c & 0xF0u) == 0xE0u)
        {
            c = (c & 0x0Fu) << 12u;
            c |= (*src++ & 0x3Fu) << 6u;
            c |= (*src++ & 0x3Fu);
            if (c == 0xFEFF) { // bom at start
                continue; // skip it
            }
        }
        else if ((c & 0xF8u) == 0xF0u)
        {
            src += 3;
        }
        result++;
    }

    return result;
}

// Считывает строку UTF-8 вплоть до разделителя.
const Byte* fromUtf (const Byte *src, std::size_t &size, Byte stop, String &result)
{
    const Byte *end = src;
    while (size && (*end != stop))
    {
        end++;
        size--;
    }
    const std::size_t length = end - src;
    if (length == 0)
    {
        result.clear();
        return end;
    }
    auto dst = new Char[length + 1];
    memset(dst, 0, sizeof(Char)*(length + 1));
    if (!fromUtf(dst, src, length))
    {
        result.clear();
        return end;
    }
    result = dst;
    delete[] dst;
    return end;
}

String fromUtf(const std::string &text)
{
    const auto srcSize = text.length();
    if (!srcSize)
    {
        return std::wstring();
    }

    const auto *src = reinterpret_cast<const Byte*>(text.c_str());
    const auto dstSize = countUtf(src, srcSize);
    auto *dst = new Char[dstSize + 1];
    memset(dst, 0, sizeof(Char)*(dstSize+1));
    if (!fromUtf(dst, src, srcSize))
    {
        return std::wstring();
    }
    String result(dst);
    delete[] dst;
    return result;
}

/// \brief Преобразует диапазон байт в Unicode-строку.
String fromUtf (ByteSpan span)
{
    const auto size = countUtf(span.ptr, span.length);
    String result;
    result.resize(size);
    fromUtf(const_cast<Char*>(result.data()), span.ptr, span.length);
    return result;
}

// Записывает строку в UTF-8
Byte* toUtf(Byte *dst, const String &text)
{
    const std::size_t length = text.length();
    const Char *src = text.c_str();
    return toUtf(dst, src, length);
}

std::string toUtf(const String &text)
{
    const auto srcSize = text.length();
    if (!srcSize)
    {
        return std::string();
    }

    const auto *src = reinterpret_cast<const Char*>(text.c_str());
    const auto dstSize = countUtf(src, srcSize);
    auto *dst = new Byte[dstSize + 1];
    memset(dst, 0, sizeof(Byte)*(dstSize+1));
    if (!toUtf(dst, src, srcSize))
    {
        return std::string();
    }
    std::string result(reinterpret_cast<const char*>(dst));
    delete[] dst;
    return result;
}

//=========================================================

Bytes Utf8Encoding::fromUnicode(const String &text) const
{
    const auto size = countUtf(text.data(), text.size());
    Bytes result;
    result.reserve(size);
    irbis::toUtf(result.data(), text);
    return result;
}

String Utf8Encoding::toUnicode(const Byte *bytes, std::size_t count) const
{
    String result;
    result.reserve(count);
    irbis::fromUtf(const_cast<Char*>(result.data()), bytes, count);

    return result;
}

}