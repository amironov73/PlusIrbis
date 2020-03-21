// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("GblParameter_constructor_1", "[gbl]")
{
    irbis::GblParameter parameter {};
    CHECK (parameter.value.empty());
    CHECK (parameter.title.empty());
}

//=========================================================

TEST_CASE("GblStatement_constructor_1", "[gbl]")
{
    irbis::GblStatement statement;
    CHECK (statement.command   .empty());
    CHECK (statement.parameter1.empty());
    CHECK (statement.parameter2.empty());
    CHECK (statement.format1   .empty());
    CHECK (statement.format2   .empty());
}

//=========================================================

TEST_CASE("GblSettings_constructor_1", "[gbl]")
{
    irbis::GblSettings settings;
    CHECK_FALSE (settings.actualize);
    CHECK_FALSE (settings.autoin);
    CHECK       (settings.database.empty());
    CHECK       (settings.fileName.empty());
    CHECK       (settings.firstRecord == 0);
    CHECK_FALSE (settings.formalControl);
    CHECK       (settings.maxMfn == 0);
    CHECK       (settings.mfnList.empty());
    CHECK       (settings.minMfn == 0);
    CHECK       (settings.numberOfRecords == 0);
    CHECK       (settings.searchExpression.empty());
    CHECK       (settings.sequentialSearch.empty());
    CHECK       (settings.statements.empty());
    CHECK       (settings.parameters.empty());
}
