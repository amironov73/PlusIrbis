// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cwctype>

namespace irbis {

const Char TextNavigator::EOT = L'\0';

TextNavigator::TextNavigator(const String &text)
    : _text(text)
{
    this->_position = 0;
    this->_length = _text.length();
    this->_line = 1;
    this->_column = 1;
}

TextNavigator::TextNavigator(const TextNavigator &other)
    : _text(other._text)
{
    this->_position = other._position;
    this->_length = other._length;
    this->_line = other._line;
    this->_column = other._column;
}

Char TextNavigator::at(std::size_t position) const noexcept
{
    return position < this->_length ? this->_text.at(position) : EOT;
}

Char TextNavigator::front() const noexcept
{
    return this->at(0);
}

Char TextNavigator::back() const noexcept
{
    return this->at(this->_length - 1);
}

Char TextNavigator::lookAhead(std::ptrdiff_t distance) const noexcept
{
    const std::size_t newPosition = this->_position + distance;

    return this->at(newPosition);
}

Char TextNavigator::lookBehind(std::ptrdiff_t distance) const noexcept
{
    const std::size_t newPosition = this->_position - distance;

    return this->at(newPosition);
}

TextNavigator& TextNavigator::move(std::ptrdiff_t distance) noexcept
{
    // TODO Some checks

    this->_position += distance;
    this->_column += distance;

    return *this;
}

Char TextNavigator::peekChar() const noexcept
{
    return this->_position >= this->_length ? EOT : this->_text.at(_position);
}

Char TextNavigator::readChar() noexcept
{
    if (this->_position >= this->_length) {
        return EOT;
    }

    const Char result = this->_text.at(this->_position++);
    if (result == '\n') {
        this->_line++;
        this->_column = 1;
    } else {
        this->_column++;
    }

    return result;
}

String TextNavigator::peekString(std::size_t length)
{
    String result;

    if (this->eot()) {
        return result;
    }

    const std::size_t savePosition = this->_position,
        saveColumn = this->_column,
        saveLine = this->_line;
    for (std::size_t i = 0; i < length; ++i) {
        const Char c = this->readChar();
        if ((c == EOT) || (c == '\r') || (c == '\n')) {
            break;
        }

        result.push_back(c);
    }

    this->_position = savePosition;
    this->_column = saveColumn;
    this->_line = saveLine;

    return result;
}

String TextNavigator::peekTo(Char stopChar)
{
    String result;

    if (this->eot()) {
        return result;
    }

    const std::size_t savePosition = this->_position,
        saveColumn = this->_column,
        saveLine = this->_line;
    result = this->readTo(stopChar);
    this->_position = savePosition;
    this->_column = saveColumn;
    this->_line = saveLine;

    return result;
}

String TextNavigator::peekUntil(Char stopChar)
{
    String result;

    if (this->eot()) {
        return result;
    }

    const std::size_t savePosition = this->_position,
        saveColumn = this->_column,
        saveLine = this->_line;
    result = this->readUntil(stopChar);
    this->_position = savePosition;
    this->_column = saveColumn;
    this->_line = saveLine;

    return result;
}

String TextNavigator::readLine()
{
    String result;
    if (this->_position >= this->_length) {
        return result;
    }

    const std::size_t start = this->_position;
    while (this->_position < this->_length) {
        const Char c = this->_text.at(this->_position);
        if (c == '\r' || c == '\n') {
            break;
        }

        this->readChar();
    }

    result = this->_text.substr(start, this->_position - start);
    if (this->_position < this->_length) {
        Char c = this->_text.at(this->_position);
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

String TextNavigator::readInteger()
{
    String result;

    if (this->isDigit()) {
        while (this->isDigit()) {
            result.push_back(this->readChar());
        }
    }

    return result;
}

String TextNavigator::readString(std::size_t length)
{
    String result;

    if (this->eot()) {
        return result;
    }

    result.reserve(length);
    for (std::size_t i = 0; i < length; ++i) {
        const Char c = this->readChar();
        if (c == EOT) {
            break;
        }

        result.push_back(c);
    }

    return result;
}

String TextNavigator::readTo(Char stopChar)
{
    String result;

    if (this->eot()) {
        return result;
    }

    const std::size_t start = this->_position;
    std::size_t end = this->_position;
    while (true) {
        const Char c = this->readChar();
        if ((c == EOT) || (c == stopChar)) {
            break;
        }
        end = this->_position;
    }

    result = this->_text.substr(start, end - start);

    return result;
}

String TextNavigator::readUntil(Char stopChar)
{
    String result;

    if (this->eot()) {
        return result;
    }

    const std::size_t start = this->_position;
    while (true) {
        const Char c = this->peekChar();
        if ((c == EOT) || (c == stopChar)) {
            break;
        }

        this->readChar();
    }

    result = this->_text.substr(start, this->_position - start);

    return result;
}

String TextNavigator::readWhile(wchar_t goodChar)
{
    String result;

    while (true) {
        const Char c = this->peekChar();
        if ((c == EOT) || (c != goodChar)) {
            break;
        }

        result.push_back(this->readChar());
    }

    return result;
}

String TextNavigator::readWord()
{
    String result;

    while (true) {
        const Char c = this->peekChar();
        if ((c == EOT) || !std::iswalnum(c)) {
            break;
        }

        result.push_back(this->readChar());
    }

    return result;
}

String TextNavigator::remainingText() const
{
    if (this->eot()) {
        return String();
    }

    return this->_text.substr(this->_position);
}

String TextNavigator::recentText(std::ptrdiff_t length) const
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

    return this->_text.substr(start, length);
}

TextNavigator& TextNavigator::skipWhitespace()
{
    while (true) {
        const Char c = this->peekChar();
        if (!std::iswspace(c)) {
            break;
        }
        this->readChar();
    }

    return *this;
}

TextNavigator& TextNavigator::skipPunctuation()
{
    while (true) {
        const wchar_t c = this->peekChar();
        if (!std::iswpunct(c)) {
            break;
        }
        this->readChar();
    }

    return *this;
}

String TextNavigator::substr(std::size_t offset, std::size_t length) const
{
    return this->_text.substr(offset, length);
}

}
