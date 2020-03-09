// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Exemplar.cpp

    \class irbis::Exemplar

    \var irbis::Exemplar::status
        \brief Статус экземпляра. Подполе `a`.

    \var irbis::Exemplar::number
        \brief Инвентарный номер. Подполе `b`.

    \var irbis::Exemplar::date
        \brief Дата поступления. Подполе `c`.

    \var irbis::Exemplar::place
        \brief Место хранения. Подполе `d`.

    \var irbis::Exemplar::collection
        \brief Наименование коллекции. Подполе `q`.

    \var irbis::Exemplar::shelf
        \brief Расстановочный шифр (полочный индекс). Подполе `r`.

    \var irbis::Exemplar::price
        \brief Цена экземпляра. Подполе `e`.

    \var irbis::Exemplar::barcode
        \brief Штрих-код или радиометка. Подполе `h`.

    \var irbis::Exemplar::amount
        \brief Количество экземпляров. Подполе `1`.

    \var irbis::Exemplar::purpose
        \brief Специальное назначение фонда. Подполе `t`.

    \var irbis::Exemplar::coefficient
        \brief Коэффициент многоразового использования. Подполе `=`.

    \var irbis::Exemplar::offBalance
        \brief Экземпляры не на баланс. Подполе `4`.

    \var irbis::Exemplar::ksuNumber1
        \brief Номер записи КСУ (поступление). Подполе `u`.

    \var irbis::Exemplar::actNumber1
        \brief Номер акта поступления. Подполе `y`.

    \var irbis::Exemplar::channel
        \brief Канал поступления. Подполе `f`.

    \var irbis::Exemplar::onHand
        \brief Число выданных экземпляров. Подполе `2`.

    \var irbis::Exemplar::actNumber2
        \brief Номер акта списания. Подполе `v`.

    \var irbis::Exemplar::writeOff
        \brief Количество списываемых экземпляров. Подполе `x`.

    \var irbis::Exemplar::completion
        \brief Количество экземпляров для докомплектования. Подполе `k`.

    \var irbis::Exemplar::actNumber3
        \brief Номер акта передачи в другое подразделение. Подполе `w`.

    \var irbis::Exemplar::moving
        \brief Количество передаваемых экземпляров. Подполе `z`.

    \var irbis::Exemplar::newPlace
        \brief Новое место хранения. Подполе `m`.

    \var irbis::Exemplar::checkedDate
        \brief Дата проверки фонда. Подполе `s`.

    \var irbis::Exemplar::checkedAmount
        \brief Число проверенных экземпляров. Подполе `0`.

    \var irbis::Exemplar::realPlace
        \brief Реальное место нахождения книги. Подполе `!`.

    \var irbis::Exemplar::bindingIndex
        \brief Шифр подшивки. Подполе `p`.

    \var irbis::Exemplar::bindingNumber
        \brief Инвентарный номер подшивки. Подполе `i`.

    \var irbis::Exemplar::field
        \brief Поле, из которого извлечена информация.

 */

namespace irbis {

/// \brief Применение сведений об экземпляре к указанному полю.
/// \param field_ К какому полю применить.
void Exemplar::applyTo (RecordField &field_) const
{
    field_.setSubfield (L'a', this->status);
    field_.setSubfield (L'b', this->number);
    field_.setSubfield (L'c', this->date);
    field_.setSubfield (L'd', this->place);
    field_.setSubfield (L'q', this->collection);
    field_.setSubfield (L'r', this->shelf);
    field_.setSubfield (L'e', this->price);
    field_.setSubfield (L'h', this->barcode);
    field_.setSubfield (L'1', this->amount);
    field_.setSubfield (L't', this->purpose);
    field_.setSubfield (L'=', this->coefficient);
    field_.setSubfield (L'4', this->offBalance);
    field_.setSubfield (L'u', this->ksuNumber1);
    field_.setSubfield (L'y', this->actNumber1);
    field_.setSubfield (L'f', this->channel);
    field_.setSubfield (L'2', this->onHand);
    field_.setSubfield (L'v', this->actNumber2);
    field_.setSubfield (L'x', this->writeOff);
    field_.setSubfield (L'k', this->completion);
    field_.setSubfield (L'w', this->actNumber3);
    field_.setSubfield (L'z', this->moving);
    field_.setSubfield (L'm', this->newPlace);
    field_.setSubfield (L's', this->checkedDate);
    field_.setSubfield (L'0', this->checkedAmount);
    field_.setSubfield (L'!', this->realPlace);
    field_.setSubfield (L'p', this->bindingIndex);
    field_.setSubfield (L'i', this->bindingNumber);
}

/// \brief Разбор поля на сведения об экземпляре.
/// \param field_ Поле для разбора.
void Exemplar::parse (const RecordField &field_)
{
    this->field         = &(const_cast<RecordField&>(field_));
    this->status        = this->fm (L'a');
    this->number        = this->fm (L'b');
    this->date          = this->fm (L'c');
    this->place         = this->fm (L'd');
    this->collection    = this->fm (L'q');
    this->shelf         = this->fm (L'r');
    this->price         = this->fm (L'e');
    this->barcode       = this->fm (L'h');
    this->amount        = this->fm (L'1');
    this->purpose       = this->fm (L't');
    this->coefficient   = this->fm (L'=');
    this->offBalance    = this->fm (L'4');
    this->ksuNumber1    = this->fm (L'u');
    this->actNumber1    = this->fm (L'y');
    this->channel       = this->fm (L'f');
    this->onHand        = this->fm (L'2');
    this->actNumber2    = this->fm (L'v');
    this->writeOff      = this->fm (L'x');
    this->completion    = this->fm (L'k');
    this->actNumber3    = this->fm (L'w');
    this->moving        = this->fm (L'z');
    this->newPlace      = this->fm (L'm');
    this->checkedDate   = this->fm (L's');
    this->checkedAmount = this->fm (L'0');
    this->realPlace     = this->fm (L'!');
    this->bindingIndex  = this->fm (L'p');
    this->bindingNumber = this->fm (L'i');
}

/// \brief Разбор записи на экземпляры.
/// \param record Запись для разбора.
/// \return Вектор экземпляров.
std::vector<Exemplar> Exemplar::parse (const MarcRecord &record)
{
    std::vector<Exemplar> result;
    const auto fields = record.getFields (Exemplar::TAG);
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
inline String Exemplar::fm (Char code) const noexcept
{
    return this->field->getFirstSubfieldValue (code);
}

}
