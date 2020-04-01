// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

TEST_CASE("TreeNode_constructor_1", "[tree]")
{
    irbis::TreeNode node;
    CHECK (node.level == 0);
    CHECK (node.value.empty());
    CHECK (node.children.empty());
}

TEST_CASE("TreeNode_constructor_2", "[tree]")
{
    irbis::TreeNode node (L"child");
    CHECK (node.level == 0);
    CHECK (node.value == L"child");
    CHECK (node.children.empty());
}

TEST_CASE("TreeNode_constructor_3", "[tree]")
{
    irbis::String name = L"name";
    irbis::TreeNode node (std::move (name));
    CHECK (node.level == 0);
    CHECK (node.value == L"name");
    CHECK (node.children.empty());
}

TEST_CASE("TreeNode_add_1", "[tree]")
{
    irbis::String name = L"child";
    irbis::TreeNode node;
    const auto &result = node.add (name);
    CHECK (node.children.size() == 1);
    CHECK (node.children[0].value == L"child");
}

TEST_CASE("TreeNode_add_2", "[tree]")
{
    irbis::String name = L"child";
    irbis::TreeNode node;
    const auto &result = node.add (std::move (name));
    CHECK (node.children.size() == 1);
    CHECK (node.children[0].value == L"child");
}

TEST_CASE("TreeFile_constructor_1", "[tree]")
{
    irbis::TreeFile tree;
    CHECK (tree.roots.empty());
}

TEST_CASE("TreeFile_parse_1", "[tree]")
{
    auto path = irbis::IO::combinePath (whereTestData(), L"test1.tre");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    const auto lines = irbis::Text::readAnsiLines (path);
    irbis::TreeFile tree;
    tree.parse (lines);
    CHECK (tree.roots.size() == 4);
    CHECK (tree.roots[0].value == L"1 - First");
    CHECK (tree.roots[0].children.empty());
    CHECK (tree.roots[1].value == L"2 - Second");
    CHECK (tree.roots[1].children.size() == 3);
    CHECK (tree.roots[1].children[0].value == L"2.1 - Second first");
    CHECK (tree.roots[1].children[0].children.empty());
}