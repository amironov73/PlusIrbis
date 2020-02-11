// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

/*!
    \file Ean.cpp

    \class irbis::Ean13

    European Article Number, EAN (европейский номер
    товара) — европейский стандарт штрихкода,
    предназначенный для кодирования идентификатора
    товара и производителя. Является надмножеством
    американского стандарта UPC.

    Пример проверки контрольной суммы

    ```
    4600051000057 (сигареты «Прима») — код EAN-13.

    4x1 + 6x3 + 0x1 + 0x3 + 0x1 + 5x3 + 1x1 + 0x3 + 0x1 + 0x3 + 0x1 + 5x3 + 7x1=
    4 + 18 + 0 + 0 + 0 + 15 + 1 + 0 + 0 + 0 + 0 + 15 + 7= 60.

    Контрольная сумма = 0 — номер правильный.
    ```

    Источник: https://ru.wikipedia.org/wiki/European_Article_Number

    \class irbis::Ean8

    Пример проверки контрольной суммы

    ```
    46009333 (папиросы «Беломорканал») — код EAN-8.

    4x3 + 6x1 + 0x3 + 0x1 + 9x3 + 3x1 + 3x3 + 3x1=
    12 + 6 + 0 + 0 + 27 + 3 + 9 + 3= 60.

    Контрольная сумма = 0 — номер правильный.
    ```

 */

namespace irbis {

// У EAN8 по сравнению с EAN13 коэффициенты сдвинуты на одну позицию.
static int coefficients[] { 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 };

/// \brief Вычисление контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
char Ean13::computeCheckDigit (CharSpan text)
{
    assert (text.size() >= 12);
    auto sum = 0;
    for (auto i = 0; i < 12; i++)
    {
        sum = sum + (text[i] - '0') * coefficients[i];
    }
    auto result = static_cast<char> (10 - sum % 10 + '0');
    return result;
}

/// \brief Вычисление контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
Char Ean13::computeCheckDigit (WideSpan text)
{
    assert (text.size() >= 12);
    auto sum = 0;
    for (auto i = 0; i < 12; i++)
    {
        sum = sum + (text[i] - L'0') * coefficients[i];
    }
    auto result = static_cast<Char> (10 - sum % 10 + L'0');
    return result;
}

/// \brief Проверка контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
bool Ean13::checkControlDigit (CharSpan text)
{
    assert (text.size() >= 13);
    auto sum = 0;
    for (auto i = 0; i < 13; i++)
    {
        sum = sum + (text[i] - '0') * coefficients[i];
    }
    bool result = sum % 10 == 0;
    return result;
}

/// \brief Проверка контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
bool Ean13::checkControlDigit (WideSpan text)
{
    assert (text.size() >= 13);
    auto sum = 0;
    for (auto i = 0; i < 13; i++)
    {
        sum = sum + (text[i] - L'0') * coefficients[i];
    }
    bool result = sum % 10 == 0;
    return result;
}

/// \brief Вычисление контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
char Ean8::computeCheckDigit (CharSpan text)
{
    assert (text.size() >= 7);
    auto sum = 0;
    for (auto i = 0; i < 7; i++)
    {
        sum = sum + (text[i] - '0') * coefficients[i + 1];
    }
    auto result = static_cast<char> (10 - sum % 10 + '0');
    return result;
}

/// \brief Вычисление контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
Char Ean8::computeCheckDigit (WideSpan text)
{
    assert (text.size() >= 7);
    auto sum = 0;
    for (auto i = 0; i < 7; i++)
    {
        sum = sum + (text[i] - L'0') * coefficients[i + 1];
    }
    auto result = static_cast<char> (10 - sum % 10 + L'0');
    return result;
}

/// \brief Проверка контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
bool Ean8::checkControlDigit (CharSpan text)
{
    assert (text.size() >= 8);
    auto sum = 0;
    for (auto i = 0; i < 8; i++)
    {
        sum = sum + (text[i] - '0') * coefficients[i + 1];
    }
    bool result = sum % 10 == 0;
    return result;
}

/// \brief Проверка контрольной цифры.
/// \param text Цифры EAN без посторонних знаков.
/// \return Вычисленная контрольная цифра.
bool Ean8::checkControlDigit (WideSpan text)
{
    assert (text.size() >= 8);
    auto sum = 0;
    for (auto i = 0; i < 8; i++)
    {
        sum = sum + (text[i] - L'0') * coefficients[i + 1];
    }
    bool result = sum % 10 == 0;
    return result;
}

}
