// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"

TEST_CASE("TextNavigator_constructor_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    const irbis::TextNavigator navigator(hello);
    CHECK_FALSE(navigator.eot());
    CHECK(navigator.line() == std::size_t(1));
    CHECK(navigator.column() == std::size_t(1));
    CHECK(navigator.position() == std::size_t(0));
    CHECK(navigator.length() == std::size_t(13));
}

TEST_CASE("TextNavigator_constructor_2", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    const irbis::TextNavigator navigator1(hello);
    const irbis::TextNavigator navigator2(navigator1);  // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(navigator2.line() == std::size_t(1));
    CHECK(navigator2.column() == std::size_t(1));
    CHECK(navigator2.position() == std::size_t(0));
    CHECK(navigator2.length() == std::size_t(13));
}

TEST_CASE("TextNavigator_at_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    const irbis::TextNavigator navigator(hello);
    CHECK(navigator.at(0) == L'H');
    CHECK(navigator.at(-1) == irbis::TextNavigator::EOT);
    CHECK(navigator.at(100) == irbis::TextNavigator::EOT);
    CHECK(navigator.at(1) == L'e');
    CHECK(navigator.at(12) == L'!');
}

TEST_CASE("TextNavigator_front_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    const irbis::TextNavigator navigator(hello);
    CHECK(navigator.front() == L'H');
}

TEST_CASE("TextNavigator_front_2", "[navigator]")
{
    const irbis::String hello;
    const irbis::TextNavigator navigator(hello);
    CHECK(navigator.front() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_back_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    const irbis::TextNavigator navigator(hello);
    CHECK(navigator.back() == L'!');
}

TEST_CASE("TextNavigator_back_2", "[navigator]")
{
    const irbis::String hello;
    const irbis::TextNavigator navigator(hello);
    CHECK(navigator.back() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_lookAhead_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    const irbis::TextNavigator navigator(hello);
    CHECK(navigator.lookAhead() == L'e');
    CHECK(navigator.lookAhead(1) == L'e');
    CHECK(navigator.lookAhead(2) == L'l');
    CHECK(navigator.lookAhead(3) == L'l');
    CHECK(navigator.lookAhead(4) == L'o');
}

TEST_CASE("TextNavigator_lookBehind_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    irbis::TextNavigator navigator(hello);
    navigator.move(13);
    CHECK(navigator.lookBehind() == L'!');
    CHECK(navigator.lookBehind(1) == L'!');
    CHECK(navigator.lookBehind(2) == L'd');
    CHECK(navigator.lookBehind(3) == L'l');
    CHECK(navigator.lookBehind(4) == L'r');
    CHECK(navigator.lookBehind(5) == L'o');
    CHECK(navigator.lookBehind(6) == L'w');
}

TEST_CASE("TextNavigator_move_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    irbis::TextNavigator navigator(hello);
    navigator.move(10);
    CHECK(navigator.position() == 10);
}

TEST_CASE("TextNavigator_peekChar_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    irbis::TextNavigator navigator(hello);
    CHECK(navigator.peekChar() == L'H');
    CHECK(navigator.peekChar() == L'H');
    navigator.move(5);
    CHECK(navigator.peekChar() == L',');
    CHECK(navigator.peekChar() == L',');
    navigator.move(13);
    CHECK(navigator.peekChar() == irbis::TextNavigator::EOT);
    CHECK(navigator.peekChar() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_readChar_1", "[navigator]")
{
    const irbis::String hello = L"Hello, world!";
    irbis::TextNavigator navigator(hello);
    CHECK(navigator.readChar() == L'H');
    CHECK(navigator.readChar() == L'e');
    CHECK(navigator.readChar() == L'l');
    CHECK(navigator.readChar() == L'l');
    CHECK(navigator.readChar() == L'o');
    CHECK(navigator.readChar() == L',');
    CHECK(navigator.readChar() == L' ');
    CHECK(navigator.readChar() == L'w');
    CHECK(navigator.readChar() == L'o');
    CHECK(navigator.readChar() == L'r');
    CHECK(navigator.readChar() == L'l');
    CHECK(navigator.readChar() == L'd');
    CHECK(navigator.readChar() == L'!');
    CHECK(navigator.readChar() == irbis::TextNavigator::EOT);
    CHECK(navigator.readChar() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_peekString_1", "[navigator]")
{
    const irbis::String hello = L"Hello\nworld";
    irbis::TextNavigator navigator(hello);
    CHECK(navigator.peekString(5).toString() == L"Hello");
    CHECK(navigator.peekString(5).toString() == L"Hello");
    navigator.move(1);
    CHECK(navigator.peekString(5).toString() == L"ello");
    CHECK(navigator.peekString(5).toString() == L"ello");
    navigator.move(5);
    CHECK(navigator.peekString(5).toString() == L"world");
    CHECK(navigator.peekString(5).toString() == L"world");
    navigator.move(1);
    CHECK(navigator.peekString(5).toString() == L"orld");
    CHECK(navigator.peekString(5).toString() == L"orld");
}

TEST_CASE("TextNavigator_readLine_1", "[navigator]")
{
    const irbis::String hello = L"Hello\nworld";
    irbis::TextNavigator navigator(hello);
    CHECK(navigator.readLine().toString() == L"Hello");
    CHECK(navigator.readLine().toString() == L"world");
    CHECK(navigator.readLine().empty());
    CHECK(navigator.eot());
}
