// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"
#include <algorithm>

/*!

    \file Phantom.cpp

    \class irbis::PhantomRecord

    Фантомные записи не владеют памятью, на которую ссылаются.
    Они требуют гораздо меньших затрат.

    При необходимости фантомная запись может быть материализована.

 */

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Клонирование подполя.
/// \return Точная копия подполя.
PhantomSubField PhantomSubField::clone() const
{
    PhantomSubField result;
    result.code = this->code;
    result.value = this->value;
    return result;
}

/// \brief Декодирование подполя из клиентского представления.
/// \param line Строка с клиентским представлением (не должна быть пустой).
void PhantomSubField::decode (ByteSpan line)
{
    this->code = line[0];
    this->value = line.slice (1);
}

/// \brief Пустое подполе?
/// \return true, если подполе пустое.
bool PhantomSubField::empty() const noexcept
{
    return code == 0 || value.empty();
}

/// \brief Проверка, является подполе полноценным (с кодом и значением).
/// \param throwOnError Бросать исключение, если подполе не полноценное.
/// \return true, если с полем всё нормально.
bool PhantomSubField::verify (bool throwOnError) const
{
    const bool result = (this->code != 0) && !this->value.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }
    return result;
}

/// \brief Кодирование подполя в клиентское (строковое) представление.
/// \return Закодированное подполе.
std::string PhantomSubField::toString() const
{
    return std::string ("^") + std::string (1, this->code)
        + std::string (reinterpret_cast<char*>(this->value.ptr), this->value.length);
}

/// \brief Материализация фантомного подполя.
/// \return Созданное подполе.
SubField PhantomSubField::materialize() const
{
    SubField result;
    result.code = this->code;
    result.value = fromUtf (this->value);
    return result;
}

//=========================================================

PhantomField& PhantomField::add (Byte subFieldCode, ByteSpan subFieldValue)
{
    this->subfields.emplace_back(subFieldCode, subFieldValue);
    return *this;
}

PhantomField& PhantomField::clear()
{
    this->value.length = 0u;
    this->subfields.clear();
    return *this;
}

PhantomField PhantomField::clone() const
{
    PhantomField result (this->tag, this->value);
    for (const auto &sub : this->subfields) {
        result.subfields.emplace_back (sub.code, sub.value);
    }
    return result;
}

void PhantomField::decode (ByteSpan line)
{
    throw NotImplementedException();
}

bool PhantomField::empty() const noexcept
{
    return !this->tag || (this->value.empty() && this->subfields.empty());
}

PhantomSubField* PhantomField::getFirstSubfield (Byte code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar (one.code, code)) {
            return const_cast<PhantomSubField*> (&one);
        }
    }
    return nullptr;
}

ByteSpan PhantomField::getFirstSubfieldValue (Byte code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar (one.code, code)) {
            return one.value;
        }
    }
    return {};
}

/// \brief Удаление подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \return this.
PhantomField& PhantomField::removeSubfield (Byte code)
{
    const auto end = std::end (this->subfields);
    const auto trash = std::remove_if
        (
            std::begin(this->subfields),
            end,
            [code] (PhantomSubField &sf) { return sf.code == code; }
        );
    this->subfields.erase (trash, end);

    return *this;
}

PhantomField& PhantomField::setSubfield (Byte code, ByteSpan newValue)
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

bool PhantomField::verify (bool throwOnError) const
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

std::string PhantomField::toString() const
{
    std::string result = std::to_string(tag)
            + "#"
            + std::string (reinterpret_cast<char*>(this->value.ptr), this->value.length);
    for (const auto &sub : subfields) {
        result += sub.toString();
    }
    return result;
}

/// \brief Материализация фантомного поля.
/// \return Созданное поле.
RecordField PhantomField::materialize() const
{
    RecordField result (this->tag);
    result.value = fromUtf (this->value);
    for (const auto &subfield : this->subfields) {
        result.subfields.push_back(subfield.materialize());
    }
    return result;
}

//=========================================================

/// \brief Материализация фантомной записи.
/// \return Созданная запись.
MarcRecord PhantomRecord::materialize() const
{
    MarcRecord result;
    result.mfn = this->mfn;
    result.status = this->status;
    result.version = this->version;
    for (const auto &field : this->fields) {
        result.fields.push_back(field.materialize());
    }
    return result;
}

}
