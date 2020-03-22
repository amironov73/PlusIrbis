// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

TEST_CASE("MenuEntry_constructor_1", "[menu]")
{
    irbis::MenuEntry entry;
    CHECK (entry.code.empty());
    CHECK (entry.comment.empty());
}

TEST_CASE("MenuEntry_constructor_2", "[menu]")
{
    irbis::String code (L"code");
    irbis::String comment (L"comment");
    irbis::MenuEntry entry (code, comment);
    CHECK (entry.code == L"code");
    CHECK (entry.comment == L"comment");
}

TEST_CASE("MenuEntry_constructor_3", "[menu]")
{
    irbis::String code (L"code");
    irbis::String comment (L"comment");
    irbis::MenuEntry entry (std::move (code), std::move (comment));
    CHECK (entry.code == L"code");
    CHECK (entry.comment == L"comment");
}

TEST_CASE("MenuEntry_toString_1", "[menu]")
{
    irbis::MenuEntry entry {};
    CHECK (entry.toString().empty());
    entry.code = L"code";
    CHECK (entry.toString() == L"code");
    entry.comment = L"comment";
    CHECK (entry.toString() == L"code - comment");
}

//=========================================================

TEST_CASE("MenuFile_constructor_1", "[menu]")
{
    irbis::MenuFile menu;
    CHECK (menu.fileName.empty());
    CHECK (menu.entries.empty());
}

TEST_CASE("MenuFile_add_1", "[menu]")
{
    irbis::String code (L"code");
    irbis::String comment (L"comment");
    irbis::MenuFile menu;
    const auto result = menu.add (code, comment);
    CHECK (result.entries.size() == 1);
    CHECK (menu.entries.size() == 1);
    const auto &entry = menu.entries.front();
    CHECK (entry.code == L"code");
    CHECK (entry.comment == L"comment");
}

TEST_CASE("MenuFile_add_2", "[menu]")
{
    irbis::String code (L"code");
    irbis::String comment (L"comment");
    irbis::MenuFile menu;
    const auto result = menu.add (std::move (code), std::move (comment));
    CHECK (result.entries.size() == 1);
    CHECK (menu.entries.size() == 1);
    const auto &entry = menu.entries.front();
    CHECK (entry.code == L"code");
    CHECK (entry.comment == L"comment");
}

TEST_CASE("MenuFile_getValue_1", "[menu]")
{
    irbis::MenuFile menu;
    CHECK (menu.getValue (L"hello").empty());
    CHECK (menu.getValueSensitive(L"hello").empty());
}

TEST_CASE("MenuFile_getValue_2", "[menu]")
{
    irbis::MenuFile menu;
    menu.add (L"a", L"comment for a");
    menu.add (L"b", L"comment for b");
    menu.add (L"c", L"comment for c");
    CHECK (menu.getValue (L"hello").empty());
    CHECK (menu.getValueSensitive(L"hello").empty());
    CHECK (menu.getValue (L"B") == L"comment for b");
    CHECK (menu.getValueSensitive(L"B").empty());
    CHECK (menu.getValueSensitive(L"b") == L"comment for b");
}

TEST_CASE("MenuFile_getValue_3", "[menu]")
{
    irbis::MenuFile menu;
    menu.add (L"a-il", L"comment for a");
    menu.add (L"b-zil", L"comment for b");
    menu.add (L"c-topil", L"comment for c");
    CHECK (menu.getValue (L"hello").empty());
    CHECK (menu.getValueSensitive(L"hello").empty());
    CHECK (menu.getValue (L"B") == L"comment for b");
    CHECK (menu.getValueSensitive(L"B").empty());
    CHECK (menu.getValueSensitive(L"b") == L"comment for b");
}

TEST_CASE("MenuFile_getValue_4", "[menu]")
{
    irbis::MenuFile menu;
    menu.add (L"a", L"comment for a");
    menu.add (L"b", L"comment for b");
    menu.add (L"c", L"comment for c");
    CHECK (menu.getValue (L"hello", L"world") == L"world");
    CHECK (menu.getValueSensitive(L"hello", L"world") == L"world");
    CHECK (menu.getValue (L"B", L"world") == L"comment for b");
    CHECK (menu.getValueSensitive(L"B", L"world") == L"world");
    CHECK (menu.getValueSensitive(L"b", L"world") == L"comment for b");
}

TEST_CASE("MenuFile_getValue_5", "[menu]")
{
    irbis::MenuFile menu;
    menu.add (L"a", L"comment for a");
    menu.add (L"b", L"comment for b");
    menu.add (L"c", L"comment for c");
    CHECK (menu.getValue (L"hello").empty());
    CHECK (menu.getValueSensitive(L"hello").empty());
    CHECK (menu.getValue (L" B ") == L"comment for b");
    CHECK (menu.getValueSensitive(L" B ").empty());
    CHECK (menu.getValueSensitive(L" b ") == L"comment for b");
}

TEST_CASE("MenuFile_parse_1", "[menu]")
{
    irbis::StringList lines;
    irbis::MenuFile menu;
    menu.parse (lines);
    CHECK (menu.entries.empty());
}

TEST_CASE("MenuFile_parse_2", "[menu]")
{
    irbis::StringList lines;
    lines.push_back ({});
    irbis::MenuFile menu;
    menu.parse (lines);
    CHECK (menu.entries.empty());
}

TEST_CASE("MenuFile_parse_3", "[menu]")
{
    irbis::StringList lines;
    lines.push_back (L"a");
    irbis::MenuFile menu;
    menu.parse (lines);
    CHECK (menu.entries.empty());
}

TEST_CASE("MenuFile_parse_4", "[menu]")
{
    irbis::StringList lines;
    lines.push_back (L"a");
    lines.push_back (L"*****");
    irbis::MenuFile menu;
    menu.parse (lines);
    CHECK (menu.entries.empty());
}

TEST_CASE("MenuFile_readLocalFile_1", "[menu]")
{
    auto path = irbis::IO::combinePath (whereTestData(), L"org.mnu");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    irbis::MenuFile menu;
    menu.parseLocalFile (path);
    CHECK (menu.entries.size() == 9);
}
