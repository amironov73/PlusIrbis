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

    /// \brief Конструктор.
    /// \param begin Начало диапазона.
    /// \param end Конец диапазона.
    constexpr ByteRange (IteratorType begin, IteratorType end) noexcept
        : m_begin { begin }, m_current { begin }, m_end { end } {}

    ByteRange             (const ByteRange &) noexcept = default; ///< Конструктор копирования.
    ByteRange             (ByteRange &&)      noexcept = default; ///< Конструктор перемещения.
    ByteRange& operator = (const ByteRange &) noexcept = default; ///< Оператор копирования.
    ByteRange& operator = (ByteRange &&)      noexcept = default; ///< Оператор присваивания.

    /// \brief Число, возвращаемое в качестве признака конца диапазона.
    /// \return "Невозможный байт".
    static constexpr int EOT() { return CharTraits::eot(); }

    /// \brief Подглядываем байт в указанной позиции.
    /// \param index Позиция байта (отсчет от начала диапазона, от 0).
    /// \return Подсмотренный байт, либо EOT.
    IRBIS_NODISCARD
    int at (std::size_t index) const noexcept
    {
        if (index >= this->size()) {
            return -1;
        }
        return static_cast <int> (*(m_begin + index));
    }

    /// \brief Последний байт в диапазоне.
    /// \return Байт либо EOT (для пустого диапазона).
    IRBIS_NODISCARD
    int back() const noexcept { return at (this->size() - 1); }

    /// \brief Итератор, указывающий на начало диапазона.
    /// \return Итератор.
    /// \warning Для пустого диапазона UB!
    IRBIS_NODISCARD
    IteratorType begin() const noexcept { return m_begin; }

    /// \brief Сравнение двух байтов.
    /// \param left Первый байт.
    /// \param right Второй байт.
    /// \return Результат сравнения.
    IRBIS_NODISCARD
    static int compare (const irbis::Byte left, const irbis::Byte right) noexcept
    {
        return CharTraits::compare (left, right);
    }

    /// \brief Сравнивает поэлементно два диапазона (оба начиная с текущей позиции).
    /// \param that Диапазон для сравнения.
    /// \return 0, если диапазоны поэлементно равны, меньше 0, если this меньше, больше 0, если this больше.
    IRBIS_NODISCARD
    int compareTo (const ByteRange &that) const noexcept
    {
        auto thisPtr = m_current;
        auto thatPtr = that.m_current;
        const auto thatEnd = that.m_end;
        while (true) {
            if (thisPtr == m_end) {
                if (thatPtr == thatEnd) {
                    break;
                }
                return -1;
            }
            if (thatPtr == thatEnd) {
                return 1;
            }
            const auto result = compare (*thisPtr, *thatPtr);
            if (result != 0) {
                return result;
            }
            ++thisPtr;
            ++thatPtr;
        }
        return 0;
    }

    /// \brief Сравнивает поэлементно два диапазона (this начиная с текущей позиции).
    /// \tparam Iter Тип итератора.
    /// \param thatBegin Начало диапазона для сравнения.
    /// \param thatEnd Конец диапазона для сравнения.
    /// \return 0, если диапазоны поэлементно равны, меньше 0, если this меньше, больше 0, если this больше.
    template <class Iter>
    IRBIS_NODISCARD
    int compareTo (const Iter thatBegin, const Iter thatEnd) const noexcept
    {
        auto thisPtr = m_current;
        auto thatPtr = thatBegin;
        while (true) {
            if (thisPtr == m_end) {
                if (thatPtr == thatEnd) {
                    return 0;
                }
                return -1;
            }
            if (thatPtr == thatEnd) {
                return 1;
            }
            const auto result = compare (*thisPtr, *thatPtr);
            if (result != 0) {
                return result;
            }
            ++thisPtr;
            ++thatPtr;
        }
        return 0;
    }

    /// \brief Сравнивает поэлементно диапазон (начиная с текущей позиции) с указанным массивом байт.
    /// \param that Указатель на массив байт.
    /// \param length Длина массива
    /// \return 0, если равны, меньше 0, если this меньше, больше 0, если this больше.
    IRBIS_NODISCARD
    int compareTo (const irbis::Byte *that, std::size_t length) const noexcept
    {
        return this->compareTo (that, that + length);
    }

    /// \brief Содержит ли непрочитанная часть диапазона указанные байты?
    /// \tparam Тип байтов.
    /// \param value Искомые байты.
    /// \return true, если содержит любой из искомых байтов.
    template <class ... Args>
    IRBIS_NODISCARD
    bool contains (const Args ... args) const noexcept
    {
        const auto list = { args... };
        for (const auto c : list) {
            if (CharTraits::find (m_current, m_end, c) != m_end) {
                return true;
            }
        }
        return false;
    }

    /// \brief Итератор, указывающий на текущий символ.
    /// \return Итератор.
    /// \warning Для пустого текста UB!
    IRBIS_NODISCARD
    IteratorType current() const noexcept { return m_current; }

    /// \brief Диапазон пуст?
    /// \return true если пуст.
    IRBIS_NODISCARD
    bool empty() const noexcept { return m_begin == m_end; }

    /// \brief Итератор, указывающий за концом текста.
    /// \return Итератор.
    /// \warning Для пустого текста UB!
    IRBIS_NODISCARD
    IteratorType end() const noexcept { return m_end; }

    /// \brief Достигнут ли конец диапазона?
    /// \return true если достигнут.
    IRBIS_NODISCARD
    bool eot() const noexcept { return m_current == m_end; }

    /// \brief Совпадает ли конец данного диапазона с текущей позицией другого диапазона? Движения по данным не происходит.
    /// \param prefix Диапазон для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool endsWith (const ByteRange &suffix) const noexcept
    {
        const auto suffixSize = suffix.remainingSize();
        if ((suffixSize > this->remainingSize()) || (suffixSize == 0)) {
            return false;
        }

        auto thisPtr = m_end - suffixSize;
        auto suffixPtr = suffix.m_current;
        const auto suffixEnd = suffix.m_end;

        while (suffixPtr != suffixEnd) {
            if (!equals (*thisPtr, *suffixPtr)) {
                return false;
            }
            ++thisPtr;
            ++suffixPtr;
        }

        return true;
    }

    /// \brief Совпадает ли конец данного диапазона с указанным диапазоном? Движения по данным не происходитт.
    /// \tparam Iter Тип итератора.
    /// \param suffixBegin Начало другого диапазона.
    /// \param suffixEnd Конец другого диапазона.
    /// \return true если совпадает.
    template <class Iter>
    IRBIS_NODISCARD
    bool endsWith (const Iter suffixBegin, const Iter suffixEnd) const noexcept
    {
        const auto suffixSize = static_cast <std::size_t> (suffixEnd - suffixBegin);
        if ((suffixSize > this->remainingSize()) || (suffixSize == 0)) {
            return false;
        }

        auto thisPtr = m_end - suffixSize;
        auto suffixPtr = suffixBegin;
        while (suffixPtr != suffixEnd) {
            if (!equals (*thisPtr, *suffixPtr)) {
                return false;
            }
            ++thisPtr;
            ++suffixPtr;
        }

        return true;
    }

    /// \brief Проверка двух байтов на равенство.
    /// \param left Первый байт.
    /// \param right Второй байт.
    /// \return true, если байты считаются равными.
    IRBIS_NODISCARD
    static bool equals (const irbis::Byte left, const irbis::Byte right) noexcept
    {
        return CharTraits::equals (left, right);
    }

    /// \brief Заполнение диапазона указанным символом (начиная с текущей позиции).
    /// \param value Символ для заполнения.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    ByteRange& fill (const irbis::Byte value) noexcept
    {
        for (auto ptr = m_current; ptr != m_end; ++ptr) {
            *ptr = value;
        }
        return *this;
    }

    /// \brief Поиск указанного байта (начиная с текущей позиции).
    /// \param value Байт для поиска.
    /// \return Итератор на найденный байт либо итератор за концом диапазона.
    IRBIS_NODISCARD
    IteratorType find (irbis::Byte value) const noexcept
    {
        for (auto it = m_current; it != m_end; ++it) {
            if (equals (*it, value)) {
                return it;
            }
        }
        return m_end;
    }

    /// \brief Поиск первого вхождения любого из указанных байтов (начиная с текущей позиции).
    /// \tparam Args Тип байтов.
    /// \param args Искомые байты.
    /// \return Итератор на найденный байт либо итератор за концом диапазона.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType find (const Args ... args) const noexcept
    {
        const auto list = { args... };
        for (auto ptr = m_current; ptr != m_end; ++ptr) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                return ptr;
            }
        }
        return m_end;
    }

    /// \brief Поиск указанного байта (начиная с текущей позиции).
    /// \param value Байт для поиска.
    /// \return Итератор на найденный байт либо итератор за концом диапазона.
    IRBIS_NODISCARD
    IteratorType findLast (irbis::Byte value) const noexcept
    {
        if (m_current != m_end) {
            for (auto it = m_end - 1; it != m_current; --it) {
                if (equals (*it, value)) {
                    return it;
                }
            }
            if (equals (*m_current, value)) {
                return m_current;
            }
        }
        return m_end;
    }

    /// \brief Поиск последнего вхождения любого из указанных байтов (начиная с текущей позиции).
    /// \tparam Args Тип байтов.
    /// \param args Искомые байты.
    /// \return Итератор на найденный байт либо итератор за концом диапазона.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType findLast (const Args ... args) const noexcept
    {
        const auto list = { args... };
        if (m_current != m_end) {
            for (auto ptr = m_end - 1; ptr != m_current; --ptr) {
                const auto c = static_cast <irbis::Byte> (*ptr);
                if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                    return ptr;
                }
            }
            if (CharTraits::find (std::begin (list), std::end (list), this->value()) != std::end (list)) {
                return m_current;
            }
        }
        return m_end;
    }

    /// \brief Первый байт в диапазоне.
    /// \return Байт либо EOT (для пустого диапазона),
    IRBIS_NODISCARD
    int front() const noexcept { return at (0); }

    /// \brief Байт в текущей позиции - буква?
    /// \return true, если буква.
    IRBIS_NODISCARD
    bool isAlpha() const noexcept
    {
        const auto c = static_cast <irbis::Byte> (this->peekByte());
        return (c >= 0) && CharTraits::isAlpha (c);
    }

    /// \brief Байт в текущей позиции - управляющий?
    /// \return true, если управляющий.
    IRBIS_NODISCARD
    bool isControl() const noexcept
    {
        const auto c = static_cast <irbis::Byte> (this->peekByte());
        return (c >= 0) && CharTraits::isControl (c);
    }

    /// \brief Байт в текущей позиции - цифра?
    /// \return true, если цифра.
    IRBIS_NODISCARD
    bool isDigit() const noexcept
    {
        const auto c = static_cast <irbis::Byte> (this->peekByte());
        return (c >= 0) && CharTraits::isDigit (c);
    }

    /// \brief Байт в текущей позиции - пунктуация?
    /// \return true, если пунктуация.
    IRBIS_NODISCARD
    bool isPunctuation() const noexcept
    {
        const auto c = static_cast <irbis::Byte> (this->peekByte());
        return (c >= 0) && CharTraits::isPunctuation (c);
    }

    /// \brief Байт в текущей позиции - пробельный?
    /// \return true, если пробельный.
    IRBIS_NODISCARD
    bool isWhitespace() const noexcept
    {
        const auto c = static_cast <irbis::Byte> (this->peekByte());
        return (c >= 0) && CharTraits::isWhitespace (c);
    }

    /// \brief Заглядывание вперед на указанное количество байтов.
    /// \return Подсмотренный байт либо EOT.
    IRBIS_NODISCARD
    int lookAhead (const std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() + distance;
        return this->at (newPosition);
    }

    /// \brief Заглядывание назад на указанное количество байтов.
    /// \return Подсмотренный байт либо EOT.
    IRBIS_NODISCARD
    int lookBehind (const std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() - distance;
        return this->at (newPosition);
    }

    /// \brief Сдвиг по тексту на указанное число символов.
    /// \param distance Величина сдвига (отрицательная - назад).
    /// \return this.
    /// \warning Портит column/line.
    IRBIS_MAYBE_UNUSED
    ByteRange& move (const std::ptrdiff_t distance) noexcept
    {
        m_current  += distance;
        return *this;
    }

    /// \brief Перемещение по диапазону к указанной позиции.
    /// \param newPosition Новая позиция (отсчет от 0 от начала диапазона).
    /// \return this.
    /// \warning Портит column/line.
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

    /// \brief Быстрый и грязный разбор фрагмента (начиная с текущей позиции) как целого числа.
    /// \return Результат разбора.
    /// \warning Знак числа не ожидается. Мусор на входе -- мусор на выходе.
    int32_t parseInt32() const noexcept
    {
        int32_t result = 0;
        auto ptr = m_current;
        while (ptr != m_end) {
            result = result * 10 + (*ptr - '0');
            ++ptr;
        }
        return result;
    }

    /// \brief Быстрый и грязный разбор фрагмента (начиная с текущей позиции) как целого числа.
    /// \return Результат разбора.
    /// \warning Знак числа не ожидается. Мусор на входе -- мусор на выходе.
    int64_t parseInt64() const noexcept
    {
        int64_t result = 0;
        auto ptr = m_current;
        while (ptr != m_end) {
            result = result * 10 + (*ptr - '0');
            ++ptr;
        }
        return result;
    }

    /// \brief Подсматриваем текущий байт.
    /// \return Подсмотренный байт либо EOT.
    IRBIS_NODISCARD
    int peekByte() const noexcept
    {
        return (m_current == m_end) ? -1 : static_cast <int> (*m_current);
    }

    /// \brief Подсматриваем следующий байт, игнорируя указанные.
    /// \tparam Args Тип байтов.
    /// \param args Байты, которые должны быть проигнорированы.
    /// \return Итератор на подсмотренный Байт либо за концом диапазона.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType peekExcept (const Args ... args) const noexcept
    {
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), c) == std::end (list)) {
                return ptr;
            }
            ++ptr;
        }
        return m_end;
    }

    /// \brief Подсматриваем строку (вплоть до перевода строки либо конца текста).
    /// \return Подсмотренная строка (возможно, пустая).
    IRBIS_NODISCARD
    ByteRange peekLine() const noexcept
    {
        ByteRange result { m_current, m_current };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            if (equals (c, '\r') || equals (c, '\n')) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подсматриваем следующий байт, игнорируя все байты, отличные от указанных.
    /// \tparam Args Тип байтов.
    /// \param args Байты, которые должны быть обнаружены.
    /// \return Итератор на подсмотренный байт либо за концом диапазона.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType peekOnly (Args ... args) const noexcept
    {
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                return ptr;
            }
            ++ptr;
        }
        return m_end;
    }

    /// \brief Подглядывание строки вплоть до указанной длины.
    /// \param length Максимальная длина строки.
    /// \return Подсмотренная строка (возможно, пустая).
    /// \details Символы перевода строки также считываются и помещаются в результат.
    IRBIS_NODISCARD
    ByteRange peekString (std::size_t length) const noexcept
    {
        ByteRange result { m_current, m_current };
        auto ptr = m_current;
        while (length != 0) {
            if (ptr == m_end) {
                break;
            }
            --length;
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подглядывание вплоть до любого из указанных стоп-байтов (включая его).
    /// \tparam Тип байтов.
    /// \param args Стоп-байты.
    /// \return Подсмотренная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_NODISCARD
    ByteRange peekTo (const Args ... args) const noexcept
    {
        const auto list = { args... };
        ByteRange result { m_current, m_current };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            ++ptr;
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                break;
            }
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подглядывание вплоть до любого из указанных стоп-байтов (не включая его).
    /// \tparam Тип байтов.
    /// \param args Стоп-байты.
    /// \return Подсмотренная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_NODISCARD
    ByteRange peekUntil (const Args ... args) const noexcept
    {
        const auto list = { args... };
        ByteRange result { m_current, m_current };
        IteratorType ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подглядывание, пока встречаются перечисленные байты.
    /// \tparam Args Тип байтов.
    /// \param args Нужные байты.
    /// \return Подсмотренная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_NODISCARD
    ByteRange peekWhile (const Args ... args) const noexcept
    {
        ByteRange result { m_current, m_current };
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <irbis::Byte> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), result) == std::end (list)) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Текущая позиция в диапазоне (в байтах), отсчет от 0.
    /// \return Текущая позиция.
    IRBIS_NODISCARD
    std::size_t position() const noexcept
    {
        return static_cast<std::size_t> (m_current - m_begin);
    }

    /// \brief Считываем текущий байт. Двигаемся вперёд по диапазону.
    /// \return Прочитанный байт либо EOT.
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

    /// \brief Чтение целого числа без знака. Двигаемся вперед по тексту.
    /// \return Прочитанная строка с числом (возможно, пустая).
    /// \details Длина целого не ограничена.
    IRBIS_MAYBE_UNUSED
    ByteRange readInteger() noexcept
    {
        ByteRange<IteratorType> result { m_current, m_current };
        while (this->isDigit()) {
            ++m_current;
        }
        result.m_end = m_current;

        return result;
    }

    /// \brief Считывание строки. Происходит движение по диапазону.
    /// \return Считанная строка (возможно, пустая).
    /// \details Символы перевода строки считываются, но в результирующую строку не помещаются.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Считывание начиная с открывающего байта до закрывающего (включая их).
    /// \param openByte Открывающий байт.
    /// \param closeByte Закрывающий байт.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    ByteRange readOpenClose (const irbis::Byte openByte, const irbis::Byte closeByte) noexcept
    {
        ByteRange result { m_current, m_current };
        if (equals (this->peek(), openByte)) {
            this->read();
            while (m_current != m_end) {
                if (equals (this->read(), closeByte)) {
                    this->read();
                    result.m_end = m_current;
                    break;
                }
            }
        }
        return result;
    }

    /// \brief Считывание начиная с открывающего байта до закрывающего (включая их).
    /// \param openChar C-строка с возможными открывающими байтами.
    /// \param closeChar C-строка с возможными закрывающими байтами.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    ByteRange readOpenClose (const irbis::Byte *openChars, const irbis::Byte *closeChars) noexcept
    {
        const auto openEnd = openChars + CharTraits::length (openChars);
        const auto closeEnd = closeChars + CharTraits::length (closeChars);
        ByteRange result { m_current, m_current };
        if (CharTraits::find (openChars, openEnd, this->peek()) != openEnd) {
            this->read();
            while (m_current != m_end) {
                if (CharTraits::find (closeChars, m_current, this->peek()) != closeEnd) {
                    this->read();
                    result.m_end = m_current;
                    break;
                }
            }
        }
        return result;
    }

    /// \brief Считываем следующий байт, пропуская указанные (если они появятся). Происходит движение по диапазону.
    /// \tparam Args Тип байтов.
    /// \param args Игнорируемые байты.
    /// \return Прочитанный байт либо EOT.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    int readSkip (Args ... args) noexcept
    {
        const auto list = { args... };
        while (!this->eot()) {
            const auto result = this->read();
            if (CharTraits::find (list.begin(), list.end(), result) == list.end()) {
                return result;
            }
        }
        return EOT();
    }

    /// \brief Чтение строки вплоть до указанной длины. Происходит движение по диапазону.
    /// \param length Максимальная длина строки.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Символы перевода строки также считываются и помещаются в результат.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Считывание байтов вплоть до любого из указанных стоп-байтов (не включая его). Происходит движение по диапазону.
    /// \tparam Args Тип байтов.
    /// \param args Стоп-байты.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Встреченный стоп-байт считывается, но в результирующую строку не включается.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    ByteRange readTo (const Args ... args) noexcept
    {
        ByteRange result { m_current, m_current };
        const auto list = { args... };
        while (m_current != m_end) {
            const auto c = this->read();
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
                break;
            }
        }
        result.m_end = m_current;
        return result;
    }

    /// \brief Считывание вплоть до указанного ограничителя (разделитель не помещается в возвращаемое значение, однако, считывается).
    /// \tparam Iter Тип итератора.
    /// \param begin Начало ограничителя.
    /// \param end Конец ограничителя.
    /// \return Прочитанный фрагмент (возможно, пустой).
    template <class Iter>
    IRBIS_MAYBE_UNUSED
    ByteRange readTo (const Iter begin, const Iter end) noexcept
    {
        auto size = static_cast <std::size_t> (end - begin);
        ByteRange result { m_current, m_current };
        if (size != 0) {
            const auto found = this->search (begin, end);
            if (found != m_end) {
                while (m_current != found) {
                    this->read (); // проматываем до начала ограничителя
                }
                result.m_end = m_current;
                while (size != 0) {
                    this->read(); // проматываем ограничитель
                    --size;
                }
            }
        }
        return result;
    }

    /// \brief Считывание байтов вплоть до любого из указанных стоп-байтов (не включая его). Происходит движение по диапазону.
    /// \tparam Args Тип байтов.
    /// \param args Стоп-байты.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Встреченный стоп-байт не считывается и в результирующую строку не включается.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    ByteRange readUntil (const Args ... args) noexcept
    {
        ByteRange result { m_current, m_current };
        const auto list = { args... };
        while (m_current != m_end) {
            const auto c = this->value();
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
                break;
            }
            ++m_current;
            this->read();
        }
        result.m_end = m_current;
        return result;
    }

    /// \brief Считывание вплоть до указанного ограничителя (разделитель не помещается в возвращаемое значение и не считывается).
    /// \tparam Iter Тип итератора.
    /// \param begin Начало ограничителя.
    /// \param end Конец ограничителя.
    /// \return Прочитанный фрагмент (возможно, пустой).
    template <class Iter>
    IRBIS_MAYBE_UNUSED
    ByteRange readUntil (const Iter begin, const Iter end) noexcept
    {
        auto size = static_cast <std::size_t> (end - begin);
        ByteRange result { m_current, m_current };
        if (size != 0) {
            const auto found = this->search (begin, end);
            if (found != m_end) {
                while (m_current != found) {
                    this->read (); // проматываем до начала ограничителя
                }
                result.m_end = m_current;
            }
        }
        return result;
    }

    /// \brief Считывание байтов до тех пор, пока он состоит из указанных разрешенных байтов. Происходит движение по диапазону.
    /// \tparam Args Тип байтов.
    /// \param args Разрешенные байты.
    /// \return Считанная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    ByteRange readWhile (const Args ... args) noexcept
    {
        ByteRange result { m_current, m_current };
        const auto list = { args... };
        while (m_current != m_end) {
            const auto c = this->value();
            if (CharTraits::find (list.begin(), list.end(), c) == list.end()) {
                break;
            }
            ++m_current;
            this->read();
        }
        result.m_end = m_current;
        return result;
    }

    /// \brief Получение нескольких последних прочитанных байтов. Движения по диапазону не происходит.
    /// \param length Сколько байтов надо.
    /// \return Полученные байты (возможно, пустой фрагмент).
    IRBIS_NODISCARD
    ByteRange recent (std::size_t length) const noexcept
    {
        const auto position = this->position();
        std::size_t start = position >= length ? position - length : 0;
        return { m_begin + start, m_current };
    }

    /// \brief Получение непрочитанного остатка диапазона. Движения по диапазону не происходит.
    /// \return Непрочитанный фрагмент (возможно, пустой).
    IRBIS_NODISCARD
    ByteRange remaining() const noexcept
    {
        return ByteRange<IteratorType> { m_current, m_end };
    }

    /// \brief Получение длины непрочитанного остатка диапазона. Движения по диапазона не происходит.
    /// \return Длина непрочитанного остатка в байтах.
    std::size_t remainingSize() const noexcept
    {
        return static_cast <std::size_t> (m_end - m_current);
    }

    /// \brief Перемотка в начало.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    ByteRange& rewind() noexcept
    {
        m_current = m_begin;
        return *this;
    }

    /// \brief Поиск подстроки, заданной другим диапазоном. Движения по диапазону не происходит.
    /// \param that Искомая подстрока.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType search (const ByteRange &that) const noexcept
    {
        if (!that.empty() && m_current != m_end) {
            const auto thatSize = that.size();
            if (thatSize > this->size()) {
                return m_end;
            }
            const auto thisEnd = m_end - thatSize + 1;
            for (auto thisPtr = m_current; thisPtr != thisEnd; ++thisPtr) {
                auto found = true;
                auto ptr = thisPtr;
                for (auto thatPtr = that.m_current; thatPtr != that.m_end; ++thatPtr) {
                    const auto thisChar = static_cast <irbis::Byte> (*ptr);
                    const auto thatChar = static_cast <irbis::Byte> (*thatPtr);
                    if (!equals (thisChar, thatChar)) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return thisPtr;
                }
            }
        }
        return m_end;
    }

    /// \brief Поиск подстроки, заданной с помощью итераторов. Движения по диапазону не происходит.
    /// \tparam Iter Тип итератора.
    /// \param begin Начало строки.
    /// \param end Конец строки.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    template <class Iter>
    IRBIS_NODISCARD
    IteratorType search (const Iter begin, const Iter end) const noexcept
    {
        if (begin != end && m_current != m_end) {
            const auto thatSize = static_cast <std::size_t> (end - begin);
            if (thatSize > this->size()) {
                return m_end;
            }
            const auto thisEnd = m_end - thatSize + 1;
            for (auto thisPtr = m_current; thisPtr != thisEnd; ++thisPtr) {
                auto found = true;
                auto ptr = thisPtr;
                for (auto thatPtr = begin; thatPtr != end; ++thatPtr, ++ptr) {
                    const auto thisChar = static_cast <irbis::Byte> (*ptr);
                    const auto thatChar = static_cast <irbis::Byte> (*thatPtr);
                    if (!equals (thisChar, thatChar)) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return thisPtr;
                }
            }
        }
        return m_end;
    }

    /// \brief Поиск последнего вхождения подстроки, заданной другим диапазоном. Движения по диапазону не происходит.
    /// \param that Искомая подстрока.
    /// \return Итератор на последнюю найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType searchLast (const ByteRange &that) const noexcept
    {
        if (!that.empty() && m_current != m_end) {
            const auto thatSize = that.size();
            if (thatSize > this->size()) {
                return m_end;
            }
            for (auto thisPtr = m_end - thatSize + 1; thisPtr != m_current; --thisPtr) {
                auto found = true;
                auto ptr = thisPtr;
                for (auto thatPtr = that.m_current; thatPtr != that.m_end; ++thatPtr) {
                    const auto thisChar = static_cast <irbis::Byte> (*ptr);
                    const auto thatChar = static_cast <irbis::Byte> (*thatPtr);
                    if (!equals (thisChar, thatChar)) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return thisPtr;
                }
            }
            if (this->startsWith (that)) {
                return m_current;
            }
        }
        return m_end;
    }

    /// \brief Поиск последнего вхождения подстроки, заданной с помощью итераторов. Движения по диапазону не происходит.
    /// \tparam Iter Тип итератора.
    /// \param begin Начало строки.
    /// \param end Конец строки.
    /// \return Итератор на последнюю найденную позицию либо за концом диапазона.
    template <class Iter>
    IRBIS_NODISCARD
    IteratorType searchLast (const Iter begin, const Iter end) const noexcept
    {
        if (begin != end && m_current != m_end) {
            const auto thatSize = static_cast <std::size_t> (end - begin);
            if (thatSize > this->size()) {
                return m_end;
            }
            for (auto thisPtr = m_end - thatSize + 1; thisPtr != m_current; --thisPtr) {
                auto found = true;
                auto ptr = thisPtr;
                for (auto thatPtr = begin; thatPtr != end; ++thatPtr) {
                    const auto thisChar = static_cast <irbis::Byte> (*ptr);
                    const auto thatChar = static_cast <irbis::Byte> (*thatPtr);
                    if (!equals (thisChar, thatChar)) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return thisPtr;
                }
            }
            if (this->startsWith (begin, end)) {
                return m_current;
            }
        }
        return m_end;
    }

    /// \brief Длина диапазона в байтах.
    /// \return Длина диапазона.
    IRBIS_NODISCARD
    std::size_t size() const noexcept { return static_cast <std::size_t> (m_end - m_begin); }

    /// \brief Пропустить указанное количество байт. Происходит движение по диапазону вперед.
    /// \param nchars Число байтов.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    ByteRange& skip (std::size_t nchars) noexcept
    {
        while (nchars != 0) {
            this->read();
            --nchars;
        }
        return *this;
    }

    /// \brief Пропустить следующий байт, если он совпадает с одним из перечисленных. Происходит движение по диапазону вперед.
    /// \tparam Args Тип байтов.
    /// \param args Разрешенные байты для пропуска.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    ByteRange& skipOne (Args ... args) noexcept
    {
        const auto list = { args... };
        const auto c = this->peek();
        if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск байтов, пока не встретится любой из перечисленных. Происходит движение по диапазону.
    /// \tparam Args Тип байтов.
    /// \param args Искомые байты.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    ByteRange& skipTo (const Args ... args) noexcept
    {
        const auto list = { args ... };
        while (m_current != m_end) {
            const auto c = this->value();
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск перечисленных байтов. Происходит движение по диапазону.
    /// \tparam Args Тип байтов.
    /// \param args Пропускаемые байты.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    ByteRange& skipWhile (const Args ... args) noexcept
    {
        const auto list = { args ... };
        while (m_current != m_end) {
            const auto c = this->value();
            if (CharTraits::find (list.begin(), list.end(), c) == list.end()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск до начала следующей строки. Происходит движение по диапазону.
    /// \return this.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Пропуск пунктуации.
    /// \return this.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Пропуск пробельных байтов.
    /// \return this.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Извлечение поддиапазона.
    /// \param offset Смещение (в байтах), начиная с которого извлекается поддиапазон.
    /// \param length Желаемая длина поддиапазона (в байтах).
    /// \return Полученный поддиапазон (возможно, пустой).
    IRBIS_NODISCARD
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

    /// \brief Разбиение диапазона на фрагменты по указанному разделителю.
    /// \param delimiter Байт-разделитель.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    IRBIS_NODISCARD
    std::vector <ByteRange> split (const irbis::Byte delimiter,
                                   std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        std::vector <ByteRange> result;
        auto start = m_current;
        --nelem;
        while ((nelem != 0) && (start != m_end)) {
            auto current = start;
            while (current != m_end) {
                if (equals (*current, delimiter)) {
                    break;
                }
                ++current;
            }
            if (current != start) {
                result.emplace_back (start, current);
            }
            start = ++current;
            --nelem;
        }

        if (start != m_end) {
            result.emplace_back (start, m_end);
        }

        return result;
    }

    /// \brief Разбиение диапазона на фрагменты по указанным разделителям.
    /// \tparam Args Тип разделителей.
    /// \param delimiter Байты-разделители.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    template <class ... Args>
    IRBIS_NODISCARD
    std::vector <ByteRange> split (const Args ... args,
                                   std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        std::vector <ByteRange> result;
        const auto list = { args... };
        auto start = m_current;
        --nelem;
        while ((nelem != 0) && (start != m_end)) {
            auto current = start;
            while (current != m_end) {
                if (CharTraits::find (*current, list)) {
                    break;
                }
                ++current;
            }
            if (current != start) {
                result.emplace_back (start, current);
            }
            start = ++current;
            --nelem;
        }

        if (start != m_end) {
            result.emplace_back (start, m_end);
        }

        return result;
    }

    /// \brief Разбиение диапазона на фрагменты по указанному разделителю, заданному парой итераторов. Движения по диапазону не происходит.
    /// \tparam Iter Тип итераторов.
    /// \param begin Начало строки-разделителя.
    /// \param end Конец строки-разделителя.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    template <class Iter>
    IRBIS_NODISCARD
    std::vector <ByteRange> split (const Iter begin, const Iter end,
                                   std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        std::vector<ByteRange> result;
        const auto size = static_cast <std::size_t> (end - begin);
        const auto saveCurrent = m_current;
        --nelem;

        while ((nelem != 0) && (m_current != m_end)) {
            const auto found = this->search (begin, end);
            if (found == m_end) {
                result.emplace_back (m_current, m_end);
                m_current = m_end;
                break;
            }
            result.emplace_back (m_current, found);
            m_current = found + size;
            --nelem;
        }

        if (m_current != m_end) {
            result.emplace_back (m_current, m_end);
        }

        m_current = saveCurrent;
        return result;
    }

    /// \brief Совпадает ли текущая позиция с текущей позицией другого диапазона? Движения по диапазону не происходит.
    /// \param prefix Диапазон для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool startsWith (const ByteRange &prefix) const noexcept
    {
        const auto prefixSize = prefix.remainingSize();
        if ((prefixSize > this->remainingSize()) || (prefixSize == 0)) {
            return false;
        }

        auto thisPtr = m_current;
        auto prefixPtr = prefix.m_current;
        const auto prefixEnd = prefix.m_end;

        while (prefixPtr != prefixEnd) {
            if (!CharTraits::equals (*thisPtr, *prefixPtr)) {
                return false;
            }
            ++thisPtr;
            ++prefixPtr;
        }

        return true;
    }

    /// \brief Совпадает ли текущая позиция данного диапазона с указанным фрагментом? Движения по диапазону не происходит.
    /// \param prefix Диапазон для сравнения.
    /// \return true если совпадает.
    template <class Iter>
    IRBIS_NODISCARD
    bool startsWith (const Iter prefixBegin, const Iter prefixEnd) const noexcept
    {
        const auto prefixSize = static_cast <std::size_t> (prefixEnd - prefixBegin);
        if ((prefixSize > this->remainingSize()) || (prefixSize == 0)) {
            return false;
        }

        auto thisPtr = m_current;
        auto prefixPtr = prefixBegin;

        while (prefixPtr != prefixEnd) {
            if (!CharTraits::equals (*thisPtr, *prefixPtr)) {
                return false;
            }
            ++thisPtr;
            ++prefixPtr;
        }

        return true;
    }

    /// \brief Материализация в строку (начиная с текущей позиции). Движения по диапазону не происходит.
    /// \return Строка (возможно, пустая).
    IRBIS_NODISCARD
    std::string toString() const
    {
        std::string result;
        result.reserve (this->size());
        result.insert (std::end (result), m_current, m_end);
        return result;
    }

    /// \brief Материализация в вектор (начиная с текущей позиции). Движения по диапазону не происходит.
    /// \return Вектор байтов (возможно, пустой).
    IRBIS_NODISCARD
    std::vector <irbis::Byte> toVector() const
    {
        std::vector <irbis::Byte> result;
        result.reserve (this->size());
        result.insert (std::end (result), m_current, m_end);
        return result;
    }

    /// \brief Удаление указанных байтов с конца диапазона.
    /// \tparam Args Тип байтов.
    /// \param args Байты, подлежащие удалению.
    /// \return Диапазон с удаленными байтами.
    template <class ... Args>
    IRBIS_NODISCARD
    ByteRange trimEnd (const Args ... args) const noexcept
    {
        const auto list = { args ... };
        ByteRange result { m_current, m_end };
        if (m_current != m_end) {
            for (auto current = m_end - 1; current != m_current; --current) {
                const auto c = static_cast <irbis::Byte> (*current);
                if (CharTraits::find (list.begin (), list.end (), c) == list.end ()) {
                    result.m_end = current + 1;
                    break;
                }
            }
        }
        return result;
    }

    /// \brief Удаление указанных символов с конца диапазона.
    /// \tparam Iter Тип итератор.
    /// \param begin Начало символов, подлежащих удалению.
    /// \param end Конец.
    /// \return Диапазон с удаленными символами.
    template <class Iter>
    IRBIS_NODISCARD
    ByteRange trimEnd (const Iter begin, const Iter end) const noexcept
    {
        ByteRange result { m_current, m_end };
        if (m_current != m_end) {
            for (auto current = m_end - 1; current != m_current; --current) {
                const auto c = static_cast <irbis::Byte> (*current);
                if (CharTraits::find (begin, end, c) == end) {
                    result.m_end = current + 1;
                    break;
                }
            }
        }
        return result;
    }

    /// \brief Удаление указанных байтов с начала диапазона.
    /// \tparam Args Тип байтов.
    /// \param args Байты, подлежащие удалению.
    /// \return Диапазон с удаленными байтами.
    template <class ... Args>
    IRBIS_NODISCARD
    ByteRange trimStart (const Args ... args) const noexcept
    {
        const auto list = { args ... };
        ByteRange result { m_current, m_end };
        for (auto current = m_current; current != m_end; ++current) {
            const auto c = static_cast <irbis::Byte> (*current);
            if (CharTraits::find (list.begin(), list.end(), c) == list.end()) {
                result.m_begin = current;
                result.m_current = current;
                break;
            }
        }
        return result;
    }

    /// \brief Удаление указанных байтов с начала диапазона.
    /// \tparam Iter Тип итератора.
    /// \param begin Начало байтов, подлежащих удалению.
    /// \param end Конец.
    /// \return Диапазон с удаленными байтами.
    template <class Iter>
    IRBIS_NODISCARD
    ByteRange trimStart (const Iter begin, const Iter end) const noexcept
    {
        ByteRange result { m_current, m_end };
        for (auto current = m_current; current != m_end; ++current) {
            const auto c = static_cast <irbis::Byte> (*current);
            if (CharTraits::find (begin, end, c) == end) {
                result.m_begin = current;
                result.m_current = current;
                break;
            }
        }
        return result;
    }

    /// \brief Удаление указанных байтов с начала и с конца диапазона.
    /// \tparam Args Тип байтов.
    /// \param args Байты, подлежащие удалению.
    /// \return Диапазон с удаленными байтами.
    template <class ... Args>
    IRBIS_NODISCARD
    ByteRange trim (const Args ... args) const noexcept
    {
        return this->trimStart (args...).trimEnd (args...);
    }

    /// \brief Удаление указанных байтов с начала и с конца диапазона.
    /// \tparam Iter Тип итератора.
    /// \param begin Начало байтов, подлежащих удалению.
    /// \param end Конец.
    /// \return Диапазон с удаленными байтами.
    template <class Iter>
    IRBIS_NODISCARD
    ByteRange trim (const Iter begin, const Iter end) const noexcept
    {
        return trim->trimStart (begin, end).trimEnd (begin, end);
    }

    /// \brief Байт в текущей позиции. Для пустого диапазона или по достижении конца UB!
    /// \return Текущий байт.
    IRBIS_NODISCARD
    irbis::Byte value() const noexcept { return *m_current; }

    /// \brief Запись байта в текущую позицию. Происходит движение по диапазону.
    /// \param byte Записываемый байт.
    /// \return Признак успешности операции.
    IRBIS_MAYBE_UNUSED
    bool writeByte (irbis::Byte byte) noexcept
    {
        if (m_current == m_end) {
            return false;
        }
        *m_current = byte;
        ++m_current;
        return true;
    }

    /// \brief Оператор сравнения на равенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator == (const ByteRange &left, const ByteRange &right)
    {
        return left.compareTo (right) == 0;
    }

    /// \brief Оператор сравнения на неравенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator != (const ByteRange &left, const ByteRange &right)
    {
        return left.compareTo (right) != 0;
    }

    friend bool operator < (const ByteRange &left, const ByteRange &right)
    {
        return left.compareTo (right) < 0;
    }

    friend bool operator <= (const ByteRange &left, const ByteRange &right)
    {
        return left.compareTo (right) <= 0;
    }

    friend bool operator > (const ByteRange &left, const ByteRange &right)
    {
        return left.compareTo (right) > 0;
    }

    friend bool operator >= (const ByteRange &left, const ByteRange &right)
    {
        return left.compareTo (right) >= 0;
    }
};

using JoinedNavigator = ByteRange <irbis::JoinedData <irbis::Byte, irbis::Byte*>::iterator>;

void testByteRange()
{
    irbis::Byte data1[] { 'W', 'o', 'r', 'l', 'd' };
    irbis::Byte data2[] { 'H', 'e', 'l', 'l', 'o' };
    irbis::JoinedData <irbis::Byte, irbis::Byte*> joined;
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
