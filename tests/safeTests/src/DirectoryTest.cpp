// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "bugprone-infinite-loop"

TEST_CASE("Directory_read_1", "[directory]")
{
    auto path = irbis::toUtf (whereTestData());
    path = irbis::IO::convertSlashes (path);
    irbis::Directory directory (path);
    const auto allFiles = directory.readAll();
    CHECK (allFiles.size() == 18);

    const auto found = directory.find ("test1.iso");
    CHECK (found == "TEST1.ISO");
}
