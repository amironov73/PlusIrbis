// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

TEST_CASE("IlfEntry_constructor_1", "[ilf]")
{
    irbis::IlfEntry entry {};
    CHECK (entry.date.empty());
    CHECK (entry.name.empty());
    CHECK (entry.description.empty());
    CHECK (entry.data.empty());
    CHECK (entry.position   == 0);
    CHECK (entry.number     == 0);
    CHECK (entry.dataLength == 0);
    CHECK (entry.flags      == 0);
    CHECK_FALSE (entry.deleted);
}

//=========================================================

TEST_CASE("IlfFile_constructor_1", "[ilf]")
{
    irbis::IlfFile file {};
    CHECK (file.entries.empty());
    CHECK (file.unknown1   == 0);
    CHECK (file.slotCount   == 0);
    CHECK (file.entryCount  == 0);
    CHECK (file.writeCount  == 0);
    CHECK (file.deleteCount == 0);
}
