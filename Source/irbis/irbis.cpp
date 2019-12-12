// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iterator>
#include <sstream>

//========================================================
// –азнообразные утилиты, примен€емые в проекте.
//========================================================

namespace irbis {

// —равнение двух символов с точностью до регистра.
bool sameChar(wchar_t first, wchar_t second)
{
    return towupper(first) == towupper(second);
}

// —равнение двух строк с точностью до регистра.
bool sameString(const std::wstring &first, const std::wstring &second)
{
    return std::equal(std::begin(first), std::end(first),
        std::begin(second), std::end(second),
        [] (wchar_t a, wchar_t b) { return towupper(a) == towupper(b); });
}

// —одержит ли строка указанную подстроку?
bool contains(const std::wstring &text, const std::wstring &fragment)
{
    return text.find(fragment) != std::string::npos;
}

// Ѕыстрый и гр€зный разбор строки как целого числа без знака.
int fastParse32(const std::wstring &text)
{
    auto result = 0;
    const size_t length = text.length();
    for (size_t offset = 0; offset < length; offset++) {
        result = result * 10 + text.at(offset) - '0';
    }

    return result;
}

// Ѕыстрый и гр€зный разбор строки как целого числа без знака.
int fastParse32(const wchar_t *text)
{
    auto result = 0;
    while (*text != 0) {
        result = result * 10 + *text - '0';
        text++;
    }

    return result;
}

// Ѕыстрый и гр€зный разбор строки как целого числа без знака.
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

