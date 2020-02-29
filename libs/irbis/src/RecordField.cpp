// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/*!

    \class irbis::RecordField

    В большинстве случаев метки поля попадают в диапазон от 1 до 999,
    но на практике могут оказаться любым натуральным числом в диапазоне
    от 1 до 2 147 483 647‬.

 */


namespace irbis {

RecordField& RecordField::add (Char subFieldCode, const String &subFieldValue)
{
    this->subfields.emplace_back(subFieldCode, subFieldValue);
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
    RecordField result (this->tag, this->value);
    for (const auto &sub : this->subfields) {
        result.subfields.emplace_back (sub.code, sub.value);
    }
    return result;
}

void RecordField::decode (const String &line)
{
    const auto parts = maxSplit (line, L'#', 2);
    this->tag = fastParse32 (parts[0]);
    if (parts.size() == 1 || parts[1].empty()) {
        return;
    }
    const auto body = parts[1];
    StringList all;
    if (body[0] == L'^') {
        all = split (body, L'^');
    } else {
        const auto parts2 = maxSplit (body, L'#', 2);
        this->value = parts2[0];
        all = split (parts2[1], L'^');
    }
    for (const auto &one : all) {
        if (!one.empty()) {
            SubField subField;
            subField.decode (one);
            this->subfields.push_back (subField);
        }
    }
}

bool RecordField::empty() const noexcept
{
    return !this->tag || (this->value.empty() && this->subfields.empty());
}

SubField* RecordField::getFirstSubfield (Char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar(one.code, code)) {
            return const_cast<SubField*> (&one);
        }
    }
    return nullptr;
}

String RecordField::getFirstSubfieldValue (Char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar(one.code, code)) {
            return one.value;
        }
    }
    return String();
}

RecordField& RecordField::removeSubfield (Char code)
{
    std::remove_if (std::begin(this->subfields), std::end(this->subfields), // NOLINT(bugprone-unused-return-value)
            [code] (SubField &sf) { return sf.code == code; }
            );

    return *this;
}

RecordField& RecordField::setSubfield (Char code, const String &newValue)
{
    if (newValue.empty()) {
        this->removeSubfield (code);
        return *this;
    }
    auto *subField = this->getFirstSubfield (code);
    if (!subField) {
        this->add (code, newValue);
    }
    else {
        subField->value = newValue;
    }
    return *this;
}

bool RecordField::verify (bool throwOnError) const
{
    bool result = tag > 0;
    if (result) {
        if (subfields.empty()) {
            result = !value.empty();
        }
        else {
            for (const auto &sub : subfields) {
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

std::wostream& IRBIS_API operator << (std::wostream &stream, const RecordField &field)
{
    stream << std::to_wstring (field.tag) << std::wstring (L"#") << field.value;
    for (const auto &sub : field.subfields) {
        stream << sub;
    }

    return stream;
}

}
