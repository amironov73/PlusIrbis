// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Author.cpp

    \class irbis::Author

    \var irbis::Author::familyName
        \brief Фамилия, подполе `a`.

    \var irbis::Author::initials
        \brief Инициалы (сокращение), подполе `b`.

    \var irbis::Author::fullName
        \brief Расширение инициалов (имя и отчество), подполе `g`.

    \var irbis::Author::canBeInverted
        \brief Инвертирование имени недопустимо? Подполе `9`.

    \var irbis::Author::postfix
        \brief Неотъемлемая части (отец, сын, младший, старший и т. п.), подполе `1`.

    \var irbis::Author::appendix
        \brief Дополнения к имени кроме дат (род деятельности, звание, титул и т. д.), подполе `c`.

    \var irbis::Author::number
        \brief Династический номер (римские цифры), подполе `d`.

    \var irbis::Author::dates
        \brief Даты жизни, подполе `f`.

    \var irbis::Author::variant
        \brief Разночтение фамилии, подполе `r`.

    \var irbis::Author::workplace
        \brief Место работы автора, подполе `p`.

    \var irbis::Author::field
        \brief Поле, из которого была извлечена информация.

 */

namespace irbis {

/// \brief Применение сведений об авторе к указанному полю.
/// \param field_ К какому полю применить.
void Author::applyTo (RecordField &field_) const
{
    field_.setSubfield ('a', this->familyName);
    field_.setSubfield ('b', this->initials);
    field_.setSubfield ('g', this->fullName);
    field_.setSubfield ('9', this->canBeInverted);
    field_.setSubfield ('1', this->postfix);
    field_.setSubfield ('c', this->appendix);
    field_.setSubfield ('d', this->number);
    field_.setSubfield ('f', this->dates);
    field_.setSubfield ('r', this->variant);
    field_.setSubfield ('p', this->workplace);
}

/// \brief Разбор поля на сведения об авторе.
/// \param field_ Поле для разбора.
void Author::parse (const RecordField &field_)
{
    this->field         = &(const_cast<RecordField&>(field_));
    this->familyName    = this->fm ('a');
    this->initials      = this->fm ('b');
    this->fullName      = this->fm ('g');
    this->canBeInverted = this->fm ('9');
    this->postfix       = this->fm ('1');
    this->appendix      = this->fm ('c');
    this->number        = this->fm ('d');
    this->dates         = this->fm ('f');
    this->variant       = this->fm ('r');
    this->workplace     = this->fm ('p');
}

/// \brief Разбор записи на авторы.
/// \param record Запись для разбора.
/// \return Вектор авторов.
std::vector<Author> Author::parse (const MarcRecord &record, int tag)
{
    std::vector<Author> result;
    const auto fields = record.getFields (tag);
    for (const auto field : fields) {
        result.emplace_back();
        auto &item = result.back();
        item.parse (*field);
    }
    return result;
}

/// \brief Получение значения подполя с указанным кодом.
/// \param code Искомый код.
/// \return Значение подполя либо пустая строка.
inline String Author::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}
