// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"
#include "irbis_version.h"

#include <iterator>
#include <sstream>

/*!

    \file
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
uint32_t libraryVersion() noexcept
{
    return (IRBIS_VERSION_MAJOR << 16) + IRBIS_VERSION_MINOR;
}

/// \brief Версия библиотеки-клиента (строкой).
/// \return "major.minor beta".
std::string libraryVersionString()
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
bool sameChar(Char first, Char second) noexcept
{
    return ::towupper(first) == ::towupper(second);
}

/// \brief Сравнение двух строк с точностью до регистра символов.
/// \param first Первая строка.
/// \param second Вторая строка.
/// \return Возвращает true, если строки равны с точностью до регистра,
/// иначе false.
bool sameString(const String &first, const String &second) noexcept
{
    if (first.size() != second.size()) {
        return false;
    }
    return std::equal(first.begin(), first.end(), second.begin(),
        [] (Char a, Char b) { return towupper(a) == towupper(b); });
}

/// \brief Преобразование строки к нижнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
String toLower (String &text) noexcept
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
std::string toLower (std::string &text) noexcept
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
String toUpper(String &text) noexcept
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
std::string toUpper(std::string &text) noexcept
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
bool contains(const String &text, const String &fragment)
{
    return text.find(fragment) != std::string::npos;
}

/// \brief Содержит ли строка заданный символ?
/// \param text Текст для изучения.
/// \param c Символ для поиска.
/// \return Возвращает true, если символ найден, иначе false.
bool contains(const String &text, Char c)
{
    return text.find(c) != std::string::npos;
}

/// \brief Заменяет все вхождения одной подстроки на другую.
/// \param text Текст для обработки.
/// \param from Заменяемый текст.
/// \param to Заменяющий текст.
/// \return Обработанный текст.
std::string replace(const std::string &text, const std::string &from, const std::string &to)
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
String replace(const String &text, const String &from, const String &to)
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
String PLUSIRBIS_EXPORTS trimStart(const String &text)
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
String PLUSIRBIS_EXPORTS trimEnd(const String &text)
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
String trim(const String &text)
{
    auto length = static_cast<std::ptrdiff_t>(text.length());
    std::ptrdiff_t start = 0, end = static_cast<std::ptrdiff_t>(length - 1);
    for (; start < length; ++start) {
        if (!::iswspace(text[start])) {
            break;
        }
    }

    for (; end >= 0; --end) {
        if (!::iswspace(text[end])) {
            break;
        }
    }

    if (end - start <= 0) {
        return String ();
    }

    if (end - start == length - 1) {
        return text;
    }

    return text.substr(start, end - start + 1);
}

/// \brief Выдаёт текстовое описание ошибки по её коду.
/// \param errorCode Код ошибки, полученный из Connection::lastError.
/// \return Текстовое описание проблемы.
String describeError(int errorCode)
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
int fastParse32(const String &text)
{
    auto result = 0;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const CharSpan text)
{
    auto result = 0;
    const std::size_t length = text.length;
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text[offset] - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const WideSpan text)
{
    auto result = 0;
    const std::size_t length = text.length;
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text[offset] - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
unsigned int fastParseUnsigned32(const String &text)
{
    auto result = 0u;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const Char *text)
{
    auto result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
unsigned int fastParseUnsigned32(const Char *text)
{
    auto result = 0u;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const Char *text, std::size_t length)
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
unsigned int fastParseUnsigned32(const Char *text, std::size_t length)
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
int fastParse32(const std::string &text)
{
    auto result = 0;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
unsigned int fastParseUnsigned32(const std::string &text)
{
    auto result = 0u;
    const std::size_t length = text.length();
    for (std::size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const char *text)
{
    auto result = 0;
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
int fastParse32(const char *text, std::size_t length)
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
unsigned int fastParseUnsigned32(const char *text, std::size_t length)
{
    auto result = 0u;
    while (length > 0) {
        result = result * 10 + *text - L'0';
        text++;
        length--;
    }

    return result;
}

///
/// \param s1
/// \param s2
/// \return
const std::string& iif(const std::string& s1, const std::string &s2)
{
    if (!s1.empty())
    {
        return s1;
    }

    return s2;
}

///
/// \param s1
/// \param s2
/// \return
const std::wstring& iif(const std::wstring& s1, const std::wstring &s2)
{
    if (!s1.empty())
    {
        return s1;
    }

    return s2;
}

///
/// \param s1
/// \param s2
/// \param s3
/// \return
const std::string& iif(const std::string& s1, const std::string &s2, const std::string &s3)
{
    if (!s1.empty())
    {
        return s1;
    }

    if (!s2.empty())
    {
        return s2;
    }

    return s3;
}

///
/// \param s1
/// \param s2
/// \param s3
/// \return
const std::wstring& iif(const std::wstring& s1, const std::wstring &s2, const std::wstring &s3)
{
    if (!s1.empty())
    {
        return s1;
    }

    if (!s2.empty())
    {
        return s2;
    }

    return s3;
}

///
/// \param list
/// \param index
/// \return
String safeAt (const StringList &list, std::size_t index)
{
    if (index >= list.size()) {
        return std::wstring();
    }
    return list.at(index);
}

///
/// \param list
/// \param index
/// \return
std::string safeAt (const std::vector<std::string> &list, std::size_t index)
{
    if (index >= list.size()) {
        return std::string();
    }
    return list.at(index);
}

///
/// \param text
/// \param separator
/// \param count
/// \return
StringList maxSplit (const String &text, Char separator, int count)
{
    StringList result;
    std::size_t position = 0;
    const std::size_t length = text.length();
    while ((count > 1) && (position < length)) {
        const auto index = text.find(separator, position);
        if (index != std::wstring::npos) {
            result.push_back(text.substr(position, index - position));
            position = index + 1;
        } else {
            result.push_back(text.substr(position));
            break;
        }
        count--;
    }
    if (position < length) {
        result.push_back(text.substr(position));
    }
    return result;
}

///
/// \param text
/// \param separator
/// \param count
/// \return
std::vector<std::string> maxSplit (const std::string &text, char separator, int count)
{
    std::vector<std::string> result;
    std::size_t position = 0;
    const std::size_t length = text.length();
    while ((count > 1) && (position < length)) {
        const auto index = text.find(separator, position);
        if (index != std::wstring::npos) {
            result.push_back(text.substr(position, index - position));
            position = index + 1;
        } else {
            result.push_back(text.substr(position));
            break;
        }
        count--;
    }
    if (position < length) {
        result.push_back(text.substr(position));
    }
    return result;
}

///
/// \param text
/// \param delimiter
/// \return
std::vector<std::string> split(const std::string &text, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream stream(text);
    while (std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

///
/// \param text
/// \param delimiter
/// \return
StringList split(const String &text, Char delimiter)
{
    std::vector<std::wstring> result;
    std::wstring token;
    std::wistringstream stream(text);
    while (std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

/// \brief Разбиение на строки по разделителю.
/// \param text Текст, подлежащий разбиению.
/// \param delimiter Разделитель.
/// \return Вектор строк. Включает, кроме прочего, пустые строки.
std::vector<std::string> split (const std::string &text, const std::string &delimiter)
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
StringList split (const String &text, const String &delimiter)
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
/// \param text
/// \return
String removeComments (const String &text)
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
        Char c = text.at(index);

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
                if ((index + 1u) < length && text.at(index + 1u) == '*') {
                    while (index < length) {
                        c = text.at(index);
                        if (c == '\r' || c == '\n') {
                            result.push_back(c);
                            break;
                        }

                        index++;
                    }
                }
                else {
                    result.push_back(c);
                }
            }
            else if (c == '\'' || c == '"' || c == '|') {
                state = c;
                result.push_back(c);
            }
            else {
                result.push_back(c);
            }

            break;
        }

        index++;
    }

    return result;
}

/// \brief Подготовка формата к отсылке на сервер.
/// \param text
/// \return
std::wstring prepareFormat (const std::wstring &text)
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
    for (std::size_t i = 0; i < length; i++) {
        const Char c = text.at(i);
        if (c >= ' ') {
            result.push_back(c);
        }
    }

    return result;
}

bool isWindows()
{
#ifdef IRBIS_WINDOWS
    return true;
#else
    return false;
#endif
}

}

