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

/// \brief Произвольный диапазон текста, не обязательно непрерывный.
/// \tparam IteratorType Тип итератора.
/// \tparam CharType Тип символов.
template <class IteratorType, class CharType>
struct TextRange
{
    IteratorType m_begin;    ///< Начало данных.
    IteratorType m_current;  ///< Текущая позиция.
    IteratorType m_end;      ///< За концом данных.
    std::size_t  m_size;     ///< Размер в байтах.
    std::size_t  m_column;   ///< Номер колонки, нумерация с 1.
    std::size_t  m_line;     ///< Номер строки, нумерация с 1.
    std::size_t  m_position; ///< Смещение от начала текста, считается в символах, нумерация с 0.

    TextRange (IteratorType begin, IteratorType end) noexcept
        : m_begin { begin }, m_current { begin }, m_end { end },
        m_size { static_cast <std::size_t> (m_end - m_begin) },
        m_column { 1 }, m_line { 1 }, m_position { 0 } {}

    TextRange             (const TextRange &) noexcept = default; ///< Конструктор копирования.
    TextRange             (TextRange &&)      noexcept = default; ///< Конструктор перемещения.
    TextRange& operator = (const TextRange &) noexcept = default; ///< Оператор копирования.
    TextRange& operator = (TextRange &&)      noexcept = default; ///< Оператор присваивания.

    CharType at (std::size_t index) const noexcept
    {
        if (index >= m_size) {
            return 0;
        }
        return static_cast <CharType> (* (m_begin + index));
    }

    CharType back() const noexcept { return at (m_size - 1); }

    IteratorType begin() const noexcept { return m_begin; }

    std::size_t column() const noexcept { return m_column; }

    IteratorType current() const noexcept { return m_current; }

    bool empty() const noexcept { return m_begin == m_end; }

    IteratorType end() const noexcept { return m_end; }

    bool eot() const noexcept { return m_current == m_end; }

    IteratorType find (CharType value) const noexcept
    {
        for (auto it = m_current; it != m_end; ++it) {
            if (*it == value) {
                return it;
            }
        }
        return m_end;
    }

    CharType front() const noexcept { return at (0); }

    bool isControl () const noexcept
    {
        const auto c = this->peek();
        return (c > 0) && (c < ' ');
    }

    bool isDigit () const noexcept
    {
        const auto c = this->peek();
        return std::isdigit (c);
    }

    bool isLetter () const noexcept
    {
        const auto c = this->peek();
        return std::isalpha (c);
    }

    bool isWhitespace () const noexcept
    {
        const auto c = this->peek();
        return std::isspace (c);
    }

    std::size_t line() const noexcept { return m_line; }

    CharType lookAhead (std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() + distance;
        return this->at (newPosition);
    }

    CharType lookBehind (std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() - distance;
        return this->at (newPosition);
    }

    TextRange& move (std::ptrdiff_t distance) noexcept
    {
        m_current  += distance;
        m_column   += distance;
        m_position += distance;
        return *this;
    }

    TextRange& moveTo (IteratorType newPosition) noexcept
    {
        m_current = newPosition;
        m_position = m_current - m_begin;
        return *this;
    }

    TextRange& moveTo (std::size_t newPosition) noexcept
    {
        if (newPosition > m_size) {
            newPosition = m_size;
        }
        m_position = newPosition;
        m_current = m_begin + newPosition;
        return *this;
    }

    CharType peek() const noexcept
    {
        return (m_current == m_end) ? 0 : static_cast<CharType> (*m_current);
    }

    std::size_t position() const noexcept { return m_position; }

    CharType read() noexcept
    {
        if (m_current == m_end) {
            return 0;
        }

        const auto result = static_cast <CharType> (*m_current);
        ++m_current;
        ++m_position;
        if (result == '\n') {
            ++m_line;
            m_column = 1;
        }
        else {
            ++m_column;
        }

        return result;
    }

    TextRange readInteger() noexcept
    {
        TextRange result { m_current, m_current };
        while (this->isDigit()) {
            ++m_current;
        }
        result.m_end = m_current;

        return result;
    }

    TextRange readLine() noexcept
    {
        TextRange result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = static_cast <CharType> (*m_current);
            if (c == '\r' || c == '\n') {
                break;
            }
            this->read();
        }
        result.m_end = m_current;

        if (this->peek() == '\r') {
            this->read();
        }
        if (this->peek() == '\n') {
            this->read();
        }

        return result;
    }

    TextRange readString (std::size_t length) noexcept
    {
        TextRange result { m_current, m_current };
        while (length != 0) {
            if (m_current == m_end) {
                break;
            }
            --length;
        }
        result.m_end = m_current;

        return result;
    }

    TextRange recent (std::size_t length) const noexcept
    {
        const auto position = m_position;
        std::size_t start = position >= length ? position - length : 0;
        return { m_begin + start, m_current };
    }

    std::size_t size() const noexcept { return m_size; }

    TextRange& skipLine () noexcept
    {
        while (!this->eot()) {
            const auto c = this->read();
            if (c == '\n') {
                break;
            }
            if (c == '\r') {
                if (!this->eot()) {
                    const auto c2 = this->peek();
                    if (c2 == '\n') {
                        this->read();
                    }
                }
                break;
            }
        }
        return *this;
    }

    TextRange& skipPunctuation() noexcept
    {
        while (!this->eot()) {
            if (!this->isPunctuation()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    TextRange& skipWhitespace() noexcept
    {
        while (!this->eot()) {
            if (!this->isWhitespace()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    TextRange slice (std::size_t offset, std::size_t length) const noexcept
    {
        if (offset >= m_size) {
            return { m_end, m_end };
        }
        if (offset + length > size) {
            length = size - offset;
        }
        const auto start = m_begin + offset;
        return { start, start + length };
    }

    bool write (CharType chr) noexcept
    {
        if (m_current == m_end) {
            return false;
        }
        *m_current = chr;
        ++m_current;
        ++m_position;
        if (chr == '\n') {
            ++m_line;
            m_column = 1;
        }
        else {
            ++m_column;
        }
        return true;
    }
};

using CharRange = TextRange <std::string::iterator,  char>;
using WideRange = TextRange <std::wstring::iterator, wchar_t>;

CharRange makeRange (const std::string &text)
{
    auto &noconst = const_cast<std::string&> (text);
    return { std::begin (noconst), std::end (noconst) };
}

WideRange makeRange (const std::wstring &text)
{
    auto &noconst = const_cast<std::wstring&> (text);
    return { std::begin (noconst), std::end (noconst) };
}

/// \brief Произвольный диапазон байтов, не обязательно непрерывный.
/// \tparam IteratorType Тип итератора.
/// \details Ограничения на диапазон байтов определяются итератором.
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

    int at (std::size_t index) const noexcept
    {
        if (index >= m_size) {
            return -1;
        }
        return static_cast <int> (*(m_begin + index));
    }

    int back() const noexcept { return at (m_size - 1); }

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

    ByteRange<IteratorType>& moveTo (IteratorType newPosition) noexcept
    {
        m_current = newPosition;
        return *this;
    }

    ByteRange<IteratorType>& moveTo (std::size_t newPosition) noexcept
    {
        const auto size = this->size();
        if (newPosition > size) {
            newPosition = size;
        }
        m_current = m_begin + newPosition;
        return *this;
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

    ByteRange<IteratorType> recent (std::size_t length) const noexcept
    {
        const auto position = this->position();
        std::size_t start = position >= length ? position - length : 0;
        return { m_begin + start, m_current };
    }

    ByteRange<IteratorType> remaining() const noexcept
    {
        return ByteRange<IteratorType> { m_current, m_end };
    }

    std::size_t size() const noexcept { return m_size; }

    ByteRange<IteratorType> skipLine () noexcept
    {
        while (m_current != m_end) {
            const auto c = *m_current;
            ++m_current;
            if (c == '\n') {
                break;
            }
            if (c == '\r') {
                if (m_current != m_end) {
                    const auto c2 = *m_current;
                    if (c2 == '\n') {
                        ++m_current;
                    }
                }
                break;
            }
        }
        return *this;
    }

    ByteRange<IteratorType> skipPunctuation () noexcept
    {
        while (m_current != m_end) {
            if (!this->isPunctuation()) {
                break;
            }
            ++m_current;
        }
        return *this;
    }

    ByteRange<IteratorType> skipWhitespace () noexcept
    {
        while (m_current != m_end) {
            if (!this->isWhitespace()) {
                break;
            }
            ++m_current;
        }
        return *this;
    }

    ByteRange<IteratorType> slice (std::size_t offset, std::size_t length) const noexcept
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

    std::cout << "=============================================" << std::endl;

    std::string hello ("Hello\nworld");
    auto range = makeRange (hello);
    std::cout << range.front() << std::endl;
    std::cout << range.peek() << std::endl;
    std::cout << range.back() << std::endl;
    std::cout << range.lookAhead() << std::endl;
    std::cout << range.isControl() << std::endl;
    std::cout << range.isLetter() << std::endl;
    std::cout << range.isDigit() << std::endl;
    std::cout << range.isWhitespace() << std::endl;
    while (true) {
        const auto c = range.read();
        if (!c) {
            break;
        }
        std::cout << c;
    }
    std::cout << std::endl;

    return 0;
}
