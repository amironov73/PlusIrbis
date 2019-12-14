// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

void Version::parse(ServerResponse &response)
{
    const auto lines = response.readRemainingAnsiLines();
    if (lines.size() == 3) {
        this->version = lines[0];
        this->connectedClients = fastParse32(lines[1]);
        this->maxClients = fastParse32(lines[2]);
    } else {
        this->organization = lines[0];
        this->version = lines[1];
        this->connectedClients = fastParse32(lines[2]);
        this->maxClients = fastParse32(lines[3]);
    }
}

std::wstring Version::to_wstring() const
{
    return std::wstring(L"organization=") + organization +
        L", version=" + version +
        L", maxClients=" + std::to_wstring(maxClients) +
        L", connectedClients=" + std::to_wstring(connectedClients);
}

}
