// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

/*!
    \file Reader.cpp

    \class irbis::Reader

    \var irbis::Reader::familyName
        \brief Фамилия. Поле 10.

    \var irbis::Reader::firstName
        \brief Имя. Поле 11.

    \var irbis::Reader::patronymic
        \brief Отчество. Поле 12.

    \var irbis::Reader::birth
        \brief Дата рождения. Поле 21.

    \var irbis::Reader::ticket
        \brief Номер читательского билета. Поле 30.

    \var irbis::Reader::gender
        \brief Пол. Поле 23.

    \var irbis::Reader::category
        \brief Категория. Поле 50.

    \var irbis::Reader::address
        \brief Домашний адрес. Поле 13.

    \var irbis::Reader::workplace
        \brief Место работы. Поле 15.

    \var irbis::Reader::education
        \brief Образование. Поле 20.

    \var irbis::Reader::email
        \brief Электронная почта. Поле 32.

    \var irbis::Reader::phone
        \brief Домашний телефон. Поле 17.

    \var irbis::Reader::registration
        \brief Дата записи в библиотеку. Поле 51.

    \var irbis::Reader::enrollment
        \brief Запись/перерегистрация в библиотеке. Поле 51.

    \var irbis::Reader::registrations
        \brief Дата перерегистрации. Поле 52.

    \var irbis::Reader::enabled
        \brief Разрешенные места получения литературы. Поле 56.

    \var irbis::Reader::disabled
        \brief Запрещенные места получения литературы. Поле 57.

    \var irbis::Reader::right
        \brief Право пользования библиотекой. Поле 29.

    \var irbis::Reader::remarks
        \brief Примечания. Поле 33.

    \var irbis::Reader::photo
        \brief Фотография. Поле 950.

    \var irbis::Reader::visits
        \brief Посещения.

    \var irbis::Reader::password
        \brief Пароль.

    \var irbis::Reader::record
        \brief Запись, из которой извлечены данные о читателе.

 */

namespace irbis {

/// \brief Полное ФИО читателя.
/// \return ФИО.
String Reader::fullName() const
{
    String result = this->familyName;
    if (!this->firstName.empty()) {
        result = result + L" " + this->firstName;
    }
    if (!this->patronymic.empty()) {
        result = result + L" " + this->patronymic;
    }
    return result;
}

/// \brief Применение к записи.
/// \param record_ Запись, в которую нужно внести изменения.
void Reader::applyTo (MarcRecord &record_) const
{
    record_
        .setField (10, this->familyName)
        .setField (11, this->firstName)
        .setField (12, this->patronymic)
        .setField (21, this->birth)
        .setField (30, this->ticket)
        .setField (23, this->gender)
        .setField (50, this->category)
        .setField (15, this->workplace)
        .setField (20, this->education)
        .setField (32, this->email)
        .setField (17, this->phone)
        // TODO implement enrolment
        // TODO implement registrations
        .setField (56, this->enabled)
        .setField (57, this->disabled)
        .setField (29, this->right)
        .setField (33, this->remarks)
        .setField (950, this->photo)
        // TODO implement visits
        .setField (130, this->password);

    auto field13 = record_.getField (13);
    if (!field13) {
        field13 = &record_.add (13, L"");
    }
    this->address.applyTo (*field13);

    auto field51 = record_.getField (51);
    if (!field51) {
        field51 = &record_.add (51, L"");
    }
    this->registration.applyTo (*field51);
}

/// \brief Разбор записи.
/// \param record_ Запись для разбора.
void Reader::parse (const MarcRecord &record_)
{
    this->record = &(const_cast<MarcRecord&>(record_));
    this->mfn           = record_.mfn;
    this->familyName    = this->fm (10);
    this->firstName     = this->fm (11);
    this->patronymic    = this->fm (12);
    this->birth         = this->fm (21);
    this->ticket        = this->fm (30);
    this->gender        = this->fm (23);
    this->category      = this->fm (50);
    this->workplace     = fm (15);
    this->education     = fm (20);
    this->email         = fm (32);
    this->phone         = fm (17);
    this->enrollment    = Registration::parse (record_, 51);
    this->registrations = Registration::parse (record_, 52);
    this->enabled       = fm (56);
    this->disabled      = fm (57);
    this->right         = fm (29);
    this->remarks       = fm (33);
    this->photo         = fm (950);
    this->visits        = Visit::parse (record_);
    this->password      = fm (130);

    const auto field13 = record_.getField (13);
    if (field13) {
        this->address.parse (*field13);
    }

    const auto field51 = record_.getField (51);
    if (field51) {
        this->registration.parse (*field51);
    }
}

/// \brief Получение значения поля/подполя.
/// \param tag Метка поля.
/// \param code Код подполя.
/// \return Значение поля/подполя либо пустая строка.
inline String Reader::fm (int tag, Char code) const noexcept
{
    return this->record->fm (tag, code);
}

}
