#include "irbis.h"
#include "irbis_internal.h"

#include <iostream>
#include <algorithm>

#ifdef IRBIS_WINDOWS

#include <winsock2.h>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <csignal>
#include <unistd.h>
#define HOSTENT struct hostent
#define closesocket(__x) close(__x)

#endif

static const char *proxyVersion = "0.1";
static bool stopFlag = false;
static int currentLoad = 0;
static int64_t transactionIndex = 0;
static short localPort = 6666;
static short remotePort = 5555;
static int listenerSocket;
static volatile bool stop = false;

static void drawLine()
{
    std::cout << "--------------------------------------------------\n";
}

/// \brief Разбор аргументов командной строки.
/// \param argc
/// \param argv
/// \return true если все хорошо.
static bool parseCommandLine (int argc, char *argv[])
{
    // TODO implement
    return true;
}

#ifndef IRBIS_WINDOWS

static void handleSignal (int sig)
{
    if (sig == SIGINT) {
        stop = true;
        std::cout << "SIGINT received" << std::endl;
    }
}

#endif

/// \brief Считывание конфигурации и инициализация.
/// \return true, если все хорошо.
static bool initialize()
{
    std::cout << "Initializing..." << std::endl;

#ifdef IRBIS_WINDOWS

    WSADATA wsaData;
    if (WSAStartup (0x0202, &wsaData)) {
        return false;
    }

#else

    ::signal (SIGCHLD, SIG_IGN);
    ::signal (SIGINT, handleSignal);

#endif

    std::cout << "Initialization done" << std::endl;
    return true;
}

static bool receiveExact (int socket, irbis::ChunkedBuffer &buffer, int howMany)
{
    char temp [1024];
    try {
        int toRead = min ((int) sizeof (temp), howMany);
        while (toRead > 0) {
            int received = ::recv (socket, temp, toRead, 0);
            if (received <= 0) {
                // error occurred
                return false;
            }
            buffer.write ((irbis::Byte*) temp, 0, received);
            howMany -= received;
            toRead = min ((int) sizeof (temp), howMany);
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

static bool receiveAll (int socket, irbis::ChunkedBuffer &buffer)
{
    char temp [1024];
    try {
        while (true) {
            int received = ::recv (socket, temp, sizeof (temp), 0);
            if (received < 0) {
                // error occurred
                return false;
            }
            if (received == 0) {
                // end of data
                break;
            }
            buffer.write ((irbis::Byte*) temp, 0, received);
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

static bool sendAll(int socket, irbis::ChunkedBuffer &buffer)
{
    char temp [1024];
    try {
        while (true) {
            int chunk = buffer.read ((irbis::Byte*) temp, 0, sizeof(temp));
            if (chunk == 0) {
                break;
            }
            int sended = ::send (socket, temp, chunk, 0);
            if (sended < 0) {
                // error occurred
                return false;
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

static bool createListener()
{
    std::cout << "Creating listener socket" << std::endl;

    listenerSocket = ::socket (AF_INET, SOCK_STREAM, 0);
    if (listenerSocket == -1) {
        std::cerr << "socket() failed" << std::endl;
        return false;
    }

    struct sockaddr_in localAddress { 0 };
    localAddress.sin_family = AF_INET;
    localAddress.sin_addr.s_addr = htonl (INADDR_ANY);
    localAddress.sin_port = htons (localPort);

    auto returnCode = ::bind (listenerSocket,
        (struct sockaddr*) &localAddress,
        sizeof (localAddress));
    if (returnCode == -1) {
        std::cerr << "bind() failed" << std::endl;
        ::closesocket (listenerSocket);
        return false;
    }

    returnCode = ::listen (listenerSocket, 0);
    if (returnCode == -1) {
        std::cerr << "listen() failed" << std::endl;
        ::closesocket (listenerSocket);
        return false;
    }

    std::cout << "Listener socket created" << std::endl;
    return true;
}

template <class It>
int32_t parseInt32 (const It begin, const It end)
{
    int32_t result = 0;
    for (It it = begin; it != end; ++it) {
        auto c = (uint8_t) *it;
        result = (result * 10) + (c - '0');
    }
    return result;
}

/// \brief Обработка клиента.
static void handleClient (int clientSocket)
{
#ifndef IRBIS_WINDOWS
    auto rc = ::fork();
    if (rc == -1) {
        std::cerr << "fork() failed" << std::endl;
        ::closesocket (clientSocket);
    }

    if (rc != 0) {
        // in parent process
        ::closesocket (clientSocket);
        return;
    }
#endif

    std::cout << "Client arrived" << std::endl;

    auto downSocket = ::socket (AF_INET, SOCK_STREAM, 0);
    if (downSocket == -1) {
        std::cerr << "socket() failed" << std::endl;
        ::closesocket (clientSocket);
        return;
    }

    struct sockaddr_in serverAddress { 0 };
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons (remotePort);
    serverAddress.sin_addr.s_addr = htonl (INADDR_LOOPBACK);

    auto returnCode = ::connect (downSocket,
        (struct sockaddr *)& serverAddress,
        sizeof(serverAddress));
    if (returnCode == -1) {
        std::cerr << "connect() failed" << std::endl;
        ::closesocket (clientSocket);
        ::closesocket (downSocket);
        return;
    }

    {
        // Фаза 0
        irbis::ChunkedBuffer buffer;
        if (!receiveExact (clientSocket, buffer, 10)) {
            std::cerr << "receiveExact() failed" << std::endl;
            return;
        }
        int32_t received = buffer.size();
        auto begin = std::begin (buffer);
        auto end = std::end (buffer);
        auto found = std::find (begin, end, 0x0A);
        if (found == end) {
            std::cerr << "receiveExact() failed" << std::endl;
            return;
        }
        int32_t requestSize = parseInt32 (begin, found);
        int32_t remaining = requestSize - received + (int)(found - begin);

        // Фаза 1
        if (!receiveExact (clientSocket, buffer, remaining)) {
            std::cerr << "receiveExact() failed" << std::endl;
            return;
        }

        std::cout << "Client request: " << buffer.size() << std::endl;
        if (!sendAll (downSocket, buffer)) {
            std::cerr << "sendAll() failed" << std::endl;
            return;
        }
        //::shutdown (downSocket, SD_SEND);
    }
    {
        // Фаза 2
        irbis::ChunkedBuffer buffer;
        if (!receiveAll (downSocket, buffer)) {
            std::cerr << "receiveAll() failed" << std::endl;
            return;
        }
        std::cout << "Server response: " << buffer.size() << std::endl;
        if (!sendAll (clientSocket, buffer)) {
            std::cerr << "sendAll() failed" << std::endl;
            return;
        }
    }

    ::closesocket (clientSocket);
    std::cout << "Client handled" << std::endl;
}

/// \brief Главный серверный цикл.
/// \return
static bool serverLoop()
{
    while (!stop) {
        auto clientSocket = ::accept (listenerSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "accept() failed" << std::endl;
            ::closesocket (listenerSocket);
            return false;
        }

        handleClient (clientSocket);
    }

    return true;
}

int main (int argc, char *argv[])
{
    std::cout << "IRBIS-PROXY application version " << proxyVersion << std::endl;

    if (!parseCommandLine(argc, argv)) {
        std::cerr << "parseCommandLine() failed" << std::endl;
        return 1;
    }

    if (!initialize()) {
        std::cerr << "initialize() failed" << std::endl;
        return 2;
    }

    if (!createListener()) {
        std::cerr << "createListener() failed" << std::endl;
        return 3;
    }

    if (!serverLoop()) {
        std::cerr << "serverLoop() failed" << std::endl;
        return 4;
    }

    std::cout << "Exiting" << std::endl;

    return 0;
}
