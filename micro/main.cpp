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

    irbis::MarcRecord record;
    record.mfn = 1234u;
    record.status = irbis::RecordStatus::Last;
    record.version = 321;

    record << RF (100, { SF ('a', "SubA"),
                         SF ('b', "SubB") });

    record << RF (200, { SF ('c', "SubC"),
                         SF ('d', "SubD") });

    record << RF (300, { SF ('e', "Подполе Е"),
                         SF ('f', "Подполе Ф") });

    std::cout << record << std::endl;

    std::cout << std::endl;

    std::ostringstream output;
    output << record;
    std::string text = output.str();
    std::cout << text << std::endl;

    return 0;
}
