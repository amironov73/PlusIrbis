// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("Frugal_constructor_1", "[frugal]")
{
    irbis::Frugal<int> data;
    CHECK (data.empty());
    CHECK (data.size() == 0);
}

TEST_CASE("Frugal_push_back_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    CHECK_FALSE (data.empty());
    CHECK (data.size() == 3);
    CHECK (data.capacity() >= data.size());
    CHECK (data[0] == 1);
    CHECK (data[1] == 2);
    CHECK (data[2] == 3);
}

TEST_CASE("Frugal_push_back_2", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    data.push_back (4);
    data.push_back (5);
    data.push_back (6);
    data.push_back (7);
    data.push_back (8);
    data.push_back (9);
    CHECK_FALSE (data.empty());
    CHECK (data.size() == 9);
    CHECK (data.capacity() >= data.size());
}

TEST_CASE("Frugal_emplace_back_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.emplace_back (1);
    data.emplace_back (2);
    data.emplace_back (3);
    CHECK_FALSE (data.empty());
    CHECK (data.size() == 3);
}

TEST_CASE("Frugal_clear_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    data.clear();
    CHECK (data.empty());
}

TEST_CASE("Frugal_indexer_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    CHECK (data[0] == 1);
    CHECK (data[1] == 2);
    CHECK (data[2] == 3);
}

TEST_CASE("Frugal_indexer_2", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    data[0] = 100;
    data[1] = 200;
    data[2] = 300;
    CHECK (data[0] == 100);
    CHECK (data[1] == 200);
    CHECK (data[2] == 300);
}

TEST_CASE("Frugal_iterator_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    auto iter = std::begin (data);
    auto end = std::end (data);
    CHECK (*iter == 1);
    ++iter;
    CHECK (*iter == 2);
    ++iter;
    CHECK (*iter == 3);
    ++iter;
    CHECK (iter == end);
}

TEST_CASE("Frugal_front_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    CHECK (data.front() == 1);
    CHECK (data.back() == 3);
}

TEST_CASE("Frugal_toList_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    const auto list = data.toList();
    CHECK_FALSE (list.empty());
    CHECK (list.size() == 3);
}

TEST_CASE("Frugal_toVector_1", "[frugal]")
{
    irbis::Frugal<int> data;
    data.push_back (1);
    data.push_back (2);
    data.push_back (3);
    const auto vec = data.toVector();
    CHECK_FALSE (vec.empty());
    CHECK (vec.size() == 3);
}

