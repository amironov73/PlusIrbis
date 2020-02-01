// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Result_success_1", "[result]")
{
    auto r1 = irbis::Result<int>::Success(123);
    CHECK(r1.success);
    CHECK(123 == r1.result);
}

TEST_CASE("Result_failure_1", "[result]")
{
    auto r1 = irbis::Result<int>::Failure(L"Can't do it");
    CHECK_FALSE(r1.success);
    CHECK(irbis::String(L"Can't do it") == r1.errorMessage);
}

TEST_CASE("Result_operator_1", "[result]")
{
    auto r1 = irbis::Result<int>::Success(123);
    if (!r1)
    {
        FAIL(L"Can't be!");
    }
    const int n1 = r1;
    CHECK(123 == n1);
}
