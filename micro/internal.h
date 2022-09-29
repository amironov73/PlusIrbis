// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef IRBISINTERNAL_H
#define IRBISINTERNAL_H

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <future>
#include <ios>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <functional>

namespace irbis
{
    /// \brief Выбор первой непустой строки из произвольного количества.
    template<class T>
    T choose(T first) { return first; }

    /// \brief Выбор первой непустой строки из произвольного количества.
    template<class T1, class... T2>
    T1 choose(const T1 &first, const T2 &... others)
    {
        if (!first.empty()) {
            return first;
        }
        return choose(others...);
    }

    /// \brief Является ли данный элемент одним из перечисленных?
    template<class T>
    bool oneOf(T left, T right) { return left == right; }

    /// \brief Является ли данный элемент одним из перечисленных?
    template<class T1, class... T2>
    bool oneOf(const T1 &first, const T1 &second, const T2 &... others)
    {
        if (oneOf(first, second)) {
            return true;
        }
        return oneOf(first, others...);
    }

    /// \brief Безопасное извлечение элемента из контейнера.
    template<typename T1>
    typename T1::value_type safeAt(const T1 &container, std::size_t index)
    {
        if (index >= container.size()) {
            return typename T1::value_type();
        }
        return container[index];
    }

    bool sameChar(char first, char second) noexcept;
    bool sameString(const std::string &first, const std::string &second) noexcept;
    char firstChar(const std::string &text) noexcept;
    std::string toLower(std::string &text) noexcept;
    std::string toUpper(std::string &text) noexcept;
    bool contains(const std::string &text, char c);
    bool contains(const std::string &text, const std::string &fragment);
    std::string replace(const std::string &text, const std::string &from, const std::string &to);
    std::string trimStart(const std::string &text);
    std::string trimEnd(const std::string &text);
    std::string trim(const std::string &text);
    int fastParse32(const std::string &text) noexcept;
    int fastParse32(const char *text) noexcept;
    int fastParse32(const char *text, std::size_t length) noexcept;
    unsigned int fastParseUnsigned32(const std::string &text) noexcept;
    unsigned int fastParseUnsigned32(const char *text) noexcept;
    unsigned int fastParseUnsigned32(const char *text, std::size_t length) noexcept;
    std::vector<std::string> split(const std::string &text, char delimiter);
    std::vector<std::string> split(const std::string &text, const std::string &delimiter);
    std::vector<std::string> maxSplit(const std::string &text, char separator, int count);

    template<typename T>
    bool isDigit(T c) { return (c >= '0') && (c <= '9'); }

}

#endif // IRBISINTERNAL_H
