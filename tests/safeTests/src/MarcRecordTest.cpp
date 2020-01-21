#include "catch.hpp"
#include "irbis.h"

TEST_CASE("MarcRecord_constructor_1", "[record]")
{
    const irbis::MarcRecord record;
    CHECK(record.mfn == 0u);
    CHECK(record.status == 0u);
    CHECK(record.version == 0u);
    CHECK(record.database.empty());
    CHECK(record.fields.empty());
}

