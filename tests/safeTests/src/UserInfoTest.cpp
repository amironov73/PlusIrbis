// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("UserInfo_constructor_1", "[user]")
{
    irbis::UserInfo user;
    CHECK (user.name         .empty());
    CHECK (user.password     .empty());
    CHECK (user.cataloger    .empty());
    CHECK (user.reader       .empty());
    CHECK (user.circulation  .empty());
    CHECK (user.acquisitions .empty());
    CHECK (user.provision    .empty());
    CHECK (user.administrator.empty());
}

TEST_CASE("UserInfo_parse_1", "[user]")
{
    irbis::StringList lines;
    lines.push_back (L"1");
    lines.push_back (L"9");
    lines.push_back (L"1");
    lines.push_back (L"TylerDurden");
    lines.push_back (L"FightClub");
    lines.push_back (L"Tyler.ini"); // cataloger
    lines.push_back (L""); // reader
    lines.push_back (L""); // circulation
    lines.push_back (L""); // acquisitions
    lines.push_back (L""); // provision
    lines.push_back (L""); // administrator
    std::vector<irbis::UserInfo> users = irbis::UserInfo::parse (lines);
    CHECK (users.size() == 1);
    const irbis::UserInfo &user = users.front();
    CHECK (user.name == L"TylerDurden");
    CHECK (user.password == L"FightClub");
    CHECK (user.cataloger == L"Tyler.ini");
    CHECK (user.reader.empty());
    CHECK (user.circulation.empty());
    CHECK (user.acquisitions.empty());
    CHECK (user.provision.empty());
    CHECK (user.administrator.empty());
}

TEST_CASE("UserInfo_parse_2", "[user]")
{
    irbis::StringList lines;
    lines.push_back (L"1");
    lines.push_back (L"9");
    lines.push_back (L"1");
    lines.push_back (L"TylerDurden");
    lines.push_back (L"FightClub");
    lines.push_back (L"Tyler.ini"); // cataloger
    std::vector<irbis::UserInfo> users = irbis::UserInfo::parse (lines);
    CHECK (users.empty());
}

TEST_CASE("UserInfo_parse_3", "[user]")
{
    irbis::StringList lines;
    std::vector<irbis::UserInfo> users = irbis::UserInfo::parse (lines);
    CHECK (users.empty());
}

TEST_CASE("UserInfo_parse_4", "[user]")
{
    irbis::StringList lines;
    lines.push_back (L"0");
    std::vector<irbis::UserInfo> users = irbis::UserInfo::parse (lines);
    CHECK (users.empty());
}

TEST_CASE("UserInfo_toString_1", "[user]")
{
    irbis::UserInfo user;
    const auto text = user.toString();
    CHECK (text == L"\r\n\r\nC=;R=;B=;M=;K=;A=;");
}

TEST_CASE("UserInfo_toString_2", "[user]")
{
    irbis::UserInfo user;
    user.name = L"TylerDurden";
    user.password = L"FightClub";
    user.cataloger = L"Tyler.ini";
    const auto text = user.toString();
    CHECK (text == L"TylerDurden\r\nFightClub\r\nC=Tyler.ini;R=;B=;M=;K=;A=;");
}

TEST_CASE("UserInfo_toString_3", "[user]")
{
    irbis::UserInfo user;
    user.name = L"TylerDurden";
    user.password = L"FightClub";
    user.cataloger = L"irbisc.ini";
    const auto text = user.toString();
    CHECK (text == L"TylerDurden\r\nFightClub\r\nR=;B=;M=;K=;A=;");
}
