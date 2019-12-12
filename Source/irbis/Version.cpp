// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

void Version::parse(ServerResponse &response)
{
    throw NotImplementedException();
}

std::wstring Version::to_wstring() const
{
    return std::wstring(L"organization=") + organization +
        L", version=" + version +
        L", maxClients=" + std::to_wstring(maxClients) +
        L", connectedClients=" + std::to_wstring(connectedClients);
}


}
