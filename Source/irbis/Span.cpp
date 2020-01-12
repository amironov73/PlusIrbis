// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cctype>
#include <cwctype>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Совпадают ли спаны с точностью до регистра?
/// \param first Первый спан.
/// \param second Второй спан.
/// \return `true` если спаны равны.
bool sameString(CSpan first, CSpan second)
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
bool sameString(WSpan first, WSpan second)
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


/// \brief Удаление пробельных элементов из начала спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
CSpan trimStart(CSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    while (p1 < e1) {
        if (!std::isspace(*p1)) {
            break;
        }
        ++p1;
    }
    return CSpan (const_cast<char*>(p1), e1 - p1);
}

/// \brief Удаление пробельных элементов из начала спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
WSpan trimStart(WSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend();
    while (p1 < e1) {
        if (!std::iswspace(*p1)) {
            break;
        }
        ++p1;
    }
    return WSpan (const_cast<Char*>(p1), e1 - p1);
}

/// \brief Удаление пробельных элементов из конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
CSpan trimEnd(CSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend() - 1;
    while (p1 < e1) {
        if (!std::isspace(*e1)) {
            break;
        }
        --e1;
    }
    return CSpan (const_cast<char*>(p1), e1 - p1 + 1);
}

/// \brief Удаление пробельных элементов из конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
WSpan trimEnd(WSpan text)
{
    auto p1 = text.cbegin(), e1 = text.cend() - 1;
    while (p1 < e1) {
        if (!std::iswspace(*e1)) {
            break;
        }
        --e1;
    }
    return WSpan (const_cast<Char*>(p1), e1 - p1 + 1);
}

/// \brief Удаление пробельных элементов из начала и конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные и конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
CSpan trim(CSpan text)
{
    return trimStart(trimEnd(text));
}

/// \brief Удаление пробельных элементов из начала и конца спана.
/// \param text Проверяемый спан.
/// \return Спан, из которого убраны начальные и конечные пробелы.
///
/// Данные остаются неизменными, создаётся слайс.
WSpan trim(WSpan text)
{
    return trimStart(trimEnd(text));
}

/// \brief Перевод текста в спане в верхний регистр "по месту".
/// \param text Текст, подлежащий преобразованию.
/// \return Тот же спан, но уже с преобразованным текстом.
CSpan toupper(CSpan text)
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
WSpan toupper(WSpan text)
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
CSpan tolower(CSpan text)
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
WSpan tolower(WSpan text)
{
    auto p = text.begin(), e = text.end();
    while (p < e) {
        *p = static_cast<char>(::towlower(*p));
        ++p;
    }
    return text;
}

}
