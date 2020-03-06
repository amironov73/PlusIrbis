// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cstdlib>
#include <cassert>

namespace irbis {

static MemoryPool *_instance;

/// \brief Глобальный пул.
/// \return Ссылка на глобальный пул.
MemoryPool& MemoryPool::instance()
{
    if (!_instance) {
        _instance = new MemoryPool ();
    }
    return *_instance;
}

/// \brief Запрос блока определенного размера.
/// \param size Размер блока в байтах.
/// \return Блок из пула.
MemoryBlock MemoryPool::rent (std::size_t size)
{
    assert (size > 0);
    // TODO implement
    MemoryBlock result
    {
        ::malloc (size),
        size
    };
    this->_used.push_front (result);
    return result;
}

/// \brief Возврат блока в пул.
/// \param block Возвращаемый блок.
void MemoryPool::release (MemoryBlock block)
{
    // TODO implement
    // ::free (block.pointer);
    this->_free.push_back (block);
}

/// \brief Возврат пула в начальное состояние.
void MemoryPool::reset ()
{
    for (const auto &block : this->_free) {
        ::free (block.pointer);
    }
}

}

