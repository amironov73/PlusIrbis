// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iterator>
#include <sstream>

//========================================================
// Утилиты, используемые в проекте.
//========================================================

namespace irbis {

// Сравнение двух символов с точностью до регистра.
bool sameChar(wchar_t first, wchar_t second)
{
    return towupper(first) == towupper(second);
}

// Сравнение двух строк с точностью до регистра символов.
bool sameString(const std::wstring &first, const std::wstring &second)
{
    return std::equal(first.begin(), first.end(), second.begin(), second.end(),
        [] (wchar_t a, wchar_t b) { return towupper(a) == towupper(b); });
}

// Преобразование строки к нижнему регистру.
std::wstring toLower(std::wstring &text)
{
    for (wchar_t &i : text) {
        i = tolower(i);
    }

    return text;
}

// Преобразование строки к верхнему регистру.
std::wstring toUpper(std::wstring &text)
{
    for (wchar_t &i : text) {
        i = toupper(i);
    }

    return text;
}

// Содержит ли строка заданную подстроку?
bool contains(const std::wstring &text, const std::wstring &fragment)
{
    return text.find(fragment) != std::string::npos;
}

// Содержит ли строка заданный символ?
bool contains(const std::wstring &text, wchar_t c)
{
    return text.find(c) != std::string::npos;
}

// Удаление пробельных символов в начале строки.
std::wstring PLUSIRBIS_EXPORTS trimStart(const std::wstring &text)
{
    // TODO implement
    return text;
}

// Удаление пробельных символов в конце строки.
std::wstring PLUSIRBIS_EXPORTS trimEnd(const std::wstring &text)
{
    // TODO implement
    return text;
}

// Удаление пробельных символов в начале и в конце строки.
std::wstring trim(const std::wstring &text)
{
    // TODO implement
    return text;
}

// Выдаёт текстовое описание ошибки по её коду.
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

// Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const std::wstring &text)
{
    auto result = 0;
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

// Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const wchar_t *text)
{
    auto result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

// Быстрый и грязный разбор строки как целого числа без знака.
int fastParse32(const wchar_t *text, int length)
{
    auto result = 0;
    while (length > 0) {
        result = result * 10 + *text - L'0';
        text++;
        length--;
    }

    return result;
}

const std::string& iif(const std::string& s1, const std::string &s2)
{
    if (!s1.empty())
    {
        return s1;
    }

    return s2;
}

const std::wstring& iif(const std::wstring& s1, const std::wstring &s2)
{
    if (!s1.empty())
    {
        return s1;
    }

    return s2;
}

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

std::wstring safeAt(const StringList &list, size_t index)
{
    if (index >= list.size())
    {
        return std::wstring();
    }

    return list.at(index);
}

StringList maxSplit(const std::wstring &text, wchar_t separator, int count)
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

StringList split(const std::wstring &text, wchar_t delimiter)
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

StringList split(const std::wstring &text, const std::wstring &delimiter)
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

std::wstring removeComments(const std::wstring &text)
{
    if (text.empty()) {
        return text;
    }

    if (!contains(text, L"/*")) {
        return text;
    }

    std::wstring result;
    wchar_t state = 0;
    size_t index = 0;
    const size_t length = text.length();
    while (index < length) {
        wchar_t c = text.at(index);

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
        const wchar_t c = text.at(i);
        if (c >= ' ') {
            result.push_back(c);
        }
    }

    return result;
}

}

