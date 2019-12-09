#include "pch.h"
#include <iostream>

#include "stdafx.h"

int main()
{
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
    std::wcout << "NOOP" << std::endl;

    connection.disconnect();
    std::wcout << "disconnected" << std::endl;

    return 0;
}

