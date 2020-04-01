// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"
#include <iostream>
#include <algorithm>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

/*!

    \file Lite.cpp

    UTF-версии записи, поля и подполя.

    \class irbis::LiteRecord

    \class irbis::LiteField

    \class irbis::LiteSubField

 */

namespace irbis {

/// \brief Добавление в конец записи поля с указанными меткой и значением.
/// \param tag Метка добавляемого поля.
/// \param value Значение поля (может быть пустым).
/// \return Вновь созданное поле.
LiteField& LiteRecord::add (int tag, const std::string &value)
{
    this->fields.emplace_back (tag, value);
    return this->fields.back();
}

LiteRecord LiteRecord::clone() const
{
    LiteRecord result;
    result.mfn = this->mfn;
    result.status = this->status;
    result.version = this->version;
    result.database = this->database;
    for (const auto &one : this->fields) {
        result.fields.push_back(one.clone());
    }
    return result;
}

void LiteRecord::decode (const std::vector<std::string> &lines)
{
    if (lines.size() < 2) {
        return;
    }

    // mfn and status of the record
    const auto firstLine = split(lines[0], '#');
    this->mfn = fastParseUnsigned32(firstLine[0]);
    this->status = static_cast<RecordStatus> (fastParseUnsigned32(safeAt(firstLine, 1)));

    // version of the record
    const auto secondLine = split(lines[1], L'#');
    this->version = fastParseUnsigned32(safeAt(secondLine, 1));

    // fields
    for (std::size_t i = 2; i < lines.size(); i++) {
        const auto line = lines[i];
        if (!line.empty()) {
            LiteField field;
            field.decode(line);
            this->fields.push_back(field);
        }
    }
}

bool LiteRecord::deleted() const noexcept
{
    return (this->status & RecordStatus::Deleted) != RecordStatus::None;
}

std::string LiteRecord::encode (const std::string &delimiter) const
{
    std::string result = std::to_string (this->mfn) + "#"
                    + std::to_string (static_cast<int> (this->status)) + delimiter
                    + "0#" + std::to_string(this->version) + delimiter;
    for (const auto &field : this->fields) {
        result.append(field.toString());
        result.append(delimiter);
    }
    return result;
}

std::string LiteRecord::fm (int tag, char code) const noexcept
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

std::vector<std::string> LiteRecord::fma (int tag, char code) const
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

LiteField* LiteRecord::getField (int tag, int occurrence) const noexcept
{
    for (const auto &field : this->fields) {
        if (field.tag == tag) {
            if (!occurrence) {
                return const_cast<LiteField*> (&field);
            }
        }
    }
    return nullptr;
}

std::vector<LiteField*> LiteRecord::getFields (int tag) const
{
    std::vector<LiteField*> result;
    for (const auto &field : this->fields) {
        if (field.tag == tag) {
            auto ptr = const_cast<LiteField*>(&field);
            result.push_back(ptr);
        }
    }

    return result;
}

LiteRecord& LiteRecord::reset() noexcept
{
    this->mfn = 0;
    this->status = RecordStatus::None;
    this->version = 0;
    this->database.clear();
    return *this;
}

bool LiteRecord::verify (bool throwOnError) const
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

MarcRecord LiteRecord::materialize() const
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

LiteField& LiteField::add (char subFieldCode, const std::string &subFieldValue)
{
    this->subfields.emplace_back(subFieldCode, subFieldValue);
    return *this;
}

LiteField& LiteField::clear()
{
    this->value.clear();
    this->subfields.clear();
    return *this;
}

LiteField LiteField::clone() const
{
    LiteField result (this->tag, this->value);
    for (const auto &sub : this->subfields) {
        result.subfields.emplace_back (sub.code, sub.value);
    }
    return result;
}

void LiteField::decode (const std::string &line)
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
            LiteSubField subField;
            subField.decode(one);
            this->subfields.push_back (subField);
        }
    }
}

bool LiteField::empty() const noexcept
{
    return !this->tag || (this->value.empty() && this->subfields.empty());
}

LiteSubField* LiteField::getFirstSubfield (char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar (one.code, code)) {
            return const_cast<LiteSubField*> (&one);
        }
    }
    return nullptr;
}

std::string LiteField::getFirstSubfieldValue (char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar (one.code, code)) {
            return one.value;
        }
    }
    return std::string();
}

/// \brief Удаление подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \return this.
LiteField& LiteField::removeSubfield (char code)
{
    const auto end = std::end (this->subfields);
    const auto trash = std::remove_if
        (
            std::begin (this->subfields),
            end,
            [code] (LiteSubField &sf) { return sameChar (sf.code, code); }
        );
    this->subfields.erase (trash, end);

    return *this;
}

LiteField& LiteField::setSubfield (char code, const std::string &newValue)
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

bool LiteField::verify (bool throwOnError) const
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

std::string LiteField::toString() const
{
    std::string result = std::to_string(tag)
                    + "#" + value;
    for (const auto &sub : subfields) {
        result += sub.toString();
    }
    return result;
}

RecordField LiteField::materialize() const
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
LiteSubField LiteSubField::clone() const
{
    LiteSubField result;
    result.code = this->code;
    result.value = this->value;
    return result;
}

/// \brief Декодирование подполя из клиентского представления.
/// \param line Строка с клиентским представлением (не должна быть пустой).
void LiteSubField::decode (const std::string &line)
{
    this->code = line[0];
    this->value = line.substr (1);
}

/// \brief Пустое подполе?
/// \return true, если подполе пустое.
bool LiteSubField::empty() const noexcept
{
    return code == 0 || value.empty();
}

/// \brief Проверка, является подполе полноценным (с кодом и значением).
/// \param throwOnError Бросать исключение, если подполе не полноценное.
/// \return true, если с полем всё нормально.
bool LiteSubField::verify (bool throwOnError) const
{
    const bool result = (this->code != 0) && !this->value.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }
    return result;
}

/// \brief Кодирование подполя в клиентское (строковое) представление.
/// \return Закодированное подполе.
std::string LiteSubField::toString() const
{
    return std::string ("^") + this->code + this->value;
}

/// \brief Превращение в полноценное подполе.
/// \return Полноценное подполе.
SubField LiteSubField::materialize() const
{
    return SubField (this->code, fromUtf(this->value));
}

}
