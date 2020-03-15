// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

TEST_CASE("IniLine_constructor_1", "[ini]")
{
    irbis::IniLine line;
    CHECK       (line.key.empty());
    CHECK       (line.value.empty());
    CHECK_FALSE (line.modified());
}

TEST_CASE("IniLine_constructor_2", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    CHECK       (line.key   == L"key");
    CHECK       (line.value == L"value");
    CHECK_FALSE (line.modified());
}

TEST_CASE("IniLine_constructor_3", "[ini]")
{
    irbis::IniLine line (std::move (irbis::String (L"key")),
            std::move (irbis::String (L"value")));
    CHECK       (line.key   == L"key");
    CHECK       (line.value == L"value");
    CHECK_FALSE (line.modified());
}

TEST_CASE("IniLine_setKey_1", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    line.setKey (irbis::String (L"newKey"));
    CHECK (line.key == L"newKey");
    CHECK (line.modified());
}

TEST_CASE("IniLine_setKey_2", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    line.setKey (std::move (irbis::String (L"newKey")));
    CHECK (line.key == L"newKey");
    CHECK (line.modified());
}

TEST_CASE("IniLine_setValue_1", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    line.setValue (irbis::String (L"newValue"));
    CHECK (line.value == L"newValue");
    CHECK (line.modified());
}

TEST_CASE("IniLine_setValue_2", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    line.setValue (std::move (irbis::String (L"newValue")));
    CHECK (line.value == L"newValue");
    CHECK (line.modified());
}

TEST_CASE("IniLine_notModified_1", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    line.setKey (L"newKey");
    CHECK (line.modified());
    line.notModified();
    CHECK_FALSE (line.modified());
}

TEST_CASE("IniLine_toString_1", "[ini]")
{
    irbis::IniLine line {L"key", L"value"};
    CHECK (line.toString() == L"key=value");
}
