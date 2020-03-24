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
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    CHECK (record.fm (700, 'a') == L"Иванов");
    CHECK (record.fm (701, 'a') == L"Петров");
    CHECK (record.fm (300) == L"Первое примечание");
    CHECK (record.fm (700, 'c').empty());
    CHECK (record.fm (310).empty());

}

TEST_CASE("MarcRecord_fma_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    auto list = record.fma (700, 'a');
    REQUIRE (list.size() == 1);
    CHECK (list[0] == L"Иванов");
    list = record.fma (701, 'a');
    REQUIRE (list.size() == 1);
    CHECK (list[0] == L"Петров");
    list = record.fma (300);
    REQUIRE (list.size() == 3);
    CHECK (list[0] == L"Первое примечание");
    CHECK (list[1] == L"Второе примечание");
    CHECK (list[2] == L"Третье примечание");
    list = record.fma (700, 'c');
    REQUIRE (list.empty());
    list = record.fma (301);
    REQUIRE (list.empty());
}

TEST_CASE("MarcRecord_getField_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    auto field = record.getField (700);
    REQUIRE (field != nullptr);
    CHECK (field->tag == 700);
    field = record.getField (300, 1);
    REQUIRE (field != nullptr);
    CHECK (field->value == L"Второе примечание");
    field = record.getField (310);
    CHECK (field == nullptr);
}

TEST_CASE("MarcRecord_getFields_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    auto fields = record.getFields (700);
    REQUIRE (fields.size() == 1);
    CHECK (fields[0]->tag == 700);
    fields = record.getFields (300);
    REQUIRE (fields.size() == 3);
    CHECK (fields[1]->value == L"Второе примечание");
    fields = record.getFields (310);
    CHECK (fields.empty());

}

TEST_CASE("MarcRecord_removeField_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    record.removeField (700);
    CHECK (record.getField (700) == nullptr);
    record.removeField (300);
    CHECK (record.getField (300) == nullptr);
    CHECK (record.fields.size() == 2);
}

TEST_CASE("MarcRecord_setField_1", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    record.setField (701, L"Зайцев");
    CHECK (record.fm (701) == L"Зайцев");
    record.setField (702, L"Козлов");
    CHECK (record.fm (702) == L"Козлов");
    CHECK (record.fields.size() == 7);
    record.setField (702, L"");
    CHECK (record.fm (702).empty());
    CHECK (record.fields.size() == 6);
}

TEST_CASE("MarcRecord_setField_2", "[record]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    irbis::String text (L"Зайцев");
    record.setField (701, std::move (text));
    CHECK (record.fm (701) == L"Зайцев");
    text = L"Козлов";
    record.setField (702, std::move (text));
    CHECK (record.fm (702) == L"Козлов");
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
    record.add (700).add ('a', L"Иванов").add ('b', L"И. И.");
    record.add (701).add ('a', L"Петров").add ('b', L"П. П.");
    record.add (200).add ('a', L"Заглавие").add ('e', L"подзаголовочное").add ('f', L"И. И. Иванов, П. П. Петров");
    record.add (300, L"Первое примечание");
    record.add (300, L"Второе примечание");
    record.add (300, L"Третье примечание");
    CHECK (record.verify (false));
}
