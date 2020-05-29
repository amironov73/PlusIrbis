// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"

#include <cctype>
#include <limits>
#include <iterator>

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

struct Network final
{
    static void start();
    static void stop();
};

#ifdef IRBIS_WINDOWS

static WSADATA wsaData;

#endif

// если мы подключаемся к локальному хосту
void connectToLocalHost(struct sockaddr_in *serverAddress)
{
    serverAddress->sin_addr.s_addr = htonl (INADDR_LOOPBACK);
}

// если адрес задан четырьмя числами: "123.231.123.231"
void resolveNumbericalAddress (const char *serverName,
                               struct sockaddr_in *serverAddress)
{
#ifdef IRBIS_WINDOWS

    auto returnCode = ::inet_addr(serverName);
    if (returnCode == INADDR_NONE) {
        throw irbis::NetworkException();
    }
    ((uint32_t*)&serverAddress->sin_addr)[0] = (uint32_t)returnCode;

#else

    auto returnCode = ::inet_pton (AF_INET, serverName,
                                   &serverAddress->sin_addr);
    if (returnCode != 1) {
        std::cerr << "inet_pton() failed" << std::endl;
        std::exit (2);
    }
#endif
}

// если адрес задан именем хоста: "someserver"
void resolveHostname (const char *serverName,
                      struct sockaddr_in *serverAddress)
{
    struct hostent *host = gethostbyname (serverName);
    if (host == nullptr) {
        std::cerr << "gethostbyname() failed" << std::endl;
        std::exit (2);
    }
    ((uint32_t*)&serverAddress->sin_addr)[0] =
        ((uint32_t**)host->h_addr_list)[0][0];
}

void Network::start()
{
#ifdef IRBIS_WINDOWS

    if (WSAStartup(0x0202, &wsaData)) {
        throw irbis::NetworkException();
    }

#endif
}

void Network::stop()
{
#ifdef IRBIS_WINDOWS

    WSACleanup();

#endif
}
