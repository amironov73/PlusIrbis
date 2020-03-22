// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iomanip>
#include <iostream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/*!
    \class irbis::SubField

    Регистр символов для кода подполя не учитывается.

    Как правило, код подполя является арабской цифрой либо латинской буквой.
    Но по факту может оказаться любой печатный символ.
*/

namespace irbis {

/// \brief Клонирование подполя.
/// \return Точная копия подполя.
SubField SubField::clone() const
{
    SubField result;
    result.code  = this->code;
    result.value = this->value;
    return result;
}

/// \brief Декодирование подполя из клиентского представления.
/// \param line Строка с клиентским представлением (не должна быть пустой).
void SubField::decode (const String &line)
{
    this->code  = line[0];
    this->value = line.substr (1);
}

/// \brief Пустое подполе?
/// \return true, если подполе пустое.
bool SubField::empty() const noexcept
{
    return code == NoCode || value.empty();
}

/// \brief Кодирование подполя в клиентское (строковое) представление.
/// \return Закодированное подполе.
std::wstring SubField::toString() const
{
    return String (L"^") + this->code + this->value;
}

/// \brief Проверка, является подполе полноценным (с кодом и значением).
/// \param throwOnError Бросать исключение, если подполе не полноценное.
/// \return true, если с полем всё нормально.
bool SubField::verify (bool throwOnError) const
{
    const bool result = (this->code != NoCode) && !this->value.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }
    return result;
}

IRBIS_API std::wostream& operator << (std::wostream &stream, const SubField &subfield)
{
    return stream << std::wstring (L"^") << std::wstring (subfield.code, 1)
        << subfield.value;
}

}

