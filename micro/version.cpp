// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"
#include "micro.h"

#include <iostream>
#include <iomanip>

namespace irbis
{
    /// \brief Разбор ответа сервера.
    /// \param response Ответ сервера.
    void Version::parse
        (
            ServerResponse &response
        )
    {
        const auto lines = response.readRemainingAnsiLines();
        if (lines.size() == 3) {
            this->version          = lines[0];
            this->connectedClients = fastParse32 (lines[1]);
            this->maxClients       = fastParse32 (lines[2]);
        } else {
            this->organization     = lines[0];
            this->version          = lines[1];
            this->connectedClients = fastParse32 (lines[2]);
            this->maxClients       = fastParse32 (lines[3]);
        }
    }

    /// \brief Строковое представление.
    /// \return Строковое представление.
    std::string Version::toString() const
    {
        return std::string ("organization=") + this->organization +
               ", version=" + this->version +
               ", maxClients=" + std::to_string (this->maxClients) +
               ", connectedClients=" + std::to_string (this->connectedClients);
    }
}
