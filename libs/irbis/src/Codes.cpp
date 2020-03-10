// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Codes.cpp

    \class irbis::Codes

    \var irbis::Codes::type
        \brief Тип документа, подполе t.

    \var irbis::Codes::kind
        \brief Вид документа, подполе b.

    \var irbis::Codes::character1
        \brief Характер документа, подполе c.

    \var irbis::Codes::character2
        \brief Характер документа, подполе 2.

    \var irbis::Codes::character3
        \brief Характер документа, подполе 3.

    \var irbis::Codes::character4
        \brief Характер документа, подполе 4.

    \var irbis::Codes::character5
        \brief Характер документа, подполе 5.

    \var irbis::Codes::character6
        \brief Характер документа, подполе 6.

    \var irbis::Codes::purpose1
        \brief Код целевого назначения, подполе x.

    \var irbis::Codes::purpose2
        \brief Код целевого назначения, подполе y.

    \var irbis::Codes::purpose3
        \brief Код целевого назначения, подполе 9.

    \var irbis::Codes::restrictions
        \brief Возрастные ограничения, подполе z.

    \var irbis::Codes::field
        \brief Поле, из которого извлечена информация.

 */

namespace irbis {

/// \brief Применение к полю.
/// \param field_ Поле для применения.
void Codes::applyTo (RecordField &field_) const
{
    field_.setSubfield ('a', this->type);
    field_.setSubfield ('b', this->kind);
    field_.setSubfield ('c', this->character1);
    field_.setSubfield ('2', this->character2);
    field_.setSubfield ('3', this->character3);
    field_.setSubfield ('4', this->character4);
    field_.setSubfield ('5', this->character5);
    field_.setSubfield ('6', this->character6);
    field_.setSubfield ('x', this->purpose1);
    field_.setSubfield ('y', this->purpose2);
    field_.setSubfield ('9', this->purpose3);
    field_.setSubfield ('z', this->restrictions);
}

/// \brief Разбор поля на элементы.
/// \param field_ Поле для разбора.
void Codes::parse (const RecordField &field_)
{
    this->field = &(const_cast<RecordField&> (field_));
    this->type         = this->fm ('a');
    this->kind         = this->fm ('b');
    this->character1   = this->fm ('c');
    this->character2   = this->fm ('2');
    this->character3   = this->fm ('3');
    this->character4   = this->fm ('4');
    this->character5   = this->fm ('5');
    this->character6   = this->fm ('6');
    this->purpose1     = this->fm ('x');
    this->purpose2     = this->fm ('y');
    this->purpose3     = this->fm ('9');
    this->restrictions = this->fm ('z');
}

/// \brief Получение значения подполя.
/// \param code Код подполя.
/// \return Извлеченное значение.
String Codes::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}
