// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#if defined(IRBIS_WINDOWS) || defined(IRBIS_MINGW)

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>

#ifdef IRBIS_WINDOWS
#pragma comment (lib, "ws2_32.lib")
#endif

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define HOSTENT struct hostent
#define closesocket(__x) ::close(__x)

#endif

namespace irbis {

struct Tcp4Socket::TcpInternals
{
#ifdef IRBIS_WINDOWS

    WSADATA wsaData;
    SOCKET socket;

#else

    int socket;

#endif
};

Tcp4Socket::Tcp4Socket (const String &host_, short port_)
    : _impl {new TcpInternals}
{
    this->host = host_;
    this->port = port_;

#ifdef IRBIS_WINDOWS

    if (WSAStartup (0x0202, &this->_impl->wsaData)) {
        throw NetworkException();
    }

#endif
}

Tcp4Socket::~Tcp4Socket()
{
#ifdef IRBIS_WINDOWS

    WSACleanup();

#endif
}

void Tcp4Socket::open()
{
    this->_impl->socket = socket (AF_INET, SOCK_STREAM, 0);
    //if (internals->socket < 0) {
    //    throw NetworkException();
    //}

    sockaddr_in destinationAddress {};
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

    if (connect(this->_impl->socket, (sockaddr*)&destinationAddress, sizeof(destinationAddress))) {
        throw NetworkException();
    }
}

void Tcp4Socket::close()
{
    closesocket(this->_impl->socket);
}

void Tcp4Socket::send (const Byte *buffer, std::size_t size)
{
    const auto ptr = reinterpret_cast <const char*> (buffer);
    const auto size2 = static_cast <int> (size);
    ::send (this->_impl->socket, ptr, size2, 0);
}

std::size_t Tcp4Socket::receive (Byte *buffer, std::size_t size)
{
    const auto ptr = reinterpret_cast <char*> (buffer);
    const auto size2 = static_cast <int> (size);
    const std::size_t result = ::recv (this->_impl->socket, ptr, size2, 0);
    return result;
}

}
