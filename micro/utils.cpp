// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"
#include "micro.h"

#include <iterator>
#include <sstream>

/*!

    \file Utils.cpp
    \brief Утилиты, используемые в PlusIrbis.

    \mainpage Универсальный клиент для системы ИРБИС64.

    Универсальный программный клиент для автоматизированной
    библиотечно-информационной системы ИРБИС64 на языке С++.
    Соответствует стандарту C++ 11.

    Не требует библиотек, кроме стандартной.
    Не полагается на `irbis64_client.dll`.

    Все функции и классы располагаются в пространстве имен `irbis::`.

*/

namespace irbis
{

    /// \brief Сравнение двух символов с точностью до регистра.
    /// \param first Первый символ.
    /// \param second Второй символ.
    /// \return Возвращает true, если символы равны с точностью до регистра,
    /// иначе false.
    bool sameChar (char first, char second) noexcept
    {
        return std::toupper (first) == std::toupper (second);
    }

    /// \brief Сравнение двух строк с точностью до регистра символов.
    /// \param first Первая строка.
    /// \param second Вторая строка.
    /// \return Возвращает true, если строки равны с точностью до регистра,
    /// иначе false.
    bool sameString (const std::string &first, const std::string &second) noexcept
    {
        if (first.size() != second.size()) {
            return false;
        }
        return std::equal (first.begin(), first.end(), second.begin(),
                       [] (char a, char b) { return std::toupper (a) == std::toupper (b); });
    }

    /// \brief Первый символ строки.
    /// \param text Текст.
    /// \return Первый символ либо 0.
    char firstChar (const std::string &text) noexcept
    {
        return text.empty() ? '\0' : text[0];
    }

    /// \brief Преобразование строки к нижнему регистру.
    /// \param text Текст, подлежащий трансформации.
    /// \return Возвращает преобразованную строку.
    /// \warning Трансформация осуществляется "по месту"!
    std::string toLower (std::string &text) noexcept
    {
        for (auto &i : text) {
            i = (char) std::tolower(i);
        }
        return text;
    }

    /// \brief Преобразование строки к верхнему регистру.
    /// \param text Текст, подлежащий трансформации.
    /// \return Возвращает преобразованную строку.
    /// \warning Трансформация осуществляется "по месту"!
    std::string toUpper (std::string &text) noexcept
    {
        for (auto &i : text) {
            i = (char) std::toupper(i);
        }
        return text;
    }

    /// \brief Содержит ли строка заданную подстроку?
    /// \param text Текст для изучения.
    /// \param fragment Подстрока для поиска.
    /// \return Возвращает true, если подстрока найдена, иначе false.
    bool contains (const std::string &text, const std::string &fragment)
    {
        return text.find (fragment) != std::string::npos;
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

    /// \brief Удаление пробельных символов в начале строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trimStart (const std::string &text)
    {
        auto length = text.length();
        std::size_t i = 0;
        for (; i < length; ++i) {
            if (!std::isspace(text[i])) {
                break;
            }
        }

        if (i == 0) {
            return text;
        }

        if (i == length) {
            return {};
        }

        return text.substr(i);
    }

    /// \brief Удаление пробельных символов в конце строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trimEnd (const std::string &text)
    {
        auto length = text.length();
        auto i = static_cast<std::ptrdiff_t >(length - 1);
        for (; i >= 0; --i) {
            if (!std::isspace(text[i])) {
                break;
            }
        }

        if (i == static_cast<std::ptrdiff_t>(length - 1)) {
            return text;
        }

        if (i < 0) {
            return {};
        }

        return text.substr(0, i + 1);
    }

    /// \brief Удаление пробельных символов в начале и в конце строки.
    /// \param text Текст для обработки.
    /// \return Обработанный текст.
    std::string trim (const std::string &text)
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
            return {};
        }

        if (end - start == length - 1) {
            return text;
        }

        return text.substr (start, end - start + 1);
    }


    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    int fastParse32 (const std::string &text) noexcept
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
    unsigned int fastParseUnsigned32 (const std::string &text) noexcept
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
    int fastParse32 (const char *text) noexcept
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
    unsigned int fastParseUnsigned32 (const char *text) noexcept
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
    int fastParse32 (const char *text, std::size_t length) noexcept
    {
        auto result = 0;
        while (length > 0) {
            result = result * 10 + *text - '0';
            text++;
            length--;
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор строки как целого числа без знака.
    /// \param text Текст для разбора.
    /// \return Мусор на входе - мусор на выходе!
    unsigned int fastParseUnsigned32 (const char *text, std::size_t length) noexcept
    {
        auto result = 0u;
        while (length > 0) {
            result = result * 10 + *text - '0';
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
    StringList maxSplit (const std::string &text, char separator, int count)
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
    /// \param delimiter Разделитель.
    /// \return Вектор подстрок.
    std::vector<std::string> split (const std::string &text, char delimiter)
    {
        std::vector<std::string> result;
        std::string token;
        std::istringstream stream (text);
        while (std::getline (stream, token, delimiter)) {
            result.push_back (token);
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
}
