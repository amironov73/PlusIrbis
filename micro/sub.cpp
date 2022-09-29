// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"
#include "micro.h"

/*!
    \class irbis::SubField

    Регистр символов для кода подполя не учитывается.

    Как правило, код подполя является арабской цифрой либо латинской буквой.
    Но по факту может оказаться любой печатный символ.
*/

namespace irbis
{

    /// \brief Клонирование подполя.
    /// \return Точная копия подполя.
    SubField SubField::clone() const
    {
        SubField result;
        result.code = this->code;
        result.value = this->value;
        return result;
    }

    /// \brief Декодирование подполя из клиентского представления.
    /// \param line Строка с клиентским представлением (не должна быть пустой).
    void SubField::decode(const std::string &line)
    {
        this->code = line[0];
        this->value = line.substr(1);
    }

    /// \brief Пустое подполе?
    /// \return true, если подполе пустое.
    bool SubField::empty() const noexcept
    {
        return code == NoCode || value.empty();
    }

    /// \brief Кодирование подполя в клиентское (строковое) представление.
    /// \return Закодированное подполе.
    std::string SubField::toString() const
    {
        return std::string("^") + this->code + this->value;
    }

    std::ostream& operator<< (std::ostream &stream, const SubField &subfield)
    {
        return stream << std::string ("^") << std::string (subfield.code, 1) << subfield.value;
    }

    /// \brief Изменение значения подполя.
    /// \param value_ Новое значение подполя.
    /// \return this.
    SubField &SubField::operator<<(const std::string &value_)
    {
        this->value = value_;
        return *this;
    }

    /// \brief Изменение значения подполя.
    /// \param value_ Новое значение подполя.
    /// \return this.
    SubField &SubField::operator<<(std::string &&value_)
    {
        this->value = std::move(value_);
        return *this;
    }

    /// \brief Изменение значения подполя.
    /// \param value_ Новое значение подполя.
    /// \return this.
    SubField &SubField::operator<<(const char *value_)
    {
        this->value = value_;
        return *this;
    }
}
