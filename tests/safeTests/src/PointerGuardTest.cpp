// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"
#include "cstring"

// ReSharper disable StringLiteralTypo

TEST_CASE("PointerGuard_1", "[pointer]")
{
    irbis::PointerGuard<int> guard (new int[10]);
    memset (guard.pointer(), 0, sizeof(int) * 10);
    guard[0] = 1;
    guard[1] = 2;
    CHECK (guard[0] == 1);
    CHECK (guard[1] == 2);
}