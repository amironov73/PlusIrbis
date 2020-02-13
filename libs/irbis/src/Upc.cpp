// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

/*!
    \file Upc.cpp

    UPC или Universal Product Code (универсальный код
    товара) — американский стандарт штрихкода,
    предназначенный для отслеживания товаров в магазинах.
    UPC был разработан в 1973 году Джорджем Джосефом
    Лорером (George Joseph Laurer), работавшим инженером
    в корпорации IBM. В июне 1974 года первый UPC сканер
    производства корпорации NCR был установлен
    в супермаркете Марш (Marsh) в городе Трой (Troy)
    штата Огайо. 26 июня 1974 года кассиром этого
    супермаркета был просканирован первый товар — блок
    10 фруктовых жевательных резинок компании Wrigley.

    Пример проверки контрольной суммы

    ```
    041689300494 (бензин для зажигалки «Zippo») — код UPC-12.
    0x3 + 4x1 + 1x3 + 6x1 + 8x3 + 9x1 + 3x3 + 0x1 + 0x3 + 4x1 + 9x3 + 4x1=
    0 + 4 + 3 + 6 + 24 + 9 + 9 + 0 + 0 + 4 + 27 + 4= 90.

    Контрольная сумма = 0 — номер правильный.
    ```
 */

namespace irbis {

static int coefficients[] { 3, 1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 1 };


/// \brief Вычисление контрольной цифры.
/// \param text Цифры UPC без посторонних знаков.
/// \return Вычисленная контрольная цифра.
char Upc12::computeCheckDigit (CharSpan text)
{
    assert (text.size() >= 11);
    auto sum = 0;
    auto index = 0;
    for (const auto c : text) {
        if (index >= 11) {
            break;
        }
        sum = sum + (c - '0') * coefficients[index];
        ++index;
    }
    auto result = static_cast<char> (10 - sum % 10 + '0');
    return result;
}

/// \brief Вычисление контрольной цифры.
/// \param text Цифры UPC без посторонних знаков.
/// \return Вычисленная контрольная цифра.
Char Upc12::computeCheckDigit (WideSpan text)
{
    assert (text.size() >= 11);
    auto sum = 0;
    auto index = 0;
    for (const auto c : text) {
        if (index >= 11) {
            break;
        }
        sum = sum + (c - L'0') * coefficients[index];
        ++index;
    }
    auto result = static_cast<char> (10 - sum % 10 + L'0');
    return result;
}

/// \brief Проверка контрольной цифры.
/// \param text Цифры UPC без посторонних знаков.
/// \return Вычисленная контрольная цифра.
bool Upc12::checkControlDigit (CharSpan text)
{
    assert (text.size() >= 12);
    auto sum = 0;
    auto index = 0;
    for (const auto c : text) {
        if (index >= 12) {
            break;
        }
        sum = sum + (c - '0') * coefficients[index];
        ++index;
    }
    assert (text.size() == 12);
    bool result = sum % 10 == 0;
    return result;
}

/// \brief Проверка контрольной цифры.
/// \param text Цифры UPC без посторонних знаков.
/// \return Вычисленная контрольная цифра.
bool Upc12::checkControlDigit (WideSpan text)
{
    assert (text.size() >= 12);
    auto sum = 0;
    for (auto i = 0; i < 12; i++) {
        sum = sum + (text[i] - L'0') * coefficients[i];
    }
    bool result = sum % 10 == 0;
    return result;
}

}
