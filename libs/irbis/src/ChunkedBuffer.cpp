// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <algorithm>
#include <cstring>
#include <cassert>

namespace irbis {

/// \brief Конструктор.
/// \param size Размер куска.
ChunkedBuffer::Chunk::Chunk (const std::size_t size)
{
    assert (size > 0);
    data = static_cast<Byte*> (malloc (size));
    assert (data != nullptr);
    next = nullptr;
}

/// \brief Деструктор.
ChunkedBuffer::Chunk::~Chunk()
{
    free(data);
}

//=========================================================

/// \brief Размер куска по умолчанию.
const std::size_t ChunkedBuffer::DefaultChunkSize = 2048;

/// \brief Коструктор.
/// \param chunkSize Размер куска.
ChunkedBuffer::ChunkedBuffer (std::size_t chunkSize)
    : _first { nullptr }, _current { nullptr }, _last { nullptr },
      _chunkSize { chunkSize }, _position { 0 }, _read { 0 }
{
    assert (chunkSize > 0);
}

/// \brief Деструктор.
ChunkedBuffer::~ChunkedBuffer()
{
    auto *chunk = this->_first;
    while (chunk) {
        auto *next = chunk->next;
        chunk->~Chunk();
        chunk = next;
    }
}

bool ChunkedBuffer::_advance()
{
    if (this->_current == this->_last) {
        return false;
    }

    this->_current = this->_current->next;
    this->_read = 0;

    return true;
}

void ChunkedBuffer::_appendChunk()
{
    auto *newChunk = new Chunk (this->_chunkSize);
    if (this->_first) {
        this->_last->next = newChunk;
    } else {
        this->_first   = newChunk;
        this->_current = newChunk;
    }

    this->_last = newChunk;
    this->_position = 0;
}

/// \brief Буфер пуст?
/// \return true если пуст.
bool ChunkedBuffer::empty() const noexcept
{
    if (!this->_first) {
        return true;
    }
    if (this->_current == this->_first) {
        return this->_position == 0;
    }
    return false;
}

/// \brief Достигнут конец буфера?
/// \return true если достигнут конец.
bool ChunkedBuffer::eof() const noexcept
{
    if (!this->_current) {
        return true;
    }

    if (this->_current == this->_last) {
        return this->_read >= this->_position;
    }

    return false;
}

/// \brief Подглядывание следующего байта.
/// \return Подсмотренный байт либо -1.
int ChunkedBuffer::peek()
{
    if (!this->_current) {
        return -1;
    }

    if (this->_current == this->_last) {
        if (this->_read >= this->_position) {
            return -1;
        }
    } else {
        if (this->_read >= this->_chunkSize) {
            this->_advance();
        }
    }

    return this->_current->data [this->_read];
}

/// \brief Текущая позиция в буфере.
/// \return Смещение от начала в байтах.
std::size_t ChunkedBuffer::position() const noexcept
{
    std::size_t result = this->_read;
    for (Chunk *chunk = this->_first;
         (chunk != nullptr) && (chunk != this->_last);
         chunk = chunk->next) {
        result += this->_chunkSize;
    }
    return result;
}

/// \brief Чтение в указанный буфер.
/// \param buffer Буфер, в который должен быть помещен результат.
/// \param offset Смещение.
/// \param count Количество байт для чтения.
/// \return Количество прочитанных байт.
std::size_t ChunkedBuffer::read (Byte *buffer, std::size_t offset, std::size_t count)
{
    if (!count) {
        return 0;
    }

    if (!this->_current) {
        return 0;
    }

    std::size_t total = 0;
    do {
        const std::size_t remaining = this->_current == this->_last
                ? this->_position - this->_read
                : this->_chunkSize - this->_read;

        if (!remaining) {
            if (!this->_advance()) {
                break;
            }
        }

        const std::size_t portion = std::min (count, static_cast<std::size_t> (remaining));
        std::memcpy (buffer + offset, this->_current->data + this->_read, portion);
        this->_read += portion;
        count -= portion;
        total += portion;
    } while (count);

    return total;
}

/// \brief Чтение байта.
/// \return Прочитанный байт либо -1.
int ChunkedBuffer::readByte()
{
    if (!this->_current) {
        return -1;
    }

    if (this->_current == this->_last) {
        if (this->_read >= this->_position) {
            return -1;
        }
    } else {
        if (this->_read >= this->_chunkSize) {
            this->_advance();
        }
    }

    return this->_current->data [this->_read++];
}

/// \brief Чтение строки.
/// \return Строка в сыром виде.
Bytes ChunkedBuffer::readLine ()
{
    Bytes result;
    while (true) {
        auto c = this->readByte();
        if (c < 0) {
            break;
        }
        if (c == '\r') {
            if (this->peek() == '\n') {
                this->readByte();
            }
            break;
        }
        if (c == '\n') {
            break;
        }
        result.push_back (static_cast<Byte> (c));
    }

    return result;
}

/// \brief Чтение строки.
/// \param encoding Кодировка.
/// \return Прочитанную строку.
String ChunkedBuffer::readLine (Encoding *encoding)
{
    Bytes bytes = this->readLine();
    return encoding->toUnicode (bytes.data(), bytes.size());
}

/// \brief Получение непрочитанных байт одним большим куском.
/// \return Вектор непрочитанных байт.
/// \warning Позиция не сдвигается.
Bytes ChunkedBuffer::readRemaining () const
{
    Bytes result;
    Byte *data;
    result.reserve (this->remainingLength());
    if (this->_current) {
        if (this->_current == this->_last) {
            data = this->_current->data;
            result.insert
                (
                    std::end (result),
                    data + this->_read,
                    data + this->_position
                );
        }
        else {
            data = this->_current->data;
            result.insert
                (
                    std::end (result),
                    data + this->_read,
                    data + this->_chunkSize
                );
            for (Chunk *chunk = this->_current->next;
                chunk != this->_last;
                chunk = chunk->next) {
                result.insert
                    (
                        std::end (result),
                        chunk->data,
                        chunk->data + this->_chunkSize
                    );
            }
            data = this->_last->data;
            result.insert
                (
                    std::end (result),
                    data,
                    data + this->_position
                );
        }
    }

    return result;
}


/// \brief Количество оставшихся непрочитанными байт.
/// \return Количество байт.
std::size_t ChunkedBuffer::remainingLength () const noexcept
{
    std::size_t result { 0 };
    if (this->_current) {
        if (this->_current == this->_last) {
            result = this->_position - this->_read;
        }
        else {
            result += (this->_chunkSize - this->_read);
            for (Chunk *chunk = this->_current->next;
                 chunk != this->_last;
                 chunk = chunk->next) {
                result += this->_chunkSize;
            }
            result += this->_position;
        }
    }
    return result;
}

/// \brief Перемотка к началу.
void ChunkedBuffer::rewind()
{
    this->_current = this->_first;
    this->_read = 0;
}

/// \brief Общий размер данных, хранящихся в буфере.
/// \return Размер данных в байтах.
std::size_t ChunkedBuffer::size() const noexcept
{
    std::size_t result = this->_position;
    for (Chunk *chunk = this->_first;
        (chunk != nullptr) && (chunk != this->_last);
        chunk = chunk->next) {
        result += this->_chunkSize;
    }
    return result;
}

/// \brief Получение всех данных из буфера.
/// \return Все данные.
Bytes ChunkedBuffer::toBytes () const
{
    Bytes result;
    result.reserve (this->size());
    for (Chunk *chunk = this->_first;
         (chunk != nullptr) && (chunk != this->_last);
         chunk = chunk->next) {
        result.insert
            (
                std::end (result),
                chunk->data,
                chunk->data + this->_chunkSize
            );
    }
    if (this->_position) {
        Byte *data = this->_last->data;
        result.insert
            (
                std::end (result),
                data,
                data + this->_position
            );
    }
    return result;
}

/// \brief Запись буфера.
/// \param buffer Буфер.
/// \param offset Смещение.
/// \param count Количество байт для записи.
void ChunkedBuffer::write (const Byte *buffer, std::size_t offset, std::size_t count)
{
    if (!count) {
        return;
    }

    if (!this->_first) {
        this->_appendChunk();
    }

    do {
         std::size_t free = this->_chunkSize - this->_position;
         if (!free) {
             this->_appendChunk();
             free = this->_chunkSize;
         }

         const std::size_t portion = std::min (count, free);
         std::memcpy (this->_last->data + this->_position, buffer + offset, portion);
         this->_position += portion;
         count -= portion;
         offset += portion;
    } while (count);
}

/// \brief Запись одного байта.
/// \param value
void ChunkedBuffer::writeByte (Byte value)
{
    if (!this->_first) {
        this->_appendChunk();
    }

    if (this->_position >= this->_chunkSize) {
        _appendChunk();
    }

    this->_last->data [this->_position++] = value;
}

/// \brief Запись строки. Символ перевода строки НЕ добавляется.
/// \param text Текст для записи.
/// \param encoding Кодировка.
void ChunkedBuffer::writeLine (const String &text, Encoding *encoding)
{
    Bytes bytes = encoding->fromUnicode (text);
    this->write (bytes.data(), 0, bytes.size());
}

}
