// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"
#include <iostream>
#include <algorithm>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

/*!

    \file UtfRecord.cpp

    UTF-версии записи, поля и подполя.

    \class irbis::UtfRecord

    \class irbis::UtfField

    \class irbis::UtfSubField

 */

namespace irbis {

/// \brief Добавление в конец записи поля с указанными меткой и значением.
/// \param tag Метка добавляемого поля.
/// \param value Значение поля (может быть пустым).
/// \return Вновь созданное поле.
UtfField& UtfRecord::add (int tag, const std::string &value)
{
    this->fields.emplace_back (tag, value);
    return this->fields.back();
}

UtfRecord UtfRecord::clone() const
{
    UtfRecord result;
    result.mfn = this->mfn;
    result.status = this->status;
    result.version = this->version;
    result.database = this->database;
    for (const auto &one : this->fields) {
        result.fields.push_back(one.clone());
    }
    return result;
}

void UtfRecord::decode (const std::vector<std::string> &lines)
{
    if (lines.size() < 2) {
        return;
    }

    // mfn and status of the record
    const auto firstLine = split(lines[0], '#');
    this->mfn = fastParseUnsigned32(firstLine[0]);
    this->status = fastParseUnsigned32(safeAt(firstLine, 1));

    // version of the record
    const auto secondLine = split(lines[1], L'#');
    this->version = fastParseUnsigned32(safeAt(secondLine, 1));

    // fields
    for (std::size_t i = 2; i < lines.size(); i++) {
        const auto line = lines[i];
        if (!line.empty()) {
            UtfField field;
            field.decode(line);
            this->fields.push_back(field);
        }
    }
}

bool UtfRecord::deleted() const noexcept
{
    return (this->status & RecordStatus::Deleted) != 0u;
}

std::string UtfRecord::encode (const std::string &delimiter) const
{
    std::string result = std::to_string(this->mfn) + "#"
                    + std::to_string(this->status) + delimiter
                    + "0#" + std::to_string(this->version) + delimiter;
    for (const auto &field : this->fields) {
        result.append(field.toString());
        result.append(delimiter);
    }
    return result;
}

std::string UtfRecord::fm (int tag, char code) const noexcept
{
    for (const auto &field : this->fields) {
        if (field.tag == tag) {
            if (code) {
                for (const auto &subfield : field.subfields) {
                    if (sameChar(subfield.code, code)) {
                        return subfield.value;
                    }
                }
            } else {
                return field.value;
            }
        }
    }
    return std::string();
}

std::vector<std::string> UtfRecord::fma (int tag, char code) const
{
    std::vector<std::string> result;
    for (const auto &field : this->fields) {
        if (field.tag == tag) {
            if (code) {
                for (const auto &subfield : field.subfields) {
                    if (sameChar(subfield.code, code)) {
                        if (!subfield.value.empty()) {
                            result.push_back(subfield.value);
                        }
                    }
                }
            } else {
                if (!field.value.empty()) {
                    result.push_back(field.value);
                }
            }
        }
    }
    return result;
}

UtfField* UtfRecord::getField (int tag, int occurrence) const noexcept
{
    for (const auto &field : this->fields) {
        if (field.tag == tag) {
            if (!occurrence) {
                return const_cast<UtfField*> (&field);
            }
        }
    }
    return nullptr;
}

std::vector<UtfField*> UtfRecord::getFields (int tag) const
{
    std::vector<UtfField*> result;
    for (const auto &field : this->fields) {
        if (field.tag == tag) {
            auto ptr = const_cast<UtfField*>(&field);
            result.push_back(ptr);
        }
    }

    return result;
}

UtfRecord& UtfRecord::reset() noexcept
{
    this->mfn = 0;
    this->status = 0;
    this->version = 0;
    this->database.clear();
    return *this;
}

bool UtfRecord::verify (bool throwOnError) const
{
    bool result = true;
    for (const auto &field : this->fields) {
        if (!field.verify(throwOnError)) {
            result = false;
        }
    }
    if (!result && throwOnError) {
        throw VerificationException();
    }
    return result;
}

MarcRecord UtfRecord::materialize() const
{
    MarcRecord result;
    result.mfn = this->mfn;
    result.status = this->status;
    result.version = this->version;
    result.database = fromUtf(this->database);
    for (const auto &one : this->fields) {
        result.fields.push_back(one.materialize());
    }
    return result;
}

//=========================================================

UtfField& UtfField::add (char subFieldCode, const std::string &subFieldValue)
{
    this->subfields.emplace_back(subFieldCode, subFieldValue);
    return *this;
}

UtfField& UtfField::clear()
{
    value.clear();
    subfields.clear();
    return *this;
}

UtfField UtfField::clone() const
{
    UtfField result (this->tag, this->value);
    for (const auto &sub : this->subfields) {
        result.subfields.emplace_back (sub.code, sub.value);
    }
    return result;
}

void UtfField::decode (const std::string &line)
{
    const auto parts = maxSplit (line, '#', 2);
    this->tag = fastParse32(parts[0]);
    if (parts.size() == 1 || parts[1].empty()) {
        return;
    }
    const auto body = parts[1];
    std::vector<std::string> all;
    if (body[0] == '^') {
        all = split (body, '^');
    } else {
        const auto parts2 = maxSplit(body, '#', 2);
        this->value = parts2[0];
        all = split (parts2[1], '^');
    }
    for (const auto &one : all) {
        if (!one.empty()) {
            UtfSubField subField;
            subField.decode(one);
            this->subfields.push_back (subField);
        }
    }
}

bool UtfField::empty() const noexcept
{
    return !this->tag || (this->value.empty() && this->subfields.empty());
}

UtfSubField* UtfField::getFirstSubfield (char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar (one.code, code)) {
            return const_cast<UtfSubField*> (&one);
        }
    }
    return nullptr;
}

std::string UtfField::getFirstSubfieldValue (char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar (one.code, code)) {
            return one.value;
        }
    }
    return std::string();
}

UtfField& UtfField::removeSubfield (char code)
{
    std::remove_if (std::begin(this->subfields), std::end(this->subfields), // NOLINT(bugprone-unused-return-value)
                    [code] (UtfSubField &sf) { return sf.code == code; }
    );

    return *this;
}

UtfField& UtfField::setSubfield (char code, const std::string &newValue)
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

bool UtfField::verify (bool throwOnError) const
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

std::string UtfField::toString() const
{
    std::string result = std::to_string(tag)
                    + "#" + value;
    for (const auto &sub : subfields) {
        result += sub.toString();
    }
    return result;
}

RecordField UtfField::materialize() const
{
    RecordField result (this->tag, fromUtf(this->value));
    for (const auto &sub : this->subfields) {
        result.subfields.emplace_back (sub.code, fromUtf(sub.value));
    }
    return result;
}

//=========================================================

/// \brief Клонирование подполя.
/// \return Точная копия подполя.
UtfSubField UtfSubField::clone() const
{
    UtfSubField result;
    result.code = this->code;
    result.value = this->value;
    return result;
}

/// \brief Декодирование подполя из клиентского представления.
/// \param line Строка с клиентским представлением (не должна быть пустой).
void UtfSubField::decode (const std::string &line)
{
    this->code = line[0];
    this->value = line.substr(1);
}

/// \brief Пустое подполе?
/// \return true, если подполе пустое.
bool UtfSubField::empty() const noexcept
{
    return code == 0 || value.empty();
}

/// \brief Проверка, является подполе полноценным (с кодом и значением).
/// \param throwOnError Бросать исключение, если подполе не полноценное.
/// \return true, если с полем всё нормально.
bool UtfSubField::verify (bool throwOnError) const
{
    const bool result = (this->code != 0) && !this->value.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }
    return result;
}

/// \brief Кодирование подполя в клиентское (строковое) представление.
/// \return Закодированное подполе.
std::string UtfSubField::toString() const
{
    return std::string ("^") + this->code + this->value;
}

/// \brief Превращение в полноценное подполе.
/// \return Полноценное подполе.
SubField UtfSubField::materialize() const
{
    return SubField (this->code, fromUtf(this->value));
}

}
