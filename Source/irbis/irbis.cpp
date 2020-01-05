// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iterator>
#include <sstream>

///
/// \file irbis.cpp
/// \brief Утилиты, используемые в PlusIrbis.
///

///
/// \mainpage Универсальный клиент для системы ИРБИС64.
///
/// Универсальный программный клиент для автоматизированной
/// библиотечно-информационной системы ИРБИС64 на языке С++.
///

namespace irbis {

/// \brief Сравнение двух символов с точностью до регистра.
/// \param first Первый символ.
/// \param second Второй символ.
/// \return Возвращает true, если символы равны с точностью до регистра,
/// иначе false.
bool sameChar(Char first, Char second)
{
    return towupper(first) == towupper(second);
}

/// \brief Сравнение двух строк с точностью до регистра символов.
/// \param first Первая строка.
/// \param second Вторая строка.
/// \return Возвращает true, если строки равны с точностью до регистра,
/// иначе false.
bool sameString(const String &first, const String &second)
{
    return std::equal(first.begin(), first.end(), second.begin(), second.end(),
        [] (Char a, Char b) { return towupper(a) == towupper(b); });
}

/// \brief Преобразование строки к нижнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
String toLower(String &text)
{
    for (auto &i : text) {
        i = tolower(i);
    }

    return text;
}

/// \brief Преобразование строки к верхнему регистру.
/// \param text Текст, подлежащий трансформации.
/// \return Возвращает преобразованную строку.
/// \warning Трансформация осуществляется "по месту"!
String toUpper(String &text)
{
    for (auto &i : text) {
        i = toupper(i);
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
std::string replace(const std::string &text, const std::string &from, const std::string &to)
{
    std::string result = text;
    size_t index = 0;
    while (true) {
        index = text.find(from, index);
        if (index == std::string::npos) {
            break;
        }
        result.replace(index, to.length(), to);
        index += to.length();
    }

    return result;
}

// Заменяет все вхождения одной подстроки на другую.
std::wstring replace(const std::wstring &text, const std::wstring &from, const std::wstring &to)
{
    std::wstring result = text;
    size_t index = 0;
    while (true) {
        index = text.find(from, index);
        if (index == std::string::npos) {
            break;
        }
        result.replace(index, to.length(), to);
        index += to.length();
    }

    return result;
}

/// \brief Удаление пробельных символов в начале строки.
std::wstring PLUSIRBIS_EXPORTS trimStart(const std::wstring &text)
{
    // TODO implement
    return text;
}

/// \brief Удаление пробельных символов в конце строки.
std::wstring PLUSIRBIS_EXPORTS trimEnd(const std::wstring &text)
{
    // TODO implement
    return text;
}

/// \brief Удаление пробельных символов в начале и в конце строки.
std::wstring trim(const std::wstring &text)
{
    // TODO implement
    return text;
}

/// \brief Выдаёт текстовое описание ошибки по её коду.
std::wstring describeError(int errorCode)
{
    if (errorCode >= 0) {
        return L"Нет ошибки";
    }

    switch (errorCode) {
        case    -100: return L"Заданный MFN вне пределов БД";
        case    -101: return L"Ошибочный размер полки";
        case    -102: return L"Ошибочный номер полки";
        case    -140: return L"MFN вне пределов БД";
        case    -141: return L"Ошибка чтения";
        case    -200: return L"Указанное поле отсутствует";
        case    -201: return L"Предыдущая версия записи отсутствует";
        case    -202: return L"Заданный термин не найден (термин не существует)";
        case    -203: return L"Последний термин в списке";
        case    -204: return L"Первый термин в списке";
        case    -300: // fall through
        case    -301: return L"База данных монопольно заблокирована";
        case    -400: return L"Ошибка при открытии файлов MST или XRF (ошибка файла данных)";
        case    -401: return L"Ошибка при открытии файлов IFP (ошибка файла индекса)";
        case    -402: return L"Ошибка при записи";
        case    -403: return L"Ошибка при актуализации";
        case    -600: return L"Запись логически удалена";
        case    -601: return L"Запись физически удалена";
        case    -602: return L"Запись заблокирована на ввод";
        case    -603: return L"Запись логически удалена";
        case    -605: return L"Запись физически удалена";
        case    -607: return L"Ошибка autoin.gbl";
        case    -608: return L"Ошибка версии записи";
        case    -700: return L"Ошибка создания резервной копии";
        case    -701: return L"Ошибка восстановления из резервной копии";
        case    -702: return L"Ошибка сортировки";
        case    -703: return L"Ошибочный термин";
        case    -704: return L"Ошибка создания словаря";
        case    -705: return L"Ошибка загрузки словаря";
        case    -800: return L"Ошибка в параметрах глобальной корректировки";
        case    -801: return L"ERR_GBL_REP";
        case    -802: return L"ERR_GBL_MET";
        case   -1111: return L"Ошибка исполнения сервера (SERVER_EXECUTE_ERROR)";
        case   -2222: return L"Ошибка в протоколе (WRONG_PROTOCOL)";
        case   -3333: return L"Незарегистрированный клиент (ошибка входа на сервер) (клиент не в списке)";
        case   -3334: return L"Клиент не выполнил вход на сервер (клиент не используется)";
        case   -3335: return L"Неправильный уникальный идентификатор клиента";
        case   -3336: return L"Нет доступа к командам АРМ";
        case   -3337: return L"Клиент уже зарегистрирован";
        case   -3338: return L"Недопустимый клиент";
        case   -4444: return L"Неверный пароль";
        case   -5555: return L"Файл не существует";
        case   -6666: return L"Сервер перегружен. Достигнуто максимальное число потоков обработки";
        case   -7777: return L"Не удалось запустить/прервать поток администратора (ошибка процесса)";
        case   -8888: return L"Общая ошибка";
        case -100001: return L"Ошибка создания сокета";
        case -100002: return L"Сбой сети";
        case -100003: return L"Не подключен к серверу";
        default: return L"Неизвестная ошибка";
    }
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const String &text)
{
    auto result = 0;
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
unsigned int fastParseUnsigned32(const String &text)
{
    auto result = 0u;
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
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
int fastParse32(const Char *text, size_t length)
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
unsigned int fastParseUnsigned32(const Char *text, size_t length)
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
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

/// \brief Быстрый и грязный разбор строки как целого числа без знака.
unsigned int fastParseUnsigned32(const std::string &text)
{
    auto result = 0u;
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
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
int fastParse32(const char *text, size_t length)
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
unsigned int fastParseUnsigned32(const char *text, size_t length)
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
std::wstring safeAt(const StringList &list, size_t index)
{
    if (index >= list.size())
    {
        return std::wstring();
    }

    return list.at(index);
}

///
/// \param text
/// \param separator
/// \param count
/// \return
StringList maxSplit(const String &text, Char separator, int count)
{
    std::vector<std::wstring> result;

    size_t position = 0;
    const size_t length = text.length();
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

///
/// \param text
/// \param delimiter
/// \return
std::vector<std::string> split(const std::string &text, const std::string &delimiter)
{
    std::vector<std::string> result;
    std::string token;
    size_t start = 0, end;
    while ((end = text.find(delimiter, start)) != std::string::npos)
    {
        token = text.substr(start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
    }

    if (start < text.length())
    {
        token = text.substr(start, std::string::npos);
        result.push_back(token);
    }

    return result;
}

///
/// \param text
/// \param delimiter
/// \return
StringList split(const String &text, const String &delimiter)
{
    std::vector<std::wstring> result;
    std::wstring token;
    size_t start = 0, end;
    while ((end = text.find(delimiter, start)) != std::string::npos)
    {
        token = text.substr(start, end - start);
        result.push_back(token);
        start = end + delimiter.length();
    }

    if (start < text.length())
    {
        token = text.substr(start, std::string::npos);
        result.push_back(token);
    }

    return result;
}

//=========================================================

///
/// \param text
/// \return
String removeComments(const String &text)
{
    if (text.empty()) {
        return text;
    }

    if (!contains(text, L"/*")) {
        return text;
    }

    std::wstring result;
    Char state = 0;
    size_t index = 0;
    const size_t length = text.length();
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

///
/// \param text
/// \return
std::wstring prepareFormat(const std::wstring &text)
{
    std::wstring text2 = removeComments(text);
    const size_t length = text2.length();
    if (length == 0) {
        return text2;
    }

    bool flag = false;
    for (size_t i = 0; i  < length; i++) {
        if (text2.at(i) < ' ') {
            flag = true;
            break;
        }
    }

    if (!flag) {
        return text2;
    }

    std::wstring result;
    for (size_t i = 0; i < length; i++) {
        const Char c = text.at(i);
        if (c >= ' ') {
            result.push_back(c);
        }
    }

    return result;
}

}

