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

//=========================================================

TEST_CASE("IniSection_constructor_1", "[ini]")
{
    irbis::IniSection section;
    CHECK (section.name.empty());
    CHECK (section.lines.empty());
}

TEST_CASE("IniSection_clear_1", "[ini]")
{
    irbis::IniSection section;
    section.lines.emplace_back ();
    CHECK_FALSE (section.lines.empty());
    section.clear();
    CHECK (section.lines.empty());
}

TEST_CASE("IniSection_toString_1", "[ini]")
{
    irbis::IniSection section;
    section.name = L"Main";
    section.setValue (L"key1", L"value1");
    section.setValue (L"key2", L"value2");
    const auto text = section.toString();
    CHECK (text == L"[Main]\nkey1=value1\nkey2=value2\n");
}

//=========================================================

TEST_CASE("IniFile_constructor_1", "[ini]")
{
    irbis::IniFile ini;
    CHECK (ini.sections.empty());
    CHECK_FALSE (ini.modified());
}

TEST_CASE("IniFile_clear_1", "[ini]")
{
    irbis::IniFile ini;
    ini.sections.emplace_back();
    CHECK_FALSE (ini.sections.empty());
    ini.clear();
    CHECK (ini.sections.empty());
}

TEST_CASE("IniFile_createSection_1", "[ini]")
{
    irbis::IniFile ini;
    const auto &s1 = ini.createSection (L"Main");
    CHECK (s1.name == L"Main");
    CHECK (ini.sections.size() == 1);
    const auto &s2 = ini.createSection (L"Main");
    CHECK (s2.name == L"Main");
    CHECK (ini.sections.size() == 1);
}

TEST_CASE("IniFile_notModified_1", "[ini]")
{
    irbis::IniFile ini;
    auto &section = ini.createSection (L"Main");
    section.setValue (L"Key", L"Value");
    CHECK (ini.modified());
    ini.notModified();
    CHECK_FALSE (ini.modified());
}

TEST_CASE("IniFile_parse_1", "[ini]")
{
    auto path = irbis::IO::combinePath (whereTestData(), L"IniFile1.ini");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    const auto lines = irbis::Text::readAnsiLines (path);
    irbis::IniFile ini;
    ini.parse (lines);
    CHECK (ini.sections.size() == 2);
    CHECK (ini.getValue (L"Main", L"FirstParameter") == L"1");
    CHECK (ini.getValue (L"Private", L"ThirdParameter") == L"3");
}

TEST_CASE("IniFile_toString_1", "[ini]")
{
    irbis::IniFile ini;
    auto &section1 = ini.createSection (L"Main");
    section1.setValue (L"key11", L"value11");
    section1.setValue (L"key12", L"value12");
    auto &section2 = ini.createSection (L"Private");
    section2.setValue (L"key21", L"value21");
    section2.setValue (L"key22", L"value22");
    const auto text = ini.toString();
    CHECK (text == L"[Main]\nkey11=value11\nkey12=value12\n\n[Private]\nkey21=value21\nkey22=value22\n");
}
