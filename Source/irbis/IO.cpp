// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

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

namespace irbis {

/// \brief Чтение беззнакового 32-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Куда поместить результат.
/// \return Признак успешного выполнения операции.
bool IO::readInt32 (FILE* file, uint32_t *value)
{
    auto buffer = (Byte*)(value);
    if (::fread(buffer, 1, sizeof(uint32_t), file) != sizeof(uint32_t)) {
        return false;
    }

    *value = ntohl(*value);

    return true;
}

/// \brief Чтение беззнакового 64-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Куда поместить результат.
/// \return Признак успешного выполнения операции.
bool IO::readInt64 (FILE* file, uint64_t *value)
{
    auto buffer = (Byte*)value;
    if (::fread(buffer, 1, sizeof(uint64_t), file) != sizeof(uint64_t)) {
        return false;
    }

    auto ptr = (uint32_t*)value;
    ptr[0] = ntohl(ptr[0]);
    ptr[1] = ntohl(ptr[1]);

    return true;
}

/// \brief Запись беззнакового 32-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Записываемое значение.
/// \return Признак успешного выполнения операции.
bool IO::writeInt32 (FILE* file, uint32_t value)
{
    value = htonl(value);
    auto buffer = (Byte*)(&value);
    if (::fwrite(buffer, 1, sizeof(value), file) != sizeof(value)) {
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

