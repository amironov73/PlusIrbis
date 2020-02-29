// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef RFID_H
#define RFID_H

#include <vector>

#ifdef IRBIS_WINDOWS
#include <winscard.h>
#endif

namespace irbis {

bool IRBIS_API isRfidSupported();

/// \brief Поддержка Smart Cards
class IRBIS_API WinSmartCards final
{
public:

    ~WinSmartCards();

    bool open();
    std::vector<std::string> listReaders();
    bool connect (const std::string &name);
    std::string readCard();
    void disconnect ();
    void close();

private:

#ifdef IRBIS_WINDOWS

    SCARDCONTEXT context { 0u };
    SCARDHANDLE reader { 0u };
    DWORD protocol { 0u };

#endif
};

}

#endif // RFID_H
