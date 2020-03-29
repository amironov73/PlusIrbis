// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"

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
    for (size_t i = 0; i < sizeof (bytes); ++i) {
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

}
