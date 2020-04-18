// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

#include <sstream>

TEST_CASE("NotNull_constructor_1", "[notNull]")
{
    int value1 = 123, value2 = 321;
    irbis::NotNull<int> pointer1 (&value1);
    irbis::NotNull<int> pointer2 (&value2);

    CHECK (*pointer1 == value1);
    CHECK (*pointer2 == value2);
}

TEST_CASE("NotNull_constructor_2", "[notNull]")
{
    std::string value1 ("123"), value2 ("321");
    irbis::NotNull<std::string> pointer1 (&value1);
    irbis::NotNull<std::string> pointer2 (&value2);

    CHECK (*pointer1 == value1);
    CHECK (*pointer2 == value2);
    CHECK (pointer1->size() == 3);
    CHECK (pointer2->size() == 3);
}

TEST_CASE("NotNull_shiftLeft_1", "[notNull]")
{
    std::ostringstream stream;
    int value1 = 123, value2 = 321;
    irbis::NotNull<int> pointer1 (&value1);
    irbis::NotNull<int> pointer2 (&value2);

    stream << pointer1 << ", " << pointer2;
    CHECK (stream.str() == "123, 321");
}

TEST_CASE("NotNull_shiftLeft_2", "[notNull]")
{
    std::ostringstream stream;
    std::string value1 = "123", value2 = "321";
    irbis::NotNull<std::string> pointer1 (&value1);
    irbis::NotNull<std::string> pointer2 (&value2);

    stream << pointer1 << ", " << pointer2;
    CHECK (stream.str() == "123, 321");
}

TEST_CASE("NotNull_assignment_1", "[notNull]")
{
    int value;
    irbis::NotNull<int> pointer1 (&value);
    irbis::NotNull<int> pointer2 (pointer1.m_ptr);
    CHECK (*pointer1 == *pointer2);
}

TEST_CASE("NotNull_equals_1", "[notNull]")
{
    int value1 = 123, value2 = 321;
    irbis::NotNull<int> pointer1 (&value1);
    irbis::NotNull<int> pointer2 (&value2);

    CHECK_FALSE (pointer1 == pointer2);
    CHECK (pointer1 != pointer2);
    if (pointer1 < pointer2) {
        CHECK (pointer1 <= pointer2);
        CHECK (pointer2 >  pointer1);
        CHECK (pointer2 >= pointer1);
    }
    else {
        CHECK (pointer1 > pointer2);
        CHECK (pointer1 >= pointer2);
        CHECK (pointer2 < pointer1);
        CHECK (pointer2 <= pointer1);
    }
}
