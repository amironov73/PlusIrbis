// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

MemoryChunk::MemoryChunk(const size_t size)
{
    data = static_cast<char*>(malloc(size));
    next = nullptr;
}

MemoryChunk::~MemoryChunk()
{
    free(data);
}

NAMESPACE_IRBIS_END
