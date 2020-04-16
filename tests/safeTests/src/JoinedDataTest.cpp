// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include <vector>
#include <list>
#include <iterator>
#include <deque>
#include <forward_list>
#include <sstream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "readability-container-size-empty"

using DataType = int;
using ContainerType = std::vector <DataType>;
using IteratorType = ContainerType::iterator;
using JoinerType = irbis::JoinedData <DataType, IteratorType>;

TEST_CASE("JoinedData_constructor_1", "[joined]")
{
    JoinerType joiner;
    CHECK (joiner.empty());
    CHECK (joiner.size() == 0);
}

TEST_CASE("JoinedData_constructor_2", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner
        {
            std::begin (vector1), std::end (vector1),
            std::begin (vector2), std::end (vector2)
        };
    CHECK (joiner.size() == 6);
}

TEST_CASE("JoinedData_append_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner;
    joiner
        .append (vector1)
        .append (vector2);
    CHECK_FALSE (joiner.empty());
    CHECK (joiner.size() == 6);
}

TEST_CASE("JoinedData_indexer_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner;
    joiner
        .append (vector1)
        .append (vector2);

    CHECK (*joiner[0] == 1);
    CHECK (*joiner[1] == 2);
    CHECK (*joiner[2] == 3);
    CHECK (*joiner[3] == 4);
    CHECK (*joiner[4] == 5);
    CHECK (*joiner[5] == 6);
}

TEST_CASE("JoinedData_front_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner;
    joiner
        .append (vector1)
        .append (vector2);

    CHECK (joiner.front() == 1);
    CHECK (joiner.back() == 6);
}

TEST_CASE("JoinedData_algorithm_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner;
    joiner
        .append (vector1)
        .append (vector2);
    std::reverse (std::begin (joiner), std::end (joiner));

    CHECK (*joiner[0] == 6);
    CHECK (*joiner[1] == 5);
    CHECK (*joiner[2] == 4);
    CHECK (*joiner[3] == 3);
    CHECK (*joiner[4] == 2);
    CHECK (*joiner[5] == 1);
}

TEST_CASE("JoinedData_iterator_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner;
    joiner
        .append (std::begin (vector1), std::end (vector1))
        .append (std::begin (vector2), std::end (vector2));

    auto begin = joiner.begin();
    auto iter = begin;
    auto end = joiner.end();
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
    CHECK (*iter == 6);
    ++iter;
    CHECK (iter == end);

    --iter;
    CHECK (*iter == 6);
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

TEST_CASE("JoinedData_slice_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner1;
    joiner1
        .append (vector1)
        .append (vector2);

    auto joiner2 = joiner1.slice (2, 3);
    CHECK_FALSE (joiner2.empty());
    CHECK (joiner2.size() == 3);
    CHECK (*joiner2[0] == 3);
    CHECK (*joiner2[1] == 4);
    CHECK (*joiner2[2] == 5);
}

TEST_CASE("JoinedData_for_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner1;
    joiner1
        .append (vector1)
        .append (vector2);

    std::stringstream stream;
    for (const auto &item : joiner1) {
        stream << item << ", ";
    }
    CHECK (stream.str() == "1, 2, 3, 4, 5, 6, ");
}

TEST_CASE("JoinedData_toString_1", "[joined]")
{
    using DT = char;
    using CT = std::vector <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT vector1 { 'H', 'e', 'l', 'l', 'o' };
    CT vector2 { ',', ' ' };
    CT vector3 { 'w', 'o', 'r', 'l', 'd' };
    JT joiner;
    joiner
        .append (vector1)
        .append (vector2)
        .append (vector3);

    const auto text = joiner.toString();
    CHECK (text == "Hello, world");
}

TEST_CASE("JoinedData_toVector_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner;
    joiner
        .append (vector1)
        .append (vector2);

    auto vector3 = joiner.toVector();
    CHECK (vector3.size() == 6);
}

TEST_CASE("JoinedData_operator_1", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner1;
    joiner1.append (vector1);
    JoinerType joiner2;
    joiner2.append (vector2);

    auto joiner3 = joiner1 + joiner2;
    CHECK (joiner3.size() == 6);
}

TEST_CASE("JoinedData_operator_2", "[joined]")
{
    ContainerType vector1 { 1, 2, 3 };
    ContainerType vector2 { 4, 5, 6 };
    JoinerType joiner1;
    joiner1.append (vector1);
    JoinerType joiner2;
    joiner2.append (vector2);

    joiner1 += joiner2;
    CHECK (joiner1.size() == 6);
}

TEST_CASE("JoinedData_array_1", "[joined]")
{
    using DT = char;
    using CT = char[];
    using IT = char*;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 'H', 'e', 'l', 'l', 'o' };
    CT container2 { ',', ' ' };
    CT container3 { 'w', 'o', 'r', 'l', 'd' };

    JT joiner;
    joiner
        .append(container1)
        .append(container2)
        .append(container3);

    const auto text = joiner.toString();
    CHECK (text == "Hello, world");
}

TEST_CASE("JoinedData_array_2", "[joined]")
{
    using DT = int;
    using CT = int[];
    using IT = int*;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 1, 2, 3 };
    CT container2 { 4, 5, 6 };
    JT joiner;
    joiner
        .append(container1)
        .append(container2);
    std::reverse (std::begin (joiner), std::end (joiner));

    CHECK (*joiner[0] == 6);
    CHECK (*joiner[1] == 5);
    CHECK (*joiner[2] == 4);
    CHECK (*joiner[3] == 3);
    CHECK (*joiner[4] == 2);
    CHECK (*joiner[5] == 1);
}

TEST_CASE("JoinedData_list_1", "[joined]")
{
    using DT = char;
    using CT = std::list <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 'H', 'e', 'l', 'l', 'o' };
    CT container2 { ',', ' ' };
    CT container3 { 'w', 'o', 'r', 'l', 'd' };

    JT joiner;
    joiner
        .append(container1)
        .append(container2)
        .append(container3);

    const auto text = joiner.toString();
    CHECK (text == "Hello, world");
}

TEST_CASE("JoinedData_list_2", "[joined]")
{
    using DT = int;
    using CT = std::list <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 1, 2, 3 };
    CT container2 { 4, 5, 6 };
    JT joiner;
    joiner
        .append(container1)
        .append(container2);
    std::reverse (std::begin (joiner), std::end (joiner));

    CHECK (*joiner[0] == 6);
    CHECK (*joiner[1] == 5);
    CHECK (*joiner[2] == 4);
    CHECK (*joiner[3] == 3);
    CHECK (*joiner[4] == 2);
    CHECK (*joiner[5] == 1);
}

TEST_CASE("JoinedData_deque_1", "[joined]")
{
    using DT = char;
    using CT = std::deque <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 'H', 'e', 'l', 'l', 'o' };
    CT container2 { ',', ' ' };
    CT container3 { 'w', 'o', 'r', 'l', 'd' };

    JT joiner;
    joiner
        .append(container1)
        .append(container2)
        .append(container3);

    const auto text = joiner.toString();
    CHECK (text == "Hello, world");
}

TEST_CASE("JoinedData_deque_2", "[joined]")
{
    using DT = int;
    using CT = std::deque <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 1, 2, 3 };
    CT container2 { 4, 5, 6 };
    JT joiner;
    joiner
        .append(container1)
        .append(container2);
    std::reverse (std::begin (joiner), std::end (joiner));

    CHECK (*joiner[0] == 6);
    CHECK (*joiner[1] == 5);
    CHECK (*joiner[2] == 4);
    CHECK (*joiner[3] == 3);
    CHECK (*joiner[4] == 2);
    CHECK (*joiner[5] == 1);
}

TEST_CASE("JoinedData_forward_list_1", "[joined]")
{
    using DT = char;
    using CT = std::forward_list <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 'H', 'e', 'l', 'l', 'o' };
    CT container2 { ',', ' ' };
    CT container3 { 'w', 'o', 'r', 'l', 'd' };

    JT joiner;
    joiner
        .append(container1)
        .append(container2)
        .append(container3);

    const auto text = joiner.toString();
    CHECK (text == "Hello, world");
}

TEST_CASE("JoinedData_forward_list_2", "[joined]")
{
    using DT = int;
    using CT = std::forward_list <DT>;
    using IT = CT::iterator;
    using JT = irbis::JoinedData <DT, IT>;

    CT container1 { 1, 2, 3 };
    CT container2 { 4, 5, 6 };
    JT joiner;
    joiner
        .append(container1)
        .append(container2);
    std::reverse (std::begin (joiner), std::end (joiner));

    CHECK (*joiner[0] == 6);
    CHECK (*joiner[1] == 5);
    CHECK (*joiner[2] == 4);
    CHECK (*joiner[3] == 3);
    CHECK (*joiner[4] == 2);
    CHECK (*joiner[5] == 1);
}

TEST_CASE("JoinedData_span_1", "[joined]")
{
    using DT = char;
    using CT = irbis::Span <DT>;
    using IT = DT*;
    using JT = irbis::JoinedData <DT, IT>;

    DT data1[] { 'H', 'e', 'l', 'l', 'o' };
    DT data2[] { ',', ' ' };
    DT data3[] { 'w', 'o', 'r', 'l', 'd' };
    CT container1 (data1, irbis::size (data1));
    CT container2 (data2, irbis::size (data2));
    CT container3 (data3, irbis::size (data3));

    JT joiner;
    joiner
        .append (container1)
        .append (container2)
        .append (container3);

    const auto text = joiner.toString();
    CHECK (text == "Hello, world");
}

TEST_CASE("JoinedData_span_2", "[joined]")
{
    using DT = int;
    using CT = irbis::Span <DT>;
    using IT = DT*;
    using JT = irbis::JoinedData <DT, IT>;

    DT data1[] { 1, 2, 3 };
    DT data2[] { 4, 5, 6 };
    CT container1 (data1, irbis::size (data1));
    CT container2 (data2, irbis::size (data2));
    JT joiner;
    joiner
        .append(container1)
        .append(container2);
    std::reverse (std::begin (joiner), std::end (joiner));

    CHECK (*joiner[0] == 6);
    CHECK (*joiner[1] == 5);
    CHECK (*joiner[2] == 4);
    CHECK (*joiner[3] == 3);
    CHECK (*joiner[4] == 2);
    CHECK (*joiner[5] == 1);
}
