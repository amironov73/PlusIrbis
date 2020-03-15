// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Codes_constructor_1", "[codes]")
{
    irbis::Codes codes;
    CHECK (codes.type.empty());
    CHECK (codes.kind.empty());
    CHECK (codes.character1.empty());
    CHECK (codes.character2.empty());
    CHECK (codes.character3.empty());
    CHECK (codes.character4.empty());
    CHECK (codes.character5.empty());
    CHECK (codes.character6.empty());
    CHECK (codes.purpose1.empty());
    CHECK (codes.purpose2.empty());
    CHECK (codes.purpose3.empty());
    CHECK (codes.restrictions.empty());
}

TEST_CASE("Codes_parse_1", "[codes]")
{
    irbis::RecordField field (900);
    field.add ('a', L"a");
    field.add ('b', L"05");
    field.add ('c', L"454");
    field.add ('2', L"19");
    field.add ('x', L"m");
    field.add ('y', L"e");

    irbis::Codes codes;
    codes.parse (field);
    CHECK (codes.field == &field);
    CHECK (codes.type         == field.getFirstSubfieldValue ('a'));
    CHECK (codes.kind         == field.getFirstSubfieldValue ('b'));
    CHECK (codes.character1   == field.getFirstSubfieldValue ('c'));
    CHECK (codes.character2   == field.getFirstSubfieldValue ('2'));
    CHECK (codes.character3   == field.getFirstSubfieldValue ('3'));
    CHECK (codes.character4   == field.getFirstSubfieldValue ('4'));
    CHECK (codes.character5   == field.getFirstSubfieldValue ('5'));
    CHECK (codes.character6   == field.getFirstSubfieldValue ('6'));
    CHECK (codes.purpose1     == field.getFirstSubfieldValue ('x'));
    CHECK (codes.purpose2     == field.getFirstSubfieldValue ('y'));
    CHECK (codes.purpose3     == field.getFirstSubfieldValue ('9'));
    CHECK (codes.restrictions == field.getFirstSubfieldValue ('z'));
}

TEST_CASE("Codes_applyTo_1", "[codes]")
{
    irbis::Codes codes;
    codes.type       = L"a";
    codes.kind       = L"05";
    codes.character1 = L"454";
    codes.character2 = L"19";
    codes.purpose1   = L"m";
    codes.purpose2   = L"e";

    irbis::RecordField field (900);
    codes.applyTo (field);
    CHECK (codes.type         == field.getFirstSubfieldValue ('a'));
    CHECK (codes.kind         == field.getFirstSubfieldValue ('b'));
    CHECK (codes.character1   == field.getFirstSubfieldValue ('c'));
    CHECK (codes.character2   == field.getFirstSubfieldValue ('2'));
    CHECK (codes.character3   == field.getFirstSubfieldValue ('3'));
    CHECK (codes.character4   == field.getFirstSubfieldValue ('4'));
    CHECK (codes.character5   == field.getFirstSubfieldValue ('5'));
    CHECK (codes.character6   == field.getFirstSubfieldValue ('6'));
    CHECK (codes.purpose1     == field.getFirstSubfieldValue ('x'));
    CHECK (codes.purpose2     == field.getFirstSubfieldValue ('y'));
    CHECK (codes.purpose3     == field.getFirstSubfieldValue ('9'));
    CHECK (codes.restrictions == field.getFirstSubfieldValue ('z'));
}
