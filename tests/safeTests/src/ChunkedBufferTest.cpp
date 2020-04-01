// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("ChunkedBuffer_constructor_1", "[chunked]")
{
    irbis::ChunkedBuffer buffer;
    CHECK (buffer.eof());
    CHECK (buffer.empty());
    CHECK (buffer.peek() < 0);
    CHECK (buffer.readByte() < 0);
    CHECK (buffer.position() == 0);
}

TEST_CASE("ChunkedBuffer_readLine_1", "[chunked]")
{
    irbis::ChunkedBuffer buffer;
    irbis::Byte bytes[] { 'H', 'e', 'l', 'l', 'o', '\r', '\n', '1' };
    buffer.write (bytes, 0, sizeof (bytes));
    buffer.rewind();
    irbis::Encoding *ansi = irbis::Encoding::ansi();
    irbis::String text = buffer.readLine (ansi);
    CHECK (text == L"Hello");
}

TEST_CASE("ChunkedBuffer_readRemaining_1", "[chunked]")
{
    irbis::ChunkedBuffer buffer;
    irbis::Byte bytes[] { 'H', 'e', 'l', 'l', 'o', '\r', '\n', '1' };
    buffer.write (bytes, 0, sizeof (bytes));
    buffer.rewind();
    CHECK (buffer.readByte() == 'H');
    irbis::Bytes remaining = buffer.readRemaining();
    CHECK (remaining.size() == sizeof (bytes) - 1);
    CHECK (remaining[0] == 'e');
    CHECK (remaining[1] == 'l');
    CHECK (remaining[2] == 'l');
    CHECK (remaining[3] == 'o');
    CHECK (remaining[4] == '\r');
    CHECK (remaining[5] == '\n');
    CHECK (remaining[6] == '1');
}

TEST_CASE("ChunkedBuffer_toBytes_1", "[chunked]")
{
    irbis::ChunkedBuffer buffer;
    irbis::Byte bytes[] { 'H', 'e', 'l', 'l', 'o', '\r', '\n', '1' };
    buffer.write (bytes, 0, sizeof (bytes));
    buffer.rewind();
    CHECK (buffer.readByte() == 'H');
    irbis::Bytes remaining = buffer.toBytes();
    CHECK (remaining.size() == sizeof (bytes));
    CHECK (remaining[0] == 'H');
    CHECK (remaining[1] == 'e');
    CHECK (remaining[2] == 'l');
    CHECK (remaining[3] == 'l');
    CHECK (remaining[4] == 'o');
    CHECK (remaining[5] == '\r');
    CHECK (remaining[6] == '\n');
    CHECK (remaining[7] == '1');
}

TEST_CASE("ChunkedBuffer_writeByte_1", "[chunked]")
{
    irbis::ChunkedBuffer buffer;
    irbis::Byte bytes[] { 3, 14, 15, 9, 26, 5 };
    for (auto byte : bytes) {
        buffer.writeByte (byte);
    }
    CHECK (buffer.size() == sizeof (bytes));
    CHECK_FALSE (buffer.empty());
    buffer.rewind();
    CHECK (buffer.position() == 0);
    for (std::size_t i = 0; i < sizeof (bytes); ++i) {
        auto byte = buffer.readByte();
        CHECK (byte == bytes[i]);
    }
    CHECK (buffer.position() == sizeof (bytes));
}

TEST_CASE("ChunkedBuffer_writeLine_1", "[chunked]")
{
    irbis::ChunkedBuffer buffer;
    irbis::Encoding *ansi = irbis::Encoding::ansi();
    irbis::String hello (L"Hello");
    buffer.writeLine (hello, ansi);
    irbis::Byte bytes[] { 'H', 'e', 'l', 'l', 'o' };
    buffer.rewind();
    irbis::String text = buffer.readLine (ansi);
    CHECK (text == hello);
    irbis::Bytes all = buffer.toBytes();
    for (std::size_t i = 0; i < sizeof (bytes); ++i) {
        CHECK (bytes[i] == all[i]);
    }
}
