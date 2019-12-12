// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

RecordField& RecordField::add(wchar_t subFieldCode, const std::wstring &subFieldValue)
{
    subfields.push_back({subFieldCode, subFieldValue});

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
    for (const auto &sub : subfields)
    {
        result += sub.wstr();
    }

    return result;
}

PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const RecordField &field)
{
    stream << std::to_wstring(field.tag) << std::wstring(L"#") << field.value;
    for (const SubField &sub : field.subfields)
    {
        stream << sub;
    }

    return stream;
}

}
