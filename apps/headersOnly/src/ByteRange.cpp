// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"

#include <cctype>
#include <limits>
#include <iterator>

template <class CharType>
struct ByteTraits final
{
    static int eot() { return -1; }

    template <class IteratorType>
    static IteratorType find (IteratorType begin, IteratorType end, CharType c)
    {
        return std::find (begin, end, c);
    }

    static int  compare (CharType left, CharType right) { return static_cast <int> (left - right); }
    static bool equals  (CharType left, CharType right) { return left == right; }
    static bool less    (CharType left, CharType right) { return left < right; }
    static std::size_t length (const CharType *text)    { return ::strlen (text); }

    static bool isAlpha      (CharType c) { return static_cast <bool> (::isalpha (static_cast <int> (c))); }
    static bool isControl    (CharType c) { return c < ' '; }
    static bool isDigit      (CharType c) { return static_cast <bool> (::isdigit (static_cast <int> (c))); }
    static bool isWhitespace (CharType c) { return static_cast <bool> (::isspace (static_cast <int> (c))); }

    static CharType toLower (CharType c) { return static_cast <CharType> (::tolower (static_cast <int> (c))); }
    static CharType toUpper (CharType c) { return static_cast <CharType> (::toupper (static_cast <int> (c))); }
};

/// \brief Произвольный диапазон байтов, не обязательно непрерывный.
/// \tparam IteratorType Тип итератора.
/// \tparam CharTraits Специфичные для символов операции.
/// \details Ограничения на диапазон байтов определяются итератором.
template <class IteratorType, class CharTraits = ByteTraits <irbis::Byte>>
struct ByteRange final
{
    IteratorType m_begin;   ///< Начало данных.
    IteratorType m_current; ///< Текущая позиция.
    IteratorType m_end;     ///< За концом данных.

    constexpr ByteRange (IteratorType begin, IteratorType end) noexcept
        : m_begin { begin }, m_current { begin }, m_end { end }{}

    ByteRange             (const ByteRange &) noexcept = default; ///< Конструктор копирования.
    ByteRange             (ByteRange &&)      noexcept = default; ///< Конструктор перемещения.
    ByteRange& operator = (const ByteRange &) noexcept = default; ///< Оператор копирования.
    ByteRange& operator = (ByteRange &&)      noexcept = default; ///< Оператор присваивания.

    int at (std::size_t index) const noexcept
    {
        if (index >= this->size()) {
            return -1;
        }
        return static_cast <int> (*(m_begin + index));
    }

    int back() const noexcept { return at (this->size() - 1); }

    IteratorType begin() const noexcept { return m_begin; }

    IteratorType current() const noexcept { return m_current; }

    bool empty() const noexcept { return m_begin == m_end; }

    IteratorType end() const noexcept { return m_end; }

    bool eot() const noexcept { return m_current == m_end; }

    IteratorType find (irbis::Byte value) const noexcept
    {
        for (auto it = m_current; it != m_end; ++it) {
            if (*it == value) {
                return it;
            }
        }
        return m_end;
    }

    IRBIS_NODISCARD
    int front() const noexcept { return at (0); }

    IRBIS_NODISCARD
    bool isAlpha() const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isAlpha (static_cast <irbis::Byte> (c));
    }

    IRBIS_NODISCARD
    bool isControl() const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isControl (static_cast <irbis::Byte> (c));
    }

    IRBIS_NODISCARD
    bool isDigit() const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isDigit (static_cast <irbis::Byte> (c));
    }

    IRBIS_NODISCARD
    bool isPunctuation() const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isPunctuation (static_cast <irbis::Byte> (c));
    }

    IRBIS_NODISCARD
    bool isWhitespace() const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isWhitespace (static_cast <irbis::Byte> (c));
    }

    IRBIS_NODISCARD
    int lookAhead (const std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() + distance;
        return this->at (newPosition);
    }

    IRBIS_NODISCARD
    int lookBehind (const std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() - distance;
        return this->at (newPosition);
    }

    IRBIS_MAYBE_UNUSED
    ByteRange& moveTo (const IteratorType newPosition) noexcept
    {
        m_current = newPosition;
        return *this;
    }

    IRBIS_MAYBE_UNUSED
    ByteRange& moveTo (std::size_t newPosition) noexcept
    {
        const auto size = this->size();
        if (newPosition > size) {
            newPosition = size;
        }
        m_current = m_begin + newPosition;
        return *this;
    }

    IRBIS_NODISCARD
    int peekByte() const noexcept
    {
        return (m_current == m_end) ? -1 : static_cast <int> (*m_current);
    }

    IRBIS_NODISCARD
    std::size_t position() const noexcept { return static_cast<std::size_t> (m_current - m_begin); }

    IRBIS_MAYBE_UNUSED
    int readByte () noexcept
    {
        if (m_current == m_end) {
            return -1;
        }
        int result = *m_current;
        ++m_current;
        return result;
    }

    ByteRange readInteger() noexcept
    {
        ByteRange<IteratorType> result { m_current, m_current };
        while (this->isDigit()) {
            ++m_current;
        }
        result.m_end = m_current;

        return result;
    }

    ByteRange readLine() noexcept
    {
        ByteRange<IteratorType> result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = *m_current;
            if (CharTraits::equals (c, '\r') || CharTraits::equals (c, '\n')) {
                break;
            }
            ++m_current;
        }
        result.m_end = m_current;

        if (CharTraits::equals (this->peekByte(), '\r')) {
            this->readByte();
        }
        if (CharTraits::equals (this->peekByte(), '\n')) {
            this->readByte();
        }

        return result;
    }

    ByteRange readString (std::size_t length) noexcept
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

    ByteRange recent (std::size_t length) const noexcept
    {
        const auto position = this->position();
        std::size_t start = position >= length ? position - length : 0;
        return { m_begin + start, m_current };
    }

    ByteRange remaining() const noexcept
    {
        return ByteRange<IteratorType> { m_current, m_end };
    }

    std::size_t size() const noexcept { return static_cast <std::size_t> (m_end - m_begin); }

    ByteRange skipLine () noexcept
    {
        while (m_current != m_end) {
            const auto c = *m_current;
            ++m_current;
            if (CharTraits::equals (c, '\n')) {
                break;
            }
            if (CharTraits::equals (c, '\r')) {
                if (m_current != m_end) {
                    const auto c2 = *m_current;
                    if (CharTraits::equals (c2, '\n')) {
                        ++m_current;
                    }
                }
                break;
            }
        }
        return *this;
    }

    ByteRange skipPunctuation () noexcept
    {
        while (m_current != m_end) {
            if (!this->isPunctuation()) {
                break;
            }
            ++m_current;
        }
        return *this;
    }

    ByteRange skipWhitespace () noexcept
    {
        while (m_current != m_end) {
            if (!this->isWhitespace()) {
                break;
            }
            ++m_current;
        }
        return *this;
    }

    ByteRange slice (std::size_t offset, std::size_t length) const noexcept
    {
        const auto size = this->size();
        if (offset >= size) {
            return { m_end, m_end };
        }
        if (offset + length > size) {
            length = size - offset;
        }
        const auto start = m_begin + offset;
        return { start, start + length };
    }

    irbis::Byte value() const noexcept { return *m_current; }

    bool writeByte (irbis::Byte byte) noexcept
    {
        if (m_current == m_end) {
            return false;
        }
        *m_current = byte;
        ++m_current;
        return true;
    }

};

using JoinedNavigator = ByteRange <irbis::JoinedData <irbis::Byte, irbis::Byte*>::iterator>;

void testByteRange()
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
    std::cout << navigator.isAlpha() << std::endl;
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
}
