// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#include <cwctype>
#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

/*!
    \class irbis::TextNavigator

    \warning Сломается, если в тексте будет присутствовать символ `\0`.
 */

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Признак достигнутого конца текста.
const Char TextNavigator::EOT = L'\0';

/// \brief Конструктор.
/// \param text Текст.
/// \param length Длина текста в символах.
TextNavigator::TextNavigator (const Char* text, std::size_t length) noexcept
{
    assert (length >= 0);
    this->_text     = text;
    this->_length   = length;
    this->_position = 0;
    this->_line     = 1;
    this->_column   = 1;
}

/// \brief Конструктор.
/// \param text Текст.
TextNavigator::TextNavigator (WideSpan text) noexcept
{
    this->_text     = text.data();
    this->_length   = text.size();
    this->_position = 0;
    this->_line     = 1;
    this->_column   = 1;
}

/// \brief Конструктор.
/// \param text Текст.
TextNavigator::TextNavigator (const String &text) noexcept
{
    this->_text     = text.data();
    this->_length   = text.size();
    this->_position = 0;
    this->_line     = 1;
    this->_column   = 1;
}

/// \brief Конструктор копирования.
/// \param other Навигатор для копирования.
TextNavigator::TextNavigator (const TextNavigator &other) noexcept
{
    this->_text     = other._text;
    this->_position = other._position;
    this->_length   = other._length;
    this->_line     = other._line;
    this->_column   = other._column;
}

/// \brief Номер текущей колонки, отсчет от 1.
/// \return Номер колонки.
std::size_t TextNavigator::column() const noexcept
{
    return this->_column;
}

/// \brief Номер текущей строки, отсчет от 1.
/// \return Номер строки.
std::size_t TextNavigator::line() const noexcept
{
    return this->_line;
}

/// \brief Длина текста в символах (включая все служебные).
/// \return Длина текста.
std::size_t TextNavigator::length() const noexcept
{
    return this->_length;
}

/// \brief Текущая позиция в тексте (в символах), отсчет от 0.
/// \return Текущая позиция.
std::size_t TextNavigator::position() const noexcept
{
    return this->_position;
}

/// \brief Итератор, указывающий на начало текста.
/// \return Итератор.
/// \warning Для пустого текста UB!
Char* TextNavigator::begin() const noexcept
{
    return const_cast <Char*> (this->_text);
}

/// \brief Итератор, указывающий на начало текста.
/// \return Итератор.
/// \warning Для пустого текста UB!
const Char* TextNavigator::cbegin() const noexcept
{
    return this->_text;
}

/// \brief Итератор, указывающий на текущий символ.
/// \return Итератор.
/// \warning Для пустого текста UB!
Char* TextNavigator::current() const noexcept
{
    return const_cast <Char*> (this->_text + this->_position);
}

/// \brief Константный тератор, указывающий на текущий символ.
/// \return Итератор.
/// \warning Для пустого текста UB!
const Char* TextNavigator::ccurrent() const noexcept
{
    return this->_text + this->_position;
}

/// \brief Итератор, указывающий за концом текста.
/// \return Итератор.
/// \warning Для пустого текста UB!
Char* TextNavigator::end() const noexcept
{
    return const_cast <Char*> (this->_text + this->_length);
}

/// \brief Константный итератор, указывающий за концом текста.
/// \return Итератор.
/// \warning Для пустого текста UB!
const Char* TextNavigator::cend() const noexcept
{
    return this->_text + this->_length;
}

/// \brief Достигнут конец текста?
/// \return true если достигнут.
bool TextNavigator::eot() const noexcept
{
    return this->_position >= this->_length;
}

/// \brief Подглядываем символ в указанной позиции (отсчет от начала текста, от 0).
/// \return Подсмотренный символ, либо EOT.
Char TextNavigator::at (std::size_t position) const noexcept
{
    return position < this->_length ? this->_text [position] : EOT;
}

/// \brief Первый символ в тексте.
/// \return Символ либо EOT (для пустого текста),
Char TextNavigator::front() const noexcept
{
    return this->at(0);
}

/// \brief Последний символ в тексте.
/// \return Символ либо EOT (для пустого текста).
Char TextNavigator::back() const noexcept
{
    return this->at (this->_length - 1);
}

/// \brief Заглядывание вперед на указанное количество символов.
/// \return Подсмотренный символ либо EOT.
Char TextNavigator::lookAhead (std::ptrdiff_t distance) const noexcept
{
    const auto newPosition = this->_position + distance;
    return this->at (newPosition);
}

/// \brief Заглядывание назад на указанное количество символов.
/// \return Подсмотренный символ либо EOT.
Char TextNavigator::lookBehind (std::ptrdiff_t distance) const noexcept
{
    const auto newPosition = this->_position - distance;
    return this->at (newPosition);
}

/// \brief Перемещение по тексту вперёд/назад.
/// \return this.
TextNavigator& TextNavigator::move (std::ptrdiff_t distance) noexcept
{
    // TODO Some checks
    this->_position += distance;
    this->_column   += distance;
    return *this;
}

/// \brief Подсматриваем текущий символ.
/// \return Подсмотренный символ либо EOT.
Char TextNavigator::peekChar() const noexcept
{
    return this->at (this->_position);
}

/// \brief Подсматриваем текущий символ (пропускаем перевод строки).
/// \return Подсмотренный символ либо EOT.
Char TextNavigator::peekCharNoCrLf() const noexcept
{
    std::ptrdiff_t distance { 0 };
    while (true) {
        const auto result = this->at (this->_position + distance);
        if (result != '\r' && result != '\n') {
            return result;
        }
        ++distance;
    }
}

/// \brief Считываем текущий символ. Двигаемся вперёд по тексту.
/// \return Прочитанный символ либо EOT.
Char TextNavigator::readChar() noexcept
{
    if (this->eot()) {
        return EOT;
    }

    const auto result = this->at (this->_position++);
    if (result == '\n') {
        this->_line++;
        this->_column = 1;
    } else {
        this->_column++;
    }

    return result;
}

/// \brief Считываем текущий символ (пропуская перевод строки). Двигаемся вперёд по тексту.
/// \return Прочитанный символ либо EOT.
Char TextNavigator::readCharNoCrLf() noexcept
{
    while (true) {
        const auto result = this->readChar();
        if (result != '\r' && result != '\n') {
            return result;
        }
    }
}


/// \brief Подглядывание строки вплоть до указанной длины.
/// \return Подсмотренная строка (возможно, пустая).
WideSpan TextNavigator::peekString (std::size_t length) const noexcept
{
    WideSpan result (this->ccurrent(), 0);
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
/// \return Подсмотренная строка (возможно, пустая).
WideSpan TextNavigator::peekTo (Char stopChar) const noexcept
{
    WideSpan result (this->ccurrent(), 0);
    const auto length = this->_length - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead (i);
        ++result.length;
        if (c == stopChar) {
            break;
        }
    }

    return result;
}

/// \brief Подглядывание вплоть до указанного символа (не включая его).
/// \return Подсмотренная строка (возможно, пустая).
WideSpan TextNavigator::peekUntil(Char stopChar) const noexcept
{
    WideSpan result (this->ccurrent(), 0);
    const auto length = this->_length - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead (i);
        if (c == stopChar) {
            break;
        }
        ++result.length;
    }

    return result;
}

/// \brief Считывание строки.
/// \return Считанная строка (возможно, пустая).
WideSpan TextNavigator::readLine() noexcept
{
    WideSpan result (this->ccurrent(), 0);
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

    result = this->substr (start, this->_position - start);
    if (!this->eot()) {
        auto c = this->peekChar();
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

/// \brief Управляющий символ?
/// \return true, если управляющий символ.
bool TextNavigator::isControl() const noexcept
{
    const auto c = this->peekChar();
    return (c > 0) && (c < L' ');
}

/// \brief Цифра?
/// \return true, если цифра.
bool TextNavigator::isDigit() const noexcept
{
    const auto c = this->peekChar();
    return irbis::isDigit (c);
}

/// \brief Буква?
/// \return true, если буква.
bool TextNavigator::isLetter() const noexcept
{
    const Char c = this->peekChar();

    return std::iswalpha(c);
}

/// \brief Пробельный символ?
/// \return true, если пробельный символ.
bool TextNavigator::isWhitespace() const noexcept
{
    const Char c = this->peekChar();

    return std::iswspace(c);
}

/// \brief Извлечение целого числа без знака.
///
WideSpan TextNavigator::extractInteger() noexcept
{
    // Сначала пропускаем нечисловые символы
    while (!this->eot() && !this->isDigit()) {
        this->readChar();
    }
    return this->readInteger();
}

/// \brief Чтение целого числа без знака.
/// \return Прочитанная строка с числом (возможно, пустая).
WideSpan TextNavigator::readInteger() noexcept
{
    WideSpan result;
    result.ptr = const_cast<Char*> (this->_text + this->_position);
    while (this->isDigit()) {
        ++result.length;
        this->readChar();
    }

    return result;
}

/// \brief Чтение строки вплоть до указанной длины.
/// \param length Максимальная длина строки.
/// \return Прочитанная строка, возможно, пустая.
WideSpan TextNavigator::readString (std::size_t length) noexcept
{
    WideSpan result {this->_text + this->_position, 0 };
    if (this->eot()) {
        return result;
    }

    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->readChar();
        if (c == EOT) {
            break;
        }
        ++result.length;
    }

    return result;
}

/// \brief Считывание вплоть до указанного символа (не включая его).
/// \param stopChar Стоп-символ.
/// \return Прочитанная строка, возможно, пустую.
/// \details Сам стоп-символ считывается, но в результирующую строку не помещается.
WideSpan TextNavigator::readTo (Char stopChar) noexcept
{
    WideSpan result;
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

    result = this->substr (start, end - start);
    return result;
}

/// \brief Считывание вплоть до указанного символа (не включая его).
/// \param stopChar Стоп-символ.
/// \return Прочитанная строка, возможно, пустая.
/// \details Сам стоп-символ не считывается.
WideSpan TextNavigator::readUntil (Char stopChar) noexcept
{
    WideSpan result;
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

    result = this->substr (start, this->_position - start);
    return result;
}

/// \brief Считывание, пока встречается указанный символ.
/// \param goodChar "Хороший" символ.
/// \return Прочитанная строка, возможно, пустая.
WideSpan TextNavigator::readWhile (Char goodChar) noexcept
{
    WideSpan result {this->ccurrent(), 0 };
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

/// \brief Чтение слова.
/// \return Прочитанное слово (возможно, пустое).
WideSpan TextNavigator::readWord() noexcept
{
    WideSpan result (this->ccurrent(), 0);
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

/// \brief Получение непрочитанного остатка текста.
/// \return Непрочитанный фрагмент (возможно, пустой).
WideSpan TextNavigator::remainingText() const noexcept
{
    if (this->eot()) {
        return WideSpan { this->ccurrent(), 0 };
    }

    return this->substr(this->_position, this->_length - this->_position);
}

/// \brief Получение нескольких последних прочитанных символов.
/// \param length Сколько символов надо.
/// \return Полученные символы (возможно, пустой фрагмент).
WideSpan TextNavigator::recentText (std::ptrdiff_t length) const noexcept
{
    std::ptrdiff_t start = this->_position - length;
    if (start < 0) {
        length += start;
        start = 0;
    }

    if (static_cast<std::size_t>(start) >= this->_length) {
        start = 0;
        length = 0;
    }

    if (length < 0) {
        length = 0;
    }

    return this->substr (start, length);
}

/// \brief Пропуск символов, не входящих в слово.
/// \return this.
TextNavigator& TextNavigator::skipNonWord() noexcept
{
    while (!this->eot()) {
        const auto c = this->peekChar();
        if (std::iswalnum(c)) {
            break;
        }
        this->readChar();
    }

    return *this;
}

/// \brief Пропуск пробельных символов.
/// \return this.
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

/// \brief Пропуск пунктуации.
/// \return this.
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

/// \brief Извлечение подстроки.
/// \param offset Смещение, начиная с которого извлекается строка.
/// \param length Желаемая длина строки.
/// \return Полученная строка.
WideSpan TextNavigator::substr (std::ptrdiff_t offset, std::size_t length) const noexcept
{
    return {this->_text + offset, length };
}

}
