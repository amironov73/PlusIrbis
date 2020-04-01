// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

TEST_CASE("AlphabetTable_constructor", "[alphabet]")
{
    const irbis::AlphabetTable table;
    CHECK (table.characters.empty());
}

TEST_CASE("AlphabetTable_instance_1", "[alphabet]")
{
    const auto table = irbis::AlphabetTable::instance();
    CHECK_FALSE (table.characters.empty());
}

TEST_CASE("AlphabetTable_readLocalFile_1", "[alphabet]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"ISISACW.TAB");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    auto table = irbis::AlphabetTable::readLocalFile (path);
    CHECK (table.verify (false));
}

TEST_CASE("AlphabetTable_trimText_1", "[alphabet]")
{
    const auto table = irbis::AlphabetTable::instance();
    REQUIRE (table.verify (false));
    CHECK (table.trimText(irbis::String()).empty());
    CHECK (table.trimText (L"") == L"");
    CHECK (table.trimText (L"Hello") == L"Hello");
    CHECK (table.trimText (L"  Hello") == L"Hello");
    CHECK (table.trimText (L"Hello  ") == L"Hello");
    CHECK (table.trimText (L"  Hello  ") == L"Hello");
}

TEST_CASE("AlphabetTable_splitWords_1", "[alphabet]")
{
    const auto table = irbis::AlphabetTable::instance();
    REQUIRE (table.verify (false));
    const irbis::String text = L"Hello, world! \u0421\u044A\u0435\u0448\u044C \u0435\u0449\u0451(\u044D\u0442\u0438\u0445)\u043C\u044F\u0433\u043A\u0438\u0445 \u0444\u0440\u0430\u043D\u0446\u0443\u0437\u0441\u043A\u0438\u0445 \u0431\u0443\u043B\u043E\u043A?12345 \u0432\u044B\u0448\u0435\u043B \u0437\u0430\u0439\u0447\u0438\u043A \u043F\u043E\u0433\u0443\u043B\u044F\u0442\u044C.";
    const auto words = table.splitWords (text);
    CHECK (words.size() == 11);
    CHECK (table.splitWords (L"").empty());
}