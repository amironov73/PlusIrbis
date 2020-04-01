// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("MemoryPool_instance_1", "[memoryPool]")
{
    auto pool = irbis::MemoryPool::instance();
    auto block = pool.rent(100);
    CHECK (block.pointer != nullptr);
    CHECK (block.size >= 100);
    pool.release (block);
}
