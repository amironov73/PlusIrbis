﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

bool SubField::empty() const
{
    return code == NoCode || value.empty();
}

std::wstring SubField::wstr() const
{
    return std::wstring(L"^") + code + value;
}

bool SubField::verify(bool throwOnError) const
{
    const bool result = (code != NoCode) && !value.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }

    return result;
}

PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const SubField &subfield)
{
    return stream << std::wstring(L"^") << std::wstring(subfield.code, 1)
        << subfield.value;
}

}
