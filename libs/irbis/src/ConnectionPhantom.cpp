// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#include <random>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

PhantomRecord ConnectionPhantom::readPhantomRecord (Mfn mfn)
{
    PhantomRecord result;
    if (!this->_checkConnection()) {
        return result;
    }

    result.database = toUtf (this->database);
    return result;
}

int ConnectionPhantom::writePhantomRecord (PhantomRecord &record)
{
    if (!this->_checkConnection()) {
        return 0;
    }
    return 0;
}

}
