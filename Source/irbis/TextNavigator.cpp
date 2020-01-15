// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cwctype>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

/*!
    \class irbis::TextNavigator

    \warning Сломается, если в тексте будет присутствовать символ `\0`.
 */

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

const Char TextNavigator::EOT = L'\0';

// \brief Конструктор.
TextNavigator::TextNavigator(const Char* text, std::size_t length) noexcept
{
    this->_text = text;
    this->_length = length;
    this->_position = 0;
    this->_line = 1;
    this->_column = 1;
}

// \brief Конструктор.
TextNavigator::TextNavigator(WSpan text) noexcept
{
    this->_text = text.data();
    this->_length = text.size();
    this->_position = 0;
    this->_line = 1;
    this->_column = 1;
}

// \brief Конструктор.
TextNavigator::TextNavigator (const String &text) noexcept
{
    this->_text = text.data();
    this->_position = 0;
    this->_length = text.length();
    this->_line = 1;
    this->_column = 1;
}

// \brief Конструктор.
TextNavigator::TextNavigator (const TextNavigator &other) noexcept
{
    this->_text = other._text;
    this->_position = other._position;
    this->_length = other._length;
    this->_line = other._line;
    this->_column = other._column;
}

/// \brief Номер текущей колонки, отсчет от 1.
std::size_t TextNavigator::column() const noexcept
{
    return this->_column;
}

/// \brief Номер текущей строки, отсчет от 1.
std::size_t TextNavigator::line() const noexcept
{
    return this->_line;
}

/// \brief Длина текста в символах (включая все служебные).
std::size_t TextNavigator::length() const noexcept
{
    return this->_length;
}

/// \brief Текущая позиция в тексте (в символах), отсчет от 0.
std::size_t TextNavigator::position() const noexcept
{
    return this->_position;
}

/// \brief Итератор, указывающий на начало текста.
Char* TextNavigator::begin() const noexcept
{
    return const_cast<Char*>(this->_text);
}

/// \brief Итератор, указывающий на начало текста.
const Char* TextNavigator::cbegin() const noexcept
{
    return this->_text;
}

/// \brief Итератор, указывающий на текущий символ.
Char* TextNavigator::current() const noexcept
{
    return const_cast<Char*>(this->_text + this->_position);
}

/// \brief Итератор, указывающий на текущий символ.
const Char* TextNavigator::ccurrent() const noexcept
{
    return this->_text + this->_position;
}

/// \brief Итератор, указывающий за концом текста.
Char* TextNavigator::end() const noexcept
{
    return const_cast<Char*>(this->_text + this->_length);
}

/// \brief Итератор, указывающий за концом текста.
const Char* TextNavigator::cend() const noexcept
{
    return this->_text + this->_length;
}

/// \brief Достигнут конец текста?
bool TextNavigator::eot() const noexcept
{
    return this->_position >= this->_length;
}

/// \brief Подглядываем символ в указанной позиции (отсчет от начала текста, от 0).
Char TextNavigator::at(std::size_t position) const noexcept
{
    return position < this->_length ? this->_text[position] : EOT;
}

/// \brief Первый символ в тексте.
Char TextNavigator::front() const noexcept
{
    return this->at(0);
}

/// \brief Последний символ в тексте.
Char TextNavigator::back() const noexcept
{
    return this->at(this->_length - 1);
}

/// \brief Заглядывание вперед на указанное количество символов.
Char TextNavigator::lookAhead(std::ptrdiff_t distance) const noexcept
{
    const auto newPosition = this->_position + distance;
    return this->at(newPosition);
}

/// \brief Заглядывание назад на указанное количество символов.
Char TextNavigator::lookBehind(std::ptrdiff_t distance) const noexcept
{
    const auto newPosition = this->_position - distance;

    return this->at(newPosition);
}

TextNavigator& TextNavigator::move(std::ptrdiff_t distance) noexcept
{
    // TODO Some checks

    this->_position += distance;
    this->_column += distance;

    return *this;
}

/// \brief Подсматриваем текущий символ.
Char TextNavigator::peekChar() const noexcept
{
    return this->at(this->_position);
}

/// \brief Считываем текущий символ.
Char TextNavigator::readChar() noexcept
{
    if (this->eot()) {
        return EOT;
    }

    const auto result = this->at(this->_position++);
    if (result == '\n') {
        this->_line++;
        this->_column = 1;
    } else {
        this->_column++;
    }

    return result;
}

/// \brief Подглядывание строки вплоть до указанной длины.
WSpan TextNavigator::peekString(std::size_t length) const noexcept
{
    WSpan result ( this->_text + this->_position, 0);
    if (this->eot()) {
        return result;
    }

    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead(i);
        if ((c == EOT) || (c == '\r') || (c == '\n')) {
            break;
        }
        ++result.length;
    }

    return result;
}

/// \brief Подглядывание вплоть до указанного символа (включая его).
WSpan TextNavigator::peekTo (Char stopChar) const noexcept
{
    WSpan result (this->_text + this->_position, 0);
    const auto length = this->_length - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead(i);
        ++result.length;
        if (c == stopChar) {
            break;
        }
    }

    return result;
}

/// \brief Подглядывание вплоть до указанного символа (не включая его).
WSpan TextNavigator::peekUntil(Char stopChar) const noexcept
{
    WSpan result (this->_text + this->_position, 0);
    const auto length = this->_length - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead(i);
        if (c == stopChar) {
            break;
        }
        ++result.length;
    }

    return result;
}

/// \brief Считывание строки.
WSpan TextNavigator::readLine() noexcept
{
    WSpan result (this->_text + this->_position, 0);
    if (this->eot()) {
        return result;
    }

    const auto start = this->_position;
    while (!this->eot()) {
        const auto c = this->peekChar();
        if (c == '\r' || c == '\n') {
            break;
        }

        this->readChar();
    }

    result = this->substr(start, this->_position - start);
    if (!this->eot()) {
        Char c = this->peekChar();
        if (c == '\r') {
            this->readChar();
            c = this->peekChar();
        }

        if (c == '\n') {
            this->readChar();
        }
    }

    return result;
}

bool TextNavigator::isControl() const noexcept
{
    const Char c = this->peekChar();

    return (c > 0) && (c < ' ');
}

bool TextNavigator::isDigit() const noexcept
{
    const Char c = this->peekChar();

    return std::iswdigit(c);
}

bool TextNavigator::isLetter() const noexcept
{
    const Char c = this->peekChar();

    return std::iswalpha(c);
}

bool TextNavigator::isWhitespace() const noexcept
{
    const Char c = this->peekChar();

    return std::iswspace(c);
}

WSpan TextNavigator::readInteger() noexcept
{
    WSpan result { this->_text, 0 };

    while (this->isDigit()) {
        ++result.length;
        this->readChar();
    }

    return result;
}

WSpan TextNavigator::readString(std::size_t length) noexcept
{
    WSpan result { this->_text + this->_position, 0 };
    if (this->eot()) {
        return result;
    }

    for (std::size_t i = 0; i < length; ++i) {
        const Char c = this->readChar();
        if (c == EOT) {
            break;
        }

        ++result.length;
    }

    return result;
}

WSpan TextNavigator::readTo(Char stopChar) noexcept
{
    WSpan result { this->_text + this->_position, 0 };

    if (this->eot()) {
        return result;
    }

    const auto start = this->_position;
    auto end = this->_position;
    while (true) {
        const auto c = this->readChar();
        if ((c == EOT) || (c == stopChar)) {
            break;
        }
        end = this->_position;
    }

    result = this->substr(start, end - start);

    return result;
}

WSpan TextNavigator::readUntil(Char stopChar) noexcept
{
    WSpan result { this->_text + this->_position, 0 };

    if (this->eot()) {
        return result;
    }

    const auto start = this->_position;
    while (true) {
        const auto c = this->peekChar();
        if ((c == EOT) || (c == stopChar)) {
            break;
        }

        this->readChar();
    }

    result = this->substr(start, this->_position - start);
    return result;
}

WSpan TextNavigator::readWhile (Char goodChar) noexcept
{
    WSpan result { this->_text + this->_position, 0 };

    while (true) {
        const auto c = this->peekChar();
        if ((c == EOT) || (c != goodChar)) {
            break;
        }

        ++result.length;
        this->readChar();
    }

    return result;
}

WSpan TextNavigator::readWord() noexcept
{
    WSpan result (this->_text + this->_position, 0);

    while (true) {
        const auto c = this->peekChar();
        if ((c == EOT) || !std::iswalnum(c)) {
            break;
        }

        ++result.length;
        this->readChar();
    }

    return result;
}

WSpan TextNavigator::remainingText() const noexcept
{
    if (this->eot()) {
        return WSpan (this->_text + this->_position, 0);
    }

    return this->substr(this->_position, this->_length - this->_position);
}

WSpan TextNavigator::recentText (std::ptrdiff_t length) const noexcept
{
    std::ptrdiff_t start = this->_position - length;
    if (start < 0) {
        length += start;
        start = 0;
    }

    if (static_cast<size_t>(start) >= this->_length) {
        start = 0;
        length = 0;
    }

    if (length < 0) {
        length = 0;
    }

    return this->substr (start, length);
}

TextNavigator& TextNavigator::skipWhitespace() noexcept
{
    while (!this->eot()) {
        const auto c = this->peekChar();
        if (!std::iswspace (c)) {
            break;
        }
        this->readChar();
    }

    return *this;
}

TextNavigator& TextNavigator::skipPunctuation() noexcept
{
    while (!this->eot()) {
        const auto c = this->peekChar();
        if (!std::iswpunct (c)) {
            break;
        }
        this->readChar();
    }

    return *this;
}

WSpan TextNavigator::substr (std::size_t offset, std::size_t length) const noexcept
{
    return {this->_text + offset, length };
}

}
