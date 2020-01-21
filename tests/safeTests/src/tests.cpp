#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "irbis.h"

TEST_CASE("libraryVersion_1", "[version]")
{
    REQUIRE(irbis::libraryVersion() != 0);
}

TEST_CASE("libraryVersionString_1", "[version]")
{
    REQUIRE(!irbis::libraryVersionString().empty());
}
