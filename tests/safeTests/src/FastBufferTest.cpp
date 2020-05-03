// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis_internal.h"
#include <vector>
#include <iterator>
#include <sstream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "readability-container-size-empty"

TEST_CASE("FastBuffer_constructor_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    CHECK (buffer.empty());
    CHECK (buffer.size() == 0);
    CHECK (buffer.capacity() == 16);
}

TEST_CASE("FastBuffer_constructor_2", "[fast]")
{
    irbis::FastBuffer<> buffer;
    CHECK (buffer.empty());
    CHECK (buffer.size() == 0);
    CHECK (buffer.capacity() == 256);
}

TEST_CASE("FastBuffer_push_back_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    CHECK_FALSE (buffer.empty());
    CHECK (buffer.size() == 5);
    CHECK (buffer.capacity() == 16);
}

TEST_CASE("FastBuffer_push_back_2", "[fast]")
{
    irbis::FastBuffer<8> buffer;
    buffer.push_back ('a'); // 1
    buffer.push_back ('b'); // 2
    buffer.push_back ('c'); // 3
    buffer.push_back ('d'); // 4
    buffer.push_back ('e'); // 5
    buffer.push_back ('f'); // 6
    buffer.push_back ('g'); // 7
    buffer.push_back ('h'); // 8
    buffer.push_back ('i'); // 9
    buffer.push_back ('j'); // 10
    CHECK_FALSE (buffer.empty());
    CHECK (buffer.size() == 10);
    CHECK (buffer.capacity() == 16);
    CHECK (buffer[0] == 'a');
    CHECK (buffer[7] == 'h');
    CHECK (buffer[8] == 'i');
    CHECK (buffer[9] == 'j');
}

TEST_CASE("FastBuffer_push_back_3", "[fast]")
{
    irbis::FastBuffer<8> buffer;
    buffer.push_back ('a'); // 1
    buffer.push_back ('b'); // 2
    buffer.push_back ('c'); // 3
    buffer.push_back ('d'); // 4
    buffer.push_back ('e'); // 5
    buffer.push_back ('f'); // 6
    buffer.push_back ('g'); // 7
    buffer.push_back ('h'); // 8
    buffer.push_back ('i'); // 9
    buffer.push_back ('j'); // 10
    buffer.push_back ('k'); // 11
    buffer.push_back ('l'); // 12
    buffer.push_back ('m'); // 13
    buffer.push_back ('n'); // 14
    buffer.push_back ('o'); // 15
    buffer.push_back ('p'); // 16
    buffer.push_back ('q'); // 17
    buffer.push_back ('r'); // 18
    buffer.push_back ('s'); // 19
    buffer.push_back ('t'); // 20
    CHECK_FALSE (buffer.empty());
    CHECK (buffer.size() == 20);
    CHECK (buffer.capacity() == 32);
    CHECK (buffer[0]  == 'a');
    CHECK (buffer[7]  == 'h');
    CHECK (buffer[8]  == 'i');
    CHECK (buffer[9]  == 'j');
    CHECK (buffer[15] == 'p');
    CHECK (buffer[16] == 'q');
    CHECK (buffer[19] == 't');
}

TEST_CASE("FastBuffer_clear_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    buffer.clear();
    CHECK (buffer.empty());
    CHECK (buffer.size() == 0);
    CHECK (buffer.capacity() == 16);
}

TEST_CASE("FastBuffer_clear_2", "[fast]")
{
    irbis::FastBuffer<8> buffer;
    buffer.push_back ('a'); // 1
    buffer.push_back ('b'); // 2
    buffer.push_back ('c'); // 3
    buffer.push_back ('d'); // 4
    buffer.push_back ('e'); // 5
    buffer.push_back ('f'); // 6
    buffer.push_back ('g'); // 7
    buffer.push_back ('h'); // 8
    buffer.push_back ('i'); // 9
    buffer.push_back ('j'); // 10
    buffer.clear();
    CHECK (buffer.empty());
    CHECK (buffer.size() == 00);
    CHECK (buffer.capacity() == 16);
}

TEST_CASE("FastBuffer_reserve_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.reserve (10);
    CHECK (buffer.capacity() == 16);
    buffer.reserve (100);
    CHECK (buffer.capacity() == 100);
}

TEST_CASE("FastBuffer_resize_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.resize(10);
    CHECK (buffer.size() == 10);
    CHECK (buffer.capacity() == 16);
    buffer.resize (100);
    CHECK (buffer.size() == 100);
    CHECK (buffer.capacity() == 100);
}

TEST_CASE("FastBuffer_indexer_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    CHECK (buffer[0] == 'H');
    CHECK (buffer[1] == 'e');
    CHECK (buffer[2] == 'l');
    CHECK (buffer[3] == 'l');
    CHECK (buffer[4] == 'o');
}

TEST_CASE("FastBuffer_indexer_2", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    buffer[0] = 'h';
    buffer[1] = 'E';
    buffer[2] = 'L';
    buffer[3] = 'L';
    buffer[4] = 'O';
    CHECK (buffer[0] == 'h');
    CHECK (buffer[1] == 'E');
    CHECK (buffer[2] == 'L');
    CHECK (buffer[3] == 'L');
    CHECK (buffer[4] == 'O');
}

TEST_CASE("FastBuffer_front_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    CHECK (buffer.front() == 'H');
    CHECK (buffer.back()  == 'o');
}

TEST_CASE("FastBuffer_front_2", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    buffer.front() = 'h';
    buffer.back()  = 'O';
    CHECK (buffer[0] == 'h');
    CHECK (buffer[4] == 'O');
    CHECK (buffer.front() == 'h');
    CHECK (buffer.back()  == 'O');
}

TEST_CASE("FastBuffer_begin_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    const auto begin = std::begin (buffer);
    const auto end   = std::end   (buffer);
    auto current = begin;
    CHECK (*current == 'H');
    CHECK (current != end);
    ++current;
    CHECK (*current == 'e');
    CHECK (current != end);
    ++current;
    CHECK (*current == 'l');
    CHECK (current != end);
    ++current;
    CHECK (*current == 'l');
    CHECK (current != end);
    ++current;
    CHECK (*current == 'o');
    CHECK (current != end);
    ++current;
    CHECK (current == end);

    --current;
    CHECK (*current == 'o');
    CHECK (current != begin);
    --current;
    CHECK (*current == 'l');
    CHECK (current != begin);
    --current;
    CHECK (*current == 'l');
    CHECK (current != begin);
    --current;
    CHECK (*current == 'e');
    CHECK (current != begin);
    --current;
    CHECK (*current == 'H');
    CHECK (current == begin);
}

TEST_CASE("FastBuffer_algorithm_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    std::reverse (std::begin (buffer), std::end (buffer));
    CHECK (buffer[0] == 'o');
    CHECK (buffer[1] == 'l');
    CHECK (buffer[2] == 'l');
    CHECK (buffer[3] == 'e');
    CHECK (buffer[4] == 'H');
}

TEST_CASE("FastBuffer_for_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    CHECK (buffer.front() == 'H');
    CHECK (buffer.back()  == 'o');

    std::stringstream stream;
    for (const auto item : buffer) {
        stream << item << ", ";
    }
    CHECK (stream.str() == "H, e, l, l, o, ");
}

TEST_CASE("FastBuffer_iterator_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    const auto begin = std::begin (buffer);
    const auto end   = std::end   (buffer);
    auto iter = begin + 2;
    CHECK (*iter == 'l');
    iter += 3;
    CHECK (iter == end);
    iter -= 4;
    CHECK (*iter == 'e');
    iter = end + (-5);
    CHECK (iter == begin);
}


TEST_CASE("FastBuffer_toString_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    const auto text = buffer.toString();
    CHECK (text == "Hello");
}

TEST_CASE("FastBuffer_toVector_1", "[fast]")
{
    irbis::FastBuffer<16> buffer;
    buffer.push_back ('H');
    buffer.push_back ('e');
    buffer.push_back ('l');
    buffer.push_back ('l');
    buffer.push_back ('o');
    const auto vector = buffer.toVector();
    CHECK (vector[0] == 'H');
    CHECK (vector[1] == 'e');
    CHECK (vector[2] == 'l');
    CHECK (vector[3] == 'l');
    CHECK (vector[4] == 'o');
}

TEST_CASE("FastBuffer_write_1", "[fast]")
{
    irbis::FastBuffer<8> buffer;
    irbis::Byte data[16] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    buffer.write (data, irbis::size (data));
    CHECK (buffer.size () == 16);
    CHECK (buffer[0] == 1);
    CHECK (buffer[1] == 2);
    CHECK (buffer[2] == 3);
    CHECK (buffer[3] == 4);
    CHECK (buffer[4] == 5);
    CHECK (buffer[5] == 6);
    CHECK (buffer[6] == 7);
    CHECK (buffer[7] == 8);
    CHECK (buffer[8] == 9);
    CHECK (buffer[9] == 10);
    CHECK (buffer[10] == 11);
    CHECK (buffer[11] == 12);
    CHECK (buffer[12] == 13);
    CHECK (buffer[13] == 14);
    CHECK (buffer[14] == 15);
    CHECK (buffer[15] == 16);
}

