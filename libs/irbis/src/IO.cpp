// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#ifdef  IRBIS_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "ws2_32.lib")

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#endif

/*!
    \class irbis::IO

    \details У ИРБИС64 довольно оригинальный формат записи целых чисел на диске.

    32-битные целые записываются в сетевом формате (Big Endian).
    64-битные записываются как пара 32-битных, причем сначала идет младшее,
    затем старшее.

 */

namespace irbis {

/// \brief Чтение беззнакового 32-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Куда поместить результат.
/// \return Признак успешного выполнения операции.
bool IO::readInt32 (FILE* file, uint32_t *value)
{
    uint32_t buffer;
    if (::fread((Byte*)&buffer, 1, sizeof(uint32_t), file) != sizeof(uint32_t)) {
        return false;
    }

    *value = ntohl(buffer);
    return true;
}

/// \brief Чтение беззнакового 64-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Куда поместить результат.
/// \return Признак успешного выполнения операции.
bool IO::readInt64 (FILE* file, uint64_t *value)
{
    uint32_t buffer[2];

    if (::fread((Byte*)buffer, 1, sizeof(uint64_t), file) != sizeof(uint64_t)) {
        return false;
    }

    buffer[0] = ntohl(buffer[0]);
    buffer[1] = ntohl(buffer[1]);
    *value = (((uint64_t)buffer[1]) << 32) + ((uint64_t)buffer[0]);

    return true;
}

/// \brief Запись беззнакового 32-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Записываемое значение.
/// \return Признак успешного выполнения операции.
bool IO::writeInt32 (FILE* file, uint32_t value)
{
    value = htonl(value);
    if (::fwrite((Byte*)&value, 1, sizeof(value), file) != sizeof(value)) {
        return false;
    }

    return true;
}

/// \brief Запись беззнакового 64-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Записываемое значение.
/// \return Признак успешного выполнения операции.
bool IO::writeInt64 (FILE* file, uint64_t value)
{
    auto ptr = (uint32_t*)&value;
    uint32_t temp[2];
    temp[0] = htonl(ptr[0]);
    temp[1] = htonl(ptr[1]);
    auto buffer = (Byte*)temp;
    if (::fwrite(buffer, 1, sizeof(value), file) != sizeof(value)) {
        return false;
    }

    return true;
}

}

