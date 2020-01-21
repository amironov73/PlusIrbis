#include <iostream>

#include "irbis.h"

static std::string _C(std::wstring s)
{
    return irbis::unicode_to_cp866(s);
}

int main()
{
    //setlocale(LC_ALL, ""); // for cyrillic output

    std::cout << "Client version: " << irbis::libraryVersionString() << std::endl;

    try {

        irbis::Connection connection;
        connection.host = L"127.0.0.1";
        connection.port = 6666;
        connection.username = L"librarian";
        connection.password = L"secret";
        const auto isConnected = connection.connect();
        if (!isConnected) {
            std::cout << "Not connected!\n";
            std::cout << "Error code: " << connection.lastError << "\n";
            return -1;
        }

        const auto version = connection.getServerVersion();
        std::cout << "Server version: " << _C(version.version) << std::endl;

        const auto maxMfn = connection.getMaxMfn(L"IBIS");
        std::cout << "max MFN: " << maxMfn << std::endl;

        connection.noOp();
        std::wcout << "NOOP" << std::endl;

        //auto f = connection.noOpAsync();
        //f.get();
        //std::cout << "NOOP ASYNC" << std::endl;

        const auto formatted = connection.formatRecord(L"@brief", 1);
        std::cout << "FORMAT: " << _C(formatted) << std::endl;

        const auto record = connection.readRecord(1);
        std::cout << "READ: " << _C(record.fm(200, L'a')) << std::endl;

        irbis::MfnList found = connection.search(L"K=alg$");
        std::cout << "SEARCH:";
        for (auto mfn : found) {
            std::cout << " " << std::to_string(mfn);
        }
        std::cout << std::endl;

        connection.disconnect();
        std::cout << "disconnected" << std::endl;
    }
    catch (std::exception &exception) {
        std::cout << exception.what() << std::endl;
    }

    return 0;
}
