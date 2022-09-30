#include <iostream>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#endif

#include "micro.h"

using RF = irbis::RecordField;
using SF = irbis::SubField;

int main()
{
    #ifdef WIN32
    // нужно только в Windows
    SetConsoleOutputCP (65001);
    #endif

//    irbis::MarcRecord record;
//    record.mfn = 1234u;
//    record.status = irbis::RecordStatus::Last;
//    record.version = 321;
//
//    record << RF (100, { SF ('a', "SubA"),
//                         SF ('b', "SubB") });
//
//    record << RF (200, { SF ('c', "SubC"),
//                         SF ('d', "SubD") });
//
//    record << RF (300, { SF ('e', "Подполе Е"),
//                         SF ('f', "Подполе Ф") });
//
//    std::cout << record << std::endl;
//
//    std::cout << std::endl;
//
//    std::ostringstream output;
//    output << record;
//    std::string text = output.str();
//    std::cout << text << std::endl;
//
//    std::cout << std::endl << std::endl;

    try
    {

        irbis::Connection connection;
        connection.host = "127.0.0.1";
        connection.port = 6666;
        connection.username = "librarian";
        connection.password = "secret";
        const auto isConnected = connection.connect();
        if (!isConnected) {
            std::cout << "Not connected!" << std::endl;
            std::cout << "Error code: " << connection.lastError << std::endl;
            return -1;
        }

        //const auto version = connection.getServerVersion();
        //std::cout << L"Server version: " << version.version << std::endl;

        const auto maxMfn = connection.getMaxMfn ("IBIS");
        std::cout << "max MFN: " << maxMfn << std::endl;

        connection.noOp();
        std::cout << L"NOOP" << std::endl;

        connection.disconnect();
        std::cout << "disconnected" << std::endl;
    }
    catch (std::exception &exception) {
        std::cout << "Error: " << exception.what() << std::endl;
    }

    return 0;
}
