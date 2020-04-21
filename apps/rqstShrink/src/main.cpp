// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include "irbis.h"
#include "irbis_internal.h"

int main (int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "rqstShrink -- IRBIS64 RQST database trimmer" << std::endl;
        std::cout << "USAGE: rqstShrink <connectionString>" << std::endl << std::endl;
        return 0;
    }

    std::cout << "Client version: " << irbis::libraryVersionString() << std::endl;

    try {

        irbis::String connectionString = irbis::string2wide(argv[1]);
        irbis::Connection connection;
        connection.parseConnectionString(connectionString);
        if (!connection.connect()) {
            std::cerr << "Not connected!" << std::endl;
            std::cerr << "Error code: " << connection.lastError << std::endl;
            return -1;
        }

        if (!irbis::sameString (connection.workstation, irbis::String(L"A"))) {
            std::cerr << "Not administrator! Exiting" << std::endl;
            return -1;
        }

        const auto version = connection.getServerVersion();
        std::cout << "Server version: " << irbis::wide2string(version.version) << std::endl;

        const auto maxMfn = connection.getMaxMfn(connection.database);
        std::cout << "Max MFN: " << maxMfn << std::endl;
        irbis::String expression (L"\"I=0\" + \"I=2\""); // Невыполненные и зарезервированные
        const auto found = connection.search(expression); // TODO searchAll
        if (found.size() == maxMfn) {
            std::cout << "No truncation needed, exiting" << std::endl;
            return 0;
        }

        // TODO ReadAllRecords
        auto goodRecords = connection.readRecords(found);
        std::cout << "Good records loaded: " << goodRecords.size() << std::endl;
        for (auto &record : goodRecords) {
            record.reset();
            record.database = connection.database;
        }

        std::cout << "Truncating database... ";
        connection.truncateDatabase(connection.database);
        if (connection.getMaxMfn(connection.database) > 1) {
            std::cerr << "Error while truncating the database, exiting" << std::endl;
            return -1;
        }
        std::cout << "done" << std::endl;

        // TODO WriteAllRecords
        if (!connection.writeRecords(goodRecords, false, true, true)) {
            std::cerr << "Error while writing records" << std::endl;
            return -1;
        }
        std::cout << "Good records restored" << std::endl;

        connection.disconnect();
        std::cout << "Disconnected" << std::endl;
    }
    catch (std::exception &exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
    }

    return 0;
}
