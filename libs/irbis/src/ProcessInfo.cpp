// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

std::vector<ProcessInfo> ProcessInfo::parse (ServerResponse &response)
{
    std::vector<ProcessInfo> result;
    if (response.eot()) {
        return result;
    }

    const auto processCount = response.readInteger();
    const auto linesPerProcess = response.readInteger();
    if (processCount == 0 || linesPerProcess == 0) {
        return result;
    }

    for (auto i = 0; i < processCount; i++) {
        ProcessInfo process;
        process.number = response.readAnsi();
        process.ipAddress = response.readAnsi();
        process.name = response.readAnsi();
        process.clientId = response.readAnsi();
        process.workstation = response.readAnsi();
        process.started = response.readAnsi();
        process.lastCommand = response.readAnsi();
        process.commandNumber = response.readAnsi();
        process.processId = response.readAnsi();
        process.state = response.readAnsi();

        result.push_back(process);
    }

    return result;

}

}
