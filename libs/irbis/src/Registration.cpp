// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Registration.cpp

    \class irbis::Registration

    \var date
        Дата. Подполе *.

    \var chair
        Место (кафедра обслуживания). Подпое c.

    \var number
        Номер приказа. Подполе a.

    \var reason
        Причина. Подполе b.

    \var field
        Поле, из которого была извлечена информация о регистрации.

 */

namespace irbis {

/// \brief Применение к полю.
/// \param field_ Поле для применения.
void Registration::applyTo (RecordField &field_) const
{
    field_.value = this->date;
    field_.setSubfield ( 'c', this->chair);
    field_.setSubfield ( 'a', this->number);
    field_.setSubfield ( 'b', this->reason);
}

/// \brief Разбор поля на элементы адреса.
/// \param field_ Поле для разбора.
void Registration::parse (const RecordField &field_)
{
    this->field  = &(const_cast<RecordField&> (field_));
    this->date   = field_.value;
    this->chair  = this->fm ('c');
    this->number = this->fm ('a');
    this->reason = this->fm ('b');
}

/// \brief Разбор записи на регистрации.
/// \param record Запись.
/// \param tag Метка поля.
/// \return Вектор регистраций.
std::vector<Registration> Registration::parse (const MarcRecord &record, int tag)
{
    std::vector<Registration> result;
    const auto fields = record.getFields (tag);
    for (const auto &field : fields) {
        Registration registration;
        registration.parse (*field);
        result.push_back (registration);
    }
    return result;
}


/// \brief Получение значения подполя.
/// \param code Код подполя.
/// \return Извлеченное значение.
String Registration::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}

