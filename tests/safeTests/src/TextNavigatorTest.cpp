// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"

TEST_CASE("TextNavigator_constructor_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    const irbis::TextNavigator navigator (hello);
    CHECK_FALSE (navigator.eot());
    CHECK (navigator.line()     == std::size_t(1));
    CHECK (navigator.column()   == std::size_t(1));
    CHECK (navigator.position() == std::size_t(0));
    CHECK (navigator.length()   == std::size_t(13));
}

TEST_CASE("TextNavigator_constructor_2", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    const irbis::TextNavigator navigator1 (hello);
    const irbis::TextNavigator navigator2 (navigator1);  // NOLINT(performance-unnecessary-copy-initialization)
    CHECK (navigator2.line()     == std::size_t(1));
    CHECK (navigator2.column()   == std::size_t(1));
    CHECK (navigator2.position() == std::size_t(0));
    CHECK (navigator2.length()   == std::size_t(13));
}

TEST_CASE("TextNavigator_at_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    const irbis::TextNavigator navigator (hello);
    CHECK (navigator.at (0)   == L'H');
    CHECK (navigator.at (-1)  == irbis::TextNavigator::EOT);
    CHECK (navigator.at (100) == irbis::TextNavigator::EOT);
    CHECK (navigator.at (1)   == L'e');
    CHECK (navigator.at (12)  == L'!');
}

TEST_CASE("TextNavigator_front_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    const irbis::TextNavigator navigator (hello);
    CHECK (navigator.front() == L'H');
}

TEST_CASE("TextNavigator_front_2", "[navigator]")
{
    const irbis::String hello;
    const irbis::TextNavigator navigator (hello);
    CHECK (navigator.front() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_back_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    const irbis::TextNavigator navigator (hello);
    CHECK (navigator.back() == L'!');
}

TEST_CASE("TextNavigator_back_2", "[navigator]")
{
    const irbis::String hello;
    const irbis::TextNavigator navigator (hello);
    CHECK (navigator.back() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_lookAhead_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    const irbis::TextNavigator navigator (hello);
    CHECK (navigator.lookAhead()  == L'e');
    CHECK (navigator.lookAhead(1) == L'e');
    CHECK (navigator.lookAhead(2) == L'l');
    CHECK (navigator.lookAhead(3) == L'l');
    CHECK (navigator.lookAhead(4) == L'o');
}

TEST_CASE("TextNavigator_lookBehind_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    irbis::TextNavigator navigator (hello);
    navigator.move (13);
    CHECK (navigator.lookBehind()  == L'!');
    CHECK (navigator.lookBehind(1) == L'!');
    CHECK (navigator.lookBehind(2) == L'd');
    CHECK (navigator.lookBehind(3) == L'l');
    CHECK (navigator.lookBehind(4) == L'r');
    CHECK (navigator.lookBehind(5) == L'o');
    CHECK (navigator.lookBehind(6) == L'w');
}

TEST_CASE("TextNavigator_move_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    irbis::TextNavigator navigator (hello);
    navigator.move (10);
    CHECK (navigator.position() == 10);
}

TEST_CASE("TextNavigator_peekChar_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    irbis::TextNavigator navigator (hello);
    CHECK (navigator.peekChar() == L'H');
    CHECK (navigator.peekChar() == L'H');
    navigator.move (5);
    CHECK (navigator.peekChar() == L',');
    CHECK (navigator.peekChar() == L',');
    navigator.move (13);
    CHECK (navigator.peekChar() == irbis::TextNavigator::EOT);
    CHECK (navigator.peekChar() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_readChar_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world!" };
    irbis::TextNavigator navigator (hello);
    CHECK (navigator.readChar() == L'H');
    CHECK (navigator.readChar() == L'e');
    CHECK (navigator.readChar() == L'l');
    CHECK (navigator.readChar() == L'l');
    CHECK (navigator.readChar() == L'o');
    CHECK (navigator.readChar() == L',');
    CHECK (navigator.readChar() == L' ');
    CHECK (navigator.readChar() == L'w');
    CHECK (navigator.readChar() == L'o');
    CHECK (navigator.readChar() == L'r');
    CHECK (navigator.readChar() == L'l');
    CHECK (navigator.readChar() == L'd');
    CHECK (navigator.readChar() == L'!');
    CHECK (navigator.readChar() == irbis::TextNavigator::EOT);
    CHECK (navigator.readChar() == irbis::TextNavigator::EOT);
}

TEST_CASE("TextNavigator_peekString_1", "[navigator]")
{
    const irbis::String hello { L"Hello\nworld" };
    irbis::TextNavigator navigator (hello);
    CHECK (navigator.peekString (5).toString() == L"Hello");
    CHECK (navigator.peekString (5).toString() == L"Hello");
    navigator.move (1);
    CHECK (navigator.peekString (5).toString() == L"ello");
    CHECK (navigator.peekString (5).toString() == L"ello");
    navigator.move (5);
    CHECK (navigator.peekString (5).toString() == L"world");
    CHECK (navigator.peekString (5).toString() == L"world");
    navigator.move (1);
    CHECK (navigator.peekString (5).toString() == L"orld");
    CHECK (navigator.peekString (5).toString() == L"orld");
}

TEST_CASE("TextNavigator_readLine_1", "[navigator]")
{
    const irbis::String hello { L"Hello\nworld" };
    irbis::TextNavigator navigator (hello);
    CHECK (navigator.readLine().toString() == L"Hello");
    CHECK (navigator.readLine().toString() == L"world");
    CHECK (navigator.readLine().empty());
    CHECK (navigator.eot());
}

TEST_CASE("TextNavigator_isControl_1", "[navigator]")
{
    const irbis::Char chars[] { '1', '\b', '2' };
    irbis::TextNavigator navigator { chars, irbis::size (chars) };
    CHECK_FALSE (navigator.isControl());
    navigator.readChar();
    CHECK (navigator.isControl());
    navigator.readChar();
    CHECK_FALSE (navigator.isControl());
}

TEST_CASE("TextNavigator_isDigit_1", "[navigator]")
{
    const irbis::Char chars[] { '1', '\b', '2' };
    irbis::TextNavigator navigator { chars, irbis::size (chars) };
    CHECK (navigator.isDigit());
    navigator.readChar();
    CHECK_FALSE (navigator.isDigit());
    navigator.readChar();
    CHECK (navigator.isDigit());
}

TEST_CASE("TextNavigator_isLetter_1", "[navigator]")
{
    const irbis::Char chars[] { '1', 'a', '2' };
    irbis::TextNavigator navigator { chars, irbis::size (chars) };
    CHECK_FALSE (navigator.isLetter());
    navigator.readChar();
    CHECK (navigator.isLetter());
    navigator.readChar();
    CHECK_FALSE (navigator.isLetter());
}

TEST_CASE("TextNavigator_isWhitespace_1", "[navigator]")
{
    const irbis::Char chars[] { '1', '\t', '2' };
    irbis::TextNavigator navigator { chars, irbis::size (chars) };
    CHECK_FALSE (navigator.isWhitespace());
    navigator.readChar();
    CHECK (navigator.isWhitespace());
    navigator.readChar();
    CHECK_FALSE (navigator.isWhitespace());
}

TEST_CASE("TextNavigator_extractInteger_1", "[navigator]")
{
    const irbis::String hello { L"Hello 12345 world" };
    irbis::TextNavigator navigator (hello);
    const auto extracted = navigator.extractInteger();
    CHECK (extracted == irbis::String (L"12345"));
}

TEST_CASE("TextNavigator_readString_1", "[navigator]")
{
    const irbis::String hello { L"Hello world" };
    irbis::TextNavigator navigator (hello);
    navigator.readChar();
    const auto read = navigator.readString (4);
    CHECK (read == irbis::String (L"ello"));
}

TEST_CASE("TextNavigator_readTo_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto read = navigator.readTo (',');
    CHECK (read == irbis::String (L"Hello"));
    read = navigator.readTo (',');
    CHECK (read == irbis::String (L" world"));
}

TEST_CASE("TextNavigator_readUntil_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto read = navigator.readUntil (',');
    CHECK (read == irbis::String (L"Hello"));
    read = navigator.readUntil (',');
    CHECK (read.empty());
    navigator.readChar();
    read = navigator.readUntil (',');
    CHECK (read == irbis::String (L" world"));
}

TEST_CASE("TextNavigator_readWhile_1", "[navigator]")
{
    const irbis::String hello { L"xxxHello, world" };
    irbis::TextNavigator navigator (hello);
    auto read = navigator.readWhile ('x');
    CHECK (read == irbis::String (L"xxx"));
    read = navigator.readWhile ('x');
    CHECK (read.empty());
}

TEST_CASE("TextNavigator_readWord_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto read = navigator.readWord ();
    CHECK (read == irbis::String (L"Hello"));
    read = navigator.readWord ();
    CHECK (read.empty());
    navigator.move (2);
    read = navigator.readWord ();
    CHECK (read == irbis::String (L"world"));
}

TEST_CASE("TextNavigator_remainingText_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto read = navigator.readWord ();
    CHECK (read == irbis::String (L"Hello"));
    read = navigator.remainingText();
    CHECK (read == irbis::String (L", world"));
}

TEST_CASE("TextNavigator_recentText_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto read = navigator.readWord();
    CHECK (read == irbis::String (L"Hello"));
    read = navigator.recentText (4);
    CHECK (read == irbis::String (L"ello"));
}

TEST_CASE("TextNavigator_skipNonWord_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    navigator.readWord();
    navigator.skipNonWord();
    auto read = navigator.readWord();
    CHECK (read == irbis::String (L"world"));
}

TEST_CASE("TextNavigator_skipWhitespace_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    navigator.readWord();
    navigator.skipPunctuation();
    navigator.skipWhitespace();
    auto read = navigator.readWord();
    CHECK (read == irbis::String (L"world"));
}

TEST_CASE("TextNavigator_iterators_1", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto iterator = navigator.cbegin();
    CHECK (*iterator == 'H');
    ++iterator;
    CHECK (*iterator == 'e');
    CHECK (iterator != navigator.cend());
}

TEST_CASE("TextNavigator_iterators_2", "[navigator]")
{
    const irbis::String hello { L"Hello, world" };
    irbis::TextNavigator navigator (hello);
    auto iterator = navigator.begin();
    CHECK (*iterator == 'H');
    ++iterator;
    CHECK (*iterator == 'e');
    CHECK (iterator != navigator.end());
}



