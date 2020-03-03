// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Address.cpp

    \class irbis::Address

    \var postcode
        Почтовый индекс, подполе a.

    \var country
        Страна/республика, подполе b.

    \var city
        Город, подполе c.

    \var street
        Улица, подполе d.

    \var building
        Номер дома, подполе e.

    \var entrance
        Номер подъезда, подполе g.

    \var apartment
        Номер квартиры, подполе h.

    \var additional
        Дополнительные данные, подполе f.

    \var field
        Поле, из которого извлечены данные об адресе.

 */

namespace irbis {

/// \brief Применение к полю.
/// \param field_ Поле для применения.
void Address::applyTo (RecordField &field_) const
{
    field_.setSubfield ('a', this->postcode);
    field_.setSubfield ('b', this->country);
    field_.setSubfield ('c', this->city);
    field_.setSubfield ('d', this->street);
    field_.setSubfield ('e', this->building);
    field_.setSubfield ('g', this->entrance);
    field_.setSubfield ('h', this->apartment);
    field_.setSubfield ('f', this->additional);
}

/// \brief Разбор поля на элементы адреса.
/// \param field_ Поле для разбора.
void Address::parse (const RecordField &field_)
{
    this->field = &(const_cast<RecordField&> (field_));
    this->postcode   = this->fm ('a');
    this->country    = this->fm ('b');
    this->city       = this->fm ('c');
    this->street     = this->fm ('d');
    this->building   = this->fm ('e');
    this->entrance   = this->fm ('g');
    this->apartment  = this->fm ('h');
    this->additional = this->fm ('f');
}

/// \brief Получение значения подполя.
/// \param code Код подполя.
/// \return Извлеченное значение.
String Address::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}

