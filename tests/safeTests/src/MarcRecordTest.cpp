// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("MarcRecord_constructor_1", "[record]")
{
    const irbis::MarcRecord record;
    CHECK(record.mfn == 0u);
    CHECK(record.status == irbis::RecordStatus::None);
    CHECK(record.version == 0u);
    CHECK(record.database.empty());
    CHECK(record.fields.empty());
}

