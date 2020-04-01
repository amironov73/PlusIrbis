// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"

TEST_CASE("ProcessInfo_constructor_1", "[process]")
{
    irbis::ProcessInfo info;
    CHECK (info.number       .empty());
    CHECK (info.ipAddress    .empty());
    CHECK (info.name         .empty());
    CHECK (info.clientId     .empty());
    CHECK (info.workstation  .empty());
    CHECK (info.started      .empty());
    CHECK (info.lastCommand  .empty());
    CHECK (info.commandNumber.empty());
    CHECK (info.processId    .empty());
    CHECK (info.state        .empty());
}

TEST_CASE("ProcessInfo_parse_1", "[process]")
{
    auto response = std::unique_ptr<irbis::ServerResponse>(irbis::ServerResponse::emptyResonse());
    const auto processes = irbis::ProcessInfo::parse (*response);
    CHECK (processes.empty());
}
