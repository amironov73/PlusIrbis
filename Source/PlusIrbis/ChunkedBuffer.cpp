// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include <cstring>

NAMESPACE_IRBIS_BEGIN

const size_t ChunkedBuffer::DefaultChunkSize = 2048;

ChunkedBuffer::ChunkedBuffer(size_t chunkSize)
    : _first(nullptr), _current(nullptr), _last(nullptr),
      _chunkSize(chunkSize), _position(0), _read(0) {
}

ChunkedBuffer::~ChunkedBuffer() {
    MemoryChunk *chunk = _first;
    while (chunk) {
        MemoryChunk *next = chunk->next;
        chunk->~MemoryChunk();
        chunk = next;
    }
}

bool ChunkedBuffer::_advance() {
    if (_current == _last) {
        return false;
    }

    _current = _current->next;
    _read = 0;

    return true;
}

void ChunkedBuffer::_appendChunk() {
    auto *newChunk = new MemoryChunk(_chunkSize);
    if (_first) {
        _last->next = newChunk;
    } else {
        _first = newChunk;
        _current = newChunk;
    }

    _last = newChunk;
    _position = 0;
}

constexpr bool ChunkedBuffer::eof() const {
    if (!_current) {
        return true;
    }

    if (_current == _last) {
        return _read >= _position;
    }

    return false;
}

int ChunkedBuffer::peek() {
    if (!_current) {
        return -1;
    }

    if (_current == _last) {
        if (_read >= _position) {
            return -1;
        }
    } else {
        if (_read >= _chunkSize) {
            _advance();
        }
    }

    return _current->data[_read];
}

size_t ChunkedBuffer::read(char *buffer, size_t offset, size_t count) {
    if (!count) {
        return 0;
    }

    if (!_current) {
        return 0;
    }

    size_t total = 0;
    do {
        const size_t remaining = _current == _last
                ? _position - _read
                : _chunkSize - _read;

        if (!remaining) {
            if (!_advance()) {
                break;
            }
        }

        const size_t portion = std::min(count, static_cast<size_t>(remaining));
        std::memcpy(buffer + offset, _current->data + _read, portion);
        _read += portion;
        count -= portion;
        total += portion;
    } while (count);

    return total;
}

int ChunkedBuffer::readByte() {
    if (!_current) {
        return -1;
    }

    if (_current == _last) {
        if (_read >= _position) {
            return -1;
        }
    } else {
        if (_read >= _chunkSize) {
            _advance();
        }
    }

    return _current->data[_read++];
}

void ChunkedBuffer::rewind() {
    _current = _first;
    _read = 0;
}

void ChunkedBuffer::write(const char *buffer, size_t offset, size_t count) {
    if (!count) {
        return;
    }

    if (!_first) {
        _appendChunk();
    }

    do {
         size_t free = _chunkSize - _position;
         if (!free) {
             _appendChunk();
             free = _chunkSize;
         }

         const size_t portion = std::min(count, free);
         std::memcpy(_last->data + _position, buffer + offset, portion);
         _position += portion;
         count -= portion;
         offset += portion;
    } while (count);
}

void ChunkedBuffer::writeByte(char value) {
    if (!_first) {
        _appendChunk();
    }

    if (_position >= _chunkSize) {
        _appendChunk();
    }

    _last->data[_position++] = value;
}

NAMESPACE_IRBIS_END
