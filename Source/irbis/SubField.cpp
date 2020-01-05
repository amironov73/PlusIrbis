// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iomanip>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Клонирование подполя.
/// \return Точная копия подполя.
SubField SubField::clone() const
{
    return SubField {this->code, this->value };
}

/// \brief Декодирование подполя из клиентского представления.
/// \param line Строка с клиентским представлением (не должна быть пустой).
void SubField::decode(const String &line)
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
std::wstring SubField::toString() const
{
    return std::wstring(L"^") + code + value;
}

/// \brief Проверка, является подполе полноценным (с кодом и значением).
/// \param throwOnError Бросать исключение, если подполе не полноценное.
/// \return true, если с полем всё нормально.
bool SubField::verify(bool throwOnError) const
{
    const bool result = (code != NoCode) && !value.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }

    return result;
}

PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const SubField &subfield)
{
    return stream << std::wstring(L"^") << std::wstring(subfield.code, 1)
        << subfield.value;
}

}

