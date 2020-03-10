// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE ("Title_constructor_1", "[title]")
{
    irbis::Title title;
    CHECK (title.number.empty());
    CHECK (title.title.empty());
    CHECK (title.specific.empty());
    CHECK (title.general.empty());
    CHECK (title.subtitle.empty());
    CHECK (title.first.empty());
    CHECK (title.other.empty());
}

TEST_CASE ("Title_parse_1", "[title]")
{
    irbis::RecordField field (200);
    field.add ('a', L"\u041F\u0438\u043A\u0430\u0441\u0441\u043E \u0441\u0435\u0433\u043E\u0434\u043D\u044F");
    field.add ('e', L"[\u0441\u0431\u043E\u0440\u043D\u0438\u043A \u0441\u0442\u0430\u0442\u0435\u0439]");
    field.add ('f', L"\u0410. \u0410. \u0411\u0430\u0431\u0438\u043D");
    field.add ('g', L"\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0430\u043A\u0430\u0434\u0435\u043C\u0438\u044F \u0445\u0443\u0434\u043E\u0436\u0435\u0441\u0442\u0432");

    irbis::Title title;
    title.parse (field);
    CHECK (title.field    == &field);
    CHECK (title.number   == field.getFirstSubfieldValue ('n'));
    CHECK (title.title    == field.getFirstSubfieldValue ('a'));
    CHECK (title.specific == field.getFirstSubfieldValue ('u'));
    CHECK (title.general  == field.getFirstSubfieldValue ('b'));
    CHECK (title.subtitle == field.getFirstSubfieldValue ('e'));
    CHECK (title.first    == field.getFirstSubfieldValue ('f'));
    CHECK (title.other    == field.getFirstSubfieldValue ('g'));
}

TEST_CASE ("Title_applyTo_1", "[title]")
{
    irbis::Title title;
    title.title    = L"\u041F\u0438\u043A\u0430\u0441\u0441\u043E \u0441\u0435\u0433\u043E\u0434\u043D\u044F";
    title.subtitle = L"[\u0441\u0431\u043E\u0440\u043D\u0438\u043A \u0441\u0442\u0430\u0442\u0435\u0439]";
    title.first    = L"\u0410. \u0410. \u0411\u0430\u0431\u0438\u043D";
    title.other    = L"\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0430\u043A\u0430\u0434\u0435\u043C\u0438\u044F \u0445\u0443\u0434\u043E\u0436\u0435\u0441\u0442\u0432";

    irbis::RecordField field (200);
    title.applyTo (field);
    CHECK (title.number   == field.getFirstSubfieldValue ('n'));
    CHECK (title.title    == field.getFirstSubfieldValue ('a'));
    CHECK (title.specific == field.getFirstSubfieldValue ('u'));
    CHECK (title.general  == field.getFirstSubfieldValue ('b'));
    CHECK (title.subtitle == field.getFirstSubfieldValue ('e'));
    CHECK (title.first    == field.getFirstSubfieldValue ('f'));
    CHECK (title.other    == field.getFirstSubfieldValue ('g'));
}

TEST_CASE ("Title_fullTitle_1", "[title]")
{
    irbis::Title title;
    title.title    = L"\u041F\u0438\u043A\u0430\u0441\u0441\u043E \u0441\u0435\u0433\u043E\u0434\u043D\u044F";
    title.subtitle = L"[\u0441\u0431\u043E\u0440\u043D\u0438\u043A \u0441\u0442\u0430\u0442\u0435\u0439]";
    title.first    = L"\u0410. \u0410. \u0411\u0430\u0431\u0438\u043D";
    title.other    = L"\u0420\u043E\u0441\u0441\u0438\u0439\u0441\u043A\u0430\u044F \u0430\u043A\u0430\u0434\u0435\u043C\u0438\u044F \u0445\u0443\u0434\u043E\u0436\u0435\u0441\u0442\u0432";

    const auto full = title.fullTitle();
    CHECK (full == L"\u041F\u0438\u043A\u0430\u0441\u0441\u043E \u0441\u0435\u0433\u043E\u0434\u043D\u044F : [\u0441\u0431\u043E\u0440\u043D\u0438\u043A \u0441\u0442\u0430\u0442\u0435\u0439]");
}

