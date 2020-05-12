// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("MarcRecord_constructor_1", "[record]")
{
    const irbis::MarcRecord record;
    CHECK (record.mfn == 0u);
    CHECK (record.status == irbis::RecordStatus::None);
    CHECK (record.version == 0u);
    CHECK (record.database.empty());
    CHECK (record.fields.empty());
}

TEST_CASE("MarcRecord_constructor_2", "[record]")
{
    using RF = irbis::RecordField;
    using SF = irbis::SubField;
    irbis::MarcRecord record ({
        RF ( 200, { SF { 'a', L"SubA" }, SF { 'b', L"SubB" } } ),
        RF ( 300, L"Field300" )
    });
    CHECK (record.mfn == 0u);
    CHECK (record.status == irbis::RecordStatus::None);
    CHECK (record.version == 0u);
    CHECK (record.database.empty());
    CHECK (record.fields.size() == 2);
}

TEST_CASE("MarcRecord_constructor_3", "[record]")
{
    using RF = irbis::RecordField;
    irbis::MarcRecord record ({
        RF { 200, { { 'a', L"SubA" }, { 'b', L"SubB" } } },
        RF { 300, L"Field300" }
    });
    CHECK (record.mfn == 0u);
    CHECK (record.status == irbis::RecordStatus::None);
    CHECK (record.version == 0u);
    CHECK (record.database.empty());
    CHECK (record.fields.size() == 2);
}

TEST_CASE("MarcRecord_constructor_4", "[record]")
{
    irbis::MarcRecord record ({
        { 200, { { 'a', L"SubA" }, { 'b', L"SubB" } } },
        { 300, L"Field300" }
    });
    CHECK (record.mfn == 0u);
    CHECK (record.status == irbis::RecordStatus::None);
    CHECK (record.version == 0u);
    CHECK (record.database.empty());
    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->tag == 200);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    ++field;
    CHECK (field->tag == 300);
    CHECK (field->value == L"Field300");
    CHECK (field->subfields.empty());
}

TEST_CASE("MarcRecord_add_1", "[record]")
{
    irbis::MarcRecord record;
    irbis::String text (L"value");
    const auto field = record.add (123, text);
    CHECK (record.fields.size() == 1);
    CHECK (field.tag == 123);
    CHECK (field.value == text);
}

TEST_CASE("MarcRecord_add_2", "[record]")
{
    irbis::MarcRecord record;
    irbis::String text (L"value");
    const auto field = record.add (123, std::move (text));
    REQUIRE (record.fields.size() == 1);
    CHECK (field.tag == 123);
    CHECK (field.value == L"value");
}

TEST_CASE("MarcRecord_clone_1", "[record]")
{
    irbis::MarcRecord first;
    first.mfn = 1;
    first.version = 2;
    first.status = irbis::RecordStatus::Locked;
    first.database = L"ISTU";
    first.add (123, L"field123");
    first.add (234).add ('a', L"subfield a").add ('b', L"subfield b");
    irbis::MarcRecord second = first.clone();
    CHECK (first.mfn      == second.mfn);
    CHECK (first.version  == second.version);
    CHECK (first.status   == second.status);
    CHECK (first.database == second.database);
    REQUIRE (first.fields.size() == second.fields.size());
    auto pfirst = std::begin (first.fields);
    auto psecond = std::begin (second.fields);
    CHECK (pfirst->tag == psecond->tag);
    CHECK (pfirst->value == psecond->value);
    CHECK (pfirst->subfields.size() == psecond->subfields.size());
    ++pfirst;
    ++psecond;
    CHECK (pfirst->tag == psecond->tag);
    CHECK (pfirst->value == psecond->value);
    CHECK (pfirst->subfields.size() == psecond->subfields.size());
}

TEST_CASE("MarcRecord_decode_1", "[record]")
{
    irbis::StringList lines;
    lines.push_back (L"123#64");
    lines.push_back (L"0#12");
    lines.push_back (L"123#field123");
    lines.push_back (L"234#^asubfield a^bsubfield b");
    irbis::MarcRecord record;
    record.decode (lines);
    CHECK (record.mfn == 123);
    CHECK (record.status == irbis::RecordStatus::Locked);
    CHECK (record.version == 12);
    REQUIRE (record.fields.size() == 2);
    auto pfield = std::begin (record.fields);
    CHECK (pfield->tag == 123);
    CHECK (pfield->value == L"field123");
    ++pfield;
    CHECK (pfield->tag == 234);
    CHECK (pfield->subfields.size() == 2);
}

TEST_CASE("MarcRecord_deleted_1", "[record]")
{
    irbis::MarcRecord record;
    CHECK_FALSE (record.deleted());
    record.status = irbis::RecordStatus::LogicallyDeleted;
    CHECK (record.deleted());
    record.status = irbis::RecordStatus::PhysicallyDeleted;
    CHECK (record.deleted());
}

TEST_CASE("MarcRecord_encode_1", "[record]")
{
    irbis::MarcRecord record;
    record.mfn = 123456;
    record.status = irbis::RecordStatus::Locked;
    record.version = 12;
    record.add (123, L"field123");
    record.add (234).add ('a', L"subfield a").add ('b', L"subfield b");
    const auto text = record.encode(L"\n");
    CHECK (text == L"123456#64\n0#12\n123#field123\n234#^asubfield a^bsubfield b\n");
}

TEST_CASE("MarcRecord_fm_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    CHECK (record.fm (700, 'a') == L"\u0418\u0432\u0430\u043D\u043E\u0432");
    CHECK (record.fm (701, 'a') == L"\u041F\u0435\u0442\u0440\u043E\u0432");
    CHECK (record.fm (300) == L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    CHECK (record.fm (700, 'c').empty());
    CHECK (record.fm (310).empty());

}

TEST_CASE("MarcRecord_fma_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    auto list = record.fma (700, 'a');
    REQUIRE (list.size() == 1);
    CHECK (list[0] == L"\u0418\u0432\u0430\u043D\u043E\u0432");
    list = record.fma (701, 'a');
    REQUIRE (list.size() == 1);
    CHECK (list[0] == L"\u041F\u0435\u0442\u0440\u043E\u0432");
    list = record.fma (300);
    REQUIRE (list.size() == 3);
    CHECK (list[0] == L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    CHECK (list[1] == L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    CHECK (list[2] == L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    list = record.fma (700, 'c');
    REQUIRE (list.empty());
    list = record.fma (301);
    REQUIRE (list.empty());
}

TEST_CASE("MarcRecord_getField_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    auto field = record.getField (700);
    REQUIRE (field != nullptr);
    CHECK (field->tag == 700);
    field = record.getField (300, 1);
    REQUIRE (field != nullptr);
    CHECK (field->value == L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    field = record.getField (310);
    CHECK (field == nullptr);
}

TEST_CASE("MarcRecord_getFields_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    auto fields = record.getFields (700);
    REQUIRE (fields.size() == 1);
    CHECK (fields[0]->tag == 700);
    fields = record.getFields (300);
    REQUIRE (fields.size() == 3);
    CHECK (fields[1]->value == L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    fields = record.getFields (310);
    CHECK (fields.empty());

}

TEST_CASE("MarcRecord_removeField_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.removeField (700);
    CHECK (record.getField (700) == nullptr);
    record.removeField (300);
    CHECK (record.getField (300) == nullptr);
    CHECK (record.fields.size() == 2);
}

TEST_CASE("MarcRecord_setField_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.setField (701, L"\u0417\u0430\u0439\u0446\u0435\u0432");
    CHECK (record.fm (701) == L"\u0417\u0430\u0439\u0446\u0435\u0432");
    record.setField (702, L"\u041A\u043E\u0437\u043B\u043E\u0432");
    CHECK (record.fm (702) == L"\u041A\u043E\u0437\u043B\u043E\u0432");
    CHECK (record.fields.size() == 7);
    record.setField (702, L"");
    CHECK (record.fm (702).empty());
    CHECK (record.fields.size() == 6);
}

TEST_CASE("MarcRecord_setField_2", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    irbis::String text (L"\u0417\u0430\u0439\u0446\u0435\u0432");
    record.setField (701, std::move (text));
    CHECK (record.fm (701) == L"\u0417\u0430\u0439\u0446\u0435\u0432");
    text = L"\u041A\u043E\u0437\u043B\u043E\u0432";
    record.setField (702, std::move (text));
    CHECK (record.fm (702) == L"\u041A\u043E\u0437\u043B\u043E\u0432");
    CHECK (record.fields.size() == 7);
    text.clear();
    record.setField (702, std::move (text));
    CHECK (record.fm (702).empty());
    CHECK (record.fields.size() == 6);
}

TEST_CASE("MarcRecord_reset_1")
{
    irbis::MarcRecord record;
    record.mfn      = 1;
    record.version  = 2;
    record.status   = irbis::RecordStatus::Locked;
    record.database = L"ISTU";
    record.reset();
    CHECK (record.mfn     == 0);
    CHECK (record.status  == irbis::RecordStatus::None);
    CHECK (record.version == 0);
    CHECK (record.database.empty());
}

TEST_CASE("MarcRecord_verify_1", "[record]")
{
    irbis::MarcRecord record;
    CHECK_FALSE (record.verify (false));
    record.add (700).add ('a', L"\u0418\u0432\u0430\u043D\u043E\u0432").add ('b', L"\u0418. \u0418.");
    record.add (701).add ('a', L"\u041F\u0435\u0442\u0440\u043E\u0432").add ('b', L"\u041F. \u041F.");
    record.add (200).add ('a', L"\u0417\u0430\u0433\u043B\u0430\u0432\u0438\u0435").add ('e', L"\u043F\u043E\u0434\u0437\u0430\u0433\u043E\u043B\u043E\u0432\u043E\u0447\u043D\u043E\u0435").add ('f', L"\u0418. \u0418. \u0418\u0432\u0430\u043D\u043E\u0432, \u041F. \u041F. \u041F\u0435\u0442\u0440\u043E\u0432");
    record.add (300, L"\u041F\u0435\u0440\u0432\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0412\u0442\u043E\u0440\u043E\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    record.add (300, L"\u0422\u0440\u0435\u0442\u044C\u0435 \u043F\u0440\u0438\u043C\u0435\u0447\u0430\u043D\u0438\u0435");
    CHECK (record.verify (false));
}

using irbis::operator""_field;
using irbis::operator""_sub;

TEST_CASE("MarcRecord_shift_1", "[record]")
{
    irbis::MarcRecord record;
    record << (100_field << ('a'_sub << "SubA") << ('b'_sub << "SubB"))
        << (200_field << ('a'_sub << "SubA") << ('b'_sub << "SubB"))
        << (300_field << "Field300");

    CHECK (record.fields.size() == 3);
    auto field = std::begin (record.fields);
    CHECK (field->tag == 100);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->tag == 200);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->tag == 300);
    CHECK (field->value == L"Field300");
    CHECK (field->subfields.empty());
}

TEST_CASE("MarcRecord_shift_2", "[record]")
{
    irbis::MarcRecord record;
    record << (100_field << L"^aSubA^bSubB")
        << (200_field << L"^aSubA^bSubB")
        << (300_field << L"Field300");

    CHECK (record.fields.size() == 3);
    auto field = std::begin (record.fields);
    CHECK (field->tag == 100);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->tag == 200);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->tag == 300);
    CHECK (field->value == L"Field300");
    CHECK (field->subfields.empty());
}

TEST_CASE("MarcRecord_shift_3", "[record]")
{
    irbis::MarcRecord record;
    record << 100 << 'a' && L"SubA";
    record << 'b' && L"SubB";
    record << 200 << 'c' && L"SubC";
    record << 'd' && L"SubD";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'c');
    CHECK (field->subfields[0].value == L"SubC");
    CHECK (field->subfields[1].code == 'd');
    CHECK (field->subfields[1].value == L"SubD");
}

TEST_CASE("MarcRecord_shift_4", "[record]")
{
    irbis::MarcRecord record;
    record << 100 << 'a' << L"SubA";
    record << 'b' << L"SubB";
    record << 200 << 'c' << L"SubC";
    record << 'd' << L"SubD";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'c');
    CHECK (field->subfields[0].value == L"SubC");
    CHECK (field->subfields[1].code == 'd');
    CHECK (field->subfields[1].value == L"SubD");
}

TEST_CASE("MarcRecord_shift_5", "[record]")
{
    irbis::MarcRecord record;
    record << 100
        << 'a' << L"SubA"
        << 'b' << L"SubB"
        << 200
        << 'c' << L"SubC"
        << 'd' << L"SubD";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'c');
    CHECK (field->subfields[0].value == L"SubC");
    CHECK (field->subfields[1].code == 'd');
    CHECK (field->subfields[1].value == L"SubD");
}

TEST_CASE("MarcRecord_shift_6", "[record]")
{
    irbis::MarcRecord record;
    record << 100
        << L"^aSubA^bSubB"
        << 200
        << L"^cSubC^dSubD";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'c');
    CHECK (field->subfields[0].value == L"SubC");
    CHECK (field->subfields[1].code == 'd');
    CHECK (field->subfields[1].value == L"SubD");
}

TEST_CASE("MarcRecord_shift_7", "[record]")
{
    irbis::MarcRecord record;
    record << 100
        << L"Field100"
        << 200
        << L"Field200";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value == L"Field100");
    CHECK (field->subfields.empty());
    ++field;
    CHECK (field->value == L"Field200");
    CHECK (field->subfields.empty());
}

TEST_CASE("MarcRecord_shift_8", "[record]")
{
    irbis::MarcRecord record;
    record << L"100#^aSubA^bSubB"
           << L"200#^cSubC^dSubD";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'a');
    CHECK (field->subfields[0].value == L"SubA");
    CHECK (field->subfields[1].code == 'b');
    CHECK (field->subfields[1].value == L"SubB");
    ++field;
    CHECK (field->value.empty());
    CHECK (field->subfields.size() == 2);
    CHECK (field->subfields[0].code == 'c');
    CHECK (field->subfields[0].value == L"SubC");
    CHECK (field->subfields[1].code == 'd');
    CHECK (field->subfields[1].value == L"SubD");
}

TEST_CASE("MarcRecord_shift_9", "[record]")
{
    irbis::MarcRecord record;
    record << L"100#Field100"
           << L"200#Field200";

    CHECK (record.fields.size() == 2);
    auto field = std::begin (record.fields);
    CHECK (field->value == L"Field100");
    CHECK (field->subfields.empty());
    ++field;
    CHECK (field->value == L"Field200");
    CHECK (field->subfields.empty());
}
