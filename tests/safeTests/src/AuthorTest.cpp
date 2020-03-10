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
    author.familyName = L"\u041F\u0443\u0448\u043A\u0438\u043D";
    author.initials   = L"\u0410. \u0421.";
    author.fullName   = L"\u0410\u043B\u0435\u043A\u0441\u0430\u043D\u0434\u0440 \u0421\u0435\u0440\u0433\u0435\u0435\u0432\u0438\u0447";
    author.appendix   = L"\u0440\u0443\u0441\u0441\u043A\u0438\u0439 \u043F\u043E\u044D\u0442";
    author.dates      = L"1799-1837";
    author.variant    = L"\u0411\u0435\u043B\u043A\u0438\u043D, \u0418\u0432\u0430\u043D \u041F\u0435\u0442\u0440\u043E\u0432\u0438\u0447";
    author.workplace  = L"\u043A\u0430\u043C\u0435\u0440-\u044E\u043D\u043A\u0435\u0440";

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
        .add ('a', L"\u041F\u0443\u0448\u043A\u0438\u043D")
        .add ('b', L"\u0410. \u0421.")
        .add ('g', L"\u0410\u043B\u0435\u043A\u0441\u0430\u043D\u0434\u0440 \u0421\u0435\u0440\u0433\u0435\u0435\u0432\u0438\u0447")
        .add ('c', L"\u0440\u0443\u0441\u0441\u043A\u0438\u0439 \u043F\u043E\u044D\u0442")
        .add ('f', L"1799-1837")
        .add ('r', L"\u0411\u0435\u043B\u043A\u0438\u043D, \u0418\u0432\u0430\u043D \u041F\u0435\u0442\u0440\u043E\u0432\u0438\u0447")
        .add ('p', L"\u043A\u0430\u043C\u0435\u0440-\u044E\u043D\u043A\u0435\u0440");

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
            .add ('a', L"\u041F\u0443\u0448\u043A\u0438\u043D")
            .add ('b', L"\u0410. \u0421.")
            .add ('g', L"\u0410\u043B\u0435\u043A\u0441\u0430\u043D\u0434\u0440 \u0421\u0435\u0440\u0433\u0435\u0435\u0432\u0438\u0447")
            .add ('c', L"\u0440\u0443\u0441\u0441\u043A\u0438\u0439 \u043F\u043E\u044D\u0442")
            .add ('f', L"1799-1837")
            .add ('r', L"\u0411\u0435\u043B\u043A\u0438\u043D, \u0418\u0432\u0430\u043D \u041F\u0435\u0442\u0440\u043E\u0432\u0438\u0447")
            .add ('p', L"\u043A\u0430\u043C\u0435\u0440-\u044E\u043D\u043A\u0435\u0440");

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
