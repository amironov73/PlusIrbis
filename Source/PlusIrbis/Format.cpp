// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

std::wstring Format::removeComments(const std::wstring &text)
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

std::wstring Format::prepareFormat(const std::wstring &text)
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

NAMESPACE_IRBIS_END