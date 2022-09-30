// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"

#include <iostream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#ifdef WIN32

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

namespace irbis
{
    class ClientSocketImpl final
    {
        public:

        #ifdef WIN32

        WSADATA wsaData;
        SOCKET socket;

        #else

        int socket;

        #endif
    };

    ClientSocket::ClientSocket
        (
            const std::string &host,
            short port
        )
        : _impl (new ClientSocketImpl)
    {
        this->host = host;
        this->port = port;

        #ifdef WIN32

        if (WSAStartup (0x0202, &this->_impl->wsaData)) {
            throw std::exception();
        }

        #endif
    }

    ClientSocket::~ClientSocket()
    {
        #ifdef WIN32

        WSACleanup();

        #endif
    }


    void ClientSocket::open()
    {
        this->_impl->socket = socket (AF_INET, SOCK_STREAM, 0);

        sockaddr_in destinationAddress {};
        destinationAddress.sin_family = AF_INET;
        destinationAddress.sin_port = htons (this->port);
        auto ansi = this->host;
        auto inaddr = inet_addr (ansi.c_str());
        if (inaddr != INADDR_NONE) {
            destinationAddress.sin_addr.s_addr = inaddr;
        }
        else {

            HOSTENT *hostentry = gethostbyname (ansi.c_str());
            if (hostentry) {
                ((unsigned long*) &destinationAddress.sin_addr)[0] =
                    ((unsigned long **) hostentry->h_addr_list)[0][0];
            }
            else {
                throw std::exception();
            }
        }

        if (connect (this->_impl->socket, (sockaddr*)&destinationAddress, sizeof(destinationAddress))) {
            throw std::exception();
        }
    }

    void ClientSocket::close()
    {
        closesocket (this->_impl->socket);
    }

    void ClientSocket::send
        (
            const unsigned char *buffer,
            std::size_t size
        )
    {
        const auto ptr = reinterpret_cast <const char*> (buffer);
        const auto size2 = static_cast <int> (size);
        ::send (this->_impl->socket, ptr, size2, 0);
    }

    std::size_t ClientSocket::receive
        (
            unsigned char *buffer,
            std::size_t size
        )
    {
        const auto ptr = reinterpret_cast <char*> (buffer);
        const auto size2 = static_cast <int> (size);
        const std::size_t result = ::recv (this->_impl->socket, ptr, size2, 0);
        return result;
    }
}
