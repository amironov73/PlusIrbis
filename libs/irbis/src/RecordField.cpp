// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

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

/// \brief Конструктор.
/// \param tag Метка поля.
/// \param subfields_ Перечень подполей.
RecordField::RecordField (int tag_, std::initializer_list <SubField> subfields_)
    : tag { tag_ }, subfields { subfields_ }
{
}

/// \brief Пользовательский литерал для формирования поля.
/// \param tag Метка поля.
/// \return Сконструированное поле.
RecordField operator "" _field (unsigned long long int tag)
{
    return RecordField (static_cast <int> (tag));
}

/// \brief Пользовательский литерал для формирования поля.
/// \param text Закодированное представление поля.
/// \param size Размер закодированного представления.
/// \return Сконструированное поле.
RecordField operator "" _field (const char *text, std::size_t size)
{
    RecordField result;
    result.decode (fromUtf (text));
    return result;
}

/// \brief Пользовательский литерал для формирования поля.
/// \param text Закодированное представление поля.
/// \param size Размер закодированного представления.
/// \return Сконструированное поле.
RecordField operator "" _field (const wchar_t *text, std::size_t size)
{
    RecordField result;
    result.decode (text);
    return result;
}

RecordField& RecordField::_ (const char *value_)
{
    this->value = fromUtf (value_);
    return *this;
}

RecordField& RecordField::_ (const wchar_t *value_)
{
    this->value = String (value_);
    return *this;
}

/// \brief Добавление подполя.
/// \param subFieldCode Код подполя.
/// \param subFieldValue Значение подполя.
/// \return this.
RecordField& RecordField::add (Char subFieldCode, const String &subFieldValue)
{
    this->subfields.emplace_back (subFieldCode, subFieldValue);
    return *this;
}

/// \brief Добавление подполя.
/// \param subFieldCode Код подполя.
/// \param subFieldValue Значение подполя.
/// \return this.
RecordField& RecordField::add (Char subFieldCode,  String &&subFieldValue)
{
    this->subfields.emplace_back (subFieldCode, std::move (subFieldValue));
    return *this;
}

/// \brief Очистка (удаление всех подполей).
/// \return this.
RecordField& RecordField::clear()
{
    value.clear();
    this->subfields.clear();
    return *this;
}

/// \brief Создание глубокой копии поля.
/// \return Глубокая копия.
RecordField RecordField::clone() const
{
    RecordField result (this->tag, this->value);
    for (const auto &sub : this->subfields) {
        result.subfields.emplace_back (sub.code, sub.value);
    }
    return result;
}

/// \brief Декодирование текстового представления тела поля.
/// \param body Текст для декодирования.
void RecordField::decodeBody (const String &body)
{
    StringList all;
    if (body[0] == L'^') {
        all = split (body, L'^');
    } else {
        const auto parts2 = maxSplit (body, L'#', 2);
        this->value = parts2[0];
        all = parts2.size() == 1 ? StringList () : split (parts2[1], L'^');
    }
    for (const auto &one : all) {
        if (!one.empty()) {
            SubField subField;
            subField.decode (one);
            this->subfields.push_back (subField);
        }
    }
}

/// \brief Декодирование текстового представления поля,
/// \param line Текст для декодирования.
void RecordField::decode (const String &line)
{
    const auto parts = maxSplit (line, L'#', 2);
    this->tag = fastParse32 (parts[0]);
    if (parts.size() == 1 || parts[1].empty()) {
        return;
    }
    const auto body = parts[1];
    this->decodeBody (body);
}

/// \brief Пустое поле (нет значения и подполей)?
/// \return true если пустое.
bool RecordField::empty() const noexcept
{
    return !this->tag || (this->value.empty() && this->subfields.empty());
}

/// \brief Получение указателя на первое подполе с указанным кодом.
/// \param code Искомый код подполя.
/// \return Указатель на подполе либо `nullptr`.
SubField* RecordField::getFirstSubfield (Char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar(one.code, code)) {
            return const_cast<SubField*> (&one);
        }
    }
    return nullptr;
}

/// \brief Получение значения первого подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \return Значение первого подполя либо пустая строка.
String RecordField::getFirstSubfieldValue (Char code) const noexcept
{
    for (const auto &one : this->subfields) {
        if (sameChar(one.code, code)) {
            return one.value;
        }
    }
    return {};
}

/// \brief Удаление подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \return this.
RecordField& RecordField::removeSubfield (Char code)
{
    const auto end = std::end (this->subfields);
    const auto trash = std::remove_if
        (
            std::begin(this->subfields),
            end,
            [code] (SubField &sf) { return sameChar (sf.code, code); }
        );
    this->subfields.erase (trash, end);

    return *this;
}

/// \brief Установка значения подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \param newValue Новое значение поля.
/// \return this.
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

/// \brief Установка значения подполя с указанным кодом.
/// \param code Искомый код подполя.
/// \param newValue Новое значение поля.
/// \return this.
RecordField& RecordField::setSubfield (Char code, String &&newValue)
{
    if (newValue.empty()) {
        this->removeSubfield (code);
        return *this;
    }
    auto *subField = this->getFirstSubfield (code);
    if (!subField) {
        this->add (code, std::move (newValue));
    }
    else {
        subField->value = std::move (newValue);
    }
    return *this;
}

/// \brief Верификация поля.
/// \param throwOnError Бросать исключение при ошибке.
/// \return true если поле верифицировано.
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

/// \brief Получение строкового представления поля.
/// \return Строковое представление поля.
String RecordField::toString() const
{
    String result = std::to_wstring(tag)
        + std::wstring(L"#") + value;
    for (const auto &sub : subfields) {
        result += sub.toString();
    }
    return result;
}

IRBIS_API std::wostream& operator << (std::wostream &stream, const RecordField &field)
{
    stream << std::to_wstring (field.tag) << std::wstring (L"#") << field.value;
    for (const auto &sub : field.subfields) {
        stream << sub;
    }

    return stream;
}

}
