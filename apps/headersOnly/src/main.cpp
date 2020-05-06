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

template <class CharType>
struct NarrowTraits final
{
    static CharType eot() { return 0; }

    template <class IteratorType>
    static IteratorType find (IteratorType begin, IteratorType end, CharType c)
    {
        return std::find (begin, end, c);
    }

    static int  compare (CharType left, CharType right) { return static_cast <int> (left - right); }
    static bool equals  (CharType left, CharType right) { return left == right; }
    static bool less    (CharType left, CharType right) { return left < right; }
    static std::size_t length (const CharType *text)    { return ::strlen (text); }

    static bool isAlpha      (CharType c) { return static_cast <bool> (::isalpha (c)); }
    static bool isControl    (CharType c) { return c < ' '; }
    static bool isDigit      (CharType c) { return static_cast <bool> (::isdigit (c)); }
    static bool isWhitespace (CharType c) { return static_cast <bool> (::isspace (c)); }

    static CharType toLower (CharType c) { return ::tolower (c); }
    static CharType toUpper (CharType c) { return ::toupper (c); }
};

template <class CharType>
struct WideTraits final
{
    static CharType eot() { return 0; }

    template <class IteratorType>
    static IteratorType find (IteratorType begin, IteratorType end, CharType c)
    {
        return std::find (begin, end, c);
    }

    static int  compare (CharType left, CharType right) { return static_cast <int> (left - right); }
    static bool equals  (CharType left, CharType right) { return left == right; }
    static bool less    (CharType left, CharType right) { return left < right; }
    static std::size_t length (const CharType *text)    { return ::wcslen (text); }

    static bool isAlpha      (CharType c) { return static_cast <bool> (::iswalpha (c)); }
    static bool isControl    (CharType c) { return c < ' '; }
    static bool isDigit      (CharType c) { return static_cast <bool> (::iswdigit (c)); }
    static bool isWhitespace (CharType c) { return static_cast <bool> (::iswspace (c)); }

    static CharType toLower (CharType c) { return ::towlower(c); }
    static CharType toUpper (CharType c) { return ::towupper (c); }
};

/// \brief Произвольный диапазон текста, не обязательно непрерывный.
/// \tparam IteratorType Тип итератора.
/// \tparam CharType Тип символов.
template <class IteratorType, class CharType, class CharTraits>
struct TextRange final
{
    IteratorType m_begin;    ///< Начало данных.
    IteratorType m_current;  ///< Текущая позиция.
    IteratorType m_end;      ///< За концом данных.
    std::size_t  m_column;   ///< Номер колонки, нумерация с 1.
    std::size_t  m_line;     ///< Номер строки, нумерация с 1.

    TextRange (IteratorType begin, IteratorType end) noexcept
        : m_begin { begin }, m_current { begin }, m_end { end },
        m_column { 1 }, m_line { 1 } {}

    TextRange             (const TextRange &) noexcept = default; ///< Конструктор копирования.
    TextRange             (TextRange &&)      noexcept = default; ///< Конструктор перемещения.
    TextRange& operator = (const TextRange &) noexcept = default; ///< Оператор копирования.
    TextRange& operator = (TextRange &&)      noexcept = default; ///< Оператор присваивания.

    /// \brief Символ, возвращаемый в качестве признака конца текста.
    /// \return "Невозможный символ".
    IRBIS_NODISCARD
    static CharType EOT() { return CharTraits::eot(); }

    /// \brief Подглядываем символ в указанной позиции.
    /// \param index Позиция символа (отсчет от начала текста, от 0).
    /// \return Подсмотренный символ, либо EOT.
    IRBIS_NODISCARD
    CharType at (const std::size_t index) const noexcept
    {
        if (index >= this->size()) {
            return EOT();
        }
        return static_cast <CharType> (* (m_begin + static_cast <std::ptrdiff_t> (index)));
    }

    /// \brief Последний символ в тексте.
    /// \return Символ либо EOT (для пустого текста).
    IRBIS_NODISCARD
    CharType back() const noexcept { return at (this->size() - 1); }

    /// \brief Итератор, указывающий на начало текста.
    /// \return Итератор.
    /// \warning Для пустого текста UB!
    IRBIS_NODISCARD
    IteratorType begin() const noexcept { return m_begin; }

    /// \brief Номер текущей колонки, отсчет от 1.
    /// \return Номер колонки.
    IRBIS_NODISCARD
    std::size_t column() const noexcept { return m_column; }

    /// \brief Сравнение двух символов.
    /// \param left Первый символ.
    /// \param right Второй символ.
    /// \return Результат сравнения
    IRBIS_NODISCARD
    static int compare (const CharType left, const CharType right) noexcept
    {
        return CharTraits::compare (left, right);
    }

    /// \brief Сравнивает поэлементно два диапазона (оба начиная с текущей позиции).
    /// \param other Диапазон для сравнения.
    /// \return 0, если диапазоны поэлементно равны, меньше 0, если this меньше, больше 0, если this больше.
    IRBIS_NODISCARD
    int compareTo (const TextRange &other) const noexcept
    {
        auto thisPtr = m_current;
        auto otherPtr = other.m_current;
        const auto otherEnd = other.m_end;
        while (true) {
            if (thisPtr == m_end) {
                if (otherPtr == otherEnd) {
                    break;
                }
                return -1;
            }
            if (otherPtr == otherEnd) {
                return 1;
            }
            const auto result = compare (*thisPtr, *otherPtr);
            if (result != 0) {
                return result;
            }
        }
        return 0;
    }

    /// \brief Содержит ли непрочитанная часть диапазона указанные символы?
    /// \tparam Тип символовов.
    /// \param value Искомые символы.
    /// \return true, если содержит любой из искомых символов.
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

    /// \brief Совпадает ли конец данного диапазона с текущей позицией другого диапазона? Движения по тексту не происходит.
    /// \param prefix Диапазон для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool endsWith (const TextRange &suffix) const noexcept
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

    /// \brief Совпадает ли конец данного диапазона с указанным диапазоном? Движения по тексту не происходитт.
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

    /// \brief Совпадает ли конец данного диапазона с указанной строкой? Движения по тексту не происходит.
    /// \param suffix C-строка для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool endsWith (const CharType *suffix) const noexcept
    {
        return this->endsWith (suffix, suffix + CharTraits::length (suffix));
    }

    /// \brief Совпадает ли конец данного диапазона с указанной строкой? Движения по тексту не происходит.
    /// \param suffix C++-строка для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool endsWidth (const std::basic_string <CharType> &suffix) const noexcept
    {
        return this->endsWith (std::begin (suffix), std::end (suffix));
    }

    /// \brief Достигнут конец текста?
    /// \return true если достигнут.
    IRBIS_NODISCARD
    bool eot() const noexcept { return m_current == m_end; }

    /// \brief Проверка двух символов на равенство.
    /// \param left Первый символ.
    /// \param right Второй символ.
    /// \return true, если символы считаются равными.
    IRBIS_NODISCARD
    static bool equals (const CharType left, const CharType right) noexcept
    {
        return CharTraits::equals (left, right);
    }

    /// \brief Извлечение целого числа без знака.
    /// \return Поддиапазон с числом (возможно, пустой).
    IRBIS_NODISCARD
    TextRange extractInteger () noexcept
    {
        // Сначала пропускаем нечисловые символы
        while (!this->eot() && !this->isDigit()) {
            this->read();
        }
        return this->readInteger();
    }

    /// \brief Поиск указанного символа (начиная с текущей позиции).
    /// \param value Символ для поиска.
    /// \return Итератор на указанный символ либо итератор за концом текста.
    IRBIS_NODISCARD
    IteratorType find (const CharType value) const noexcept
    {
        return CharTraits::find (m_current, m_end, value);
    }

    /// \brief Заполнение диапазона указанным символом (начиная с текущей позиции).
    /// \param value Символ для заполнения.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& fill (const CharType value) noexcept
    {
        for (auto ptr = m_current; ptr != m_end; ++ptr) {
            *ptr = value;
        }
        return *this;
    }

    /// \brief Поиск первого вхождения любого из указанных символов (начиная с текущей позиции).
    /// \tparam Args Тип символов.
    /// \param args Искомые символы.
    /// \return Итератор на найденный символ либо итератор за концом текста.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType find (const Args ... args) const noexcept
    {
        const auto list = { args... };
        for (auto ptr = m_current; ptr != m_end; ++ptr) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                return ptr;
            }
        }
        return m_end;
    }

    /// \brief Поиск последнего вхождения любого из указанных символов (начиная с текущей позиции).
    /// \tparam Args Тип символы.
    /// \param args Искомые символы.
    /// \return Итератор на найденный символ либо итератор за концом текста.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType findLast (const Args ... args) const noexcept
    {
        const auto list = { args... };
        if (m_current != m_end) {
            for (auto ptr = m_end - 1; ptr != m_current; --ptr) {
                const char c = static_cast <CharType> (*ptr);
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

    /// \brief Первый символ в тексте.
    /// \return Символ либо EOT (для пустого текста),
    IRBIS_NODISCARD
    CharType front() const noexcept { return at (0); }

    /// \brief Символ в текущей позиции - буква?
    /// \return true, если буква.
    IRBIS_NODISCARD
    bool isAlpha() const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isAlpha (c);
    }

    /// \brief Символ в текущей позиции - управляющий?
    /// \return true, если управляющий.
    IRBIS_NODISCARD
    bool isControl() const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isControl (c);
    }

    /// \brief Символ в текущей позиции - цифра?
    /// \return true, если цифра.
    IRBIS_NODISCARD
    bool isDigit() const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isDigit (c);
    }

    /// \brief Символ в текущей позиции - пунктуация?
    /// \return true, если пунктуация.
    IRBIS_NODISCARD
    bool isPunctuation() const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isPunctuation (c);
    }

    /// \brief Символ в текущей позиции - пробельный?
    /// \return true, если пробельный.
    IRBIS_NODISCARD
    bool isWhitespace() const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isWhitespace (c);
    }

    /// \brief Номер текущей строки, отсчет от 1.
    /// \return Номер строки.
    IRBIS_NODISCARD
    std::size_t line() const noexcept { return m_line; }

    /// \brief Заглядывание вперед на указанное количество символов.
    /// \return Подсмотренный символ либо EOT.
    IRBIS_NODISCARD
    CharType lookAhead (const std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() + distance;
        return this->at (newPosition);
    }

    /// \brief Заглядывание назад на указанное количество символов.
    /// \return Подсмотренный символ либо EOT.
    IRBIS_NODISCARD
    CharType lookBehind (const std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() - distance;
        return this->at (newPosition);
    }

    /// \brief Сдвиг по тексту на указанное число символов.
    /// \param distance Величина сдвига (отрицательная - назад).
    /// \return this.
    /// \warning Портит column/line.
    IRBIS_MAYBE_UNUSED
    TextRange& move (const std::ptrdiff_t distance) noexcept
    {
        m_current  += distance;
        m_column   += distance;
        return *this;
    }

    /// \brief Перемещение по тексту к указанной позиции.
    /// \param newPosition Новая позиция (отсчет от 0 от начала текста).
    /// \return this.
    /// \warning Портит column/line.
    IRBIS_MAYBE_UNUSED
    TextRange& moveTo (const IteratorType newPosition) noexcept
    {
        m_current = newPosition;
        return *this;
    }

    /// \brief Перемещение по тексту к указанной позиции.
    /// \param newPosition Итератор на новую позицию.
    /// \return this.
    /// \warning Портит column/line.
    IRBIS_MAYBE_UNUSED
    TextRange& moveTo (std::size_t newPosition) noexcept
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

    /// \brief Подсматриваем текущий символ.
    /// \return Подсмотренный символ либо EOT.
    IRBIS_NODISCARD
    CharType peek() const noexcept
    {
        return (m_current == m_end) ? EOT() : this->value();
    }

    /// \brief Подсматриваем следующий символ, игнорируя указанные.
    /// \tparam Args Тип символов.
    /// \param args Символы, которые должны быть проигнорированы.
    /// \return Итератор на подсмотренный символ либо за концом текста.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType peekExcept (const Args ... args) const noexcept
    {
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
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
    TextRange peekLine() const noexcept
    {
        TextRange result { m_current, m_current };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (equals (c, '\r') || equals (c, '\n')) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подсматриваем следующий символ, игнорируя все символы, отличные от указанных.
    /// \tparam Args Тип символов.
    /// \param args Символы, которые должны быть обнаружены.
    /// \return Итератор на подсмотренный символ либо за концом текста.
    template <class ... Args>
    IRBIS_NODISCARD
    IteratorType peekOnly (Args ... args) const noexcept
    {
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
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
    TextRange peekString (std::size_t length) const noexcept
    {
        TextRange result { m_current, m_current };
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

    /// \brief Подглядывание вплоть до любого из указанных стоп-символов (включая его).
    /// \tparam Тип символов.
    /// \param args Стоп-спимволы.
    /// \return Подсмотренная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_NODISCARD
    TextRange peekTo (const Args ... args) const noexcept
    {
        const auto list = { args... };
        TextRange result { m_current, m_current };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            ++ptr;
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                break;
            }
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подглядывание вплоть до любого из указанных стоп-символов (не включая его).
    /// \tparam Тип символов.
    /// \param args Стоп-спимволы.
    /// \return Подсмотренная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_NODISCARD
    TextRange peekUntil (const Args ... args) const noexcept
    {
        const auto list = { args... };
        TextRange result { m_current, m_current };
        IteratorType ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), c) != std::end (list)) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подглядывание, пока встречаются перечисленные символы.
    /// \tparam Args Тип символов.
    /// \param args Нужные символы.
    /// \return Подсмотренная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_NODISCARD
    TextRange peekWhile (const Args ... args) const noexcept
    {
        TextRange result { m_current, m_current };
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (std::begin (list), std::end (list), result) == std::end (list)) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Текущая позиция в тексте (в символах), отсчет от 0.
    /// \return Текущая позиция.
    IRBIS_NODISCARD
    std::size_t position() const noexcept
    {
        return static_cast <std::size_t> (m_current - m_begin);
    }

    /// \brief Считываем текущий символ. Двигаемся вперёд по тексту.
    /// \return Прочитанный символ либо EOT.
    IRBIS_MAYBE_UNUSED
    CharType read() noexcept
    {
        if (m_current == m_end) {
            return EOT();
        }

        const auto result = static_cast <CharType> (*m_current);
        ++m_current;
        if (result == '\n') {
            ++m_line;
            m_column = 1;
        }
        else {
            ++m_column;
        }

        return result;
    }

    /// \brief Чтение целого числа без знака. Двигаемся вперед по тексту.
    /// \return Прочитанная строка с числом (возможно, пустая).
    /// \details Длина целого не ограничена.
    IRBIS_MAYBE_UNUSED
    TextRange readInteger() noexcept
    {
        TextRange result { m_current, m_current };
        while (this->isDigit()) {
            this->read();
        }
        result.m_end = m_current;

        return result;
    }

    /// \brief Считывание строки. Происходит движение по тексту.
    /// \return Считанная строка (возможно, пустая).
    /// \details Символы перевода строки считываются, но в результирующую строку не помещаются.
    IRBIS_MAYBE_UNUSED
    TextRange readLine() noexcept
    {
        TextRange result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = this->read();
            if (equals (c, '\r')) {
                result.m_end = m_current - 1;
                if (equals (this->peek(), '\n')) {
                    this->read();
                }
                break;
            }
            if (equals (c, '\n')) {
                result.m_end = m_current - 1;
                break;
            }
        }
        return result;
    }

    /// \brief Считывание начиная с открывающего символа до закрывающего (включая их).
    /// \param openChar Открывающий символ.
    /// \param closeChar Закрывающий символ.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    TextRange readOpenClose (const CharType openChar, const CharType closeChar) noexcept
    {
        TextRange result { m_current, m_current };
        if (equals (this->peek(), openChar)) {
            this->read();
            while (m_current != m_end) {
                if (equals (this->read(), closeChar)) {
                    this->read();
                    result.m_end = m_current;
                    break;
                }
            }
        }
        return result;
    }

    /// \brief Считывание начиная с открывающего символа до закрывающего (включая их).
    /// \param openChar C-строка с возможными открывающими символами.
    /// \param closeChar C-строка с возможными закрывающими символами.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    TextRange readOpenClose (const CharType *openChars, const CharType *closeChars) noexcept
    {
        const auto openEnd = openChars + CharTraits::length (openChars);
        const auto closeEnd = closeChars + CharTraits::length (closeChars);
        TextRange result { m_current, m_current };
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

    /// \brief Считываем следующий символ, пропуская указанные (если они появятся). Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Игнорируемые символы.
    /// \return Прочитанный символ либо EOT.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    CharType readSkip (Args ... args) noexcept
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

    /// \brief Чтение строки вплоть до указанной длины. Происходит движение по тексту.
    /// \param length Максимальная длина строки.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Символы перевода строки также считываются и помещаются в результат.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Считывание текста вплоть до любого из указанных стоп-символов (не включая его). Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Стоп-символы.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Встреченный стоп-символ считывается, но в результирующую строку не включается.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange readTo (const Args ... args) noexcept
    {
        TextRange result { m_current, m_current };
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
    TextRange readTo (const Iter begin, const Iter end) noexcept
    {
        auto size = static_cast <std::size_t> (end - begin);
        TextRange result { m_current, m_current };
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

    /// \brief Считывание вплоть до указанного ограничителя (разделитель не помещается в возвращаемое значение, однако, считывается).
    /// \param text C-строка-ограничитель.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    TextRange readTo (const CharType *text) noexcept
    {
        return this->readTo (text, text + CharTraits::length (text));
    }

    /// \brief Считывание вплоть до указанного ограничителя (разделитель не помещается в возвращаемое значение, однако, считывается).
    /// \param text C++-строка-ограничитель.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    TextRange readTo (const std::basic_string <CharType> &text) noexcept
    {
        return this->readTo (std::begin (text), std::end (text));
    }

    /// \brief Считывание текста вплоть до любого из указанных стоп-символов (не включая его). Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Стоп-символы.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Встреченный стоп-символ не считывается и в результирующую строку не включается.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange readUntil (const Args ... args) noexcept
    {
        TextRange result { m_current, m_current };
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
    TextRange readUntil (const Iter begin, const Iter end) noexcept
    {
        auto size = static_cast <std::size_t> (end - begin);
        TextRange result { m_current, m_current };
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

    /// \brief Считывание вплоть до указанного ограничителя (разделитель не помещается в возвращаемое значение и не считывается).
    /// \param text C-строка-ограничитель.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    TextRange readUntil (const CharType *text) noexcept
    {
        return this->readUntil (text, text + CharTraits::length (text));
    }

    /// \brief Считывание вплоть до указанного ограничителя (разделитель не помещается в возвращаемое значение и не считывается).
    /// \param text C++-строка-ограничитель.
    /// \return Прочитанный фрагмент (возможно, пустой).
    IRBIS_MAYBE_UNUSED
    TextRange readUntil (const std::basic_string <CharType> &text) noexcept
    {
        return this->readUntil (std::begin (text), std::end (text));
    }

    /// \brief Считывание текста до тех пор, пока он состоит из указанных разрешенных символов. Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Разрешенные символы.
    /// \return Считанная строка (возможно, пустая).
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange readWhile (const Args ... args) noexcept
    {
        TextRange result { m_current, m_current };
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

    /// \brief Чтение слова (состоящего из алфавитно-цифровых символов). Происходит движение по тексту.
    /// \return Прочитанное слово (возможно, пустое).
    IRBIS_MAYBE_UNUSED
    TextRange readWord() noexcept
    {
        TextRange result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = this->value();
            if (!CharTraits::isAlpha (c)) {
                break;
            }
            this->read();
        }
        result.m_end = m_current;
        return result;
    }

    /// \brief Чтение слова (состоящего из алфавитно-цифровых символов). Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Дополнительные символы, засчитываемые как входящие в слово.
    /// \return Прочитанное слово (возможно, пустое).
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange readWord (const Args ... args) noexcept
    {
        const auto list = { args ... };
        TextRange result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = this->value();
            if (!CharTraits::isAlpha (c)
                && CharTraits::find (std::begin (list), std::end (list), c) == std::end (list)) {
                break;
            }
            this->read();
        }
        result.m_end = m_current;
        return result;
    }

    /// \brief Получение нескольких последних прочитанных символов. Движения по тексту не происходит.
    /// \param length Сколько символов надо.
    /// \return Полученные символы (возможно, пустой фрагмент).
    IRBIS_NODISCARD
    TextRange recent (const std::size_t length) const noexcept
    {
        const auto position = this->position();
        std::size_t start = position >= length ? position - length : 0;
        return { m_begin + start, m_current };
    }

    /// \brief Получение непрочитанного остатка текста. Движения по тексту не происходит.
    /// \return Непрочитанный фрагмент (возможно, пустой).
    IRBIS_NODISCARD
    TextRange remaining() const noexcept
    {
        return { m_current, m_end };
    }

    /// \brief Получение длины непрочитанного остатка текста. Движения по тексту не происходит.
    /// \return Длина непрочитанного остатка в символах.
    std::size_t remainingSize() const noexcept
    {
        return static_cast <std::size_t> (m_end - m_current);
    }

    /// \brief Перемотка в начало.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& rewind() noexcept
    {
        m_current = m_begin;
        m_line = 1;
        m_column = 1;
        return *this;
    }

    /// \brief Поиск подстроки, заданной другим диапазоном. Движения по тексту не происходит.
    /// \param that Искомая подстрока.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType search (const TextRange &that) const noexcept
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
                   const auto thisChar = static_cast <CharType> (*ptr);
                   const auto thatChar = static_cast <CharType> (*thatPtr);
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

    /// \brief Поиск подстроки, заданной с помощью итераторов. Движения по тексту не происходит.
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
                    const auto thisChar = static_cast <CharType> (*ptr);
                    const auto thatChar = static_cast <CharType> (*thatPtr);
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

    /// \brief Поиск подстроки. Движения по тексту не происходит.
    /// \param text Искомая C-строка.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType search (const CharType *text) const noexcept
    {
        return this->search (text, text + CharTraits::length (text));
    }

    /// \brief Поиск подстроки. Движения по тексту не происходит.
    /// \param text Искомая C++-строка.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType search (const std::basic_string <CharType> &text) const noexcept
    {
        return this->search (std::begin (text), std::end (text));
    }

    /// \brief Поиск последнего вхождения подстроки, заданной другим диапазоном. Движения по тексту не происходит.
    /// \param that Искомая подстрока.
    /// \return Итератор на последнюю найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType searchLast (const TextRange &that) const noexcept
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
                    const auto thisChar = static_cast <CharType> (*ptr);
                    const auto thatChar = static_cast <CharType> (*thatPtr);
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

    /// \brief Поиск последнего вхождения подстроки, заданной с помощью итераторов. Движения по тексту не происходит.
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
                    const auto thisChar = static_cast <CharType> (*ptr);
                    const auto thatChar = static_cast <CharType> (*thatPtr);
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

    /// \brief Поиск последнего вхождения подстроки. Движения по тексту не происходит.
    /// \param text Искомая C-строка.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType searchLast (const CharType *text) const noexcept
    {
        return this->searchLast (text, text + CharTraits::length (text));
    }

    /// \brief Поиск последнего вхождения подстроки. Движения по тексту не происходит.
    /// \param text Искомая C++-строка.
    /// \return Итератор на первую найденную позицию либо за концом диапазона.
    IRBIS_NODISCARD
    IteratorType searchLast (const std::basic_string <CharType> &text) const noexcept
    {
        return this->searchLast (std::begin (text), std::end (text));
    }

    /// \brief Длина текста в символах (включая все служебные).
    /// \return Длина текста.
    IRBIS_NODISCARD
    std::size_t size() const noexcept { return static_cast <std::size_t> (m_end - m_begin); }

    /// \brief Пропустить указанное количество символов. Происходит движение по тексту вперед.
    /// \param nchars Число символов.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& skip (std::size_t nchars) noexcept
    {
        while (nchars != 0) {
            this->read();
            --nchars;
        }
        return *this;
    }

    /// \brief Пропустить следующий символ, если он совпадает с одним из перечисленных. Происходит движение по тексту вперед.
    /// \tparam Args Тип символов.
    /// \param args Разрешенные символы для пропуска.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange& skipOne (Args ... args) noexcept
    {
        const auto list = { args... };
        const auto c = this->peek();
        if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск символов, пока не встретится любой из перечисленных. Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Искомые символы.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange& skipTo (const Args ... args) noexcept
    {
        const auto list = { args ... };
        while (m_current != m_end) {
            const auto c = static_cast <CharType> (*m_current);
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск перечисленных символов. Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Пропускаемые символы.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange& skipWhile (const Args ... args) noexcept
    {
        const auto list = { args ... };
        while (m_current != m_end) {
            const auto c = static_cast <CharType> (*m_current);
            if (CharTraits::find (list.begin(), list.end(), c) == list.end()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск до начала следующей строки. Происходит движение по тексту.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& skipLine() noexcept
    {
        while (!this->eot()) {
            const auto c = this->read();
            if (CharTraits::equals (c, '\n')) {
                break;
            }
            if (CharTraits::equals (c, '\r')) {
                if (!this->eot()) {
                    const auto c2 = this->peek();
                    if (CharTraits::equals (c2, '\n')) {
                        this->read();
                    }
                }
                break;
            }
        }
        return *this;
    }

    /// \brief Пропуск символов, не составляющих идентификатор. Происходит движение по тексту.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& skipNonWord() noexcept
    {
        while (m_current != m_end) {
            const auto c = this->value();
            if (CharTraits::isLetter(c)) {
                break;
            }
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск символов, не составляющих идентификатор. Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Дополнительные символы, считающиеся принадлежащими идентификатору.
    /// \return this.
    template <class ... Args>
    IRBIS_MAYBE_UNUSED
    TextRange& skipNonWord (const Args ... args) noexcept
    {
        const auto list = { args ... };
        while (m_current != m_end) {
            const auto c = this->value();
            if (CharTraits::isLetter(c)
                || CharTraits::find (list.begin(), list.end(), c) != list.end()) {
                break;
            }
            this->read();
        }
        return *this;
    }

    /// \brief Пропуск пунктуации.
    /// \return this.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Пропуск пробельных символов.
    /// \return this.
    IRBIS_MAYBE_UNUSED
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

    /// \brief Извлечение поддиапазона.
    /// \param offset Смещение (в символах), начиная с которого извлекается поддиапазон.
    /// \param length Желаемая длина поддиапазона (в символах).
    /// \return Полученный поддиапазон (возможно, пустой).
    IRBIS_NODISCARD
    TextRange slice (const std::size_t offset, std::size_t length) const noexcept
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

    /// \brief Разбиение текста на фрагменты по указанному разделителю.
    /// \param delimiter Символ-разделитель.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    IRBIS_NODISCARD
    std::vector <TextRange> split (const CharType delimiter,
        std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        std::vector <TextRange> result;
        auto start = m_current;
        --nelem;
        while ((nelem != 0) && (start != m_end)) {
            auto current = start;
            while (current != m_end) {
                if (CharTraits::equal (*current, delimiter)) {
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

    /// \brief Разбиение текста на фрагменты по указанным разделителям.
    /// \tparam Args Тип разделителей.
    /// \param delimiter Символы-разделители.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    template <class ... Args>
    IRBIS_NODISCARD
    std::vector <TextRange> split (const Args ... args,
        std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        std::vector <TextRange> result;
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

    /// \brief Разбиение текста на фрагменты по указанному разделителю, заданному парой итераторов. Движения по тексту не происходит.
    /// \tparam Iter Тип итераторов.
    /// \param begin Начало строки-разделителя.
    /// \param end Конец строки-разделителя.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    template <class Iter>
    IRBIS_NODISCARD
    std::vector <TextRange> split (const Iter begin, const Iter end,
        std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        std::vector<TextRange> result;
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

    /// \brief Разбиение текста на фрагменты по указанному разделителю, заданному C-строкой. Движения по тексту не происходит.
    /// \param delimiter Строка-разделитель.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    IRBIS_NODISCARD
    std::vector <TextRange> split (const CharType *delimiter,
        std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        const auto length = CharTraits::length (delimiter);
        assert (length != 0);
        return this->split (delimiter, delimiter + length, nelem);
    }

    /// \brief Разбиение текста на фрагменты по указанному разделителю, заданному C++-строкой. Движения по тексту не происходит.
    /// \param delimiter Строка-разделитель.
    /// \param nelem Максимальное количество элементов в результате.
    /// \return Вектор фрагментов.
    IRBIS_NODISCARD
    std::vector <TextRange> split (const std::basic_string <CharType> &delimiter,
        std::size_t nelem = std::numeric_limits<std::size_t>::max()) const
    {
        assert (!delimiter.empty());
        return this->split (std::begin (delimiter), std::end (delimiter), nelem);
    }

    /// \brief Совпадает ли текущая позиция с текущей позицией другого диапазона? Движения по тексту не происходит.
    /// \param prefix Диапазон для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool startsWith (const TextRange &prefix) const noexcept
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

    /// \brief Совпадает ли текущая позиция данного диапазона с указанным фрагментом? Движения по тексту не происходит.
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

    /// \brief Совпадает ли текущая позиция данного диапазона с указанной строкой? Движения по тексту не происходит.
    /// \param prefix C-строка для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool startsWith (const CharType *prefix) const noexcept
    {
        return this->startsWith (prefix, prefix + CharTraits::length (prefix));
    }

    /// \brief Совпадает ли текущая позиция данного диапазона с указанной строкой? Движения по тексту не происходит.
    /// \param prefix C++-строка для сравнения.
    /// \return true если совпадает.
    IRBIS_NODISCARD
    bool startsWith (const std::basic_string <CharType> &prefix) const noexcept
    {
        return this->startsWith (std::begin (prefix), std::end (prefix));
    }

    /// \brief Материализация в строку (начиная с текущей позиции). Движения по тексту не происходит.
    /// \return Строка (возможно, пустая).
    IRBIS_NODISCARD
    std::basic_string <CharType> toString() const
    {
        std::basic_string <CharType> result;
        result.reserve (this->size());
        result.insert (std::end (result), m_current, m_end);
        return result;
    }

    /// \brief Материализация в вектор (начиная с текущей позиции). Движения по тексту не происходит.
    /// \return Вектор символов (возможно, пустой).
    IRBIS_NODISCARD
    std::vector <CharType> toVector() const
    {
        std::vector <CharType> result;
        result.reserve (this->size());
        result.insert (std::end (result), m_current, m_end);
        return result;
    }

    /// \brief Преобразование символов (начиная с текущей позиции) к нижнему регистру.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& toLower() noexcept
    {
        for (auto ptr = m_current; ptr != m_end; ++ptr) {
            *ptr = CharTraits::toLower (*ptr);
        }
        return *this;
    }

    /// \brief Преобразование символов (начиная с текущей позиции) к нижнему регистру.
    /// \return this.
    IRBIS_MAYBE_UNUSED
    TextRange& toUpper() noexcept
    {
        for (auto ptr = m_current; ptr != m_end; ++ptr) {
            *ptr = CharTraits::toUpper (*ptr);
        }
        return *this;
    }

    /// \brief Удаление указанных символов с конца строки.
    /// \tparam Args Тип символов.
    /// \param args Символы, подлежащие удалению.
    /// \return Диапазон с удаленными символами.
    template <class ... Args>
    IRBIS_NODISCARD
    TextRange trimEnd (const Args ... args) const noexcept
    {
        const auto list = { args ... };
        TextRange result { m_current, m_end };
        if (m_current != m_end) {
            for (auto current = m_end - 1; current != m_current; --current) {
                const auto c = static_cast <CharType> (*current);
                if (CharTraits::find (list.begin (), list.end (), c) == list.end ()) {
                    result.m_end = current + 1;
                    break;
                }
            }
        }
        return result;
    }

    /// \brief Удаление указанных символов с конца строки.
    /// \tparam Iter Тип итератор.
    /// \param begin Начало символов, подлежащих удалению.
    /// \param end Конец.
    /// \return Диапазон с удаленными символами.
    template <class Iter>
    IRBIS_NODISCARD
    TextRange trimEnd (const Iter begin, const Iter end) const noexcept
    {
        TextRange result { m_current, m_end };
        if (m_current != m_end) {
        for (auto current = m_end - 1; current != m_current; --current) {
            const auto c = static_cast <CharType> (*current);
            if (CharTraits::find (begin, end, c) == end) {
                result.m_end = current + 1;
                break;
            }
        }
        }
        return result;
    }

    /// \brief Удаление указанных символов с конца строки.
    /// \param symbols C-строка с символами, подлежашими удалению.
    /// \return Диапазон с удаленными символами.
    IRBIS_NODISCARD
    TextRange trimEnd (const CharType *symbols) const noexcept
    {
        return this->trimEnd (symbols, symbols + CharTraits::length (symbols));
    }

    /// \brief Удаление указанных символов с конца строки.
    /// \param symbols C++-строка с символами, подлежащими удалению.
    /// \return Диапазон с удаленными символами.
    IRBIS_NODISCARD
    TextRange trimEnd (const std::basic_string <CharType> &symbols) const noexcept
    {
        return this->trimEnd (std::begin (symbols), std::end (symbols));
    }

    /// \brief Удаление указанных символов с начала строки.
    /// \tparam Args Тип символов.
    /// \param args Символы, подлежащие удалению.
    /// \return Диапазон с удаленными символами.
    template <class ... Args>
    IRBIS_NODISCARD
    TextRange trimStart (const Args ... args) const noexcept
    {
        const auto list = { args ... };
        TextRange result { m_current, m_end };
        for (auto current = m_current; current != m_end; ++current) {
            const auto c = static_cast <CharType> (*current);
            if (CharTraits::find (list.begin(), list.end(), c) == list.end()) {
                result.m_begin = current;
                result.m_current = current;
                break;
            }
        }
        return result;
    }

    /// \brief Удаление указанных символов с начала строки.
    /// \tparam Iter Тип итератора.
    /// \param begin Начало символов, подлежащих удалению.
    /// \param end Конец.
    /// \return Диапазон с удаленными символами.
    template <class Iter>
    IRBIS_NODISCARD
    TextRange trimStart (const Iter begin, const Iter end) const noexcept
    {
        TextRange result { m_current, m_end };
        for (auto current = m_current; current != m_end; ++current) {
            const auto c = static_cast <CharType> (*current);
            if (CharTraits::find (begin, end, c) == end) {
                result.m_begin = current;
                result.m_current = current;
                break;
            }
        }
        return result;
    }

    /// \brief Удаление указанных символов с начала строки.
    /// \param symbols C-строка с символами, подлежащими удалению.
    /// \return Диапазон с удаленными символами.
    IRBIS_NODISCARD
    TextRange trimStart (const CharType *symbols) const noexcept
    {
        return this->trimStart (symbols, symbols + CharTraits::length (symbols));
    }

    /// \brief Удаление указанных символов с начала строки.
    /// \param symbols C++-строка с символами, подлежащими удалению.
    /// \return Диапазон с удаленными символами.
    IRBIS_NODISCARD
    TextRange trimStart (const std::basic_string <CharType> &symbols) const noexcept
    {
        return this->trimStart (std::begin (symbols), std::end (symbols));
    }

    /// \brief Удаление указанных символов с начала и с конца строки.
    /// \tparam Args Тип символов.
    /// \param args Символы, подлежащие удалению.
    /// \return Диапазон с удаленными символами.
    template <class ... Args>
    IRBIS_NODISCARD
    TextRange trim (const Args ... args) const noexcept
    {
        return this->trimStart (args...).trimEnd (args...);
    }

    /// \brief Удаление указанных символов с начала и с конца строки.
    /// \param symbols C-строка с символами, подлежащими удалению.
    /// \return Диапазон с удаленными символами.
    IRBIS_NODISCARD
    TextRange trim (const CharType *symbols) const noexcept
    {
        return this->trimStart (symbols).trimEnd (symbols);
    }

    /// \brief Удаление указанных символов с начала и с конца строки.
    /// \param symbols C++-строка с символами, подлежашими удалению.
    /// \return Диапазон с удаленными символами.
    IRBIS_NODISCARD
    TextRange trim (const std::basic_string <CharType> &symbols) const noexcept
    {
        return this->trimStart (symbols).trimEnd (symbols);
    }

    /// \brief Символ в текущей позиции. Для пустого диапазона или по достижении конца текста UB!
    /// \return Текущий символ.
    IRBIS_NODISCARD
    CharType value() const noexcept { return static_cast<CharType> (*m_current); }

    /// \brief Запись символа в текущую позицию. Происходит движение по тексту.
    /// \param chr Записываемый символ.
    /// \return Признак успешности операции.
    IRBIS_MAYBE_UNUSED
    bool write (const CharType chr) noexcept
    {
        if (m_current == m_end) {
            return false;
        }
        *m_current = chr;
        ++m_current;
        if (CharTraits::equals (chr, '\n')) {
            ++m_line;
            m_column = 1;
        }
        else {
            ++m_column;
        }
        return true;
    }

    /// \brief Оператор сравнения на равенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator == (const TextRange &left, const TextRange &right)
    {
        return left.compareTo (right) == 0;
    }

    /// \brief Оператор сравнение на равенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator == (const TextRange &left, const std::basic_string <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) == 0;
    }

    /// \brief Оператор сравнения на равенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator == (const TextRange &left, const std::vector <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) == 0;
    }

    /// \brief Оператор сравнения на неравенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator != (const TextRange &left, const TextRange &right)
    {
        return left.compareTo (right) != 0;
    }

    /// \brief Оператор сравнения на неравенство.
    /// \param left
    /// \param right
    /// \return
    friend bool operator != (const TextRange &left, const std::basic_string <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) != 0;
    }

    friend bool operator != (const TextRange &left, const std::vector <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) != 0;
    }

    friend bool operator < (const TextRange &left, const TextRange &right)
    {
        return left.compareTo (right) < 0;
    }

    friend bool operator < (const TextRange &left, const std::basic_string <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) < 0;
    }

    friend bool operator < (const TextRange &left, const std::vector <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) < 0;
    }

    friend bool operator <= (const TextRange &left, const TextRange &right)
    {
        return left.compareTo (right) <= 0;
    }

    friend bool operator <= (const TextRange &left, const std::basic_string <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) <= 0;
    }

    friend bool operator <= (const TextRange &left, const std::vector <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) <= 0;
    }

    friend bool operator > (const TextRange &left, const TextRange &right)
    {
        return left.compareTo (right) > 0;
    }

    friend bool operator > (const TextRange &left, const std::basic_string <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) > 0;
    }

    friend bool operator > (const TextRange &left, const std::vector <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) > 0;
    }

    friend bool operator >= (const TextRange &left, const TextRange &right)
    {
        return left.compareTo (right) >= 0;
    }

    friend bool operator >= (const TextRange &left, const std::basic_string <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) >= 0;
    }

    friend bool operator >= (const TextRange &left, const std::vector <CharType> &right)
    {
        TextRange temp (std::begin (right), std::end (right));
        return left.compareTo (temp) >= 0;
    }

    friend std::ostream& operator << (std::ostream &stream, const TextRange &range)
    {
        std::ostream_iterator<CharType> it (stream);
        std::copy (range.current(), range.end (), it);
        return stream;
    }

};

using NarrowRange    = TextRange <std::string::iterator,  char,    NarrowTraits <char>>;
using RawNarrowRange = TextRange <char*,                  char,    NarrowTraits <char>>;
using WideRange      = TextRange <std::wstring::iterator, wchar_t, WideTraits   <wchar_t>>;
using RawWideRange   = TextRange <wchar_t*,               wchar_t, WideTraits   <wchar_t>>;

/// \brief Создание диапазона символов из узкой C++-строки.
/// \param text Текст для превращения в диапазон.
/// \return Диапазон.
NarrowRange makeRange (const std::string &text)
{
    auto &noconst = const_cast<std::string&> (text);
    return { std::begin (noconst), std::end (noconst) };
}

/// \brief Создание диапазона символов из узкой C-строки.
/// \param text Текст для превращения в диапазон. Завершающий 0 не включается в диапазон.
/// \return Диапазон.
RawNarrowRange makeRange (const char *text)
{
    auto noconst = const_cast<char*> (text);
    return { noconst, noconst + ::strlen (text) };
}

/// \brief Создание диапазона символов из широкой C++-строки.
/// \param text Текст для превращения в диапазон.
/// \return Диапазон.
WideRange makeRange (const std::wstring &text)
{
    auto &noconst = const_cast<std::wstring&> (text);
    return { std::begin (noconst), std::end (noconst) };
}

/// \brief Создание диапазона символов из широкой C-строки.
/// \param text Текст для превращения в диапазон. Завершающий 0 не включается в диапазон.
/// \return Диапазон.
RawWideRange makeRange (const wchar_t *text)
{
    auto noconst = const_cast<wchar_t*> (text);
    return { noconst, noconst + ::wcslen (text) };
}

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

    ByteRange (IteratorType begin, IteratorType end) noexcept
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

    std::cout << "=============================================" << std::endl;

    {
        std::string hello ("Hello\nworld");
        auto range = makeRange (hello);
        std::cout << range.front () << std::endl;
        std::cout << range.peek () << std::endl;
        std::cout << range.back () << std::endl;
        std::cout << range.lookAhead () << std::endl;
        std::cout << range.isControl () << std::endl;
        std::cout << range.isAlpha () << std::endl;
        std::cout << range.isDigit () << std::endl;
        std::cout << range.isWhitespace () << std::endl;
        while (true) {
            const auto c = range.read ();
            if (!c) {
                break;
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }

    std::cout << "=============================================" << std::endl;

    {
        const char *hello = "Hello\nworld";
        const char *needle = "world";
        auto range = makeRange (hello);
        std::cout << range.front () << std::endl;
        std::cout << range.peek () << std::endl;
        std::cout << range.back () << std::endl;
        std::cout << range.lookAhead () << std::endl;
        std::cout << range.isControl () << std::endl;
        std::cout << range.isAlpha () << std::endl;
        std::cout << range.isDigit () << std::endl;
        std::cout << range.isWhitespace () << std::endl;
        std::cout << range.contains ('u', 'H') << std::endl;
        std::cout << "equals=" << RawNarrowRange::equals ('H', 'h') << std::endl;
        std::cout << "equals=" << RawNarrowRange::equals ('h', 'h') << std::endl;
        std::cout << range.search (needle, needle + 5) << std::endl;
        while (true) {
            const auto c = range.read ();
            if (!c) {
                break;
            }
            std::cout << c;
        }
        std::cout << std::endl;
    }

    std::cout << "=============================================" << std::endl;

    {
        const char *hello = "\t \n12345\t \n";
        auto range = makeRange (hello);
        const auto trimmed = range.trim (' ', '\t', '\n');
        std::cout << "trimmed=" << trimmed << std::endl;
        const auto number = range.extractInteger();
        std::cout << "extractInteger=" << number << std::endl;
    }

    return 0;
}
