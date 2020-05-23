// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

namespace irbis {

/// \brief Конструктор из компонентов даты.
/// \param year Год.
/// \param month Месяц (1-12).
/// \param day День (1-31).
Date::Date (int year, int month, int day)
{
    this->date.tm_sec = 0;
    this->date.tm_min = 0;
    this->date.tm_hour = 0;
    this->date.tm_mday = day;
    this->date.tm_mon = month - 1;
    this->date.tm_year = year - 1900;
    this->date.tm_wday = 0;
    this->date.tm_yday = 0;
    this->date.tm_isdst = 0;
    this->text = convert (&this->date);
}

/// \brief Конструктор из даты.
/// \param text_ Строковое представление в виде YYYYMMDD.
Date::Date(const String &text_)  // NOLINT(modernize-pass-by-value)
    : text(text_)
{
    parse(this->text, &this->date);
}

/// \brief Конструктор из структуры.
/// \param date_ Компоненты даты в виде структуры.
Date::Date(const struct tm *date_)
    : text (convert (date_)), date (*date_)
{
}

/// \brief Преобразование компонентов даты в строку.
/// \param date Компоненты даты в виде структуры.
/// \return Текстовое представление даты.
String Date::convert(const struct tm *date)
{
    Char buf[10];
    std::wcsftime (buf, sizeof(buf)/ sizeof(buf[0]), L"%Y%m%d", date);
    return String (buf);
}

/// \brief Разбор текстового представления даты.
/// \param text Представление даты в виде YYYYMMDD (8 символов).
/// \param date Структура для размещения результата.
/// \return Признак успешного выполнения.
bool Date::parse (const String &text, struct tm *date)
{
    const auto data = text.data();
    date->tm_sec   = 0;
    date->tm_min   = 0;
    date->tm_hour  = 0;
    date->tm_mday  = fastParse32 (data + 6, 2);
    date->tm_mon   = fastParse32 (data + 4, 2) - 1;
    date->tm_year  = fastParse32 (data, 4) - 1900;
    date->tm_wday  = 0;
    date->tm_yday  = 0;
    date->tm_isdst = 0;
    return true;
}

/// \brief Сегодняшняя (локальная) дата.
/// \return Сегодняшняя дата.
Date Date::today()
{
    auto t1 = std::time (nullptr);
    auto t2 = std::localtime (&t1);
    return Date (t2);
}

}
