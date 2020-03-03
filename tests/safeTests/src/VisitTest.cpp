// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Visit_constructor_1", "[visit]")
{
    irbis::Visit visit;
    CHECK (visit.index.empty());
    CHECK (visit.inventory.empty());
    CHECK (visit.description.empty());
    CHECK (visit.sigla.empty());
    CHECK (visit.department.empty());
    CHECK (visit.given.empty());
    CHECK (visit.timeIn.empty());
    CHECK (visit.timeOut.empty());
    CHECK (visit.expected.empty());
    CHECK (visit.returned.empty());
    CHECK (visit.prolong.empty());
    CHECK (visit.lost.empty());
    CHECK (visit.database.empty());
    CHECK (visit.barcode.empty());
    CHECK (visit.responsible.empty());
    CHECK (visit.count.empty());
    CHECK (visit.isVisit());
    CHECK_FALSE (visit.isReturned());
}

static irbis::RecordField getField()
{
    irbis::RecordField result (irbis::Visit::TAG);
    result.decodeBody (L"^A84(2=\u0420\u0443\u0441)5/\u0426 27-422726^B1539325^C\u0426\u0432\u0435\u0442\u0430\u0435\u0432\u0430 \u041C. \u0418. \u0421\u0442\u0438\u0445\u043E\u0442\u0432\u043E\u0440\u0435\u043D\u0438\u044F^K\u0424302^V*^D20170930^1172914^E20171014^F20171028^GIBIS^HE00401004C75DB57^Isamo");
    return result;
}

TEST_CASE("Visit_apply_1", "[visit]")
{
    irbis::Visit visit;
    visit.index = L"84(2=\u0420\u0443\u0441)5/\u0426 27-422726";
    visit.inventory = L"1539325";
    visit.description = L"\u0426\u0432\u0435\u0442\u0430\u0435\u0432\u0430 \u041C. \u0418. \u0421\u0442\u0438\u0445\u043E\u0442\u0432\u043E\u0440\u0435\u043D\u0438\u044F";
    visit.sigla = L"\u0424302";
    visit.department = L"*";
    visit.given = L"20170930";
    visit.timeIn = L"172914";
    visit.expected = L"20171014";
    visit.returned = L"20171028";
    visit.database = L"IBIS";
    visit.barcode = L"E00401004C75DB57";
    visit.responsible = L"samo";
    CHECK_FALSE (visit.isVisit());
    CHECK (visit.isReturned());
    irbis::RecordField field;
    visit.applyTo (field);
    CHECK (field.getFirstSubfieldValue ('a') == visit.index);
    CHECK (field.getFirstSubfieldValue ('b') == visit.inventory);
    CHECK (field.getFirstSubfieldValue ('c') == visit.description);
    CHECK (field.getFirstSubfieldValue ('k') == visit.sigla);
    CHECK (field.getFirstSubfieldValue ('v') == visit.department);
    CHECK (field.getFirstSubfieldValue ('d') == visit.given);
    CHECK (field.getFirstSubfieldValue ('1') == visit.timeIn);
    CHECK (field.getFirstSubfieldValue ('e') == visit.expected);
    CHECK (field.getFirstSubfieldValue ('f') == visit.returned);
    CHECK (field.getFirstSubfieldValue ('g') == visit.database);
    CHECK (field.getFirstSubfieldValue ('h') == visit.barcode);
    CHECK (field.getFirstSubfieldValue ('i') == visit.responsible);
    CHECK (field.getFirstSubfieldValue ('l').empty());
    CHECK (field.getFirstSubfieldValue ('u').empty());
    CHECK (field.getFirstSubfieldValue ('2').empty());
    CHECK (field.getFirstSubfieldValue ('4').empty());
}

TEST_CASE("Visit_parse_1", "[visit]")
{
    const auto field = getField();
    irbis::Visit visit;
    visit.parse (field);
    CHECK_FALSE (visit.isVisit());
    CHECK (visit.isReturned());
    CHECK (visit.field == &field);
    CHECK (visit.index       == field.getFirstSubfieldValue ('a'));
    CHECK (visit.inventory   == field.getFirstSubfieldValue ('b'));
    CHECK (visit.description == field.getFirstSubfieldValue ('c'));
    CHECK (visit.sigla       == field.getFirstSubfieldValue ('k'));
    CHECK (visit.department  == field.getFirstSubfieldValue ('v'));
    CHECK (visit.given       == field.getFirstSubfieldValue ('d'));
    CHECK (visit.timeIn      == field.getFirstSubfieldValue ('1'));
    CHECK (visit.timeOut     == field.getFirstSubfieldValue ('2'));
    CHECK (visit.expected    == field.getFirstSubfieldValue ('e'));
    CHECK (visit.returned    == field.getFirstSubfieldValue ('f'));
    CHECK (visit.database    == field.getFirstSubfieldValue ('g'));
    CHECK (visit.barcode     == field.getFirstSubfieldValue ('h'));
    CHECK (visit.responsible == field.getFirstSubfieldValue ('i'));
    CHECK (visit.prolong     == field.getFirstSubfieldValue ('l'));
    CHECK (visit.lost        == field.getFirstSubfieldValue ('u'));
    CHECK (visit.count       == field.getFirstSubfieldValue ('4'));
}

TEST_CASE("Visit_parse_2", "[visit]")
{
    irbis::MarcRecord record;
    const auto field = getField();
    record.fields.push_back (field);
    const auto visits = irbis::Visit::parse (record);
    REQUIRE (visits.size() == 1);
    const auto &visit = visits[0];
    CHECK_FALSE (visit.isVisit());
    CHECK (visit.isReturned());
    CHECK (visit.index       == field.getFirstSubfieldValue ('a'));
    CHECK (visit.inventory   == field.getFirstSubfieldValue ('b'));
    CHECK (visit.description == field.getFirstSubfieldValue ('c'));
    CHECK (visit.sigla       == field.getFirstSubfieldValue ('k'));
    CHECK (visit.department  == field.getFirstSubfieldValue ('v'));
    CHECK (visit.given       == field.getFirstSubfieldValue ('d'));
    CHECK (visit.timeIn      == field.getFirstSubfieldValue ('1'));
    CHECK (visit.timeOut     == field.getFirstSubfieldValue ('2'));
    CHECK (visit.expected    == field.getFirstSubfieldValue ('e'));
    CHECK (visit.returned    == field.getFirstSubfieldValue ('f'));
    CHECK (visit.database    == field.getFirstSubfieldValue ('g'));
    CHECK (visit.barcode     == field.getFirstSubfieldValue ('h'));
    CHECK (visit.responsible == field.getFirstSubfieldValue ('i'));
    CHECK (visit.prolong     == field.getFirstSubfieldValue ('l'));
    CHECK (visit.lost        == field.getFirstSubfieldValue ('u'));
    CHECK (visit.count       == field.getFirstSubfieldValue ('4'));
}
