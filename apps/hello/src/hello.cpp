#include <iostream>

#include "irbis.h"
#include "irbis_private.h"

static std::string _C(std::wstring s)
{
    return irbis::unicode_to_cp866(s);
}

static std::wstring _U(std::string s)
{
    return irbis::cp1251_to_unicode(s);
}

int main()
{
    setlocale(LC_ALL, ""); // for cyrillic output

    //std::cout << "Client version: " << irbis::libraryVersionString() << std::endl;
    std::wcout << L"Client version: " << _U(irbis::libraryVersionString()) << std::endl;

    try {

        irbis::Connection connection;
        connection.host = L"127.0.0.1";
        connection.port = 6666;
        connection.username = L"librarian";
        connection.password = L"secret";
        const auto isConnected = connection.connect();
        if (!isConnected) {
            std::wcout << L"Not connected!\n";
            std::wcout << L"Error code: " << connection.lastError << "\n";
            return -1;
        }

        const auto version = connection.getServerVersion();
        //std::cout << "Server version: " << _C(version.version) << std::endl;
        std::wcout << L"Server version: " << version.version << std::endl;

        const auto maxMfn = connection.getMaxMfn(L"IBIS");
        //std::cout << "max MFN: " << maxMfn << std::endl;
        std::wcout << L"max MFN: " << maxMfn << std::endl;

        connection.noOp();
        //std::cout << "NOOP" << std::endl;
        std::wcout << L"NOOP" << std::endl;

        //auto f = connection.noOpAsync();
        //f.get();
        //std::cout << "NOOP ASYNC" << std::endl;

        const auto formatted = connection.formatRecord(L"@brief", 1);
        //std::cout << "FORMAT: " << _C(formatted) << std::endl;
        std::wcout << L"FORMAT: " << formatted << std::endl;

        const auto record = connection.readRecord(1);
        //std::cout << "READ: " << _C(record.fm(200, L'a')) << std::endl;
        std::wcout << "READ: " << record.fm(200, L'a') << std::endl;

        irbis::MfnList found = connection.search(L"K=alg$");
        //std::cout << "SEARCH:";
        std::wcout << "SEARCH:";
        for (auto mfn : found) {
            //std::cout << " " << mfn;
            std::wcout << L" " << mfn;
        }
        //std::cout << std::endl;
        std::wcout << std::endl;

        connection.disconnect();
        std::cout << "disconnected" << std::endl;
    }
    catch (std::exception &exception) {
        std::wcout << _U(exception.what()) << std::endl;
    }

    return 0;
}
