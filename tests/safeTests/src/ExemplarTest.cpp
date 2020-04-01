// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("Exemplar_apply_1", "[exemplar]")
{
    irbis::Exemplar exemplar;
    exemplar.status = L"0";
    exemplar.number = L"123456";
    exemplar.place = L"FKH";
    exemplar.date = L"20200207";
    irbis::RecordField field;
    exemplar.applyTo(field);
    CHECK (field.getFirstSubfieldValue(L'a') == L"0");
    CHECK (field.getFirstSubfieldValue(L'b') == L"123456");
    CHECK (field.getFirstSubfieldValue(L'c') == L"20200207");
    CHECK (field.getFirstSubfieldValue(L'd') == L"FKH");
}

TEST_CASE("Exemplar_parse_1", "[exemplar]")
{
    irbis::RecordField field (910);
    field.add (L'a', L"0");
    field.add (L'b', L"123456");
    field.add (L'c', L"20200207");
    field.add (L'd', L"FKH");
    irbis::Exemplar exemplar;
    exemplar.parse (field);
    CHECK (exemplar.status == L"0");
    CHECK (exemplar.number == L"123456");
    CHECK (exemplar.place == L"FKH");
    CHECK (exemplar.date == L"20200207");
}

TEST_CASE("Exemplar_parse_2", "[exemplar]") {
    irbis::MarcRecord record;
    irbis::RecordField repeat1(910);
    repeat1.add(L'a', L"0");
    repeat1.add(L'b', L"123456");
    repeat1.add(L'c', L"20200207");
    repeat1.add(L'd', L"FKH");
    record.fields.push_back(repeat1);
    irbis::RecordField repeat2(910);
    repeat2.add(L'a', L"1");
    repeat2.add(L'b', L"234567");
    repeat2.add(L'c', L"20200207");
    repeat2.add(L'd', L"F101");
    record.fields.push_back(repeat2);
    std::vector<irbis::Exemplar> exemplars = irbis::Exemplar::parse(record);
    CHECK (exemplars.size() == 2);
    CHECK (exemplars[0].status == L"0");
    CHECK (exemplars[0].number == L"123456");
    CHECK (exemplars[0].place == L"FKH");
    CHECK (exemplars[0].date == L"20200207");
    CHECK (exemplars[1].status == L"1");
    CHECK (exemplars[1].number == L"234567");
    CHECK (exemplars[1].place == L"F101");
    CHECK (exemplars[1].date == L"20200207");
}