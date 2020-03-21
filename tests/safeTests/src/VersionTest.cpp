// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Version_constructor_1", "[version]")
{
    irbis::Version version;
    CHECK (version.organization.empty());
    CHECK (version.version.empty());
    CHECK (version.maxClients == 0);
    CHECK (version.connectedClients == 0);
}

TEST_CASE("Version_toString_1", "[version]")
{
    irbis::Version version;
    version.organization = L"\u0418\u0440\u043A\u0443\u0442\u0441\u043A\u0438\u0439 \u0433\u043E\u0441\u0443\u0434\u0430\u0440\u0441\u0442\u0432\u0435\u043D\u043D\u044B\u0439 \u0442\u0435\u0445\u043D\u0438\u0447\u0435\u0441\u043A\u0438\u0439 \u0443\u043D\u0438\u0432\u0435\u0440\u0441\u0438\u0442\u0435\u0442";
    version.version = L"64.2015.1";
    version.maxClients = 100000;
    version.connectedClients = 10;
    const auto text = version.toString();
    CHECK (text == L"organization=\u0418\u0440\u043A\u0443\u0442\u0441\u043A\u0438\u0439 \u0433\u043E\u0441\u0443\u0434\u0430\u0440\u0441\u0442\u0432\u0435\u043D\u043D\u044B\u0439 \u0442\u0435\u0445\u043D\u0438\u0447\u0435\u0441\u043A\u0438\u0439 \u0443\u043D\u0438\u0432\u0435\u0440\u0441\u0438\u0442\u0435\u0442, version=64.2015.1, maxClients=100000, connectedClients=10");
}