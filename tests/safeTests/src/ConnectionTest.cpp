// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

TEST_CASE("ConnectionBase_constructor_1", "[connection]")
{
    irbis::ConnectionBase connection;
    CHECK (connection.lastError == 0);
    CHECK_FALSE (connection.connected());
}
