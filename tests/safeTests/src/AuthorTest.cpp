// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Author_construction_1", "[author]")
{
    irbis::Author author;
    CHECK (author.familyName.empty());
    CHECK (author.initials.empty());
    CHECK (author.fullName.empty());
    CHECK (author.canBeInverted.empty());
    CHECK (author.postfix.empty());
    CHECK (author.appendix.empty());
    CHECK (author.number.empty());
    CHECK (author.dates.empty());
    CHECK (author.variant.empty());
    CHECK (author.workplace.empty());
    CHECK (author.field == nullptr);
}

TEST_CASE("Author_applyTo_1", "[author]")
{
    irbis::Author author;
    author.familyName = L"Пушкин";
    author.initials   = L"А. С.";
    author.fullName   = L"Александр Сергеевич";
    author.appendix   = L"русский поэт";
    author.dates      = L"1799-1837";
    author.variant    = L"Белкин, Иван Петрович";
    author.workplace  = L"камер-юнкер";

    irbis::RecordField field (700);
    author.applyTo (field);
    CHECK (author.familyName    == field.getFirstSubfieldValue ('a'));
    CHECK (author.initials      == field.getFirstSubfieldValue ('b'));
    CHECK (author.fullName      == field.getFirstSubfieldValue ('g'));
    CHECK (author.canBeInverted == field.getFirstSubfieldValue ('9'));
    CHECK (author.postfix       == field.getFirstSubfieldValue ('1'));
    CHECK (author.appendix      == field.getFirstSubfieldValue ('c'));
    CHECK (author.number        == field.getFirstSubfieldValue ('d'));
    CHECK (author.dates         == field.getFirstSubfieldValue ('f'));
    CHECK (author.variant       == field.getFirstSubfieldValue ('r'));
    CHECK (author.workplace     == field.getFirstSubfieldValue ('p'));
}

TEST_CASE("Author_parse_1", "[author]")
{
    irbis::RecordField field(700);
    field
        .add ('a', L"Пушкин")
        .add ('b', L"А. С.")
        .add ('g', L"Александр Сергеевич")
        .add ('c', L"русский поэт")
        .add ('f', L"1799-1837")
        .add ('r', L"Белкин, Иван Петрович")
        .add ('p', L"камер-юнкер");

    irbis::Author author;
    author.parse (field);
    CHECK (author.field         == &field);
    CHECK (author.familyName    == field.getFirstSubfieldValue ('a'));
    CHECK (author.initials      == field.getFirstSubfieldValue ('b'));
    CHECK (author.fullName      == field.getFirstSubfieldValue ('g'));
    CHECK (author.canBeInverted == field.getFirstSubfieldValue ('9'));
    CHECK (author.postfix       == field.getFirstSubfieldValue ('1'));
    CHECK (author.appendix      == field.getFirstSubfieldValue ('c'));
    CHECK (author.number        == field.getFirstSubfieldValue ('d'));
    CHECK (author.dates         == field.getFirstSubfieldValue ('f'));
    CHECK (author.variant       == field.getFirstSubfieldValue ('r'));
    CHECK (author.workplace     == field.getFirstSubfieldValue ('p'));
}

TEST_CASE("Author_parse_2", "[author]")
{
    irbis::MarcRecord record;
    record.add (700)
            .add ('a', L"Пушкин")
            .add ('b', L"А. С.")
            .add ('g', L"Александр Сергеевич")
            .add ('c', L"русский поэт")
            .add ('f', L"1799-1837")
            .add ('r', L"Белкин, Иван Петрович")
            .add ('p', L"камер-юнкер");

    std::vector<irbis::Author> authors = irbis::Author::parse (record, 700);
    CHECK (authors[0].familyName    == record.fm (700, 'a'));
    CHECK (authors[0].initials      == record.fm (700, 'b'));
    CHECK (authors[0].fullName      == record.fm (700, 'g'));
    CHECK (authors[0].canBeInverted == record.fm (700, '9'));
    CHECK (authors[0].postfix       == record.fm (700, '1'));
    CHECK (authors[0].appendix      == record.fm (700, 'c'));
    CHECK (authors[0].number        == record.fm (700, 'd'));
    CHECK (authors[0].dates         == record.fm (700, 'f'));
    CHECK (authors[0].variant       == record.fm (700, 'r'));
    CHECK (authors[0].workplace     == record.fm (700, 'p'));
}
