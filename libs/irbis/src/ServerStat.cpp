// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

/*!
    \class irbis::ServerStat

    \details Содержит информацию о клиентах,
        подключенных к серверу в данный момент.

    \note Неперемещаемый класс.
 */

namespace irbis {

/// \brief Разбор ответа сервера.
/// \param response Ответ сервера.
void ServerStat::parse(ServerResponse &response)
{
    this->totalCommandCount = response.readInteger();
    this->clientCount = response.readInteger();
    const auto linesPerClient = response.readInteger();
    if (linesPerClient == 0) {
        return;
    }

    for (auto i = 0; i < this->clientCount; i++) {
        this->runningClients.emplace_back();
        auto &client = this->runningClients.back();
        client.number = response.readAnsi();
        client.ipAddress = response.readAnsi();
        client.port = response.readAnsi();
        client.name = response.readAnsi();
        client.id = response.readAnsi();
        client.workstation = response.readAnsi();
        client.registered = response.readAnsi();
        client.acknowledged = response.readAnsi();
        client.lastCommand = response.readAnsi();
        client.commandNumber = response.readAnsi();
    }
}

}
