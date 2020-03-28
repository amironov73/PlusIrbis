// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

namespace irbis {

/// \brief Первоначальная настройка.
/// \param str Префиксная часть.
/// \param number Значение.
/// \return Признак успешности.
bool NumberText::Chunk::setUp (const String &str, const String &number)
{
    auto result = !str.empty();
    this->prefix = str;
    if (number.empty()) {
        this->haveValue = false;
        this->value = 0;
    }
    else {
        result = true;
        this->haveValue = true;
        this->value = fastParse64 (number);
    }
    this->length = 0;
    return result;
}

/// \brief Сравнение с другим фрагментом.
/// \param other Фрагмент для сравнения.
/// \return Результат сравнения: меньше нуля, если данный фрагмент меньше.
int NumberText::Chunk::compareTo (const Chunk &other) const noexcept
{
    int result = prefix.compare (other.prefix);
    if (!result) {
        result = this->haveValue && other.haveValue
                ? sign (this->value - other.value)
                : this->haveValue - other.haveValue;
    }
    return result;
}

/// \brief Фрагмент содержит префикс?
/// \return true, если содержит.
bool NumberText::Chunk::havePrefix() const noexcept
{
    return !this->prefix.empty();
}

/// \brief Преобразование в текст.
String NumberText::Chunk::toString() const
{
    auto result = this->prefix;
    if (this->haveValue) {
        // TODO: handle this->length
        result.append (std::to_wstring (this->value));
    }
    return result;
}

//=========================================================

/// \brief Пустой текст?
/// \return true если пустой.
bool NumberText::empty() const noexcept
{
    return this->chunks.empty();
}

/// \brief Количество фрагментов.
/// \return Количество фрагментов.
std::size_t NumberText::size() const noexcept
{
    return this->chunks.size();
}

/// \brief Номер последнего сегмента.
/// \return Номер последнего сегмента.
std::size_t NumberText::lastIndex() const noexcept
{
    return this->size() - 1;
}

/// \brief Разбор текста.
/// \param text Текс для разбора.
/// \return Набор фрагментов.
NumberText NumberText::parse (const String &text)
{
    NumberText result {};
    if (text.empty()) {
        return result;
    }

    Chunk chunk {};
    String str {};
    String number {};
    bool textPart { true };
    for (Char c : text) {
        if (textPart) {
            if (isDigit (c)) {
                number.push_back (c);
                textPart = false;
            }
            else {
                str.push_back (c);
            }
        }
        else {
            if (isDigit(c)) {
                number.push_back (c);
            }
            else {
                if (chunk.setUp (str, number)) {
                    result.chunks.push_back (chunk);
                }
                chunk = Chunk {};
                str.clear();
                str.push_back (c);
                number.clear();
                textPart = true;
            }
        }
    }

    if (chunk.setUp (str, number)) {
        result.chunks.push_back (chunk);
    }

    return result;
}

/// \brief Инкремент указанного фрагмента.
/// \param index Индекс фрагмента. Отрицательное число -- позиция с конца.
/// \param delta Величина инкремента.
/// \return this.
NumberText& NumberText::increment (std::ptrdiff_t index, int64_t delta)
{
    if (index < 0) {
        index = this->chunks.size() + index;
    }

    auto &chunk = this->chunks[index];
    chunk.value += delta;

    return *this;
}

/// \brief Добавление пустого фрагмента.
/// \return Ссылка на добавленный фрагмент.
NumberText::Chunk& NumberText::append()
{
    NumberText::Chunk chunk {};
    this->chunks.push_back (chunk);
    return this->chunks.back();
}


/// \brief Добавление фрагмента в конец.
/// \param prefix_ Префикс.
/// \param value_ Число.
/// \param length_ Длина.
/// \return this.
NumberText& NumberText::append (const String &prefix_, int64_t value_, int length_)
{
    auto &chunk = this->append();
    chunk.prefix = prefix_;
    chunk.value = value_;
    chunk.length = length_;
    chunk.haveValue = true;
    return *this;
}

/// \brief Добавление фрагмента в конец.
/// \param prefix Префикс.
/// \return this.
NumberText& NumberText::append (const String &prefix)
{
    auto &chunk = this->append();
    chunk.prefix = prefix;
    chunk.value = 0;
    chunk.haveValue = false;
    chunk.length = 0;
    return *this;
}

/// \brief Добавление фрагмента в конец.
/// \param value_ Число.
/// \param length_ Длина.
/// \return this.
NumberText& NumberText::append (int64_t value_, int length_)
{
    auto &chunk = this->append();
    chunk.value = value_;
    chunk.haveValue = true;
    chunk.length = length_;
    return *this;
}

/// \brief Преобразование в строку.
/// \return Полученная строка.
String NumberText::toString () const
{
    String result;
    for (const auto &chunk : this->chunks) {
        result.append (chunk.toString());
    }

    return result;
}

/// \brief Последний фрагмент.
/// \return Ссылка на последний фрагмент.
NumberText::Chunk& NumberText::back () const noexcept
{
    return const_cast<NumberText::Chunk&> (this->chunks.back());
}

/// \brief Первый фрагмент.
/// \return Ссылка на первый фрагмент.
NumberText::Chunk& NumberText::front () const noexcept
{
    return const_cast<NumberText::Chunk&> (this->chunks.front());
}

/// \brief Сравнение с другим текстом.
/// \param other Другой текст.
/// \return -1, 0 или 1.
int NumberText::compareTo (const NumberText &other) const
{
    const auto thisEnd = std::end (this->chunks);
    const auto otherEnd = std::end (other.chunks);
    auto thisPtr = std::begin (this->chunks);
    auto otherPtr = std::begin (other.chunks);
    while (thisPtr != thisEnd) {
        if (otherPtr != otherEnd) {
            int result = thisPtr->compareTo (*otherPtr);
            if (result) {
                return result;
            }
            ++otherPtr;
        }
        else {
            return -1;
        }
        ++thisPtr;
    }
    if (otherPtr != otherEnd) {
        return -1;
    }
    else {
        return 0;
    }
}

}
