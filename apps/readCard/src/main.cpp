// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <algorithm>
#include "irbis.h"
#include "rfid.h"

#ifdef IRBIS_WINDOWS
#pragma comment(lib, "winscard.lib")
#endif

int main()
{
    irbis::WinSmartCards mgr;

    if (!mgr.open()) {
        std::cerr << "Can't open" << std::endl;
        return 1;
    }

    const auto readers = mgr.listReaders();
    for (const auto r : readers) {
        std::cout << r << std::endl;
    }

    if (!mgr.connect("OMNIKEY CardMan 5x21-CL 0")) {
        std::cerr << "Can't connect" << std::endl;
        return 1;
    }

    const auto card = mgr.readCard();
    std::cout << "Card: " << card << std::endl;

    return 0;
}
