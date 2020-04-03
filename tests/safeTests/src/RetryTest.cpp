// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("Retry_action_1", "[retry]")
{
    irbis::Retry retry;
    int counter { 0 };
    retry.action ([&] { ++counter; });
    CHECK (counter == 1);
}

TEST_CASE("Retry_action_2", "[retry]")
{
    int counter { 0 };
    irbis::Retry (10, 2).action ([&] { ++counter; });
    CHECK (counter == 1);
}

TEST_CASE("Retry_execute_1", "[retry]")
{
    irbis::Retry retry;
    const auto value = retry.execute ([] { return 123; });
    CHECK (value == 123);
}

TEST_CASE("Retry_execute_2", "[retry]")
{
    const auto value = irbis::Retry (10, 2).execute([] { return 123; });
    CHECK (value == 123);
}
