// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Exemplar.cpp

    \class irbis::Exemplar

    \var status
        Статус экземпляра. Подполе `a`.

    \var number
        Инвентарный номер. Подполе `b`.

    \var date
        Дата поступления. Подполе `c`.

    \var place
        Место хранения. Подполе `d`.

    \var collection
        Наименование коллекции. Подполе `q`.

    \var shelf
        Расстановочный шифр (полочный индекс). Подполе `r`.

    \var price
        Цена экземпляра. Подполе `e`.

    \var barcode
        Штрих-код или радиометка. Подполе `h`.

    \var amount
        Количество экземпляров. Подполе `1`.

    \var purpose
        Специальное назначение фонда. Подполе `t`.

    \var coefficient
        Коэффициент многоразового использования. Подполе `=`.

    \var offBalance
        Экземпляры не на баланс. Подполе `4`.

    \var ksuNumber1
        Номер записи КСУ (поступление). Подполе `u`.

    \var actNumber1
        Номер акта поступления. Подполе `y`.

    \var channel
        Канал поступления. Подполе `f`.

    \var onHand
        Число выданных экземпляров. Подполе `2`.

    \var actNumber2
        Номер акта списания. Подполе `v`.

    \var writeOff
        Количество списываемых экземпляров. Подполе `x`.

    \var completion
        Количество экземпляров для докомплектования. Подполе `k`.

    \var actNumber3
        Номер акта передачи в другое подразделение. Подполе `w`.

    \var moving
        Количество передаваемых экземпляров. Подполе `z`.

    \var newPlace
        Новое место хранения. Подполе `m`.

    \var checkedDate
        Дата проверки фонда. Подполе `s`.

    \var checkedAmount
        Число проверенных экземпляров. Подполе `0`.

    \var realPlace
        Реальное место нахождения книги. Подполе `!`.

    \var bindingIndex
        Шифр подшивки. Подполе `p`.

    \var bindingNumber
        Инвентарный номер подшивки. Подполе `i`.

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
    this->status        = field_.getFirstSubfieldValue (L'a');
    this->number        = field_.getFirstSubfieldValue (L'b');
    this->date          = field_.getFirstSubfieldValue (L'c');
    this->place         = field_.getFirstSubfieldValue (L'd');
    this->collection    = field_.getFirstSubfieldValue (L'q');
    this->shelf         = field_.getFirstSubfieldValue (L'r');
    this->price         = field_.getFirstSubfieldValue (L'e');
    this->barcode       = field_.getFirstSubfieldValue (L'h');
    this->amount        = field_.getFirstSubfieldValue (L'1');
    this->purpose       = field_.getFirstSubfieldValue (L't');
    this->coefficient   = field_.getFirstSubfieldValue (L'=');
    this->offBalance    = field_.getFirstSubfieldValue (L'4');
    this->ksuNumber1    = field_.getFirstSubfieldValue (L'u');
    this->actNumber1    = field_.getFirstSubfieldValue (L'y');
    this->channel       = field_.getFirstSubfieldValue (L'f');
    this->onHand        = field_.getFirstSubfieldValue (L'2');
    this->actNumber2    = field_.getFirstSubfieldValue (L'v');
    this->writeOff      = field_.getFirstSubfieldValue (L'x');
    this->completion    = field_.getFirstSubfieldValue (L'k');
    this->actNumber3    = field_.getFirstSubfieldValue (L'w');
    this->moving        = field_.getFirstSubfieldValue (L'z');
    this->newPlace      = field_.getFirstSubfieldValue (L'm');
    this->checkedDate   = field_.getFirstSubfieldValue (L's');
    this->checkedAmount = field_.getFirstSubfieldValue (L'0');
    this->realPlace     = field_.getFirstSubfieldValue (L'!');
    this->bindingIndex  = field_.getFirstSubfieldValue (L'p');
    this->bindingNumber = field_.getFirstSubfieldValue (L'i');
    this->field         = &(const_cast<RecordField&>(field_));
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
        item.parse(*field);
    }
    return result;
}

}
