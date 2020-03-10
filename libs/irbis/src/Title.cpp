// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!

    \file Title.cpp

    \class irbis::Title

    \var irbis::Title::number
        \brief Обозначение и номер тома, подполе v.

    \var irbis::Title::title
        \brief Заглавие, подполе a.

    \var irbis::Title::specific
        \brief Нехарактерное заглавие, подполе u.

    \var irbis::Title::general
        \brief Общее обозначение материала, подполе b.

    \var irbis::Title::subtitle
        \brief Сведения, относящиеся к заглавию, подполе e.

    \var irbis::Title::first
        \brief Первые сведения об ответственности, подполе f.

    \var irbis::Title::other
        \brief Последующие сведения об ответственности, подполе g.

    \var irbis::Title::field
        \brief Поле, из которого была извлечена информация.

 */

namespace irbis {

/// \brief Применение к полю.
/// \param field_ Поле для применения.
void Title::applyTo (RecordField &field_) const
{
    field_.setSubfield ('v', this->number);
    field_.setSubfield ('a', this->title);
    field_.setSubfield ('u', this->specific);
    field_.setSubfield ('b', this->general);
    field_.setSubfield ('e', this->subtitle);
    field_.setSubfield ('f', this->first);
    field_.setSubfield ('g', this->other);
}

/// \brief Заглавие полностью,
/// \return
String Title::fullTitle() const
{
    String result;

    if (!this->number.empty()) {
        result += this->number;
    }

    if (!this->title.empty()) {
        if (!result.empty()) {
            result += L". ";
        }
        result += this->title;
    }

    if (!this->general.empty()) {
        if (!result.empty()) {
            result += L" ";
        }
        result = result + L"[" + this->general + L"]";
    }

    if (!this->subtitle.empty()) {
        if (!result.empty()) {
            result = result + L" : " + this->subtitle;
        }
    }

    return result;
}

/// \brief Разбор поля на элементы.
/// \param field_ Поле для разбора.
void Title::parse (const RecordField &field_)
{
    this->field    = &(const_cast<RecordField&> (field_));
    this->number   = this->fm ('v');
    this->title    = this->fm ('a');
    this->specific = this->fm ('u');
    this->general  = this->fm ('b');
    this->subtitle = this->fm ('e');
    this->first    = this->fm ('f');
    this->other    = this->fm ('g');
}

/// \brief Получение значения подполя.
/// \param code Код подполя.
/// \return Извлеченное значение.
String Title::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}
