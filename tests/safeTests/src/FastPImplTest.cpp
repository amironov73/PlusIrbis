// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

namespace {

struct TestStruct
{
    struct impl;

    TestStruct (int height, int width);

    int getArea ();

    irbis::FastPImpl <impl, 8, 4> m_pimpl;
};

struct TestStruct::impl
{
    int height;
    int width;

    impl (int height_, int width_)
        : height (height_), width (width_)
    {}

    ~impl () = default;

    int getArea () const noexcept
    { return height * width; }
};

TestStruct::TestStruct (int height, int width)
    : m_pimpl (height, width)
{}

int TestStruct::getArea ()
{
    return m_pimpl->getArea ();
}

}

TEST_CASE("FastPImpl_constructor_1", "[pimpl]")
{
    TestStruct test (10, 20);
    CHECK (test.getArea() == 200);
}

TEST_CASE("FastPImpl_copy_1", "[pimpl]")
{
    TestStruct test1 (10, 20);
    TestStruct test2 (test1);
    CHECK (test2.getArea() == 200);
}

TEST_CASE("FastPImpl_copy_2", "[pimpl]")
{
    TestStruct test1 (10, 20);
    TestStruct test2 = test1;
    CHECK (test2.getArea() == 200);
}

TEST_CASE("FastPImpl_move_1", "[pimpl]")
{
    TestStruct test1 (10, 20);
    TestStruct test2 (std::move (test1));
    CHECK (test2.getArea() == 200);
}

TEST_CASE("FastPImpl_move_2", "[pimpl]")
{
    TestStruct test1 (10, 20);
    TestStruct test2 = std::move (test1);
    CHECK (test2.getArea() == 200);
}

TEST_CASE("FastPImpl_star_1", "[pimpl]")
{
    TestStruct test (10, 20);
    const TestStruct::impl &ref = *test.m_pimpl;
    CHECK (ref.width == 20);
    CHECK (ref.height == 10);
}
TEST_CASE("FastPImpl_arrow_1", "[pimpl]")
{
    TestStruct test (10, 20);
    CHECK (test.m_pimpl->width == 20);
    CHECK (test.m_pimpl->height == 10);
}
