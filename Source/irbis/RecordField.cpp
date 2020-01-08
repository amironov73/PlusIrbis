// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iomanip>
#include <iostream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

RecordField& RecordField::add(wchar_t subFieldCode, const String &subFieldValue)
{
    SubField subField;
    subField.code = subFieldCode;
    subField.value = subFieldValue;
    subfields.push_back(subField);
    return *this;
}

RecordField& RecordField::clear()
{
    value.clear();
    subfields.clear();

    return *this;
}

RecordField RecordField::clone() const
{
    RecordField result;
    result.tag = this->tag;
    result.value = this->value;
    for (const auto &sub : this->subfields) {
        result.subfields.push_back(sub.clone());
    }

    return result;
}

void RecordField::decode(const String &line)
{
    const auto parts = maxSplit(line, L'#', 2);
    this->tag = fastParse32(parts[0]);
    if (parts.size() == 1 || parts[1].empty()) {
        return;
    }

    const auto body = parts[1];
    StringList all;
    if (body[0] == L'^') {
        all = split(body, L'^');
    } else {
        const auto parts2 = maxSplit(body, L'#', 2);
        this->value = parts2[0];
        all = split(parts2[1], L'^');
    }

    for (const auto &one : all) {
        if (!one.empty()) {
            SubField subField;
            subField.decode(one);
            this->subfields.push_back(subField);
        }
    }
}

bool RecordField::empty() const noexcept
{
    return !this->tag || (this->value.empty() && this->subfields.empty());
}

SubField* RecordField::getFirstSubfield(wchar_t code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar(one.code, code)) {
            return const_cast<SubField*>(&one);
        }
    }

    return nullptr;
}

std::wstring RecordField::getFirstSubfieldValue(wchar_t code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar(one.code, code)) {
            return one.value;
        }
    }

    return String();
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

String RecordField::toString() const
{
    String result = std::to_wstring(tag)
        + std::wstring(L"#") + value;
    for (const auto &sub : subfields) {
        result += sub.toString();
    }

    return result;
}

PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const RecordField &field)
{
    stream << std::to_wstring(field.tag) << std::wstring(L"#") << field.value;
    for (const auto &sub : field.subfields) {
        stream << sub;
    }

    return stream;
}

}
