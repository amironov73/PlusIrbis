// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

// ReSharper disable StringLiteralTypo

TEST_CASE("Text_fromIrbisToDos_1", "[text]")
{
    irbis::String text;
    CHECK (irbis::Text::fromIrbisToDos (text).empty());

    text = L" ";
    CHECK (irbis::Text::fromIrbisToDos (text) == L" ");

    text = L"Hello";
    CHECK (irbis::Text::fromIrbisToDos (text) == L"Hello");

    text = L"\u001F\u001E";
    CHECK (irbis::Text::fromIrbisToDos (text) == L"\r\n");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromIrbisToDos (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\u001F\u001E\u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromIrbisToDos (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\r\n\u0441\u043E\u0431\u0430\u043A\u0430");
}

TEST_CASE("Text_fromDosToIrbis_1", "[text]")
{
    irbis::String text;
    CHECK (irbis::Text::fromDosToIrbis (text).empty());

    text = L" ";
    CHECK (irbis::Text::fromDosToIrbis (text) == L" ");

    text = L"Hello";
    CHECK (irbis::Text::fromDosToIrbis (text) == L"Hello");

    text = L"\r\n";
    CHECK (irbis::Text::fromDosToIrbis (text) == L"\u001F\u001E");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromDosToIrbis (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\r\n\u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromDosToIrbis (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\u001F\u001E\u0441\u043E\u0431\u0430\u043A\u0430");
}

TEST_CASE("Text_fromIrbisToUnix_1", "[text]")
{
    irbis::String text;
    CHECK (irbis::Text::fromIrbisToUnix (text).empty());

    text = L" ";
    CHECK (irbis::Text::fromIrbisToUnix (text) == L" ");

    text = L"Hello";
    CHECK (irbis::Text::fromIrbisToUnix (text) == L"Hello");

    text = L"\u001F\u001E";
    CHECK (irbis::Text::fromIrbisToUnix (text) == L"\n");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromIrbisToUnix (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\u001F\u001E\u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromIrbisToUnix (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\n\u0441\u043E\u0431\u0430\u043A\u0430");
}

TEST_CASE("Text_fromUnixToIrbis_1", "[text]")
{
    irbis::String text;
    CHECK (irbis::Text::fromUnixToIrbis (text).empty());

    text = L" ";
    CHECK (irbis::Text::fromUnixToIrbis (text) == L" ");

    text = L"Hello";
    CHECK (irbis::Text::fromUnixToIrbis (text) == L"Hello");

    text = L"\n";
    CHECK (irbis::Text::fromUnixToIrbis (text) == L"\u001F\u001E");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromUnixToIrbis (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\n\u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromUnixToIrbis (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\u001F\u001E\u0441\u043E\u0431\u0430\u043A\u0430");
}

TEST_CASE("Text_fromDosToUnix_1", "[text]")
{
    irbis::String text;
    CHECK (irbis::Text::fromDosToUnix (text).empty());

    text = L" ";
    CHECK (irbis::Text::fromDosToUnix (text) == L" ");

    text = L"Hello";
    CHECK (irbis::Text::fromDosToUnix (text) == L"Hello");

    text = L"\r\n\r\n";
    CHECK (irbis::Text::fromDosToUnix (text) == L"\n\n");

    text = L"\n\n";
    CHECK (irbis::Text::fromDosToUnix (text) == L"\n\n");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromDosToUnix (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\r\n\u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromDosToUnix (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\n\u0441\u043E\u0431\u0430\u043A\u0430");
}

TEST_CASE("Text_fromUnixToDos_1", "[text]")
{
    irbis::String text;
    CHECK (irbis::Text::fromUnixToDos (text).empty());

    text = L" ";
    CHECK (irbis::Text::fromUnixToDos (text) == L" ");

    text = L"Hello";
    CHECK (irbis::Text::fromUnixToDos (text) == L"Hello");

    text = L"\n\n";
    CHECK (irbis::Text::fromUnixToDos (text) == L"\r\n\r\n");

    text = L"\n";
    CHECK (irbis::Text::fromUnixToDos (text) == L"\r\n");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromUnixToDos (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430 \u0441\u043E\u0431\u0430\u043A\u0430");

    text = L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\n\u0441\u043E\u0431\u0430\u043A\u0430";
    CHECK (irbis::Text::fromUnixToDos (text) == L"\u0423 \u043F\u043E\u043F\u0430 \u0431\u044B\u043B\u0430\r\n\u0441\u043E\u0431\u0430\u043A\u0430");
}

TEST_CASE("Text_fromFullDelimiter_1", "[text]")
{
    irbis::String text;
    irbis::StringList list = irbis::Text::fromFullDelimiter (text);
    CHECK (list.empty());

    text = L" ";
    list = irbis::Text::fromFullDelimiter (text);
    CHECK (list.size() == 1);
    CHECK (list[0] == L" ");

    text = L"\u001F\u001E";
    list = irbis::Text::fromFullDelimiter (text);
    CHECK (list.size() == 1);
    CHECK (list[0].empty());

    text = L"\u001F\u001EHello";
    list = irbis::Text::fromFullDelimiter (text);
    CHECK (list.size() == 2);
    CHECK (list[0].empty());
    CHECK (list[1] == L"Hello");

    text = L"Hello\u001F\u001Eworld";
    list = irbis::Text::fromFullDelimiter (text);
    CHECK (list.size() == 2);
    CHECK (list[0] == L"Hello");
    CHECK (list[1] == L"world");
}

TEST_CASE("Text_fromShortDelimiter_1", "[text]")
{
    irbis::String text;
    irbis::StringList list = irbis::Text::fromFullDelimiter (text);
    CHECK (list.empty());

    text = L" ";
    list = irbis::Text::fromShortDelimiter (text);
    CHECK (list.size() == 1);
    CHECK (list[0] == L" ");

    text = L"\u001E";
    list = irbis::Text::fromShortDelimiter (text);
    CHECK (list.size() == 1);
    CHECK (list[0].empty());

    text = L"\u001EHello";
    list = irbis::Text::fromShortDelimiter (text);
    CHECK (list.size() == 2);
    CHECK (list[0].empty());
    CHECK (list[1] == L"Hello");

    text = L"Hello\u001Eworld";
    list = irbis::Text::fromShortDelimiter (text);
    CHECK (list.size() == 2);
    CHECK (list[0] == L"Hello");
    CHECK (list[1] == L"world");
}

TEST_CASE("Text_readAllAnsi_1", "[text]")
{
    auto path = whereDatai();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"ibis.par");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    auto content = irbis::Text::readAllAnsi (path);
    CHECK (content.size() == 178);
}

TEST_CASE("Text_readAllUtf_1", "[text]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"utf8.txt");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    auto content = irbis::Text::readAllUtf (path);
    CHECK (content.size() == 119);
}

TEST_CASE("Text_readAnsiLines_1", "[text]")
{
    auto path = whereDatai();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"ibis.par");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    auto content = irbis::Text::readAnsiLines (path);
    CHECK (content.size() == 11);
    CHECK (content[0] == L"1=.\\datai\\ibis\\");
}

TEST_CASE("Text_readUtfLines_1", "[text]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"utf8.txt");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    auto content = irbis::Text::readUtfLines (path);
    CHECK (content.size() == 3);
    CHECK (content[0] == L"\u042D\u0442\u043E\u0442 \u0444\u0430\u0439\u043B \u0441\u043E\u0434\u0435\u0440\u0436\u0438\u0442 \u0442\u0435\u043A\u0441\u0442 \u0432 \u043A\u043E\u0434\u0438\u0440\u043E\u0432\u043A\u0435 UTF-8");
    CHECK (content[2] == L"\u041F\u043E\u043B\u0435\u0437\u043D\u044B\u0445 \u0434\u0430\u043D\u043D\u044B\u0445 \u0432 \u043D\u0451\u043C \u043D\u0435\u0442");
}
