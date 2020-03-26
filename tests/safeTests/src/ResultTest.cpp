// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

#include <functional>
#include <memory>
#include <utility>

TEST_CASE("Result_success_1", "[result]")
{
    auto r1 = irbis::Result <int>::Success (123);
    CHECK (r1.success);
    CHECK (r1);
    CHECK (r1.result == 123);
    CHECK ((int)r1 == 123);
}

TEST_CASE("Result_success_2", "[result]")
{
    auto r1 = irbis::Result <std::string>::Success ("123");
    CHECK (r1.success);
    CHECK (r1);
    CHECK (r1.result == "123");
    CHECK ((std::string)r1 == "123");
}

TEST_CASE("Result_success_3", "[result]")
{
    auto r1 = irbis::Result <irbis::String>::Success (L"123");
    CHECK (r1.success);
    CHECK (r1);
    CHECK (r1.result == L"123");
    CHECK ((irbis::String)r1 == L"123");
}

TEST_CASE("Result_success_4", "[result]")
{
    irbis::String text (L"123");
    auto r1 = irbis::Result <irbis::String>::Success (std::move (text));
    CHECK (r1.success);
    CHECK (r1);
    CHECK (r1.result == L"123");
    CHECK ((irbis::String)r1 == L"123");
}

TEST_CASE("Result_success_5", "[result]")
{
    irbis::MenuEntry entry (L"code", L"comment");
    auto r1 = irbis::Result <irbis::MenuEntry>::Success (std::move (entry));
    CHECK (r1.success);
    CHECK (r1);
    CHECK (r1.result.code == L"code");
    CHECK (r1.result.comment == L"comment");
}

TEST_CASE("Result_failure_1", "[result]")
{
    auto r1 = irbis::Result<int>::Failure (L"Can't do it");
    CHECK_FALSE (r1.success);
    CHECK_FALSE (r1);
    CHECK (r1.errorMessage == irbis::String (L"Can't do it"));
}

TEST_CASE("Result_failure_2", "[result]")
{
    auto r1 = irbis::Result<std::string>::Failure (L"Can't do it");
    CHECK_FALSE (r1.success);
    CHECK_FALSE (r1);
    CHECK (r1.errorMessage == irbis::String (L"Can't do it"));
}

TEST_CASE("Result_failure_3", "[result]")
{
    auto r1 = irbis::Result<irbis::String>::Failure (L"Can't do it");
    CHECK_FALSE (r1.success);
    CHECK_FALSE (r1);
    CHECK (r1.errorMessage == irbis::String (L"Can't do it"));
}

TEST_CASE("Result_failure_4", "[result]")
{
    auto r1 = irbis::Result<irbis::MenuEntry>::Failure (L"Can't do it");
    CHECK_FALSE (r1.success);
    CHECK_FALSE (r1);
    CHECK (r1.errorMessage == irbis::String (L"Can't do it"));
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
    CHECK (destructionCounter >= 2);
}

TEST_CASE("Result_resultOr_1", "[result]")
{
    auto r1 = irbis::Result<int>::Success (123);
    auto func = [] () { return 321; };
    CHECK (r1.resultOr (func) == 123);

    auto r2 = irbis::Result<int>::Failure (L"Failed");
    CHECK (r2.resultOr (func) == 321);
}

template <typename T>
using OptionalReference = irbis::Result<std::reference_wrapper<T>>;

using OptionalNarrow = OptionalReference<std::string>;
using OptionalWide = OptionalReference<irbis::String>;

static int globalCounter { 123456 };

static OptionalReference<int> func1()
{
    OptionalReference<int> result
    {
        true,
        std::ref (globalCounter),
        {}
    };
    return result;
}

static OptionalReference<int> func2()
{
    OptionalReference<int> result
    {
        false,
        std::ref (globalCounter),
        L"No reference"

    };
    return result;
}

TEST_CASE("Result_reference_1", "[result]")
{
    auto r1 = func1();
    CHECK (r1.success);
    CHECK (r1.result.get() == globalCounter);

    auto r2 = func2();
    CHECK_FALSE (r2.success);
}
