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

template <class CharType>
struct NarrowTraits final
{
    static CharType eot() { return 0; }

    template <class IteratorType>
    static IteratorType find (IteratorType begin, IteratorType end, CharType c)
    {
        return std::find (begin, end, c);
    }

    static bool equals (CharType left, CharType right) { return left == right; }
    static bool less   (CharType left, CharType right) { return left < right; }

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

    static bool equals (CharType left, CharType right) { return left == right; }
    static bool less   (CharType left, CharType right) { return left < right; }

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
    static CharType EOT() { return CharTraits::eot(); }

    /// \brief Подглядываем символ в указанной позиции.
    /// \param index Позиция символа (отсчет от начала текста, от 0).
    /// \return Подсмотренный символ, либо EOT.
    CharType at (std::size_t index) const noexcept
    {
        if (index >= this->size()) {
            return EOT();
        }
        return static_cast <CharType> (* (m_begin + index));
    }

    /// \brief Последний символ в тексте.
    /// \return Символ либо EOT (для пустого текста).
    CharType back() const noexcept { return at (this->size() - 1); }

    /// \brief Итератор, указывающий на начало текста.
    /// \return Итератор.
    /// \warning Для пустого текста UB!
    IteratorType begin() const noexcept { return m_begin; }

    /// \brief Номер текущей колонки, отсчет от 1.
    /// \return Номер колонки.
    std::size_t column() const noexcept { return m_column; }

    /// \brief Итератор, указывающий на текущий символ.
    /// \return Итератор.
    /// \warning Для пустого текста UB!
    IteratorType current() const noexcept { return m_current; }

    /// \brief Диапазон пуст?
    /// \return true если пуст.
    bool empty() const noexcept { return m_begin == m_end; }

    /// \brief Итератор, указывающий за концом текста.
    /// \return Итератор.
    /// \warning Для пустого текста UB!
    IteratorType end() const noexcept { return m_end; }

    /// \brief Достигнут конец текста?
    /// \return true если достигнут.
    bool eot() const noexcept { return m_current == m_end; }

    /// \brief Извлечение целого числа без знака.
    /// \return Поддиапазон с числом (возможно, пустой).
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
    IteratorType find (CharType value) const noexcept
    {
        return CharTraits::find (m_current, m_end, value);
    }

    /// \brief Первый символ в тексте.
    /// \return Символ либо EOT (для пустого текста),
    CharType front() const noexcept { return at (0); }

    /// \brief Символ в текущей позиции - буква?
    /// \return true, если буква.
    bool isAlpha () const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isAlpha (c);
    }

    /// \brief Символ в текущей позиции - управляющий?
    /// \return true, если управляющий.
    bool isControl () const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isControl (c);
    }

    /// \brief Символ в текущей позиции - цифра?
    /// \return true, если цифра.
    bool isDigit () const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isDigit (c);
    }

    /// \brief Символ в текущей позиции - пунктуация?
    /// \return true, если пунктуация.
    bool isPunctuation () const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isPunctuation (c);
    }

    /// \brief Символ в текущей позиции - пробельный?
    /// \return true, если пробельный.
    bool isWhitespace () const noexcept
    {
        const auto c = this->peek();
        return (c != EOT()) && CharTraits::isWhitespace (c);
    }

    /// \brief Номер текущей строки, отсчет от 1.
    /// \return Номер строки.
    std::size_t line() const noexcept { return m_line; }

    /// \brief Заглядывание вперед на указанное количество символов.
    /// \return Подсмотренный символ либо EOT.
    CharType lookAhead (std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() + distance;
        return this->at (newPosition);
    }

    /// \brief Заглядывание назад на указанное количество символов.
    /// \return Подсмотренный символ либо EOT.
    CharType lookBehind (std::size_t distance = 1) const noexcept
    {
        const auto newPosition = this->position() - distance;
        return this->at (newPosition);
    }

    /// \brief Сдвиг по тексту на указанное число символов.
    /// \param distance Величина сдвига (отрицательная - назад).
    /// \return this.
    /// \warning Портит column/line.
    TextRange& move (std::ptrdiff_t distance) noexcept
    {
        m_current  += distance;
        m_column   += distance;
        return *this;
    }

    /// \brief Перемещение по тексту к указанной позиции.
    /// \param newPosition Новая позиция (отсчет от 0 от начала текста).
    /// \return this.
    /// \warning Портит column/line.
    TextRange& moveTo (IteratorType newPosition) noexcept
    {
        m_current = newPosition;
        return *this;
    }

    /// \brief Перемещение по тексту к указанной позиции.
    /// \param newPosition Итератор на новую позицию.
    /// \return this.
    /// \warning Портит column/line.
    TextRange& moveTo (std::size_t newPosition) noexcept
    {
        const auto size = this->size();
        if (newPosition > size) {
            newPosition = size;
        }
        m_current = m_begin + newPosition;
        return *this;
    }

    /// \brief Подсматриваем текущий символ.
    /// \return Подсмотренный символ либо EOT.
    CharType peek() const noexcept
    {
        return (m_current == m_end) ? EOT() : this->value();
    }

    /// \brief Подсматриваем следующий символ, игнорируя указанные.
    /// \tparam Args Тип символов.
    /// \param args Символы, которые должны быть проигнорированы.
    /// \return Итератор на подсмотренный символ либо за концом текста.
    template <class ... Args>
    IteratorType peekExcept (Args ... args) const noexcept
    {
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (list.begin(), list.end(), c) == list.end()) {
                return ptr;
            }
            ++ptr;
        }
        return m_end;
    }

    /// \brief Подсматриваем строку (вплоть до перевода строки либо конца текста).
    /// \return Подсмотренная строка (возможно, пустая).
    TextRange peekLine() const noexcept
    {
        TextRange result { m_current, m_current };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::equal (c, '\r') || CharTraits::equal (c, '\n')) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Подсматриваем следующий символ при условии, игнорируя все следу
    /// \tparam Args Тип символов.
    /// \param args Символы, которые должны быть обнаружены.
    /// \return Итератор на подсмотренный символ либо за концом текста.
    template <class ... Args>
    IteratorType peekOnly (Args ... args) const noexcept
    {
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
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
    TextRange peekTo (Args ... args) const noexcept
    {
        const auto list = { args... };
        TextRange result { m_current, m_current };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            ++ptr;
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
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
    TextRange peekUntil (Args ... args) const noexcept
    {
        const auto list = { args... };
        TextRange result { m_current, m_current };
        IteratorType ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (list.begin(), list.end(), c) != list.end()) {
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
    TextRange peekWhile (Args ... args) const noexcept
    {
        TextRange result { m_current, m_current };
        const auto list = { args... };
        auto ptr = m_current;
        while (ptr != m_end) {
            const auto c = static_cast <CharType> (*ptr);
            if (CharTraits::find (list.begin(), list.end(), result) == list.end()) {
                break;
            }
            ++ptr;
        }
        result.m_end = ptr;
        return result;
    }

    /// \brief Текущая позиция в тексте (в символах), отсчет от 0.
    /// \return Текущая позиция.
    std::size_t position() const noexcept
    {
        return static_cast <std::size_t> (m_current - m_begin);
    }

    /// \brief Считываем текущий символ. Двигаемся вперёд по тексту.
    /// \return Прочитанный символ либо EOT.
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

    /// \brief Чтение целого числа без знака.
    /// \return Прочитанная строка с числом (возможно, пустая).
    /// \details Длина целого не ограничена.
    TextRange readInteger() noexcept
    {
        TextRange result { m_current, m_current };
        while (this->isDigit()) {
            ++m_current;
        }
        result.m_end = m_current;

        return result;
    }

    /// \brief Считывание строки. Происходит движение по тексту.
    /// \return Считанная строка (возможно, пустая).
    /// \details Символы перевода строки считываются, но в результирующую строку не помещаются.
    TextRange readLine() noexcept
    {
        TextRange result { m_current, m_current };
        while (m_current != m_end) {
            const auto c = static_cast <CharType> (*m_current);
            if (CharTraits::equals (c, '\r') || CharTraits::equals (c, '\n')) {
                break;
            }
            this->read();
        }
        result.m_end = m_current;

        if (CharTraits::equals (this->peek(), '\r')) {
            this->read();
        }
        if (CharTraits::equals (this->peek(), '\n')) {
            this->read();
        }

        return result;
    }

    /// \brief Считываем следующий символ, пропуская указанные (если они появятся). Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Игнорируемые символы.
    /// \return Прочитанный символ либо EOT.
    template <class ... Args>
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
    TextRange readTo (Args ... args) noexcept
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

    /// \brief Считывание текста вплоть до любого из указанных стоп-символов (не включая его). Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Стоп-символы.
    /// \return Прочитанная строка (возможно, пустая).
    /// \details Встреченный стоп-символ не считывается и в результирующую строку не включается.
    template <class ... Args>
    TextRange readUntil (Args ... args) noexcept
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

    /// \brief Считывание текста до тех пор, пока он состоит из указанных разрешенных символов. Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Разрешенные символы.
    /// \return Считанная строка (возможно, пустая).
    template <class ... Args>
    TextRange readWhile (Args ... args) noexcept
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

    /// \brief Получение нескольких последних прочитанных символов. Движения по тексту не происходит.
    /// \param length Сколько символов надо.
    /// \return Полученные символы (возможно, пустой фрагмент).
    TextRange recent (std::size_t length) const noexcept
    {
        const auto position = this->position();
        std::size_t start = position >= length ? position - length : 0;
        return { m_begin + start, m_current };
    }

    /// \brief Получение непрочитанного остатка текста. Движения по тексту не происходит.
    /// \return Непрочитанный фрагмент (возможно, пустой).
    TextRange remaining() const noexcept
    {
        return { m_current, m_end };
    }

    /// \brief Длина текста в символах (включая все служебные).
    /// \return Длина текста.
    std::size_t size() const noexcept { return static_cast <std::size_t> (m_end - m_begin); }

    /// \brief Пропуск перечисленных символов. Происходит движение по тексту.
    /// \tparam Args Тип символов.
    /// \param args Пропускаемые символы.
    /// \return this.
    template <class ... Args>
    TextRange& skip (Args ... args)
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
    TextRange& skipLine () noexcept
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

    /// \brief Пропуск пунктуации.
    /// \return this.
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
    TextRange slice (std::size_t offset, std::size_t length) const noexcept
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

    /// \brief Символ в текущей позиции. Для пустого диапазона или по достижении конца текста UB!
    /// \return Текущий символ.
    CharType value() const noexcept { return static_cast<CharType> (*m_current); }

    /// \brief Запись символа в текущую позицию. Происходит движение по тексту.
    /// \param chr Записываемый символ.
    /// \return Признак успешности операции.
    bool write (CharType chr) noexcept
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
};

using NarrowRange = TextRange <std::string::iterator,  char,    NarrowTraits <char>>;
using WideRange   = TextRange <std::wstring::iterator, wchar_t, WideTraits <wchar_t>>;

NarrowRange makeRange (const std::string &text)
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
/// \tparam CharTraits Специфичные для символов операции.
/// \details Ограничения на диапазон байтов определяются итератором.
template <class IteratorType, class CharTraits = NarrowTraits <irbis::Byte>>
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

    int front() const noexcept { return at (0); }

    bool isAlpha () const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isAlpha (c);
    }

    bool isControl () const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isControl (c);
    }

    bool isDigit () const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isDigit (c);
    }

    bool isPunctuation () const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isPunctuation (c);
    }

    bool isWhitespace () const noexcept
    {
        const auto c = this->peekByte();
        return (c >= 0) && CharTraits::isWhitespace (c);
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

    std::string hello ("Hello\nworld");
    auto range = makeRange (hello);
    std::cout << range.front() << std::endl;
    std::cout << range.peek() << std::endl;
    std::cout << range.back() << std::endl;
    std::cout << range.lookAhead() << std::endl;
    std::cout << range.isControl() << std::endl;
    std::cout << range.isAlpha() << std::endl;
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

    std::cout << "=============================================" << std::endl;

    return 0;
}
