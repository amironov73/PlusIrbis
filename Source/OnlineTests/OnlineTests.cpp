#include "pch.h"
#include <iostream>

#include "stdafx.h"

int main()
{
    setlocale(LC_ALL, ""); // for cyrillic output

    irbis::Connection connection;
    connection.host = L"localhost";
    connection.port = 6666;
    connection.username = L"librarian";
    connection.password = L"secret";
    const auto isConnected = connection.connect();
    std::wcout << L"connected: " << isConnected << std::endl;
    if (!isConnected) {
        return -1;
    }

    const auto maxMfn = connection.getMaxMfn(L"IBIS");
    std::wcout << L"max MFN: " << maxMfn << std::endl;

    connection.noOp();
    std::wcout << L"NOOP" << std::endl;

    const auto formatted = connection.formatRecord(L"@brief", 1);
    std::wcout << L"FORMAT: " << formatted << std::endl;

    const auto record = connection.readRawRecord(L"", 1);
    std::wcout << L"READ RAW:" << std::endl << record.encode(L"\n") << std::endl;

    connection.disconnect();
    std::wcout << L"disconnected" << std::endl;

    return 0;
}

