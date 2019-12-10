// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

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

#define HOSTENT struct hostent
#define closesocket(__x) ::close(__x)

#endif

NAMESPACE_IRBIS_BEGIN

struct TcpInternals
{
#ifdef IRBIS_WINDOWS

    WSADATA wsaData;
    SOCKET socket;

#else

    int socket;

#endif

};

Tcp4Socket::Tcp4Socket(const std::wstring& host_, short port_)
    : _impl(new TcpInternals)
{
    host = host_;
    port = port_;

#ifdef IRBIS_WINDOWS

    auto internals = static_cast<TcpInternals*>(this->_impl);

    if (WSAStartup(0x0202, &internals->wsaData)) {
        throw NetworkException();
    }

#endif
}

Tcp4Socket::~Tcp4Socket()
{
    const auto internals = static_cast<TcpInternals*>(this->_impl);

#ifdef IRBIS_WINDOWS

    WSACleanup();

#endif

    delete (internals);
}

void Tcp4Socket::open()
{
    const auto internals = static_cast<TcpInternals*>(this->_impl);
    internals->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (internals->socket < 0) {
        throw NetworkException();
    }

    sockaddr_in destinationAddress;
    destinationAddress.sin_family = AF_INET;
    destinationAddress.sin_port = htons(this->port);
    auto ansi = unicode_to_cp1251(this->host);
    auto inaddr = inet_addr(ansi.c_str());
    if (inaddr != INADDR_NONE) {
        destinationAddress.sin_addr.s_addr = inaddr;
    }
    else {

        HOSTENT *hostentry = gethostbyname(ansi.c_str());
        if (hostentry) {
            ((unsigned long*)&destinationAddress.sin_addr)[0] =
                    ((unsigned long **)hostentry->h_addr_list)[0][0];
        }
        else {
            throw NetworkException();
        }
    }

    if (connect(internals->socket, (sockaddr*)&destinationAddress, sizeof(destinationAddress))) {
        throw NetworkException();
    }
}

void Tcp4Socket::close()
{
    auto *internals = static_cast<TcpInternals*>(this->_impl);
    closesocket(internals->socket);
}

void Tcp4Socket::send(const BYTE *buffer, size_t size)
{
    auto *internals = static_cast<TcpInternals*>(this->_impl);
    auto ptr = reinterpret_cast<const char*>(buffer);
    ::send(internals->socket, ptr, size, 0);
}

size_t Tcp4Socket::receive(BYTE *buffer, size_t size)
{
    auto *internals = static_cast<TcpInternals*>(this->_impl);
    auto ptr = reinterpret_cast<char*>(buffer);
    auto result = ::recv(internals->socket, ptr, size, 0);
    return result;
}

NAMESPACE_IRBIS_END
