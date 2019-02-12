﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN
const int RecordField::NoTag = 0;

RecordField& RecordField::add(wchar_t code, const std::wstring &value)
{
    subfields.push_back({code, value});

    return *this;
}

RecordField& RecordField::clear()
{
    value.clear();
    subfields.clear();

    return *this;
}

bool RecordField::empty() const
{
    return !tag || (value.empty() && subfields.empty());
}

bool RecordField::verify(bool throwOnError) const
{
    bool result = tag > 0;

    if (result) {
        if (subfields.empty()) {
            result = !value.empty();
        }
        else {
            for (const SubField &sub : subfields) {
                if (!sub.verify(throwOnError)) {
                    result = false;
                    break;
                }
            }
        }
    }

    if (!result && throwOnError) {
        throw VerificationException();
    }

    return result;
}

std::wstring RecordField::wstr() const
{
    std::wstring result = std::to_wstring(tag)
        + std::wstring(L"#") + value;
    for (const SubField &sub : subfields)
    {
        result += sub.wstr();
    }

    return result;
}

std::wostream& operator << (std::wostream &stream, const RecordField &field)
{
    stream << std::to_wstring(field.tag) << L"#" << field.value;
    for (const SubField &sub : field.subfields)
    {
        stream << sub;
    }

    return stream;
}

NAMESPACE_IRBIS_END