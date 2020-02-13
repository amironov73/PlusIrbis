// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "rfid.h"

namespace irbis {

/// \brief Поддерживается ли RFID на данной системе?
/// \return `true` если поддерживается.
bool isRfidSupported()
{
#ifdef IRBIS_WINDOWS

    return true;

#else

    return false;

#endif
}

}
