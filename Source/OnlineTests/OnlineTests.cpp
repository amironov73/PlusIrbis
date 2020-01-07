// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../irbis/irbis.h"

#include <iostream>

// ReSharper disable StringLiteralTypo

#pragma comment (lib, "irbis.lib")

int main()
{
    setlocale(LC_ALL, ""); // for cyrillic output

    try {

    irbis::Connection connection;
    connection.host = L"192.168.1.35";
    connection.port = 6666;
    connection.username = L"librarian";
    connection.password = L"secret";
    const auto isConnected = connection.connect();
    std::wcout << L"connected: " << isConnected << std::endl;
    if (!isConnected) {
        return -1;
    }

    const auto version = connection.getServerVersion();
    std::wcout << L"Version: " << version.version << std::endl;

    const auto maxMfn = connection.getMaxMfn(L"IBIS");
    std::wcout << L"max MFN: " << maxMfn << std::endl;

    connection.noOp();
    std::wcout << L"NOOP" << std::endl;

    auto f = connection.noOpAsync();
    f.get();
    std::wcout << L"NOOP ASYNC" << std::endl;

    const auto formatted = connection.formatRecord(L"@brief", 1);
    std::wcout << L"FORMAT: " << formatted << std::endl;

    const auto record = connection.readRecord(1);
    std::wcout << L"READ: " << record.fm(200, L'a') << std::endl;

    irbis::MfnList found = connection.search(L"K=alg$");
    std::wcout << L"SEARCH:";
    for (auto mfn : found) {
        std::wcout << L" " << std::to_wstring(mfn);
    }
    std::wcout << std::endl;

    connection.disconnect();
    std::wcout << L"disconnected" << std::endl;

    }
    catch (std::exception &exception) {
        std::cout << exception.what() << std::endl;
    }

    return 0;
}


