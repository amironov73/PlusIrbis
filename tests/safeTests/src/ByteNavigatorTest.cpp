// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"

//-V::801

TEST_CASE("ByteNavigator_constructor_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.size() == 0);
    CHECK (navigator.position() == 0);
    CHECK (navigator.front() == irbis::ByteNavigator::EOT);
    CHECK (navigator.back() == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_constructor_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof(array)));
    CHECK (navigator.size() == sizeof (array));
    CHECK (navigator.position() == 0);
    CHECK (navigator.front() == 1);
    CHECK (navigator.back() == 7);
    CHECK (*navigator.ccurrent() == 1);
    CHECK (navigator.ccurrent() == navigator.cbegin());
    CHECK (navigator.current() == navigator.ccurrent());
    CHECK (navigator.current() == navigator.begin());
    CHECK ((navigator.cend() - navigator.cbegin()) == navigator.size());
    CHECK ((navigator.end() - navigator.begin()) == navigator.size());
}

TEST_CASE("ByteNavigator_at_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.at (-1) == irbis::ByteNavigator::EOT);
    CHECK (navigator.at (0) == irbis::ByteNavigator::EOT);
    CHECK (navigator.at (1) == irbis::ByteNavigator::EOT);
    CHECK (navigator.at (2) == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_at_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.at (-1) == irbis::ByteNavigator::EOT);
    CHECK (navigator.at (0) == 1);
    CHECK (navigator.at (1) == 2);
    CHECK (navigator.at (2) == 3);
    CHECK (navigator.at (3) == 4);
    CHECK (navigator.at (4) == 5);
    CHECK (navigator.at (5) == 6);
    CHECK (navigator.at (6) == 7);
    CHECK (navigator.at (7) == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_remaining_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    const auto remaining = navigator.remaining();
    CHECK (remaining.empty());
}

TEST_CASE("ByteNavigator_remaining_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto remaining = navigator.remaining();
    CHECK_FALSE (remaining.empty());
    CHECK (remaining.size() == sizeof (array));
    CHECK (remaining[0] == 1);
    CHECK (remaining[1] == 2);
    navigator.move(3);
    remaining = navigator.remaining();
    CHECK_FALSE (remaining.empty());
    CHECK (remaining.size() == sizeof (array) - 3);
    CHECK (remaining[0] == 4);
    CHECK (remaining[1] == 5);
}

TEST_CASE("ByteNavigator_lookAhead_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.lookAhead(1) == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_lookAhead_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.lookAhead (1) == 2);
    CHECK (navigator.lookAhead (2) == 3);
    navigator.move (3);
    CHECK (navigator.lookAhead (1) == 5);
    CHECK (navigator.lookAhead (2) == 6);
    navigator.move (3);
    CHECK (navigator.lookAhead (1) == irbis::ByteNavigator::EOT);
    CHECK (navigator.lookAhead (2) == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_lookBehind_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK(navigator.lookBehind (1) == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_lookBehind_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.lookBehind (1) == irbis::ByteNavigator::EOT);
    CHECK (navigator.lookBehind (2) == irbis::ByteNavigator::EOT);
    navigator.move (3);
    CHECK (navigator.lookBehind (1) == 3);
    CHECK (navigator.lookBehind (2) == 2);
    navigator.move (3);
    CHECK (navigator.lookBehind (1) == 6);
    CHECK (navigator.lookBehind (2) == 5);
}

TEST_CASE("ByteNavigator_move_1", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    navigator.move (3);
    CHECK (navigator.position() == 3);
    CHECK_FALSE (navigator.eot());
    navigator.move (3);
    CHECK (navigator.position() == 6);
    CHECK_FALSE (navigator.eot());
    navigator.move (3);
    CHECK (navigator.position() == 9);
    CHECK (navigator.eot());
}

TEST_CASE("ByteNavigator_peekByte_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.peekByte() == irbis::ByteNavigator::EOT);
    CHECK (navigator.peekByte() == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_peekByte_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.peekByte() == 1);
    CHECK (navigator.peekByte() == 1);
    navigator.move (3);
    CHECK (navigator.peekByte() == 4);
    CHECK (navigator.peekByte() == 4);
    navigator.move (3);
    CHECK (navigator.peekByte() == 7);
    CHECK (navigator.peekByte() == 7);
    navigator.move (3);
    CHECK (navigator.peekByte() == irbis::ByteNavigator::EOT);
    CHECK (navigator.peekByte() == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_readByte_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readByte() == irbis::ByteNavigator::EOT);
    CHECK (navigator.readByte() == irbis::ByteNavigator::EOT);
}

TEST_CASE("ByteNavigator_readByte_2", "[navigator]")
{
    irbis::Byte array[] { 1, 2, 3, 4, 5, 6, 7 };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.readByte() == 1);
    CHECK (navigator.position() == 1);
    CHECK (navigator.readByte() == 2);
    CHECK (navigator.position() == 2);
    navigator.move (3);
    CHECK (navigator.position() == 5);
    CHECK (navigator.readByte() == 6);
    CHECK (navigator.position() == 6);
    CHECK (navigator.readByte() == 7);
    CHECK (navigator.position() == 7);
    CHECK (navigator.readByte() == irbis::ByteNavigator::EOT);
    CHECK (navigator.position() == 7);
    CHECK (navigator.readByte() == irbis::ByteNavigator::EOT);
    CHECK (navigator.position() == 7);
}

TEST_CASE("ByteNavigator_peekString_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.peekString (10).empty());
}

TEST_CASE("ByteNavigator_peekString_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', '\n', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    const auto s = navigator.peekString (10);
    CHECK (s.size() == 5);
    CHECK (navigator.position() == 0);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
}

TEST_CASE("ByteNavigator_peekTo_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.peekTo ('o').empty());
}

TEST_CASE("ByteNavigator_peekTo_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', '\n', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    const auto s = navigator.peekTo ('o');
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
}

TEST_CASE("ByteNavigator_peekUntil_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.peekUntil ('o').empty());
}

TEST_CASE("ByteNavigator_peekUntil_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', '\n', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    const auto s = navigator.peekUntil ('o');
    CHECK (s.size() == 4);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
}

TEST_CASE("ByteNavigator_readLine_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readLine ().empty());
}

TEST_CASE("ByteNavigator_readLine_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', '\n', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readLine();
    CHECK (navigator.position() == 6);
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
    s = navigator.readLine();
    CHECK (navigator.position() == sizeof (array));
    CHECK (s.size() == 5);
    CHECK (s[0] == 'w');
    CHECK (s[1] == 'o');
    CHECK (s[2] == 'r');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'd');
    s = navigator.readLine();
    CHECK (navigator.position() == sizeof (array));
    CHECK (s.empty());
}

TEST_CASE("ByteNavigator_isControl_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readLine ().empty());
}

TEST_CASE("ByteNavigator_isControl_2", "[navigator]")
{
    irbis::Byte array[] { '\1', '1', 'A', ' ' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.isControl());
    navigator.readByte();
    CHECK_FALSE (navigator.isControl());
    CHECK (navigator.isDigit());
    navigator.readByte();
    CHECK_FALSE (navigator.isDigit());
    CHECK (navigator.isLetter());
    navigator.readByte();
    CHECK_FALSE (navigator.isLetter());
    CHECK (navigator.isWhitespace());
    navigator.readByte();
    CHECK_FALSE (navigator.isWhitespace());
}

TEST_CASE("ByteNavigator_readInteger_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readInteger ().empty());
}

TEST_CASE("ByteNavigator_readInteger_2", "[navigator]")
{
    irbis::Byte array[] { '1', '2', '3', '\n', '4', '5', '6' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readInteger();
    CHECK (s.size() == 3);
    CHECK (s[0] == '1');
    CHECK (s[1] == '2');
    CHECK (s[2] == '3');
    CHECK (navigator.position() == 3);
    CHECK (*navigator.ccurrent() == '\n');
    s = navigator.readInteger();
    CHECK (s.empty());
    CHECK (*navigator.ccurrent() == '\n');
}

TEST_CASE("ByteNavigator_readString_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readString (10).empty());
}

TEST_CASE("ByteNavigator_readString_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readString (5);
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
    CHECK (navigator.position() == 5);
    s = navigator.readString (15);
    CHECK (s.size() == 5);
    CHECK (s[0] == 'w');
    CHECK (s[1] == 'o');
    CHECK (s[2] == 'r');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'd');
    CHECK (navigator.position() == 10);
    CHECK (navigator.readString (15).empty());
    CHECK (navigator.position() == 10);
}

TEST_CASE("ByteNavigator_readTo_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readTo ('w').empty());
}

TEST_CASE("ByteNavigator_readTo_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readTo ('o');
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
    CHECK (navigator.position() == 5);
    s = navigator.readTo ('x');
    CHECK (s.size() == 5);
    CHECK (s[0] == 'w');
    CHECK (s[1] == 'o');
    CHECK (s[2] == 'r');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'd');
    CHECK (navigator.position() == 10);
    CHECK (navigator.readTo ('x').empty());
    CHECK (navigator.position() == 10);
}

TEST_CASE("ByteNavigator_readUntil_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readUntil ('w').empty());
}

TEST_CASE("ByteNavigator_readUntil_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readUntil ('w');
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
    CHECK (navigator.position() == 5);
    s = navigator.readUntil ('x');
    CHECK (s.size() == 5);
    CHECK (s[0] == 'w');
    CHECK (s[1] == 'o');
    CHECK (s[2] == 'r');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'd');
    CHECK (navigator.position() == 10);
    CHECK (navigator.readUntil ('x').empty());
    CHECK (navigator.position() == 10);
}

TEST_CASE("ByteNavigator_readWhile_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readWhile ('w').empty());
}

TEST_CASE("ByteNavigator_readWhile_2", "[navigator]")
{
    irbis::Byte array[] { '1', '1', '1', '2', '2', '2' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readWhile ('1');
    CHECK (s.size() == 3);
    CHECK (s[0] == '1');
    CHECK (s[1] == '1');
    CHECK (s[2] == '1');
    CHECK (navigator.position() == 3);
    s = navigator.readWhile ('2');
    CHECK (s.size() == 3);
    CHECK (s[0] == '2');
    CHECK (s[1] == '2');
    CHECK (s[2] == '2');
    CHECK (navigator.position() == 6);
    CHECK (navigator.readWhile ('3').empty());
    CHECK (navigator.position() == 6);
}

TEST_CASE("ByteNavigator_readWord_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.readWord ().empty());
}

TEST_CASE("ByteNavigator_readWord_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.readWord();
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
    CHECK (navigator.position() == 5);
    CHECK (navigator.readWord().empty());
    navigator.readByte();
    s = navigator.readWord();
    CHECK (s.size() == 5);
    CHECK (s[0] == 'w');
    CHECK (s[1] == 'o');
    CHECK (s[2] == 'r');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'd');
    CHECK (navigator.position() == 11);
    CHECK (navigator.readWord().empty());
    CHECK (navigator.position() == 11);
}

TEST_CASE("ByteNavigator_recent_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    CHECK (navigator.recent (3).empty());
}

TEST_CASE("ByteNavigator_recent_2", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.recent (3).empty());
    navigator.readByte();
    auto s = navigator.recent(3);
    CHECK (s.size() == 1);
    CHECK (s[0] == 'H');
    navigator.readByte();
    s = navigator.recent(3);
    CHECK (s.size() == 2);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    navigator.readByte();
    s = navigator.recent(3);
    CHECK (s.size() == 3);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
}

TEST_CASE("ByteNavigator_skipWhitespace_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    navigator.skipWhitespace();
    CHECK (navigator.position() == 0);
}

TEST_CASE("ByteNavigator_skipWhitespace_2", "[navigator]")
{
    irbis::Byte array[] { '1', '2', ' ', '\t', '\n', '3', '4' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    navigator.skipWhitespace();
    CHECK (navigator.position() == 0);
    navigator.move (2);
    navigator.skipWhitespace();
    CHECK (navigator.position() == 5);
    CHECK (*navigator.ccurrent() == '3');
    navigator.skipWhitespace();
    CHECK (navigator.position() == 5);
}

TEST_CASE("ByteNavigator_skipPunctuation_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    navigator.skipPunctuation();
    CHECK (navigator.position() == 0);
}

TEST_CASE("ByteNavigator_skipPunctuation_2", "[navigator]")
{
    irbis::Byte array[] { '1', '2', ',', ';', '!', '3', '4' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    navigator.skipPunctuation();
    CHECK (navigator.position() == 0);
    navigator.move (2);
    navigator.skipPunctuation();
    CHECK (navigator.position() == 5);
    CHECK (*navigator.ccurrent() == '3');
    navigator.skipPunctuation();
    CHECK (navigator.position() == 5);
}

TEST_CASE("ByteNavigator_skipLine_1", "[navigator]")
{
    irbis::ByteNavigator navigator;
    navigator.skipLine();
    CHECK (navigator.position() == 0);
}

TEST_CASE("ByteNavigator_skipLine_2", "[navigator]")
{
    irbis::Byte array[] { '1', '2', ' ', ' ', '\n', '3', '4' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    CHECK (navigator.position() == 0);
    navigator.skipLine();
    CHECK (navigator.position() == 5);
    CHECK (*navigator.ccurrent() == '3');
    navigator.skipLine();
    CHECK (navigator.eot());
}

TEST_CASE("ByteNavigator_slice_1", "[navigator]")
{
    irbis::Byte array[] { 'H', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd' };
    irbis::ByteNavigator navigator (irbis::ByteSpan (array, sizeof (array)));
    auto s = navigator.slice (0, 5);
    CHECK (s.size() == 5);
    CHECK (s[0] == 'H');
    CHECK (s[1] == 'e');
    CHECK (s[2] == 'l');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'o');
    s = navigator.slice (5, 5);
    CHECK (s.size() == 5);
    CHECK (s[0] == 'w');
    CHECK (s[1] == 'o');
    CHECK (s[2] == 'r');
    CHECK (s[3] == 'l');
    CHECK (s[4] == 'd');
}