// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cwctype>

namespace irbis {

const wchar_t TextNavigator::EOT = '\0';

TextNavigator::TextNavigator(const std::wstring &text)
    : _text(text)
{
    _position = 0;
    _length = _text.length();
    _line = 1;
    _column = 1;
}

TextNavigator::TextNavigator(const TextNavigator &other)
    : _text(other._text)
{
    _position = other._position;
    _length = other._length;
    _line = other._line;
    _column = other._column;
}

wchar_t TextNavigator::at(size_t position) const
{
    return position < _length ? _text.at(position) : EOT;
}

wchar_t TextNavigator::front() const
{
    return at(0);
}

wchar_t TextNavigator::back() const
{
    return at(_length - 1);
}

wchar_t TextNavigator::lookAhead(ptrdiff_t distance) const
{
    const size_t newPosition = _position + distance;

    return at(newPosition);
}

wchar_t TextNavigator::lookBehind(ptrdiff_t distance) const
{
    const size_t newPosition = _position - distance;

    return at(newPosition);
}

TextNavigator& TextNavigator::move(ptrdiff_t distance)
{
    // TODO Some checks

    _position += distance;
    _column += distance;

    return *this;
}

wchar_t TextNavigator::peekChar() const
{
    return _position >= _length ? EOT : _text.at(_position);
}

wchar_t TextNavigator::readChar()
{
    if (_position >= _length) {
        return EOT;
    }

    const wchar_t result = _text.at(_position++);
    if (result == '\n') {
        _line++;
        _column = 1;
    } else {
        _column++;
    }

    return result;
}

std::wstring TextNavigator::peekString(size_t length)
{
    std::wstring result;

    if (eot()) {
        return result;
    }

    const size_t savePosition = _position, saveColumn = _column, saveLine = _line;
    for (size_t i = 0; i < length; ++i) {
        const wchar_t c = readChar();
        if ((c == EOT) || (c == '\r') || (c == '\n')) {
            break;
        }

        result.push_back(c);
    }

    _position = savePosition;
    _column = saveColumn;
    _line = saveLine;

    return result;
}

std::wstring TextNavigator::peekTo(wchar_t stopChar)
{
    std::wstring result;

    if (eot()) {
        return result;
    }

    const size_t savePosition = _position, saveColumn = _column, saveLine = _line;
    result = readTo(stopChar);
    _position = savePosition;
    _column = saveColumn;
    _line = saveLine;

    return result;
}

std::wstring TextNavigator::peekUntil(wchar_t stopChar)
{
    std::wstring result;

    if (eot()) {
        return result;
    }

    const size_t savePosition = _position, saveColumn = _column, saveLine = _line;
    result = readUntil(stopChar);
    _position = savePosition;
    _column = saveColumn;
    _line = saveLine;

    return result;
}

std::wstring TextNavigator::readLine()
{
    std::wstring result;
    if (_position >= _length) {
        return result;
    }

    const size_t start = _position;
    while (_position < _length) {
        const wchar_t c = _text.at(_position);
        if (c == '\r' || c == '\n') {
            break;
        }

        readChar();
    }

    result = _text.substr(start, _position - start);
    if (_position < _length) {
        wchar_t c = _text.at(_position);
        if (c == '\r') {
            readChar();
            c = peekChar();
        }

        if (c == '\n') {
            readChar();
        }
    }

    return result;
}

bool TextNavigator::isControl() const
{
    const wchar_t c = peekChar();

    return (c > 0) && (c < ' ');
}

bool TextNavigator::isDigit() const
{
    const wchar_t c = peekChar();

    return std::iswdigit(c);
}

bool TextNavigator::isLetter() const
{
    const wchar_t c = peekChar();

    return std::iswalpha(c);
}

bool TextNavigator::isWhitespace() const
{
    const wchar_t c = peekChar();

    return std::iswspace(c);
}

std::wstring TextNavigator::readInteger()
{
    std::wstring result;

    if (isDigit()) {
        while (isDigit()) {
            result.push_back(readChar());
        }
    }

    return result;
}

std::wstring TextNavigator::readString(size_t length)
{
    std::wstring result;

    if (eot()) {
        return result;
    }

    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        const wchar_t c = readChar();
        if (c == EOT) {
            break;
        }

        result.push_back(c);
    }

    return result;
}

// —читывание вплоть до указанного символа
// (не включа€ его, однако, символ считываетс€).
std::wstring TextNavigator::readTo(wchar_t stopChar)
{
    std::wstring result;

    if (eot()) {
        return result;
    }

    const size_t start = _position;
    size_t end = _position;
    while (true) {
        const wchar_t c = readChar();
        if ((c == EOT) || (c == stopChar)) {
            break;
        }
        end = _position;
    }

    result = _text.substr(start, end - start);

    return result;
}

// —читывание вплоть до указанного символа
// (сам символ остаетс€ несчитанным).
std::wstring TextNavigator::readUntil(wchar_t stopChar)
{
    std::wstring result;

    if (eot()) {
        return result;
    }

    const size_t start = _position;
    while (true) {
        const wchar_t c = peekChar();
        if ((c == EOT) || (c == stopChar)) {
            break;
        }

        readChar();
    }

    result = _text.substr(start, _position - start);

    return result;
}

std::wstring TextNavigator::readWhile(wchar_t goodChar)
{
    std::wstring result;

    while (true) {
        const wchar_t c = peekChar();
        if ((c == EOT) || (c != goodChar)) {
            break;
        }

        result.push_back(readChar());
    }

    return result;
}

std::wstring TextNavigator::readWord()
{
    std::wstring result;

    while (true) {
        const wchar_t c = peekChar();
        if ((c == EOT) || !std::iswalnum(c)) {
            break;
        }

        result.push_back(readChar());
    }

    return result;
}

std::wstring TextNavigator::remainingText() const
{
    if (eot()) {
        return std::wstring();
    }

    return _text.substr(_position);
}

std::wstring TextNavigator::recentText(ptrdiff_t length) const
{
    ptrdiff_t start = _position - length;
    if (start < 0) {
        length += start;
        start = 0;
    }

    if (static_cast<size_t>(start) >= _length) {
        start = 0;
        length = 0;
    }

    if (length < 0) {
        length = 0;
    }

    return _text.substr(start, length);
}

TextNavigator& TextNavigator::skipWhitespace()
{
    while (true) {
        const wchar_t c = peekChar();
        if (!std::iswspace(c)) {
            break;
        }
        readChar();
    }

    return *this;
}

TextNavigator& TextNavigator::skipPunctuation()
{
    while (true) {
        const wchar_t c = peekChar();
        if (!std::iswpunct(c)) {
            break;
        }
        readChar();
    }

    return *this;
}

std::wstring TextNavigator::substr(size_t offset, size_t length) const
{
    return _text.substr(offset, length);
}

}
