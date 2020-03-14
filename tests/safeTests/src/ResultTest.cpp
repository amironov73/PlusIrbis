// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Result_success_1", "[result]")
{
    auto r1 = irbis::Result<int>::Success (123);
    CHECK (r1.success);
    CHECK (r1);
    CHECK (r1.result == 123);
    CHECK ((int)r1 == 123);
}

TEST_CASE("Result_failure_1", "[result]")
{
    auto r1 = irbis::Result<int>::Failure (L"Can't do it");
    CHECK_FALSE (r1.success);
    CHECK_FALSE (r1);
    CHECK (r1.errorMessage == irbis::String(L"Can't do it"));
}

TEST_CASE("Result_operator_1", "[result]")
{
    auto r1 = irbis::Result<int>::Success (123);
    if (!r1)
    {
        FAIL (L"Can't be!");
    }
    const int n1 = r1;
    CHECK (n1 == 123);
}

TEST_CASE("Result_operator_2", "[result]")
{
    auto r1 = irbis::Result<int>::Success (123);
    auto r2 = r1;
    CHECK (r2.success);
    CHECK (r2);
    CHECK (r2.result == 123);
    CHECK ((int)r2 == 123);
}

struct SomeStruct
{
    int x { 0 }, y { 0 };
    SomeStruct() = default;
    SomeStruct (int x_, int y_) : x (x_), y (y_) {}
};

TEST_CASE("makeResult_1", "[result]")
{
    auto r1 = irbis::makeResult<SomeStruct> (1, 2);
    CHECK (r1.success);
    CHECK (r1.result.x == 1);
    CHECK (r1.result.y == 2);
}

static int destructionCounter = 0;

struct Destructible
{
    int value { 0 };
    Destructible() : value(1) {}
    ~Destructible() { ++destructionCounter; }
};

TEST_CASE("Result_destruction_1", "[result]")
{
    {
        auto r1 = irbis::Result<Destructible>::Success (std::move (Destructible()));
        CHECK (r1.success);
        CHECK (r1.result.value == 1);
    }
    CHECK (destructionCounter == 3);
}

TEST_CASE("Result_resultOr_1", "[result]")
{
    auto r1 = irbis::Result<int>::Success (123);
    auto func = [] () { return 321; };
    CHECK (r1.resultOr (func) == 123);

    auto r2 = irbis::Result<int>::Failure (L"Failed");
    CHECK (r2.resultOr (func) == 321);
}
