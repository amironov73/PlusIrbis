// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

#include <sstream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "bugprone-infinite-loop"
#pragma ide diagnostic ignored "bugprone-use-after-move"

TEST_CASE("Optional_constructor_1", "[optional]")
{
    irbis::Optional<int> opt;
    CHECK_FALSE (opt.hasValue);
    CHECK_FALSE ((bool)opt);
}

TEST_CASE("Optional_constructor_2", "[optional]")
{
    irbis::Optional<int> opt (5);
    CHECK (opt.hasValue);
    CHECK ((bool)opt);
    CHECK (opt.value == 5);
    CHECK (*opt == 5);
}

TEST_CASE ("Optional_constructor_3", "[optional]")
{
    irbis::Optional<int> opt1 (5);
    irbis::Optional<int> opt2 (opt1);
    CHECK (opt1.hasValue == opt2.hasValue);
    CHECK (opt1.value == opt2.value);
}

TEST_CASE ("Optional_constructor_4", "[optional]")
{
    irbis::Optional<int> opt1 (5);
    CHECK (opt1.hasValue);
    irbis::Optional<int> opt2 (std::move (opt1));
    CHECK_FALSE (opt1.hasValue);
    CHECK (opt2.hasValue);
}

TEST_CASE ("Optional_assignment_1", "[optional]")
{
    irbis::Optional<int> opt;
    opt = 5;
    CHECK (opt.hasValue);
    CHECK (opt.value == 5);
}

TEST_CASE ("Optional_assignment_2", "[optional]")
{
    irbis::Optional<int> opt1 (5);
    irbis::Optional<int> opt2 (opt1);
    CHECK (opt1.hasValue == opt2.hasValue);
    CHECK (opt1.value == opt2.value);
}

TEST_CASE ("Optional_assignment_3", "[optional]")
{
    irbis::Optional<int> opt1 (5);
    irbis::Optional<int> opt2  = opt1;
    CHECK (opt1.hasValue == opt2.hasValue);
    CHECK (opt1.value == opt2.value);
}

TEST_CASE ("Optional_assignment_4", "[optional]")
{
    irbis::Optional<int> opt1 (5);
    irbis::Optional<int> opt2;
    opt2 = opt1;
    CHECK (opt1.hasValue == opt2.hasValue);
    CHECK (opt1.value == opt2.value);
}

TEST_CASE("Optional_assignment_5", "[optional]")
{
    irbis::Optional<int> opt (5);
    CHECK (opt.hasValue);
    opt = nullptr;
    CHECK_FALSE (opt.hasValue);
}

TEST_CASE("Optional_assignment_6", "[optional]")
{
    irbis::Optional<int> opt1 (5);
    CHECK (opt1.hasValue);
    irbis::Optional<int> opt2 = std::move (opt1);
    CHECK_FALSE (opt1.hasValue);
    CHECK (opt2.hasValue);
}

TEST_CASE("Optional_reset_1", "[optional]")
{
    irbis::Optional<int> opt (5);
    opt.reset();
    CHECK_FALSE (opt.hasValue);
}

TEST_CASE("Optional_valueOr_1", "[optional]")
{
    irbis::Optional<int> opt (5);
    CHECK (opt.valueOr(1) == 5);
}

TEST_CASE("Optional_valueOr_2", "[optional]")
{
    irbis::Optional<int> opt;
    CHECK (opt.valueOr(1) == 1);
}

TEST_CASE("Optional_valueOr_3", "[optional]")
{
    irbis::Optional<int> opt;
    auto v = opt.valueOr([]() { return 3; });
    CHECK (v == 3);
}

TEST_CASE("Optional_stream_1", "[optional]")
{
    std::stringstream s;
    irbis::Optional<int> opt;
    s << opt;
    CHECK (s.str().empty());

    opt = 123;
    s << opt;
    CHECK (s.str() == "123");
}
