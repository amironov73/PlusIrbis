// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("BookInfo_construction_1", "[bookinfo]")
{
    irbis::MarcRecord record;

    irbis::RecordField field900 (900);
    field900.add ('t', L"a");
    field900.add ('b', L"05");
    field900.add ('c', L"454");
    record.fields.push_back(field900);

    irbis::RecordField field700 (700);
    field700.add ('a', L"Shakespeare");
    record.fields.push_back (field700);

    irbis::RecordField field200 (200);
    field200.add ('a', L"Poems");
    record.fields.push_back (field200);

    record.add (920, L"PAZK");

    irbis::BookInfo book (&record);
    CHECK (book.record == &record);
    CHECK (book.type == L"a");
    CHECK (book.kind == L"05");
    CHECK (book.character == L"454");
    CHECK (book.firstAuthor == L"Shakespeare");
    CHECK (book.title == L"Poems");
    CHECK (book.description.empty());
    CHECK (book.worksheet == L"PAZK");
}

TEST_CASE("BookInfo_amount_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (910)
       .add ('a', L"0")
       .add ('b', L"123");
    record.add (910, L"")
            .add ('a', L"0")
            .add ('b', L"321");
    record.add (910)
            .add ('a', L"U")
            .add ('1', L"8");

    irbis::BookInfo book (&record);
    CHECK (book.amount() == 10);
}

TEST_CASE("BookInfo_authors_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (700).add ('a', L"Ivanov");
    record.add (701).add ('a', L"Petrov");
    record.add (701).add ('a', L"Sidorov");

    irbis::BookInfo book (&record);
    const auto authors = book.authors();
    REQUIRE (authors.size() == 3);
    CHECK (book.firstAuthor == L"Ivanov");
    CHECK (authors[0] == L"Ivanov");
    CHECK (authors[1] == L"Petrov");
    CHECK (authors[2] == L"Sidorov");
}

TEST_CASE("BookInfo_electronic_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK_FALSE (book.electronic());
}

TEST_CASE("BookInfo_electronic_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (900).add ('t', L"l");

    irbis::BookInfo book (&record);
    CHECK (book.electronic());
}

TEST_CASE("BookInfo_electronic_3", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (215).add ('1', L"r");

    irbis::BookInfo book (&record);
    CHECK (book.electronic());
}

TEST_CASE("BookInfo_electronic_4", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (951).add ('h', L"01");

    irbis::BookInfo book (&record);
    CHECK (book.electronic());
}

TEST_CASE("BookInfo_exemplars_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (910)
            .add ('a', L"0")
            .add ('b', L"123");
    record.add (910, L"")
            .add ('a', L"0")
            .add ('b', L"321");
    record.add (910)
            .add ('a', L"U")
            .add ('1', L"8");

    irbis::BookInfo book (&record);
    const auto exemplars = book.exemplars();
    REQUIRE (exemplars.size() == 3);
    CHECK (exemplars[0].number == L"123");
    CHECK (exemplars[1].number == L"321");
    CHECK (exemplars[2].amount == L"8");
}

TEST_CASE("BookInfo_foreign_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK_FALSE (book.foreign());
}

TEST_CASE("BookInfo_foreign_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (101, L"rus");

    irbis::BookInfo book (&record);
    CHECK_FALSE (book.foreign());
}

TEST_CASE("BookInfo_foreign_3", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (101, L"eng");
    record.add (101, L"fra");

    irbis::BookInfo book (&record);
    CHECK (book.foreign());
}

TEST_CASE("BookInfo_languages_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (101, L"rus");
    record.add (101, L"eng");
    record.add (101, L"fra");

    irbis::BookInfo book (&record);
    const auto languages = book.languages();
    REQUIRE (languages.size() == 3);
    CHECK (languages[0] == L"rus");
    CHECK (languages[1] == L"eng");
    CHECK (languages[2] == L"fra");
}

TEST_CASE("BookInfo_link_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK (book.link().empty());
}

TEST_CASE("BookInfo_link_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (951).add ('h', L"02") .add ('a', L"obl.jpg");

    irbis::BookInfo book (&record);
    CHECK (book.link().empty());
}

TEST_CASE("BookInfo_link_3", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (951).add ('h', L"02") .add ('a', L"obl.jpg");
    record.add (951).add ('i', L"http://google.com");

    irbis::BookInfo book (&record);
    const auto link = book.link();
    CHECK (link == L"http://google.com");
}

TEST_CASE("BookInfo_pages_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK (book.pages() == 0);
}

TEST_CASE("BookInfo_pages_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (215).add ('a', L"123");

    irbis::BookInfo book (&record);
    CHECK (book.pages() == 123);
}

TEST_CASE("BookInfo_price_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK (fabs (book.price()) < 0.01);
}

TEST_CASE("BookInfo_price_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (10).add ('d', L"123.45");

    irbis::BookInfo book (&record);
    CHECK (fabs (book.price() - 123.45) < 0.01);
}

TEST_CASE("BookInfo_publishers_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (210).add ('c', L"Наука");
    record.add (210).add ('c', L"Высшая школа");
    record.add (461).add ('g', L"Энергоатомиздат");

    irbis::BookInfo book (&record);
    const auto publishers = book.publishers();
    REQUIRE (publishers.size() == 3);
    CHECK (publishers[0] == L"Наука");
    CHECK (publishers[1] == L"Высшая школа");
    CHECK (publishers[2] == L"Энергоатомиздат");
}

TEST_CASE("BookInfo_usage_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK (book.usage() == 0);
}

TEST_CASE("BookInfo_usage_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (999, L"123");

    irbis::BookInfo book (&record);
    CHECK (book.usage() == 123);
}

TEST_CASE("BookInfo_volume_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK (book.volume().empty());
}

TEST_CASE("BookInfo_volume_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (215).add ('a', L"123");

    irbis::BookInfo book (&record);
    CHECK (book.volume() == L"123");
}

TEST_CASE ("BookInfo_year_1", "[bookinfo]")
{
    irbis::MarcRecord record;
    irbis::BookInfo book (&record);
    CHECK (book.year() == 0);
}

TEST_CASE("BookInfo_year_2", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (210).add ('d', L"2020");

    irbis::BookInfo book (&record);
    CHECK (book.year() == 2020);
}

TEST_CASE("BookInfo_year_3", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (461).add ('h', L"2020");

    irbis::BookInfo book (&record);
    CHECK (book.year() == 2020);
}

TEST_CASE("BookInfo_year_4", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (461).add ('z', L"2020");

    irbis::BookInfo book (&record);
    CHECK (book.year() == 2020);
}

TEST_CASE("BookInfo_year_5", "[bookinfo]")
{
    irbis::MarcRecord record;
    record.add (463).add ('j', L"2020");

    irbis::BookInfo book (&record);
    CHECK (book.year() == 2020);
}


