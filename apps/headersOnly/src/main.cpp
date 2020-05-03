// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
// Это простое приложение предназначено для разработки и отладки шаблонов,
// определенных в заголовочных файлах irbis.h и irbis_internal.h.
// Больше от него никакой пользы.
//==============================================================================

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"

#include <cctype>

template <class IteratorType>
struct ByteRange final
{
    IteratorType m_begin;   ///< Начало данных.
    IteratorType m_current; ///< Текущая позиция.
    IteratorType m_end;     ///< За концом данных.
    std::size_t  m_size;    ///< Размер в байтах.

    ByteRange (IteratorType begin, IteratorType end) noexcept
        : m_begin { begin }, m_current { begin }, m_end { end },
        m_size { static_cast <std::size_t> (m_end - m_begin) } {}

    ByteRange             (const ByteRange &) noexcept = default; ///< Конструктор копирования.
    ByteRange             (ByteRange &&)      noexcept = default; ///< Конструктор перемещения.
    ByteRange& operator = (const ByteRange &) noexcept = default; ///< Оператор копирования.
    ByteRange& operator = (ByteRange &&)      noexcept = default; ///< Оператор присваивания.

    int at (std::size_t position) const noexcept
    {
        if (position >= m_size) {
            return -1;
        }
        return static_cast <int> (*(m_begin + position));
    }

    int back() const noexcept { return at (m_size - 1); }

    IteratorType begin() const noexcept { return m_begin; }

    IteratorType current() const noexcept { return m_current; }

    bool empty() const noexcept { return m_current == m_end; }

    IteratorType end() const noexcept { return m_end; }

    bool eot() const noexcept { return m_current == m_end; }

    int front() const noexcept { return at (0); }

    bool isControl () const noexcept
    {
        const auto c = this->peekByte();
        return (c > 0) && (c < ' ');
    }

    bool isDigit () const noexcept
    {
        const auto c = this->peekByte();
        return std::isdigit (c);
    }

    bool isLetter () const noexcept
    {
        const auto c = this->peekByte();
        return std::isalpha (c);

    }

    bool isWhitespace () const noexcept
    {
        const auto c = this->peekByte();
        return std::isspace (c);
    }

    int lookAhead (std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() + distance;
        return this->at (newPosition);
    }

    int lookBehind (std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() - distance;
        return this->at (newPosition);
    }

    ByteRange& moveTo (std::size_t newPosition) noexcept
    {
        const auto size = this->size();
        if (newPosition > size) {
            newPosition = size;
        }
        m_current = m_begin + newPosition;
    }

    int peekByte() const noexcept
    {
        return (m_current == m_end) ? -1 : static_cast <int> (*m_current);
    }

    std::size_t position() const noexcept { return static_cast<std::size_t> (m_current - m_begin); }

    int readByte () noexcept
    {
        if (m_current == m_end) {
            return -1;
        }
        int result = *m_current;
        ++m_current;
        return result;
    }

    ByteRange<IteratorType> readInteger() noexcept
    {
        ByteRange<IteratorType> result { m_current, m_current };
        while (this->isDigit()) {
            ++m_current;
        }
        result.m_end = m_current;

        return result;
    }

    ByteRange<IteratorType> readLine() noexcept
    {
        ByteRange<IteratorType> result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = *m_current;
            if (c == '\r' || c == '\n') {
                break;
            }
            ++m_current;
        }
        result.m_end = m_current;

        if (this->peekByte() == '\r') {
            this->readByte();
        }
        if (this->peekByte() == '\n') {
            this->readByte();
        }

        return result;
    }

    ByteRange<IteratorType> readString (std::size_t length) noexcept
    {
        ByteRange<IteratorType> result { m_current, m_current };
        while (length != 0) {
            if (m_current == m_end) {
                break;
            }
            --length;
        }
        result.m_end = m_current;

        return result;
    }

    ByteRange<IteratorType> remaining() const noexcept
    {
        return ByteRange<IteratorType> { m_current, m_end };
    }

    std::size_t size() const noexcept { return m_size; }

    bool writeByte (irbis::Byte byte) noexcept
    {
        if (m_end == m_current) {
            return false;
        }
        *m_current = byte;
        ++m_current;
        return true;
    }

};

using JoinedNavigator = ByteRange <irbis::JoinedData <irbis::Byte, irbis::Byte*>::iterator>;

int main()
{
    irbis::Byte data1[] { 'W', 'o', 'r', 'l', 'd' };
    irbis::Byte data2[] { 'H', 'e', 'l', 'l', 'o' };
    irbis::JoinedData<irbis::Byte, irbis::Byte*> joined;
    joined.append (data2);
    joined.append (data1);
    JoinedNavigator navigator { std::begin (joined), std::end (joined) };

    std::cout << std::boolalpha;

    std::cout << (char) navigator.front() << std::endl;
    std::cout << (char) navigator.peekByte() << std::endl;
    std::cout << (char) navigator.back() << std::endl;
    std::cout << (char) navigator.lookAhead() << std::endl;
    std::cout << navigator.isControl() << std::endl;
    std::cout << navigator.isLetter() << std::endl;
    std::cout << navigator.isDigit() << std::endl;
    std::cout << navigator.isWhitespace() << std::endl;

    while (true) {
        auto c = navigator.readByte();
        if (c < 0) {
            break;
        }
        std::cout << (char) c;
    }
    std::cout << std::endl;

    auto remaining = navigator.remaining();
    std::cout << remaining.empty() << std::endl;

    return 0;
}
