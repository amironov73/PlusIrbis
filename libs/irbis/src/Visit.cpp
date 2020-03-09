// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Visit.cpp

    \class irbis::Visit

    \var irbis::Visit::database
        \brief Имя БД каталога, подполе g.

    \var irbis::Visit::index
        \brief Шифр документа, подполе a.

    \var irbis::Visit::inventory
        \brief Инвентарный номер экземпляра, подполе b.

    \var irbis::Visit::barcode
        \brief Штрих-код экземпляра, подполе h.

    \var irbis::Visit::sigla
        \brief Место хранения экземпляра, подполе k.

    \var irbis::Visit::given
        \brief Дата выдачи, подполе d.

    \var irbis::Visit::department
        \brief Место выдачи, подполе v.

    \var irbis::Visit::expected
        \brief Дата предполагаемого возврата, подполе e.

    \var irbis::Visit::returned
        \brief Дата фактического возврата, подполе f.

    \var irbis::Visit::prolong
        \brief Дата продления, подполе l.

    \var irbis::Visit::lost
        \brief Признак утерянной книги, подполе u.

    \var irbis::Visit::description
        \brief Краткое библиографическое описание, подполе c.

    \var irbis::Visit::responsible
        \brief Ответственное лицо, подполе i.

    \var irbis::Visit::timeIn
        \brief Время начала визита в библиотеку, подполе 1.

    \var irbis::Visit::timeOut
        \brief Время окончания визита в библиотеку, подполе 2.

    \var irbis::Visit::count
        \brief Счетчик продлений, подполе 4.

    \var irbis::Visit::field
        \brief Поле, из которого извлечены данные о посещении.

 */

namespace irbis {

/// \brief Чистое посещение?
/// \return `true`, если посещение, `false`, если с книговыдачей.
bool Visit::isVisit() const noexcept
{
    return this->index.empty();
}

/// \brief Книга возвращена?
/// \return `true`, если возвращена.
bool Visit::isReturned() const noexcept
{
    const auto &value = this->returned;
    if (value.empty()) {
        return false;
    }
    return value.at(0) != L'*';
}

/// \brief Применение сведений об экземпляре к указанному полю.
/// \param field_ К какому полю применить.
void Visit::applyTo (RecordField &field_) const
{
    field_.setSubfield ('g', this->database);
    field_.setSubfield ('a', this->index);
    field_.setSubfield ('b', this->inventory);
    field_.setSubfield ('h', this->barcode);
    field_.setSubfield ('k', this->sigla);
    field_.setSubfield ('d', this->given);
    field_.setSubfield ('v', this->department);
    field_.setSubfield ('e', this->expected);
    field_.setSubfield ('f', this->returned);
    field_.setSubfield ('l', this->prolong);
    field_.setSubfield ('u', this->lost);
    field_.setSubfield ('c', this->description);
    field_.setSubfield ('i', this->responsible);
    field_.setSubfield ('1', this->timeIn);
    field_.setSubfield ('2', this->timeOut);
    field_.setSubfield ('4', this->count);
}

/// \brief Разбор поля на сведения о посещении/выдачи.
/// \param field_ Поле для разбора.
void Visit::parse (const RecordField &field_)
{
    this->field         = &(const_cast<RecordField&>(field_));
    this->database      = this->fm ('g');
    this->index         = this->fm ('a');
    this->inventory     = this->fm ('b');
    this->barcode       = this->fm ('h');
    this->sigla         = this->fm ('k');
    this->given         = this->fm ('d');
    this->department    = this->fm ('v');
    this->expected      = this->fm ('e');
    this->returned      = this->fm ('f');
    this->prolong       = this->fm ('l');
    this->lost          = this->fm ('u');
    this->description   = this->fm ('c');
    this->responsible   = this->fm ('i');
    this->timeIn        = this->fm ('1');
    this->timeOut       = this->fm ('2');
    this->count         = this->fm ('4');
}

/// \brief Разбор записи на посещения.
/// \param record Запись для разбора.
/// \return Вектор посещения.
std::vector<Visit> Visit::parse (const MarcRecord &record)
{
    std::vector<Visit> result;
    const auto fields = record.getFields (Visit::TAG);
    for (const auto field : fields) {
        result.emplace_back();
        auto &item = result.back();
        item.parse(*field);
    }
    return result;
}

/// \brief Получение значения подполя с указанным кодом.
/// \param code Искомый код.
/// \return Значение подполя либо пустая строка.
inline String Visit::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}
