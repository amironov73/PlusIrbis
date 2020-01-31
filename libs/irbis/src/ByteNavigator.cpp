// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cctype>
#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

//-V::801

/*!
    \class irbis::ByteNavigator
 */

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Признак конца диапазона.
const int ByteNavigator::EOT = -1;

/// \brief Создает навигатор по пустому диапазону.
ByteNavigator::ByteNavigator() noexcept
    : _data()
{
}

/// \brief Создает навигатор по указанному диапазону.
/// \param data Диапазон для навигации.
ByteNavigator::ByteNavigator(ByteSpan data) noexcept
    : _data(data)
{
}

/// \brief Возвращает текущую позицию (отсчет в байтах от 0).
/// \return Текущая позиция.
std::size_t ByteNavigator::position() const noexcept
{
    return this->_position;
}

/// \brief Размер диапазона в байтах.
/// \return Размер диапазона.
std::size_t ByteNavigator::size() const noexcept
{
    return this->_data.size();
}

/// \brief Указатель на начало диапазона.
/// \return Указатель на начало.
Byte* ByteNavigator::begin() const noexcept
{
    return this->_data.begin();
}

/// \brief Константный указатель на начало диапазона.
/// \return Указатель на начало.
const Byte* ByteNavigator::cbegin() const noexcept
{
    return this->_data.cbegin();
}

/// \brief Указатель на текущий байт.
/// \return Указатель.
Byte* ByteNavigator::current() const noexcept
{
    return this->begin() + this->_position;
}

/// \brief Константный указатель на текущий байт.
/// \return Указатель.
const Byte* ByteNavigator::ccurrent() const noexcept
{
    return this->_data.cbegin() + this->_position;
}

/// \brief Указатель за концом диапазона.
/// \return Указатель.
Byte* ByteNavigator::end() const noexcept
{
    return this->_data.end();
}

/// \brief Константный указатель за концом диапазона.
/// \return Указатель.
const Byte* ByteNavigator::cend() const noexcept
{
    return this->_data.cend();
}

/// \brief Признак достигнутого конца диапазона.
/// \return `true` если достигнут конец.
bool ByteNavigator::eot() const noexcept
{
    return this->_position >= this->_data.size();
}

/// \brief Получение байта по указанному смещению.
/// \param position Смещение в байтах (отсчет от 0) с начала диапазона.
/// \return Значение байта либо EOT.
int ByteNavigator::at(std::size_t position) const noexcept
{
    return position < this->_data.size() ? this->_data[position] : EOT;
}

/// \brief Получение оставшихся данных в виде диапазона.
/// \return Диапазон (возможно, пустой).
ByteSpan ByteNavigator::remaining() const noexcept
{
    if (this->eot()) {
        return {};
    }
    return this->_data.slice(this->_position);
}

/// \brief Первый байт в диапазоне.
int ByteNavigator::front() const noexcept
{
    return this->at(0);
}

/// \brief Последний байт в диапазоне.
int ByteNavigator::back() const noexcept
{
    return this->at(this->size() - 1);
}

/// \brief Заглядывание вперед на указанное количество байтов.
int ByteNavigator::lookAhead(std::ptrdiff_t distance) const noexcept
{
    // TODO some checks
    const auto newPosition = this->_position + distance;
    return this->at(newPosition);
}

/// \brief Заглядывание назад на указанное количество байтов.
int ByteNavigator::lookBehind(std::ptrdiff_t distance) const noexcept
{
    // TODO some checks
    const auto newPosition = this->_position - distance;
    return this->at(newPosition);
}

/// \brief Перемещение по диапазону вперёд/назад.
ByteNavigator& ByteNavigator::move(std::ptrdiff_t distance) noexcept
{
    // TODO Some checks
    this->_position += distance;
    return *this;
}

/// \brief Подсматриваем текущий байт.
int ByteNavigator::peekByte() const noexcept
{
    return this->at(this->_position);
}

/// \brief Считываем текущий байт. Двигаемся вперёд по диапазону.
int ByteNavigator::readByte() noexcept
{
    return this->eot() ? EOT : this->at(this->_position++);
}

/// \brief Подглядывание строки вплоть до указанной длины.
ByteSpan ByteNavigator::peekString(std::size_t length) const noexcept
{
    if (this->eot()) {
        return {};
    }

    ByteSpan result (this->ccurrent(), 0);
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
ByteSpan ByteNavigator::peekTo(Byte stop) const noexcept
{
    ByteSpan result (this->ccurrent(), 0);
    const auto length = this->size() - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead(i);
        ++result.length;
        if (c == stop) {
            break;
        }
    }
    return result;
}

/// \brief Подглядывание вплоть до указанного байта (не включая его).
ByteSpan ByteNavigator::peekUntil(Byte stop) const noexcept
{
    ByteSpan result (this->ccurrent(), 0);
    const auto length = this->size() - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead(i);
        if (c == stop) {
            break;
        }
        ++result.length;
    }
    return result;
}

/// \brief Считывание строки.
ByteSpan ByteNavigator::readLine() noexcept
{
    ByteSpan result (this->ccurrent(), 0);
    if (this->eot()) {
        return result;
    }

    const auto start = this->_position;
    while (!this->eot()) {
        const auto c = this->peekByte();
        if (c == '\r' || c == '\n') {
            break;
        }

        this->readByte();
    }

    result = this->slice(start, this->_position - start);
    if (!this->eot()) {
        auto c = this->peekByte();
        if (c == '\r') {
            this->readByte();
            c = this->peekByte();
        }

        if (c == '\n') {
            this->readByte();
        }
    }

    return result;
}

/// \brief Управляющий символ?
bool ByteNavigator::isControl() const noexcept
{
    const auto c = this->peekByte();
    return (c > 0) && (c < ' ');
}

/// \brief Цифра?
bool ByteNavigator::isDigit() const noexcept
{
    const auto c = this->peekByte();
    return std::isdigit(c);
}

/// \brief Буква?
bool ByteNavigator::isLetter() const noexcept
{
    const auto c = this->peekByte();
    return std::isalpha(c);
}

/// \brief Пробельный символ?
bool ByteNavigator::isWhitespace() const noexcept
{
    const auto c = this->peekByte();
    return std::isspace(c);

}

ByteSpan ByteNavigator::readInteger() noexcept
{
    if (this->eot()) {
        return {};
    }

    ByteSpan result {this->current(), 0 };
    while (this->isDigit()) {
        ++result.length;
        this->readByte();
    }
    return result;
}

ByteSpan ByteNavigator::readString (std::size_t length) noexcept
{
    ByteSpan result {this->current(), 0 };
    if (this->eot()) {
        return result;
    }
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->readByte();
        if (c == EOT) {
            break;
        }
        ++result.length;
    }
    return result;
}

ByteSpan ByteNavigator::readTo (Byte stopByte) noexcept
{
    ByteSpan result {this->ccurrent(), 0 };
    if (this->eot()) {
        return result;
    }
    const auto start = this->_position;
    auto end = this->_position;
    while (true) {
        const auto c = this->readByte();
        if (c == EOT) {
            break;
        }
        end = this->_position;
        if (c == stopByte) {
            break;
        }
    }
    result = this->slice (start, end - start);
    return result;
}

ByteSpan ByteNavigator::readUntil (Byte stopByte) noexcept
{
    ByteSpan result {this->ccurrent(), 0 };
    if (this->eot()) {
        return result;
    }
    const auto start = this->_position;
    while (true) {
        const auto c = this->peekByte();
        if ((c == EOT) || (c == stopByte)) {
            break;
        }
        this->readByte();
    }
    result = this->slice(start, this->_position - start);
    return result;
}

ByteSpan ByteNavigator::readWhile (Byte goodByte) noexcept
{
    ByteSpan result {this->ccurrent(), 0 };
    while (true) {
        const auto c = this->peekByte();
        if ((c == EOT) || (c != goodByte)) {
            break;
        }
        ++result.length;
        this->readByte();
    }
    return result;
}

ByteSpan ByteNavigator::readWord() noexcept
{
    ByteSpan result (this->ccurrent(), 0);
    while (true) {
        const auto c = this->peekByte();
        if ((c == EOT) || !std::isalnum(c)) {
            break;
        }
        ++result.length;
        this->readByte();
    }
    return result;
}

ByteSpan ByteNavigator::recent(std::ptrdiff_t length) const noexcept
{
    std::ptrdiff_t start = this->_position - length;
    if (start < 0) {
        length += start;
        start = 0;
    }

    if (static_cast<std::size_t>(start) >= this->size()) {
        start = 0;
        length = 0;
    }

    if (length < 0) {
        length = 0;
    }

    return this->slice (start, length);
}

ByteNavigator& ByteNavigator::skipWhitespace() noexcept
{
    while (!this->eot()) {
        const auto c = this->peekByte();
        if (!std::isspace (c)) {
            break;
        }
        this->readByte();
    }
    return *this;
}

ByteNavigator& ByteNavigator::skipPunctuation() noexcept
{
    while (!this->eot()) {
        const auto c = this->peekByte();
        if (!std::ispunct (c)) {
            break;
        }
        this->readByte();
    }
    return *this;
}

/// \brief Пропускаем остаток строки.
ByteNavigator& ByteNavigator::skipLine() noexcept
{
    if (!this->eot()) {
        while (!this->eot())
        {
            const auto c = this->peekByte();
            if (c == '\r' || c == '\n')
            {
                break;
            }
            this->readByte();
        }

        if (!this->eot())
        {
            auto c = this->peekByte();
            if (c == '\r')
            {
                this->readByte();
                c = this->peekByte();
            }
            if (c == '\n')
            {
                this->readByte();
            }
        }
    }
    return *this;
}

ByteSpan ByteNavigator::slice(std::ptrdiff_t offset, std::size_t length) const noexcept
{
    if (this->size() == 0) {
        return {};
    }

    // TODO some checks
    return {this->cbegin() + offset, length };
}

}
