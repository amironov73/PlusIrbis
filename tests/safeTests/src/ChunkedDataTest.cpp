// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "readability-container-size-empty"

TEST_CASE("ChunkedData_constructor_1", "[chunked]")
{
    irbis::ChunkedBytes data;

    CHECK (data.empty());
    CHECK (data.size() == 0);
}

TEST_CASE("ChunkedData_append_1", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array [5] { 1, 2, 3, 4, 5 };
    data.append (array, irbis::size (array));
    CHECK_FALSE (data.empty());
    CHECK (data.size() == 5);
    CHECK (data.front() == 1);
    CHECK (data.back() == 5);
}

TEST_CASE("ChunkedData_append_2", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array1 [5] { 1, 2, 3, 4, 5 };
    irbis::Byte array2 [5] { 6, 7, 8, 9, 10 };
    data.append (array1, irbis::size(array1));
    data.append (array2, irbis::size(array2));
    CHECK_FALSE (data.empty());
    CHECK (data.size() == 10);
    CHECK (data.front() == 1);
    CHECK (data.back() == 10);
}

TEST_CASE("ChunkedData_iterator_1", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array [5] { 1, 2, 3, 4, 5 };
    data.append (array, irbis::size (array));
    const auto begin = std::begin (data);
    auto iter = begin;
    const auto end = std::end (data);
    CHECK (iter != end);
    CHECK (*iter == 1);
    ++iter;
    CHECK (*iter == 2);
    ++iter;
    CHECK (*iter == 3);
    ++iter;
    CHECK (*iter == 4);
    ++iter;
    CHECK (*iter == 5);
    ++iter;
    CHECK (iter == end);

    --iter;
    CHECK (*iter == 5);
    --iter;
    CHECK (*iter == 4);
    --iter;
    CHECK (*iter == 3);
    --iter;
    CHECK (*iter == 2);
    --iter;
    CHECK (*iter == 1);
    CHECK (iter == begin);
}

TEST_CASE("ChunkedData_iterator_2", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array[5] { 1, 2, 3, 4, 5 };
    data.append (array, irbis::size (array));
    std::reverse (std::begin (data), std::end (data));
    CHECK (array[0] == 5);
    CHECK (array[1] == 4);
    CHECK (array[2] == 3);
    CHECK (array[3] == 2);
    CHECK (array[4] == 1);
}

TEST_CASE("ChunkedData_iterator_3", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array1 [5] { 1, 2, 3, 4, 5 };
    irbis::Byte array2 [5] { 6, 7, 8, 9, 10 };
    data.append (array1, irbis::size (array1));
    data.append (array2, irbis::size (array2));
    std::reverse (std::begin (data), std::end (data));
    CHECK (array1[0] == 10);
    CHECK (array1[1] == 9);
    CHECK (array1[2] == 8);
    CHECK (array1[3] == 7);
    CHECK (array1[4] == 6);
    CHECK (array2[0] == 5);
    CHECK (array2[1] == 4);
    CHECK (array2[2] == 3);
    CHECK (array2[3] == 2);
    CHECK (array2[4] == 1);
}

TEST_CASE("ChunkedData_iterator_4", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array1[5] { 1, 2, 3, 4, 5 };
    irbis::Byte array2[5] { 6, 7, 8, 9, 10 };
    data.append (array1, irbis::size (array1));
    data.append (array2, irbis::size (array2));
    auto found = std::find (std::begin (data), std::end (data), 7);
    CHECK (*found == 7);
    found = std::find (std::begin (data), std::end (data), 77);
    CHECK (found == std::end (data));
}

TEST_CASE("ChunkedData_toVector_1", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array1[5] { 1, 2, 3, 4, 5 };
    irbis::Byte array2[5] { 6, 7, 8, 9, 10 };
    data.append (array1, irbis::size (array1));
    data.append (array2, irbis::size (array2));
    const auto vector = data.toVector();
    CHECK (vector.size() == data.size());
    CHECK (vector[0] == 1);
    CHECK (vector[1] == 2);
    CHECK (vector[2] == 3);
    CHECK (vector[3] == 4);
    CHECK (vector[4] == 5);
    CHECK (vector[5] == 6);
    CHECK (vector[6] == 7);
    CHECK (vector[7] == 8);
    CHECK (vector[8] == 9);
    CHECK (vector[9] == 10);
}

TEST_CASE("ChunkedData_indexer_1", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array1[5] { 1, 2, 3, 4, 5 };
    irbis::Byte array2[5] { 6, 7, 8, 9, 10 };
    data.append (array1, irbis::size (array1));
    data.append (array2, irbis::size (array2));
    CHECK (data[0] == 1);
    CHECK (data[1] == 2);
    CHECK (data[2] == 3);
    CHECK (data[3] == 4);
    CHECK (data[4] == 5);
    CHECK (data[5] == 6);
    CHECK (data[6] == 7);
    CHECK (data[7] == 8);
    CHECK (data[8] == 9);
    CHECK (data[9] == 10);
}

TEST_CASE("ChunkedData_slice_1", "[chunked]")
{
    irbis::ChunkedBytes data;
    irbis::Byte array1[5] { 1, 2, 3, 4, 5 };
    irbis::Byte array2[5] { 6, 7, 8, 9, 10 };
    data.append (array1, irbis::size (array1));
    data.append (array2, irbis::size (array2));

    auto slice = data.slice (0, 0);
    CHECK (slice.empty());
    CHECK (slice.size() == 0);

    slice = data.slice (3, 3);
    CHECK_FALSE(slice.empty());
    CHECK (slice.size() == 3);
    CHECK (slice[0] == 4);
    CHECK (slice[1] == 5);
    CHECK (slice[2] == 6);
}

TEST_CASE("ChunkedData_string_1", "[chunked]")
{
    irbis::ChunkedString data;
    const auto text = data.toString();
    CHECK (text.empty());
}

TEST_CASE("ChunkedData_string_2", "[chunked]")
{
    irbis::ChunkedString data;
    std::string text1 ("Hello,");
    std::string text2 (" world");
    data.append (text1);
    data.append (text2);
    auto text3 = data.toString();
    CHECK_FALSE (text3.empty());
    CHECK (text3 == "Hello, world");
}

