// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <algorithm>
#include "irbis.h"
#include "irbis_internal.h"

bool checkField (const irbis::RecordField &field, const irbis::String &number)
{
    const auto value = field.getFirstSubfieldValue ('b');
    return irbis::sameString (value, number);
}

void processRecord (irbis::Connection &connection,
        const irbis::String &newSigla,
        const irbis::String &number)
{
    const auto expression = L"\"IN=" + number + L"\"";
    const auto found = connection.search(expression);
    if (found.empty()) {
        std::cout << "not found" << std::endl;
    }

    for (const auto mfn : found) {
        auto record = connection.readRecord (mfn);
        auto fields = record.getFields (910);
        const auto target = std::find_if(std::begin(fields), std::end(fields),
                [&number](const irbis::RecordField *f) { return checkField(*f, number); } );
        if (target != std::end(fields)) {
            const auto existingSigla = (*target)->getFirstSubfieldValue('d');
            if (!irbis::sameString(existingSigla, newSigla)) {
                (*target)->removeSubfield('!')
                    .removeSubfield('s');
                connection.writeRecord(record, false, true, true);
            }
        }
    }
}

int main (int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "sigler -- IRBIS64 exemplar mover" << std::endl;
        std::cout << "USAGE: sigler <file> <sigla> <connectionString>" << std::endl << std::endl;
        return 0;
    }

    std::cout << "Client version: " << irbis::libraryVersionString() << std::endl;

    try {
        const auto fileName = irbis::string2wide(argv[1]);
        const auto sigla = irbis::string2wide(argv[2]);
        irbis::String connectionString = irbis::string2wide(argv[3]);
        irbis::Connection connection;
        connection.parseConnectionString(connectionString);
        if (!connection.connect()) {
            std::cerr << "Not connected!" << std::endl;
            std::cerr << "Error code: " << connection.lastError << std::endl;
            return -1;
        }

        const auto lines = irbis::Text::readUtfLines(fileName);
        for (const auto &number : lines) {
            processRecord (connection, sigla, number);
        }
    }
    catch (std::exception &exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
    }

    return 0;
}
