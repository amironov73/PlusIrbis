// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#include <cassert>

/*!
    \file BookInfo.cpp

    \class irbis::BookInfo

    \var irbis::BookInfo::description
        \brief Библиографическое описание.

    \var irbis::BookInfo::firstAuthor
        \brief Первый автор.

    \var irbis::BookInfo::character
        \brief Характер документа.

    \var irbis::BookInfo::title
        \brief Заглавие.

    \var irbis::BookInfo::type
        \brief Тип документа.

    \var irbis::BookInfo::kind
        \brief Вид документа.

    \var irbis::BookInfo::worksheet
        \brief Рабочий лист.

    \var irbis::BookInfo::record
        \brief Запись, из которой были извлечены сведения.

 */

namespace irbis {

/// \brief Конструктор.
/// \param record_ Запись, из которой необходимо извлечь сведения о документе.
BookInfo::BookInfo (MarcRecord *record_)
{
    assert (record_ != nullptr);
    this->record = record_;
    this->character = this->fm (900, 'c');
    this->type = this->fm (900, 't');
    this->kind = this->fm (900, 'b');
    this->firstAuthor = this->fm (700, 'a');
    this->title = this->fm (200, 'a');
    this->worksheet = this->fm (920);
}

/// \brief Количество экземпляров.
/// \return Общее число экземпляров, либо 0, если извлечь сведения не удалось.
int BookInfo::amount()
{
    int result = 0;
    const auto all910 = this->record->getFields (910);
    for (const auto item : all910) {
        const auto a = firstChar (item->getFirstSubfieldValue ('a'));
        if (a == '0' || a == '1' || a == '5' || a == '9') {
            ++result;
        }
        if (sameChar (a, 'C') || sameChar (a, 'U')) {
            auto c = item->getFirstSubfieldValue ('1');
            result += fastParse32 (c);
        }
    }
    return result;
}

/// \brief Авторы.
/// \return Вектор авторов.
StringList BookInfo::authors()
{
    StringList result;
    auto name = this->fm (700, 'a');
    if (!name.empty()) {
        result.push_back (name);
    }
    for (const auto field : this->record->getFields (701)) {
        name = field->getFirstSubfieldValue ('a');
        result.push_back (name);
    }
    return result;
}

/// \brief Электронный ресурс?
/// \return true, если электронный.
bool BookInfo::electronic()
{
    // Электронными считаются:
    // 1. Те, у которых проставлен тип документа L.
    // 2. Те, у которых единица измерения r, j или o
    // 3. Те, к которым прикреплен файл, и это не обложка

    if (sameString (this->type, L"l")
        || sameString (this->type, L"m")) {
        return true;
    }

    const auto measure = firstChar (this->fm (215, '1'));
    if (sameChar (measure, 'r')
        || sameChar (measure, 'j')
        || sameChar (measure, 'o')) {
        return true;
    }

    const auto all951 = this->record->getFields (951);
    for (const auto item : all951) {
        const auto v951h = item->getFirstSubfieldValue ('h');
        return !(sameString (v951h, L"02")
            || sameString (v951h, L"02a")
            || sameString (v951h, L"02b"));
    }

    return false;
}

/// \brief Экземпляры.
/// \return Вектор экземпляров.
std::vector<Exemplar> BookInfo::exemplars()
{
    return Exemplar::parse (*this->record);
}

/// \brief Документ на иностранном языке?
/// \return
bool BookInfo::foreign()
{
    const auto languages = this->languages();
    if (languages.empty()) {
        return false;
    }

    return !sameString (languages[0], L"rus");
}

/// \brief Языки документа.
/// \return Вектор языков.
StringList BookInfo::languages()
{
    return this->record->fma (101);
}

/// \brief Первая ссиылка на электронный ресурс.
/// \return Адрес.
String BookInfo::link()
{
    const auto all951 = this->record->getFields (951);
    for (const auto item : all951) {
        const auto v951h = item->getFirstSubfieldValue ('h');
        if (!(sameString (v951h, L"02")
            || sameString (v951h, L"02a")
            || sameString (v951h, L"02b"))) {
            const auto v951i = item->getFirstSubfieldValue ('i');
            return v951i;
        }
    }
    return String();
}

/// \brief Количество страниц.
/// \return Число страниц, либо 0, если не удалось извлечь сведения.
int BookInfo::pages()
{
    const auto volume = this->volume();
    TextNavigator navigator (volume);
    const auto span = navigator.extractInteger();
    const auto result = fastParse32 (span);
    return result;
}

/// \brief Цена, общая для всех экземпляров.
/// \return Цена, либо 0, если не удалсь извлечь сведения.
float BookInfo::price()
{
    const auto text = this->fm (10, 'd');
    return text.empty() ? 0.0f : std::stof (text);
}

/// \brief Издательства.
/// \return Вектор издательств.
StringList BookInfo::publishers()
{
    StringList result = this->record->fma (210, 'c');
    StringList temp = this->record->fma (461, 'g');
    result.insert (std::end (result), std::begin (temp), std::end (temp));
    return result;
}

/// \brief Счетчик выдач.
/// \return Число выдач, либо 0, если не удалсь извлечь сведения.
int BookInfo::usage()
{
    const auto text = this->fm (999);
    return fastParse32 (text);
}

/// \brief Объем издания (цифры).
/// \return Объем, либо 0, если не удалось извлечь сведения.
String BookInfo::volume()
{
    return this->fm (215, 'a');
}

/// \brief Год издания.
/// \return Год издания, либо 0, если не удалось извлечь сведения.
int BookInfo::year()
{
    auto text = this->fm (210, 'd');
    if (text.empty()) {
        text = this->fm (461, 'h');
    }
    if (text.empty()) {
        text = this->fm (461, 'z');
    }
    if (text.empty()) {
        text = this->fm (463, 'j');
    }
    if (text.empty()) {
        text = this->fm (934);
    }
    if (text.empty()) {
        return 0;
    }

    TextNavigator navigator (text);
    const auto span = navigator.extractInteger();
    const auto result = fastParse32 (span);
    return result;
}

String BookInfo::fm (int tag, Char code) const
{
    return this->record->fm (tag, code);
}

}
