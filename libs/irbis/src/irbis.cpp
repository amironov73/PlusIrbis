// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"
#include "irbis_version.h"

#include <iterator>
#include <sstream>

/*!

    \file irbis.cpp
    \brief Утилиты, используемые в PlusIrbis.

    \mainpage Универсальный клиент для системы ИРБИС64.

    Универсальный программный клиент для автоматизированной
    библиотечно-информационной системы ИРБИС64 на языке С++.
    Соответствует стандарту C++ 11.

    Не требует библиотек, кроме стандартной.
    Не полагается на `irbis64_client.dll`.

    Все функции и классы располагаются в пространстве имен `irbis::`.

*/

namespace irbis {

/// \brief Версия библиотеки-клиента (числом).
/// \return (major << 16) + minor.
uint32_t IRBIS_CALL libraryVersion() noexcept
{
    return (IRBIS_VERSION_MAJOR << 16u) + IRBIS_VERSION_MINOR;
}

/// \brief Версия библиотеки-клиента (строкой).
/// \return "major.minor beta".
std::string IRBIS_CALL libraryVersionString()
{
    return std::to_string(IRBIS_VERSION_MAJOR)
        + "." + std::to_string(IRBIS_VERSION_MINOR)
        + " " + IRBIS_VERSION_KIND;
}

/// \brief Сравнение двух символов с точностью до регистра.
/// \param first Первый символ.
/// \param second Второй символ.
/// \return Возвращает true, если символы равны с точностью до регистра,
/// иначе false.
bool IRBIS_CALL sameChar (Char first, Char second) noexcept
{
    return ::towupper (first) == ::towupper (second);
}

/// \brief Сравнение двух строк с точностью до регистра символов.
/// \param first Первая строка.
/// \param second Вторая строка.
/// \return Возвращает true, если строки равны с точностью до регистра,
/// иначе false.
bool IRBIS_CALL sameString (const String &first, const String &second) noexcept
{
    if (first.size() != second.size()) {
        return false;
    }
    return std::equal (first.begin(), first.end(), second.begin(),
        [] (Char a, Char b) { return towupper (a) == towupper (b); });
}

/// \param first Первая строка.
/// \param second Вторая строка.
/// \return Возвращает true, если строки равны с точностью до регистра,
/// иначе false.
bool IRBIS_CALL sameString (const std::string &first, const std::string &second) noexcept
{
    if (first.size() != second.size()) {
        return false;
    }
    return std::equal (first.begin(), first.end(), second.begin(),
        [] (Char a, Char b) { return towupper (a) == towupper (b); });
}

/// \brief Первый символ строки.
/// \param text Текст.
/// \return Первый символ либо 0.
Char IRBIS_CALL firstChar (const String &text) noexcept
{
    return text.empty() ? L'\0' : text[0];
}

/// \brief Первый символ строки.
/// \param text Текст.
/// \return Первый символ либо 0.
char IRBIS_CALL firstChar (const std::string &text) noexcept
{
    return text.empty() ? '\0' : text[0];
}

/// \brief Преобразование строки к нижнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
String IRBIS_CALL toLower (String &text) noexcept
{
    for (auto &i : text) {
        i = ::tolower(i);
    }
    return text;
}

/// \brief Преобразование строки к нижнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
std::string IRBIS_CALL toLower (std::string &text) noexcept
{
    for (auto &i : text) {
        i = ::tolower(i);
    }
    return text;
}

/// \brief Преобразование строки к верхнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
String IRBIS_CALL toUpper (String &text) noexcept
{
    for (auto &i : text) {
        i = ::toupper(i);
    }
    return text;
}

/// \brief Преобразование строки к верхнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
std::string IRBIS_CALL toUpper (std::string &text) noexcept
{
    for (auto &i : text) {
        i = ::toupper(i);
    }
    return text;
}

/// \brief Содержит ли строка заданную подстроку?
/// \param text Текст для изучения.
/// \param fragment Подстрока для поиска.
/// \return Возвращает true, если подстрока найдена, иначе false.
bool contains (const String &text, const String &fragment)
{
    return text.find (fragment) != std::string::npos;
}

/// \brief Содержит ли строка заданный символ?
/// \param text Текст для изучения.
/// \param c Символ для поиска.
/// \return Возвращает true, если символ найден, иначе false.
bool contains (const String &text, Char c)
{
    return text.find (c) != std::string::npos;
}

/// \brief Содержит ли строка заданный символ?
/// \param text Текст для изучения.
/// \param c Символ для поиска.
/// \return Возвращает true, если символ найден, иначе false.
bool contains (const std::string &text, char c)
{
    return text.find (c) != std::string::npos;
}

/// \brief Заменяет все вхождения одной подстроки на другую.
/// \param text Текст для обработки.
/// \param from Заменяемый текст.
/// \param to Заменяющий текст.
/// \return Обработанный текст.
std::string replace (const std::string &text, const std::string &from, const std::string &to)
{
    std::string result = text;
    std::size_t index = 0;
    while (true) {
        index = result.find (from, index);
        if (index == std::string::npos) {
            break;
        }
        result.replace(index, from.length(), to);
        index += to.length();
    }
    return result;
}

/// \brief Заменяет все вхождения одной подстроки на другую.
/// \param text Текст для обработки.
/// \param from Заменяемый текст.
/// \param to Заменяющий текст.
/// \return Обработанный текст.
String replace (const String &text, const String &from, const String &to)
{
    String result = text;
    std::size_t index = 0;
    while (true) {
        index = result.find (from, index);
        if (index == std::string::npos) {
            break;
        }
        result.replace (index, from.length(), to);
        index += to.length();
    }
    return result;
}

/// \brief Удаление пробельных символов в начале строки.
/// \param text Текст для обработки.
/// \return Обработанный текст.
String trimStart (const String &text)
{
    auto length = text.length();
    std::size_t i = 0;
    for (; i < length; ++i) {
        if (!::iswspace(text[i])) {
            break;
        }
    }

    if (i == 0) {
        return text;
    }

    if (i == length) {
        return String();
    }

    return text.substr(i);
}

/// \brief Удаление пробельных символов в конце строки.
/// \param text Текст для обработки.
/// \return Обработанный текст.
String trimEnd (const String &text)
{
    auto length = text.length();
    auto i = static_cast<std::ptrdiff_t >(length - 1);
    for (; i >= 0; --i) {
        if (!::iswspace(text[i])) {
            break;
        }
    }

    if (i == static_cast<std::ptrdiff_t>(length - 1)) {
        return text;
    }

    if (i < 0) {
        return String();
    }

    return text.substr(0, i + 1);
}

/// \brief Удаление пробельных символов в начале и в конце строки.
/// \param text Текст для обработки.
/// \return Обработанный текст.
String trim (const String &text)
{
    auto length = static_cast<std::ptrdiff_t> (text.length());
    std::ptrdiff_t start = 0, end = static_cast<std::ptrdiff_t> (length - 1);
    for (; start < length; ++start) {
        if (!::iswspace(text[start])) {
            break;
        }
    }

    for (; end >= 0; --end) {
        if (!::iswspace (text[end])) {
            break;
        }
    }

    if (end - start < 0) {
        return String ();
    }

    if (end - start == length - 1) {
        return text;
    }

    return text.substr (start, end - start + 1);
}

/// \brief Выдаёт текстовое описание ошибки по её коду.
/// \param errorCode Код ошибки, полученный из Connection::lastError.
/// \return Текстовое описание проблемы.
String IRBIS_CALL describeError (int errorCode)
{
    if (errorCode >= 0) {
        return L"\u041D\u0435\u0442 \u043E\u0448\u0438\u0431\u043A\u0438";
    }

    switch (errorCode) {
        case    -100: return L"\u0417\u0430\u0434\u0430\u043D\u043D\u044B\u0439 MFN \u0432\u043D\u0435 \u043F\u0440\u0435\u0434\u0435\u043B\u043E\u0432 \u0411\u0414";
        case    -101: return L"\u041E\u0448\u0438\u0431\u043E\u0447\u043D\u044B\u0439 \u0440\u0430\u0437\u043C\u0435\u0440 \u043F\u043E\u043B\u043A\u0438";
        case    -102: return L"\u041E\u0448\u0438\u0431\u043E\u0447\u043D\u044B\u0439 \u043D\u043E\u043C\u0435\u0440 \u043F\u043E\u043B\u043A\u0438";
        case    -140: return L"MFN \u0432\u043D\u0435 \u043F\u0440\u0435\u0434\u0435\u043B\u043E\u0432 \u0411\u0414";
        case    -141: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0447\u0442\u0435\u043D\u0438\u044F";
        case    -200: return L"\u0423\u043A\u0430\u0437\u0430\u043D\u043D\u043E\u0435 \u043F\u043E\u043B\u0435 \u043E\u0442\u0441\u0443\u0442\u0441\u0442\u0432\u0443\u0435\u0442";
        case    -201: return L"\u041F\u0440\u0435\u0434\u044B\u0434\u0443\u0449\u0430\u044F \u0432\u0435\u0440\u0441\u0438\u044F \u0437\u0430\u043F\u0438\u0441\u0438 \u043E\u0442\u0441\u0443\u0442\u0441\u0442\u0432\u0443\u0435\u0442";
        case    -202: return L"\u0417\u0430\u0434\u0430\u043D\u043D\u044B\u0439 \u0442\u0435\u0440\u043C\u0438\u043D \u043D\u0435 \u043D\u0430\u0439\u0434\u0435\u043D (\u0442\u0435\u0440\u043C\u0438\u043D \u043D\u0435 \u0441\u0443\u0449\u0435\u0441\u0442\u0432\u0443\u0435\u0442)";
        case    -203: return L"\u041F\u043E\u0441\u043B\u0435\u0434\u043D\u0438\u0439 \u0442\u0435\u0440\u043C\u0438\u043D \u0432 \u0441\u043F\u0438\u0441\u043A\u0435";
        case    -204: return L"\u041F\u0435\u0440\u0432\u044B\u0439 \u0442\u0435\u0440\u043C\u0438\u043D \u0432 \u0441\u043F\u0438\u0441\u043A\u0435";
        case    -300: // fall through
        case    -301: return L"\u0411\u0430\u0437\u0430 \u0434\u0430\u043D\u043D\u044B\u0445 \u043C\u043E\u043D\u043E\u043F\u043E\u043B\u044C\u043D\u043E \u0437\u0430\u0431\u043B\u043E\u043A\u0438\u0440\u043E\u0432\u0430\u043D\u0430";
        case    -400: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u043F\u0440\u0438 \u043E\u0442\u043A\u0440\u044B\u0442\u0438\u0438 \u0444\u0430\u0439\u043B\u043E\u0432 MST \u0438\u043B\u0438 XRF (\u043E\u0448\u0438\u0431\u043A\u0430 \u0444\u0430\u0439\u043B\u0430 \u0434\u0430\u043D\u043D\u044B\u0445)";
        case    -401: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u043F\u0440\u0438 \u043E\u0442\u043A\u0440\u044B\u0442\u0438\u0438 \u0444\u0430\u0439\u043B\u043E\u0432 IFP (\u043E\u0448\u0438\u0431\u043A\u0430 \u0444\u0430\u0439\u043B\u0430 \u0438\u043D\u0434\u0435\u043A\u0441\u0430)";
        case    -402: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u043F\u0440\u0438 \u0437\u0430\u043F\u0438\u0441\u0438";
        case    -403: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u043F\u0440\u0438 \u0430\u043A\u0442\u0443\u0430\u043B\u0438\u0437\u0430\u0446\u0438\u0438";
        case    -600: return L"\u0417\u0430\u043F\u0438\u0441\u044C \u043B\u043E\u0433\u0438\u0447\u0435\u0441\u043A\u0438 \u0443\u0434\u0430\u043B\u0435\u043D\u0430";
        case    -601: return L"\u0417\u0430\u043F\u0438\u0441\u044C \u0444\u0438\u0437\u0438\u0447\u0435\u0441\u043A\u0438 \u0443\u0434\u0430\u043B\u0435\u043D\u0430";
        case    -602: return L"\u0417\u0430\u043F\u0438\u0441\u044C \u0437\u0430\u0431\u043B\u043E\u043A\u0438\u0440\u043E\u0432\u0430\u043D\u0430 \u043D\u0430 \u0432\u0432\u043E\u0434";
        case    -603: return L"\u0417\u0430\u043F\u0438\u0441\u044C \u043B\u043E\u0433\u0438\u0447\u0435\u0441\u043A\u0438 \u0443\u0434\u0430\u043B\u0435\u043D\u0430";
        case    -605: return L"\u0417\u0430\u043F\u0438\u0441\u044C \u0444\u0438\u0437\u0438\u0447\u0435\u0441\u043A\u0438 \u0443\u0434\u0430\u043B\u0435\u043D\u0430";
        case    -607: return L"\u041E\u0448\u0438\u0431\u043A\u0430 autoin.gbl";
        case    -608: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0432\u0435\u0440\u0441\u0438\u0438 \u0437\u0430\u043F\u0438\u0441\u0438";
        case    -700: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0441\u043E\u0437\u0434\u0430\u043D\u0438\u044F \u0440\u0435\u0437\u0435\u0440\u0432\u043D\u043E\u0439 \u043A\u043E\u043F\u0438\u0438";
        case    -701: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0432\u043E\u0441\u0441\u0442\u0430\u043D\u043E\u0432\u043B\u0435\u043D\u0438\u044F \u0438\u0437 \u0440\u0435\u0437\u0435\u0440\u0432\u043D\u043E\u0439 \u043A\u043E\u043F\u0438\u0438";
        case    -702: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0441\u043E\u0440\u0442\u0438\u0440\u043E\u0432\u043A\u0438";
        case    -703: return L"\u041E\u0448\u0438\u0431\u043E\u0447\u043D\u044B\u0439 \u0442\u0435\u0440\u043C\u0438\u043D";
        case    -704: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0441\u043E\u0437\u0434\u0430\u043D\u0438\u044F \u0441\u043B\u043E\u0432\u0430\u0440\u044F";
        case    -705: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0437\u0430\u0433\u0440\u0443\u0437\u043A\u0438 \u0441\u043B\u043E\u0432\u0430\u0440\u044F";
        case    -800: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0432 \u043F\u0430\u0440\u0430\u043C\u0435\u0442\u0440\u0430\u0445 \u0433\u043B\u043E\u0431\u0430\u043B\u044C\u043D\u043E\u0439 \u043A\u043E\u0440\u0440\u0435\u043A\u0442\u0438\u0440\u043E\u0432\u043A\u0438";
        case    -801: return L"ERR_GBL_REP";
        case    -802: return L"ERR_GBL_MET";
        case   -1111: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0438\u0441\u043F\u043E\u043B\u043D\u0435\u043D\u0438\u044F \u0441\u0435\u0440\u0432\u0435\u0440\u0430 (SERVER_EXECUTE_ERROR)";
        case   -2222: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0432 \u043F\u0440\u043E\u0442\u043E\u043A\u043E\u043B\u0435 (WRONG_PROTOCOL)";
        case   -3333: return L"\u041D\u0435\u0437\u0430\u0440\u0435\u0433\u0438\u0441\u0442\u0440\u0438\u0440\u043E\u0432\u0430\u043D\u043D\u044B\u0439 \u043A\u043B\u0438\u0435\u043D\u0442 (\u043E\u0448\u0438\u0431\u043A\u0430 \u0432\u0445\u043E\u0434\u0430 \u043D\u0430 \u0441\u0435\u0440\u0432\u0435\u0440) (\u043A\u043B\u0438\u0435\u043D\u0442 \u043D\u0435 \u0432 \u0441\u043F\u0438\u0441\u043A\u0435)";
        case   -3334: return L"\u041A\u043B\u0438\u0435\u043D\u0442 \u043D\u0435 \u0432\u044B\u043F\u043E\u043B\u043D\u0438\u043B \u0432\u0445\u043E\u0434 \u043D\u0430 \u0441\u0435\u0440\u0432\u0435\u0440 (\u043A\u043B\u0438\u0435\u043D\u0442 \u043D\u0435 \u0438\u0441\u043F\u043E\u043B\u044C\u0437\u0443\u0435\u0442\u0441\u044F)";
        case   -3335: return L"\u041D\u0435\u043F\u0440\u0430\u0432\u0438\u043B\u044C\u043D\u044B\u0439 \u0443\u043D\u0438\u043A\u0430\u043B\u044C\u043D\u044B\u0439 \u0438\u0434\u0435\u043D\u0442\u0438\u0444\u0438\u043A\u0430\u0442\u043E\u0440 \u043A\u043B\u0438\u0435\u043D\u0442\u0430";
        case   -3336: return L"\u041D\u0435\u0442 \u0434\u043E\u0441\u0442\u0443\u043F\u0430 \u043A \u043A\u043E\u043C\u0430\u043D\u0434\u0430\u043C \u0410\u0420\u041C";
        case   -3337: return L"\u041A\u043B\u0438\u0435\u043D\u0442 \u0443\u0436\u0435 \u0437\u0430\u0440\u0435\u0433\u0438\u0441\u0442\u0440\u0438\u0440\u043E\u0432\u0430\u043D";
        case   -3338: return L"\u041D\u0435\u0434\u043E\u043F\u0443\u0441\u0442\u0438\u043C\u044B\u0439 \u043A\u043B\u0438\u0435\u043D\u0442";
        case   -4444: return L"\u041D\u0435\u0432\u0435\u0440\u043D\u044B\u0439 \u043F\u0430\u0440\u043E\u043B\u044C";
        case   -5555: return L"\u0424\u0430\u0439\u043B \u043D\u0435 \u0441\u0443\u0449\u0435\u0441\u0442\u0432\u0443\u0435\u0442";
        case   -6666: return L"\u0421\u0435\u0440\u0432\u0435\u0440 \u043F\u0435\u0440\u0435\u0433\u0440\u0443\u0436\u0435\u043D. \u0414\u043E\u0441\u0442\u0438\u0433\u043D\u0443\u0442\u043E \u043C\u0430\u043A\u0441\u0438\u043C\u0430\u043B\u044C\u043D\u043E\u0435 \u0447\u0438\u0441\u043B\u043E \u043F\u043E\u0442\u043E\u043A\u043E\u0432 \u043E\u0431\u0440\u0430\u0431\u043E\u0442\u043A\u0438";
        case   -7777: return L"\u041D\u0435 \u0443\u0434\u0430\u043B\u043E\u0441\u044C \u0437\u0430\u043F\u0443\u0441\u0442\u0438\u0442\u044C/\u043F\u0440\u0435\u0440\u0432\u0430\u0442\u044C \u043F\u043E\u0442\u043E\u043A \u0430\u0434\u043C\u0438\u043D\u0438\u0441\u0442\u0440\u0430\u0442\u043E\u0440\u0430 (\u043E\u0448\u0438\u0431\u043A\u0430 \u043F\u0440\u043E\u0446\u0435\u0441\u0441\u0430)";
        case   -8888: return L"\u041E\u0431\u0449\u0430\u044F \u043E\u0448\u0438\u0431\u043A\u0430";
        case -100001: return L"\u041E\u0448\u0438\u0431\u043A\u0430 \u0441\u043E\u0437\u0434\u0430\u043D\u0438\u044F \u0441\u043E\u043A\u0435\u0442\u0430";
        case -100002: return L"\u0421\u0431\u043E\u0439 \u0441\u0435\u0442\u0438";
        case -100003: return L"\u041D\u0435\u0442 \u043F\u043E\u0434\u043A\u043B\u044E\u0447\u0435\u043D\u0438\u044F \u043A \u0441\u0435\u0440\u0432\u0435\u0440\u0443";
        default: return L"\u041D\u0435\u0438\u0437\u0432\u0435\u0441\u0442\u043D\u0430\u044F \u043E\u0448\u0438\u0431\u043A\u0430";
    }
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const String &text) noexcept
{
    auto result = 0;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int64_t IRBIS_CALL fastParse64 (const String &text) noexcept
{
    int64_t result = 0;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const CharSpan text) noexcept
{
    auto result = 0;
    const std::size_t length = text.length;
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text[offset] - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const WideSpan text) noexcept
{
    auto result = 0;
    const std::size_t length = text.length;
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text[offset] - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
unsigned int IRBIS_CALL fastParseUnsigned32 (const String &text) noexcept
{
    auto result = 0u;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const Char *text) noexcept
{
    auto result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int64_t IRBIS_CALL fastParse64 (const Char *text) noexcept
{
    int64_t result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
unsigned int IRBIS_CALL fastParseUnsigned32 (const Char *text) noexcept
{
    auto result = 0u;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const Char *text, std::size_t length) noexcept
{
    auto result = 0;
    while (length > 0) {
        result = result * 10 + *text - L'0';
        text++;
        length--;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
unsigned int IRBIS_CALL fastParseUnsigned32 (const Char *text, std::size_t length) noexcept
{
    auto result = 0u;
    while (length > 0) {
        result = result * 10 + *text - L'0';
        text++;
        length--;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const std::string &text) noexcept
{
    auto result = 0;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int64_t IRBIS_CALL fastParse64 (const std::string &text) noexcept
{
    int64_t result = 0;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
unsigned int IRBIS_CALL fastParseUnsigned32 (const std::string &text) noexcept
{
    auto result = 0u;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int IRBIS_CALL fastParse32 (const char *text) noexcept
{
    auto result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст для разбора.
/// \return Мусор на входе - мусор на выходе!
int64_t IRBIS_CALL fastParse64 (const char *text) noexcept
{
    int64_t result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст в однобайтовой кодировке.
/// \param length Длина текста в байтах.
/// \return Результат разбора.
/// \warning Мусор на входе -- мусор на выходе!
int IRBIS_CALL fastParse32 (const char *text, std::size_t length) noexcept
{
    auto result = 0;
    while (length > 0) {
        result = result * 10 + *text - L'0';
        text++;
        length--;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
/// \param text Текст в однобайтовой кодировке.
/// \param length Длина текста в байтах.
/// \return Результат разбора.
/// \warning Мусор на входе -- мусор на выходе!
unsigned int IRBIS_CALL fastParseUnsigned32 (const char *text, std::size_t length) noexcept
{
    auto result = 0u;
    while (length > 0) {
        result = result * 10 + *text - L'0';
        text++;
        length--;
    }

    return result;
}

/// \brief Разбор строки по указанному разделителю.
/// \param text Текст для разбора.
/// \param separator Разделитель.
/// \param count Не больше указанного количества подстрок.
/// \return Вектор подстрок.
StringList IRBIS_CALL maxSplit (const String &text, Char separator, int count)
{
    StringList result;
    std::size_t position = 0;
    const std::size_t length = text.length();
    while ((count > 1) && (position < length)) {
        const auto index = text.find (separator, position);
        if (index != std::wstring::npos) {
            result.push_back(text.substr (position, index - position));
            position = index + 1;
        } else {
            result.push_back (text.substr (position));
            position = length + 1;
            break;
        }
        count--;
    }
    if (position < length) {
        result.push_back (text.substr(position));
    }
    return result;
}

/// \brief Разбор строки по указанному разделителю.
/// \param text Текст для разбора.
/// \param separator Разделитель.
/// \param count Не больше указанного количества подстрок.
/// \return Вектор подстрок.
std::vector<std::string> IRBIS_CALL maxSplit (const std::string &text, char separator, int count)
{
    std::vector<std::string> result;
    std::size_t position = 0;
    const std::size_t length = text.length();
    while ((count > 1) && (position < length)) {
        const auto index = text.find (separator, position);
        if (index != std::wstring::npos) {
            result.push_back (text.substr (position, index - position));
            position = index + 1;
        } else {
            result.push_back (text.substr (position));
            break;
        }
        count--;
    }
    if (position < length) {
        result.push_back (text.substr(position));
    }
    return result;
}

/// \brief Разбор строки по указанному разделителю.
/// \param text Текст для разбора.
/// \param delimiter Разделитель.
/// \return Вектор подстрок.
std::vector<std::string> IRBIS_CALL split (const std::string &text, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream stream (text);
    while (std::getline (stream, token, delimiter)) {
        result.push_back (token);
    }

    return result;
}

/// \brief Разбор строки по указанному разделителю.
/// \param text Текст для разбора.
/// \param delimiter Разделитель.
/// \return Вектор подстрок.
StringList IRBIS_CALL split (const String &text, Char delimiter)
{
    std::vector<std::wstring> result;
    std::wstring token;
    std::wistringstream stream (text);
    while (std::getline (stream, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

/// \brief Разбиение на строки по разделителю.
/// \param text Текст, подлежащий разбиению.
/// \param delimiter Разделитель.
/// \return Вектор строк. Включает, кроме прочего, пустые строки.
std::vector<std::string> IRBIS_CALL split (const std::string &text, const std::string &delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::size_t start = 0, end;
    while ((end = text.find (delimiter, start)) != std::string::npos) {
        token = text.substr (start, end - start);
        result.push_back (token);
        start = end + delimiter.length();
    }

    if (start < text.length()) {
        token = text.substr (start, std::string::npos);
        result.push_back(token);
    }

    return result;
}

/// \brief Разбиение строки по разделителям.
/// \param text Текст, подлежащий разбиению.
/// \param delimiter Разделитель.
/// \return Вектор строк. Включает, кроме прочего, пустые строки.
StringList IRBIS_CALL split (const String &text, const String &delimiter)
{
    StringList result;
    String token;
    std::size_t start = 0, end;
    while ((end = text.find (delimiter, start)) != std::string::npos) {
        token = text.substr(start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
    }

    if (start < text.length()) {
        token = text.substr (start, std::string::npos);
        result.push_back (token);
    }

    return result;
}

//=========================================================

/// \brief Удаление из формата комментариев.
/// Комментарии заменяются пустой строкой.
/// \param text Текст для обработки.
/// \return Обработанный текст.
String IRBIS_CALL removeComments (const String &text)
{
    if (text.empty()) {
        return text;
    }

    if (!contains (text, L"/*")) {
        return text;
    }

    String result;
    Char state = 0;
    std::size_t index = 0;
    const std::size_t length = text.length();
    while (index < length) {
        Char c = text [index];

        switch(state) {
        case '\'':
        case '"':
        case '|':
            if (c == state) {
                state = 0;
            }
            result.push_back(c);
            break;

        default:
            if (c == '/') {
                if ((index + 1u) < length && text [index + 1u] == '*') {
                    while (index < length) {
                        c = text [index];
                        if (c == '\r' || c == '\n') {
                            result.push_back (c);
                            break;
                        }

                        index++;
                    }
                }
                else {
                    result.push_back (c);
                }
            }
            else if (c == '\'' || c == '"' || c == '|') {
                state = c;
                result.push_back (c);
            }
            else {
                result.push_back (c);
            }

            break;
        }

        index++;
    }

    return result;
}

/// \brief Подготовка формата к отсылке на сервер.
/// Удаляются служебные символы, на которые сервер реагирует очень нервно.
/// Также удаляются комментарии.
/// \param text Текст формата.
/// \return Обработанный текст.
String IRBIS_CALL prepareFormat (const String &text)
{
    String text2 = removeComments (text);
    const std::size_t length = text2.length();
    if (length == 0) {
        return text2;
    }

    bool flag = false;
    for (std::size_t i = 0; i  < length; i++) {
        if (text2.at(i) < ' ') {
            flag = true;
            break;
        }
    }

    if (!flag) {
        return text2;
    }

    String result;
    result.reserve (text2.length());
    for (std::size_t i = 0; i < length; i++) {
        const Char c = text [i];
        if (c >= ' ') {
            result.push_back (c);
        }
    }

    return result;
}

// таблица для прямого преобразования текста CP1251 &lt;-&gt; UTF-8
static std::uint16_t _cp1251_unicode[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
    67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
    83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
    99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
    125, 126, 127, 1026, 1027, 8218, 1107, 8222, 8230, 8224, 8225,
    8364, 8240, 1033, 8249, 1034, 1036, 1035, 1039, 1106, 8216, 8217,
    8220, 8221, 8226, 8211, 8212, 152, 8482, 1113, 8250, 1114, 1116,
    1115, 1119, 160, 1038, 1118, 1032, 164, 1168, 166, 167, 1025, 169,
    1028, 171, 172, 173, 174, 1031, 176, 177, 1030, 1110, 1169, 181,
    182, 183, 1105, 8470, 1108, 187, 1112, 1029, 1109, 1111, 1040,
    1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048, 1049, 1050, 1051,
    1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062,
    1063, 1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073,
    1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084,
    1085, 1086, 1087, 1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095,
    1096, 1097, 1098, 1099, 1100, 1101, 1102, 1103
};

std::string IRBIS_CALL ansiToUtf (const std::string &text)
{
    std::string result;

    for (const unsigned char chr : text) {
        const std::uint16_t wide = _cp1251_unicode[chr];
        if (wide < 128)
        {
            result.push_back (static_cast<char> (wide));
        }
        else
        {
            result.push_back (static_cast<char> ((wide >> 6) | 0xC0));
            result.push_back (static_cast<char> ((wide & 0x3Fu) | 0x80u));
        }
    }

    return result;
}

std::string IRBIS_CALL utfToAnsi (const std::string &text)
{
    const std::size_t length = text.length();
    std::string result;

    std::size_t offset = 0;
    while (offset < length) {
        const unsigned char first = text[offset++];
        if ((first & 0x80) == 0)
        {
            result.push_back (static_cast<char> (first));
        }
        else {
            const unsigned char second = text[offset++];
            if ((first & 0xF0) == 0xE0) {
                result.push_back ('?');
                offset++;
                continue;
            }

            if ((first & 0xF8) == 0xF0) {
                result.push_back ('?');
                offset++;
                offset++;
                continue;
            }

            if ((first & 0xF8) == 0xF8) {
                result.push_back ('?');
                offset++;
                offset++;
                offset++;
                continue;
            }

            const auto unicode = static_cast<std::uint16_t> (((first & 0x1Fu) << 6) + (second & 0x3Fu));
            char chr           = '?';
            for (auto i = 0; i < 256; i++) {
                if (_cp1251_unicode[i] == unicode) {
                    chr = static_cast<char> (i);
                    break;
                }
            }

            result.push_back (chr);
        }
    }

    return result;
}

/// \brief Программа выполняется на Windows или на Unix-подобной системе?
/// \return true если Windows.
bool IRBIS_CALL isWindows() noexcept
{
#ifdef IRBIS_WINDOWS
    return true;
#else
    return false;
#endif
}

}

