// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cstring>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

Encoding* Encoding::_ansi = nullptr;
Encoding* Encoding::_utf = nullptr;

//=========================================================

Encoding* Encoding::ansi()
{
    if (!Encoding::_ansi) {
        Encoding::_ansi = new Cp1251Encoding;
    }
    return Encoding::_ansi;
}

String Encoding::fromAnsi (const Byte *bytes, std::size_t count)
{
    return _ansi->toUnicode(bytes, count);
}

String Encoding::fromUtf (const Byte *bytes, std::size_t count)
{
    return _utf->toUnicode(bytes, count);
}

Bytes Encoding::toAnsi (const String &text)
{
    return _ansi->fromUnicode(text);
}

Bytes Encoding::toUtf (const String &text)
{
    return _utf->fromUnicode(text);
}

Encoding* Encoding::utf()
{
    if (!Encoding::_utf) {
        Encoding::_utf = new Utf8Encoding();
    }
    return Encoding::_utf;
}

}

