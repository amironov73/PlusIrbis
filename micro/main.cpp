#include <iostream>

#include "micro.h"

using RF = irbis::RecordField;
using SF = irbis::SubField;

int main()
{
    irbis::MarcRecord record;

    record << RF (100, { SF ('a', "SubA"),
                         SF ('b', "SubB") });
    record << RF (200, { SF ('c', "SubC"),
                         SF ('d', "SubD") });

    std::cout << record << std::endl;

    return 0;
}
