// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cctype>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

// ReSharper disable CommentTypo
//-V::801

/*!
    \class irbis::ByteNavigator

    \warning Служебный класс.
    Предназначен для поддержки инфраструктуры `PlusIrbis`.
 */

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
ByteNavigator::ByteNavigator (ByteSpan data) noexcept
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
int ByteNavigator::at (std::size_t position) const noexcept
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
    return this->_data.slice (this->_position);
}

/// \brief Первый байт в диапазоне.
/// \return Прочитанный байт либо `EOT`.
int ByteNavigator::front() const noexcept
{
    return this->at (0);
}

/// \brief Последний байт в диапазоне.
/// \brief Прочитанный байт либо `EOT`.
int ByteNavigator::back() const noexcept
{
    return this->at (this->size() - 1);
}

/// \brief Заглядывание вперед на указанное количество байтов.
/// \param distance На сколько байтов заглянуть?
/// \return Прочитанный байт либо `EOT`.
int ByteNavigator::lookAhead (std::ptrdiff_t distance) const noexcept
{
    const auto newPosition = this->_position + distance;
    return this->at (newPosition);
}

/// \brief Заглядывание назад на указанное количество байтов.
/// \param distance На сколько байтов заглянуть?
/// \return Прочитанный байт либо `EOT`.
int ByteNavigator::lookBehind (std::ptrdiff_t distance) const noexcept
{
    const auto newPosition = this->_position - distance;
    return this->at(newPosition);
}

/// \brief Перемещение по диапазону вперёд/назад.
/// \param distance На сколько байтов переместиться?
/// \return `this`.
ByteNavigator& ByteNavigator::move (std::ptrdiff_t distance) noexcept
{
    this->_position += distance;
    //if (this->_position < 0) {
    //    this->_position = 0;
    //}
    return *this;
}

/// \brief Подсматриваем текущий байт.
/// \return Прочитанный байт либо `EOT`.
int ByteNavigator::peekByte() const noexcept
{
    return this->at (this->_position);
}

/// \brief Считываем текущий байт. Двигаемся вперёд по диапазону.
/// \return Прочитанный байт либо `EOT`.
int ByteNavigator::readByte() noexcept
{
    return this->eot() ? EOT : this->at (this->_position++);
}

/// \brief Записывает байт по текущему смещению.
/// \param byte Байт для записи.
/// \return Признак успешности завершения операции.
bool ByteNavigator::writeByte (Byte byte) noexcept
{
    if (this->eot()) {
        return false;
    }
    this->_data[this->_position++] = byte;
    return true;
}

/// \brief Записывает символ в кодировке UCS2 (Little Endian).
/// \param c Символ для записи.
/// \return Признак успешности завершения операции.
bool ByteNavigator::writeWideLE (Char c) noexcept
{
    if ((this->_position + 1) >= this->_data.size()) {
        return false;
    }

    auto ptr = this->current();
    *ptr++ = static_cast<Byte> (c & 0xFFu);
    *ptr = static_cast<Byte > ((c & 0xFF00u) >> 8u);
    this->_position += 2;
    return true;
}

/// \brief Записывает символ в кодировке UCS2 (Big Endian).
/// \param c Символ для записи.
/// \return Признак успешности завершения операции.
bool ByteNavigator::writeWideBE (Char c) noexcept
{
    if ((this->_position + 1) >= this->_data.size()) {
        return false;
    }

    auto ptr = this->current();
    *ptr++ = static_cast<Byte> ((c & 0xFF00u) >> 8u);
    *ptr = static_cast<Byte > (c & 0xFFu);
    this->_position += 2;
    return true;
}

/// \brief Подглядывание строки вплоть до указанной длины.
/// \param length На сколько байт вперёд заглядывать.
/// \return Что удалось прочитать.
ByteSpan ByteNavigator::peekString (std::size_t length) const noexcept
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

/// \brief Подглядывание вплоть до указанного байта (включая его).
/// \param stop Стоп-байт.
/// \return Что удалось прочитать.
ByteSpan ByteNavigator::peekTo (Byte stop) const noexcept
{
    ByteSpan result (this->ccurrent(), 0);
    const auto length = this->size() - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead (i);
        ++result.length;
        if (c == stop) {
            break;
        }
    }
    return result;
}

/// \brief Подглядывание вплоть до указанного байта (не включая его).
/// \param stop Стоп-байт.
/// \return Что удалось прочитать.
ByteSpan ByteNavigator::peekUntil (Byte stop) const noexcept
{
    ByteSpan result (this->ccurrent(), 0);
    const auto length = this->size() - this->_position;
    for (std::size_t i = 0; i < length; ++i) {
        const auto c = this->lookAhead (i);
        if (c == stop) {
            break;
        }
        ++result.length;
    }
    return result;
}

/// \brief Считывание строки (до символа `\n`).
/// \return Что удалось прочитать.
///
/// Символ перевода строки считывается,
/// но в результирующую строку не попадает.
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

    result = this->slice (start, this->_position - start);
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
/// \return `true` если да.
bool ByteNavigator::isControl() const noexcept
{
    const auto c = this->peekByte();
    return (c > 0) && (c < ' ');
}

/// \brief Цифра?
/// \return `true` если да.
bool ByteNavigator::isDigit() const noexcept
{
    const auto c = this->peekByte();
    return std::isdigit (c);
}

/// \brief Буква?
/// \return `true` если да.
bool ByteNavigator::isLetter() const noexcept
{
    const auto c = this->peekByte();
    return std::isalpha (c);
}

/// \brief Пробельный символ?
/// \return `true` если да.
bool ByteNavigator::isWhitespace() const noexcept
{
    const auto c = this->peekByte();
    return std::isspace (c);

}

/// \brief Считывание целого без знака.
/// \return Что удалось прочитать (возможно, пустой диапазон).
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

/// \brief Считывание строки вплоть до указанной длины.
/// \param length Максимальная длина считываемой строки.
/// \return Что удалось прочитать (возможно, пустой диапазон).
///
/// В результирующую строку помещаются все прочитанные байты,
/// в т. ч. перевод строки и прочие служебные символы.
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

/// \brief Считывание вплоть до указанного байта (включая его).
/// \param stopByte Стоп-байт.
/// \return Что удалось прочитать (возможно, пустой диапазон).
ByteSpan ByteNavigator::readTo (Byte stopByte) noexcept
{
    if (this->eot()) {
        return {};
    }
    ByteSpan result {this->ccurrent(), 0 };
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

/// \brief Считывание вплоть до указанного байта (не включая его).
/// \param stopByte Стоп-байт.
/// \return Что удалось прочитать (возможно, пустой диапазон).
ByteSpan ByteNavigator::readUntil (Byte stopByte) noexcept
{
    if (this->eot()) {
        return {};
    }
    ByteSpan result {this->ccurrent(), 0 };
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
        if ((c == EOT) || !std::isalnum (c)) {
            break;
        }
        ++result.length;
        this->readByte();
    }
    return result;
}

/// \brief Получаем последние несколько байтов.
/// \param length Сколько байтов надо (в макисмуме).
/// \return Полученные байты (сколько удалось получить).
ByteSpan ByteNavigator::recent (std::ptrdiff_t length) const noexcept
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

/// \brief Пропускаем пробельные символы, если есть.
/// \return `this`.
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

/// \brief Пропускаем знаки препинания, если есть.
/// \return `this`.
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
/// \return `this`.
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

/// \brief Подглядывание одного символа в кодировке UTF-8.
/// \return Прочитанный символ либо `EOT`.
int ByteNavigator::peekUtf() const noexcept
{
    if (this->eot()) {
        return EOT;
    }

    auto ptr = this->ccurrent();
    unsigned int c = *ptr++;
    if ((c & 0x80u) == 0u)
    {
        // 1-Byte sequence: 000000000xxxxxxx = 0xxxxxxx
    }
    else if ((c & 0xE0u) == 0xC0u)
    {
        if (this->_position + 1 >= this->_data.size()) {
            return EOT;
        }
        // 2-Byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
        c = (c & 0x1Fu) << 6u;
        c |= (*ptr & 0x3Fu);
    }
    else if ((c & 0xF0u) == 0xE0u)
    {
        if (this->_position + 2 >= this->_data.size()) {
            return EOT;
        }
        // 3-Byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
        c = (c & 0x0Fu) << 12u;
        c |= (*ptr++ & 0x3Fu) << 6u;
        c |= (*ptr & 0x3Fu);
    }
    else if ((c & 0xF8u) == 0xF0u)
    {
        if (this->_position + 3 >= this->_data.size()) {
            return EOT;
        }
        // 4-Byte sequence: 11101110wwwwzzzzyy + 110111yyyyxxxxxx = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
        c = (c & 0x07u) << 18u;
        c |= (*ptr++ & 0x3Fu) << 12u;
        c |= (*ptr++ & 0x3Fu) << 6u;
        c |= (*ptr & 0x3Fu);
    }

    return static_cast<int>(c);
}

/// \brief Считывание одного символа в кодировке UTF-8.
/// \return Прочитанный символ либо `EOT`.
int ByteNavigator::readUtf() noexcept
{
    if (this->eot()) {
        return EOT;
    }

    auto ptr = this->ccurrent();
    unsigned int c = *ptr++;
    ++this->_position;
    if ((c & 0x80u) == 0u)
    {
        // 1-Byte sequence: 000000000xxxxxxx = 0xxxxxxx
    }
    else if ((c & 0xE0u) == 0xC0u)
    {
        if (this->_position >= this->_data.size()) {
            return EOT;
        }
        // 2-Byte sequence: 00000yyyyyxxxxxx = 110yyyyy 10xxxxxx
        c = (c & 0x1Fu) << 6u;
        c |= (*ptr & 0x3Fu);
        ++this->_position;
    }
    else if ((c & 0xF0u) == 0xE0u)
    {
        if (this->_position + 1 >= this->_data.size()) {
            return EOT;
        }
        // 3-Byte sequence: zzzzyyyyyyxxxxxx = 1110zzzz 10yyyyyy 10xxxxxx
        c = (c & 0x0Fu) << 12u;
        c |= (*ptr++ & 0x3Fu) << 6u;
        c |= (*ptr & 0x3Fu);
        this->_position += 2;
    }
    else if ((c & 0xF8u) == 0xF0u)
    {
        if (this->_position + 2 >= this->_data.size()) {
            return EOT;
        }
        // 4-Byte sequence: 11101110wwwwzzzzyy + 110111yyyyxxxxxx = 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
        c = (c & 0x07u) << 18u;
        c |= (*ptr++ & 0x3Fu) << 12u;
        c |= (*ptr++ & 0x3Fu) << 6u;
        c |= (*ptr & 0x3Fu);
        this->_position += 3;
    }

    return static_cast<int>(c);
}

/// \brief Запись символа в кодировке UTF-8.
/// \param c Символ для записи.
/// \return Признак успешности завершения операции.
bool ByteNavigator::writeUtf (unsigned int c) noexcept
{
    if (this->eot()) {
        return false;
    }

    auto dst = this->current();
    if (c < (1u << 7u)) {
        *dst = static_cast<Byte>(c);
        ++this->_position;
    }
    else if (c < (1u << 11u)) {
        *dst++ = static_cast<Byte>((c >> 6u) | 0xC0u);
        *dst = static_cast<Byte>((c & 0x3Fu) | 0x80u);
        this->_position += 2;
    }
    else if (c < (1u << 16u)) { //-V547
        *dst++ = static_cast<Byte>((c >> 12u) | 0xE0u);
        *dst++ = static_cast<Byte>(((c >> 6u) & 0x3Fu) | 0x80u);
        *dst = static_cast<Byte>((c & 0x3Fu) | 0x80u);
        this->_position += 3;
    }
    else if (c < (1u << 21u)) {
        *dst++ = static_cast<Byte>((c >> 18u) | 0xF0u);
        *dst++ = static_cast<Byte>(((c >> 12u) & 0x3Fu) | 0x80u);
        *dst++ = static_cast<Byte>(((c >> 6u) & 0x3Fu) | 0x80u);
        *dst = static_cast<Byte>((c & 0x3Fu) | 0x80u);
        this->_position += 4;
    }
    return true;
}

/// \brief Запись строки в кодировке UTF-8.
/// \param text Текст для записи.
/// \return Признак успешности завершения операции.
bool ByteNavigator::writeUtf (WideSpan text) noexcept //-V813
{
    for (const Char c : text) {
        if (!this->writeUtf(c)) {
            return false;
        }
    }
    return true;
}

bool ByteNavigator::writeWideLE (WideSpan text) noexcept //-V813
{
    for (const Char c : text) {
        if (!this->writeWideLE(c)) {
            return false;
        }
    }
    return true;
}

bool ByteNavigator::writeWideBE (WideSpan text) noexcept //-V813
{
    for (const Char c : text) {
        if (!this->writeWideBE(c)) {
            return false;
        }
    }
    return true;
}

int ByteNavigator::peekWideLE() const noexcept
{
    if (this->_position + 1 >= this->_data.size()) {
        return EOT;
    }
    const auto ptr = this->ccurrent();
    return ((ptr[1] << 8u) + ptr[0]);
}

int ByteNavigator::readWideLE() noexcept
{
    if (this->_position + 1 >= this->_data.size()) {
        return EOT;
    }
    const auto ptr = this->ccurrent();
    this->_position += 2;
    return ((ptr[1] << 8u) + ptr[0]);
}

int ByteNavigator::peekWideBE() const noexcept
{
    if (this->_position + 1 >= this->_data.size()) {
        return EOT;
    }
    const auto ptr = this->ccurrent();
    return ((ptr[0] << 8u) + ptr[1]);
}

int ByteNavigator::readWideBE() noexcept
{
    if (this->_position + 1 >= this->_data.size()) {
        return EOT;
    }
    const auto ptr = this->ccurrent();
    this->_position += 2;
    return ((ptr[0] << 8u) + ptr[1]);
}

/// \brief Выделение поддиапазона.
/// \param offset Индекс начала поддиапазона, байты.
/// \param length Длина поддиапазона, байты.
/// \return Поддиапазон.
ByteSpan ByteNavigator::slice (std::ptrdiff_t offset, std::size_t length) const noexcept
{
    if (this->size() == 0) {
        return {};
    }

    // TODO some checks
    return {this->cbegin() + offset, length };
}

/// \brief Заполнение диапазона указанным значением.
/// \param value Байт-заполнитель.
/// \return `this`.
ByteNavigator& ByteNavigator::fill (Byte value) noexcept
{
    this->_data.fill (value);
    return *this;
}

/// \brief Поиск указанного байта, начиная с текущей позиции.
/// \param value Искомый байт.
/// \return Смещение в байтах от текущей позиции найденного байта, либо `EOT`, если байт не найден.
int ByteNavigator::find (Byte value) const noexcept
{
    if (this->eot()) {
        return EOT;
    }

    for (auto ptr = this->ccurrent(); ptr < this->cend(); ++ptr) {
        if (*ptr == value) {
            return static_cast<int> (ptr - this->ccurrent());
        }
    }

    return EOT;
}

/// \brief Поиск указанной последовательности байтов, начиная с текущей позиции.
/// \param array Искомая последовательность байтов.
/// \return Смещение в байтах от текущей позиции начала найденной позиции, либо `EOT`, если последовательность не найдена.
int ByteNavigator::find (ByteSpan array) const noexcept //-V813
{
    if (this->eot() || array.empty()) {
        return EOT;
    }

    for (auto ptr = this->ccurrent(); ptr < this->cend(); ++ptr) {
        bool found = true;
        for (std::size_t offset = 0; offset < array.size(); ++offset) {
            if (ptr[offset] != array[offset]) {
                found = false;
                break;
            }
        }
        if (found) {
            return static_cast<int> (ptr - this->ccurrent());
        }
    }

    return EOT;
}

}
