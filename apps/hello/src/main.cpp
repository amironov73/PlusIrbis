#include <iostream>

#include "irbis.h"

int main()
{
    setlocale(LC_ALL, ""); // for cyrillic output
    std::wcout.imbue (std::locale(""));

    std::wcout << L"Client version: " << irbis::string2wide(irbis::libraryVersionString()) << std::endl;
    std::wcout << L"Running on " << irbis::string2wide(irbis::isWindows() ? "Windows" : "UNIX") << std::endl;

    try {

        irbis::Connection connection;
        connection.host = L"127.0.0.1";
        connection.port = 6666;
        connection.username = L"librarian";
        connection.password = L"secret";
        const auto isConnected = connection.connect();
        if (!isConnected) {
            std::wcout << L"Not connected!" << std::endl;
            std::wcout << L"Error code: " << connection.lastError << std::endl;
            std::wcout << L"Error description: "
                << irbis::describeError(connection.lastError) << std::endl;
            return -1;
        }

        const auto version = connection.getServerVersion();
        std::wcout << L"Server version: " << version.version << std::endl;

        const auto maxMfn = connection.getMaxMfn(L"IBIS");
        std::wcout << L"max MFN: " << maxMfn << std::endl;

        connection.noOp();
        std::wcout << L"NOOP" << std::endl;

        //auto f = connection.noOpAsync();
        //f.get();
        //std::cout << "NOOP ASYNC" << std::endl;

        const auto formatted = connection.formatRecord(L"@brief", 1);
        std::wcout << L"FORMAT: " << formatted << std::endl;

        const auto record = connection.readRecord(1);
        std::wcout << "READ: " << record.fm(200, L'a') << std::endl;

        irbis::MfnList found = connection.search(L"K=alg$");
        std::wcout << "SEARCH:";
        for (auto mfn : found) {
            std::wcout << L" " << mfn;
        }
        std::wcout << std::endl;

        connection.disconnect();
        std::cout << "disconnected" << std::endl;
    }
    catch (std::exception &exception) {
        std::cout << "Error: " << exception.what() << std::endl;
    }

    return 0;
}
