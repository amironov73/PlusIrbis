#include "catch.hpp"
#include "irbis.h"

TEST_CASE("SubField_constructor_1", "[subfield]")
{
    const irbis::SubField sub;
    CHECK(sub.empty());
}

TEST_CASE("SubField_constructor_2", "[subfield]")
{
    const irbis::SubField sub('a');
    CHECK(sub.empty());
    CHECK(sub.code == 'a');
    CHECK(sub.toString() == L"^a");
}

TEST_CASE("SubField_constructor_3", "[subfield]")
{
    const irbis::SubField sub{'a', L"SubfieldA"};
    CHECK_FALSE(sub.empty());
    CHECK(sub.code == 'a');
    CHECK(sub.value == L"SubfieldA");
    CHECK(sub.toString() == L"^aSubfieldA");
}

TEST_CASE("SubField_verify_1", "[subfield]")
{
    const irbis::SubField sub;
    CHECK_FALSE(sub.verify(false));
}

TEST_CASE("SubField_verify_2", "[subfield]")
{
    const irbis::SubField sub('a');
    CHECK_FALSE(sub.verify(false));
}

TEST_CASE("SubField_verify_3", "[subfield]")
{
    const irbis::SubField sub('a', L"SubA");
    CHECK(sub.verify(false));
}
