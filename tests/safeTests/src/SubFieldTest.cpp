// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("SubField_constructor_1", "[subfield]")
{
    const irbis::SubField sub;
    CHECK (sub.empty());
}

TEST_CASE("SubField_constructor_2", "[subfield]")
{
    const irbis::SubField sub('a');
    CHECK (sub.empty());
    CHECK (sub.code == 'a');
    CHECK (sub.toString() == L"^a");
}

TEST_CASE("SubField_constructor_3", "[subfield]")
{
    const irbis::SubField sub{'a', L"SubfieldA"};
    CHECK_FALSE (sub.empty());
    CHECK (sub.code == 'a');
    CHECK (sub.value == L"SubfieldA");
    CHECK (sub.toString() == L"^aSubfieldA");
}

TEST_CASE("SubField_verify_1", "[subfield]")
{
    const irbis::SubField sub;
    CHECK_FALSE (sub.verify(false));
}

TEST_CASE("SubField_verify_2", "[subfield]")
{
    const irbis::SubField sub('a');
    CHECK_FALSE (sub.verify(false));
}

TEST_CASE("SubField_verify_3", "[subfield]")
{
    const irbis::SubField sub('a', L"SubA");
    CHECK (sub.verify(false));
}

using irbis::operator""_sub;

TEST_CASE("SubField_udl_1", "[subfield]")
{
    const auto subfield = 'a'_sub << "Hello";
    CHECK (subfield.code == 'a');
    CHECK (subfield.value == L"Hello");
}

TEST_CASE("SubField_udl_2", "[subfield]")
{
    const auto subField = "aHello"_sub;
    CHECK (subField.code == 'a');
    CHECK (subField.value == L"Hello");
}

TEST_CASE("SubField_udl_3", "[subfield]")
{
    const auto subField = L"aHello"_sub;
    CHECK (subField.code == 'a');
    CHECK (subField.value == L"Hello");
}

TEST_CASE("SubField_shift_1", "[subfield]")
{
    irbis::SubField subfield;
    subfield << irbis::String (L"Hello");
    CHECK (subfield.value == L"Hello");
}

TEST_CASE("SubField_shift_2", "[subfield]")
{
    irbis::SubField subfield;
    subfield << std::move (irbis::String (L"Hello"));
    CHECK (subfield.value == L"Hello");
}

TEST_CASE("SubField_shift_3", "[subfield]")
{
    irbis::SubField subfield;
    subfield << std::string ("Hello");
    CHECK (subfield.value == L"Hello");
}

TEST_CASE("SubField_shift_4", "[subfield]")
{
    irbis::SubField subfield;
    subfield << L"Hello";
    CHECK (subfield.value == L"Hello");
}

TEST_CASE("SubField_shift_5", "[subfield]")
{
    irbis::SubField subfield;
    subfield << "Hello";
    CHECK (subfield.value == L"Hello");
}
