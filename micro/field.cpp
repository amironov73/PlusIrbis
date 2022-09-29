// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"
#include "micro.h"

/*!

    \class irbis::RecordField

    В большинстве случаев метки поля попадают в диапазон от 1 до 999,
    но на практике могут оказаться любым натуральным числом в диапазоне
    от 1 до 2 147 483 647‬.

 */

namespace irbis
{
    /// \brief Конструктор.
    /// \param tag Метка поля.
    /// \param subfields_ Перечень подполей.
    RecordField::RecordField (int tag_, std::initializer_list <SubField> subfields_)
        : tag { tag_ }, subfields { subfields_ }
    {
        // пустое тело конструктора
    }

    /// \brief Добавление подполя.
    /// \param subFieldCode Код подполя.
    /// \param subFieldValue Значение подполя.
    /// \return this.
    RecordField& RecordField::add (char subFieldCode, const std::string &subFieldValue)
    {
        this->subfields.emplace_back (subFieldCode, subFieldValue);
        return *this;
    }

    /// \brief Добавление подполя.
    /// \param subFieldCode Код подполя.
    /// \param subFieldValue Значение подполя.
    /// \return this.
    RecordField& RecordField::add (char subFieldCode, std::string &&subFieldValue)
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
    void RecordField::decodeBody (const std::string &body)
    {
        StringList all;
        if (body[0] == '^') {
            all = split (body, '^');
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
    void RecordField::decode (const std::string &line)
    {
        const auto parts = maxSplit (line, L'#', 2);
        this->tag = fastParse32 (parts[0]);
        if (parts.size() == 1 || parts[1].empty()) {
            return;
        }
        const auto &body = parts[1];
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
    SubField* RecordField::getFirstSubfield (char code) const noexcept
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
    std::string RecordField::getFirstSubfieldValue (char code) const noexcept
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
    RecordField& RecordField::removeSubfield (char code)
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
    RecordField& RecordField::setSubfield (char code, const std::string &newValue)
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
    RecordField& RecordField::setSubfield (char code, std::string &&newValue)
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

    /// \brief Получение строкового представления поля.
    /// \return Строковое представление поля.
    std::string RecordField::toString() const
    {
        std::string result = std::to_string(tag) + std::string("#") + value;
        for (const auto &sub : subfields) {
            result += sub.toString();
        }
        return result;
    }

    std::ostream& operator << (std::ostream &stream, const RecordField &field)
    {
        stream << std::to_string (field.tag) << std::string ("#") << field.value;
        for (const auto &sub : field.subfields) {
            stream << sub;
        }

        return stream;
    }

    /// \brief Добавление подполя в конец поля.
    /// \param subfield Подполе для добавления.
    /// \return this.
    RecordField& RecordField::operator << (const SubField &subfield)
    {
        this->subfields.push_back (subfield);
        return *this;
    }

    /// \brief Добавление подполя в конец поля.
    /// \param subfield Подполе для добавления.
    /// \return this.
    RecordField& RecordField::operator << (SubField &&subfield)
    {
        this->subfields.push_back (std::move (subfield));
        return *this;
    }

    /// \brief Изменение значения поля. Если в записи присутствуют подполя, то изменяется значение последнего подполя.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField& RecordField::operator << (const std::string &body)
    {
        if (contains(body, '^')) {
            this->decodeBody (body);
        }
        else {
            if (this->subfields.empty()) {
                this->value = body;
            }
            else {
                this->subfields.back().value = body;
            }
        }
        return *this;
    }

    /// \brief Изменение значения поля. Если в записи присутствуют подполя, то изменяется значение последнего подполя.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField& RecordField::operator << (std::string &&body)
    {
        if (contains(body, '^')) {
            this->decodeBody (body);
        }
        else {
            if (this->subfields.empty()) {
                this->value = std::move (body);
            }
            else {
                this->subfields.back().value = std::move (body);
            }
        }
        return *this;
    }

    /// \brief Изменение значения поля.
    /// \param body Новое значение поля.
    /// \return this.
    RecordField& RecordField::operator << (const char *body)
    {
        return *this << std::string (body);
    }

    /// \brief Добавление подполя в конец поля.
    /// \param code Код подполя.
    /// \return this.
    RecordField& RecordField::operator << (char code)
    {
        return *this << SubField (code);
    }
}
