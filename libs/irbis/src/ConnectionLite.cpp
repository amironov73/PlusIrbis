// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

LiteRecord ConnectionLite::readLiteRecord (Mfn mfn)
{
    LiteRecord result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "C");
    query.addAnsi (this->database).newLine()
            .add (mfn);

    ServerResponse response (*this, query);
    if (response.checkReturnCode (4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingLinesUtf();
        result.decode (lines);
        result.database = toUtf (this->database);
    }
    return result;
}

}
