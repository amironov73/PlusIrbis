#include "catch.hpp"
#include "irbis.h"

TEST_CASE("removeComments_1", "[format]")
{
    CHECK(irbis::removeComments(L"") == L"");
    CHECK(irbis::removeComments(L"Hello") == L"Hello");
    CHECK(irbis::removeComments(L"Hello/*comment") == L"Hello");
}

TEST_CASE("prepareFormat_1", "[format]")
{
    CHECK(irbis::prepareFormat(L"") == L"");
    CHECK(irbis::prepareFormat(L"Hello") == L"Hello");
    CHECK(irbis::prepareFormat(L"Hello/*comment") == L"Hello");
    CHECK(irbis::prepareFormat(L"Hel\rlo") == L"Hello");
}
