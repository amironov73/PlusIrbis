// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cctype>
#include <cwctype>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/*!
    \class irbis::Span

    Шаблонный класс для невладеющего контейнера, представляющего собой
    непрерывный (типизированный) диапазон памяти произвольного происхождения
    (на стеке, в куче).

 */

namespace irbis {

/// \brief Совпадают ли спаны с точностью до регистра?
/// \param first Первый спан.
/// \param second Второй спан.
/// \return `true` если спаны равны.
bool sameString(CharSpan first, CharSpan second)
{
    auto p1 = first.cbegin(), p2 = second.cbegin(), e1 = first.cend();
    while (p1 < e1) {
        if (::towupper(*p1) != ::toupper(*p2)) {
            return false;
        }
        ++p1; ++p2;
    }

    return true;
}

/// \brief Совпадают ли спаны с точностью до регистра?
/// \param first Первый спан.
/// \param second Второй спан.
/// \return `true` если спаны равны.
bool sameString(WideSpan first, WideSpan second)
{
    auto p1 = first.cbegin(), p2 = second.cbegin(), e1 = first.cend();
    while (p1 < e1) {
        if (::towupper (*p1) != ::toupper (*p2)) {
            return false;
        }
        ++p1; ++p2;
    }

    return true;
}

/// \brief Удаление пробельных элементов из начала спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
CharSpan trimStart (CharSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    while (p1 < e1) {
        if (!std::isspace(*p1)) {
            break;
        }
        ++p1;
    }
    return { const_cast<char*>(p1), static_cast<std::size_t> (e1 - p1) };
}

/// \brief Удаление пробельных элементов из начала спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
WideSpan trimStart (WideSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    while (p1 < e1) {
        if (!std::iswspace(*p1)) {
            break;
        }
        ++p1;
    }
    return { const_cast<Char*>(p1), static_cast<std::size_t> (e1 - p1) };
}

/// \brief Удаление пробельных элементов из конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
CharSpan trimEnd (CharSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend() - 1;
    while (p1 < e1) {
        if (!std::isspace(*e1)) {
            break;
        }
        --e1;
    }
    return { const_cast<char*>(p1), static_cast<std::size_t> (e1 - p1 + 1) };
}

/// \brief Удаление пробельных элементов из конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
WideSpan trimEnd(WideSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend() - 1;
    while (p1 < e1) {
        if (!std::iswspace(*e1)) {
            break;
        }
        --e1;
    }
    return { const_cast<Char*>(p1), static_cast<std::size_t> (e1 - p1 + 1) };
}

/// \brief Удаление пробельных элементов из начала и конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные и конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
CharSpan trim(CharSpan text)
{
    return trimStart(trimEnd(text));
}

/// \brief Удаление пробельных элементов из начала и конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные и конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
WideSpan trim(WideSpan text)
{
    return trimStart(trimEnd(text));
}

/// \brief Перевод текста в спане в верхний регистр "по месту".
/// \param text Текст, подлежащий преобразованию.
/// \return Тот же спан, но уже с преобразованным текстом.
CharSpan toupper(CharSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::toupper(*p));
        ++p;
    }
    return text;
}

/// \brief Перевод текста в спане в верхний регистр "по месту".
/// \param text Текст, подлежащий преобразованию.
/// \return Тот же спан, но уже с преобразованным текстом.
WideSpan toupper(WideSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::towupper(*p));
        ++p;
    }
    return text;
}

/// \brief Перевод текста в спане в нижний регистр "по месту".
/// \param text Текст, подлежащий преобразованию.
/// \return Тот же спан, но уже с преобразованным текстом.
CharSpan tolower(CharSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::tolower(*p));
        ++p;
    }
    return text;
}

/// \brief Перевод текста в спане в нижний регистр "по месту".
/// \param text Текст, подлежащий преобразованию.
/// \return Тот же спан, но уже с преобразованным текстом.
WideSpan tolower(WideSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::towlower(*p));
        ++p;
    }
    return text;
}

}
