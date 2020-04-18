// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("RecordField_constructor_1", "[field]")
{
    const irbis::RecordField field;
    CHECK (field.empty());
    CHECK (field.subfields.empty());
}

TEST_CASE("RecordField_constructor_2", "[field]")
{
    const irbis::RecordField field (100);
    CHECK (field.empty());
    CHECK (field.subfields.empty());
    CHECK (field.tag == 100);
}

TEST_CASE("RecordField_constructor_3", "[field]")
{
    const irbis::RecordField field (100, L"Field100");
    CHECK_FALSE (field.empty());
    CHECK (field.tag == 100);
    CHECK (field.value == L"Field100");
    CHECK (field.subfields.empty());
    CHECK (field.toString() == L"100#Field100");
}

TEST_CASE("RecordField_constructor_4", "[field]")
{
    irbis::RecordField field (100);
    field.add ('a', L"SubA").add ('b', L"SubB");
    CHECK_FALSE (field.empty());
    CHECK (field.value.empty());
    CHECK (field.subfields.size() == 2);
    CHECK (field.toString() == L"100#^aSubA^bSubB");
}

TEST_CASE("RecordField_constructor_5", "[field]")
{
    using SF = irbis::SubField;
    irbis::RecordField field (100,
        { SF {'a', L"SubA"}, SF {'b', L"SubB"} } );
    CHECK_FALSE (field.empty());
    CHECK (field.value.empty());
    CHECK (field.subfields.size() == 2);
    CHECK (field.toString() == L"100#^aSubA^bSubB");
}

TEST_CASE("RecordField_constructor_6", "[field]")
{
    irbis::RecordField field (100,
        { {'a', L"SubA"}, {'b', L"SubB"} } );
    CHECK_FALSE (field.empty());
    CHECK (field.value.empty());
    CHECK (field.subfields.size() == 2);
    CHECK (field.toString() == L"100#^aSubA^bSubB");
}

TEST_CASE("RecordField_verify_1", "[field]")
{
    const irbis::RecordField field;
    CHECK_FALSE(field.verify(false));
}

TEST_CASE("RecordField_verify_2", "[field]")
{
    const irbis::RecordField field(100);
    CHECK_FALSE(field.verify(false));
}

TEST_CASE("RecordField_verify_3", "[field]")
{
    const irbis::RecordField field(100, L"Field100");
    CHECK(field.verify(false));
}

TEST_CASE("RecordField_verify_4", "[field]")
{
    irbis::RecordField field(100);
    field.add('a', L"SubA").add('b', L"SubB");
    CHECK(field.verify(false));
}
