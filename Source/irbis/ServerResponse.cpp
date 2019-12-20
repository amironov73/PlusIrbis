// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cstdarg>

namespace irbis {

ServerResponse::ServerResponse(Connection &connection, ClientQuery &query)
{
    this->_connection = &connection;
    returnCode = 0;
    _success = false;
    _position = 0;

    // Read the response
    ClientSocket &socket = *connection.socket;
    socket.host = connection.host;
    socket.port = connection.port;
    socket.open();

    auto encoded = query.encode();
    const auto data = encoded.data();
    const auto size = encoded.size();
    socket.send(data, size);

    BYTE buffer[2048];
    while(true)
    {
        const auto received = socket.receive(buffer, sizeof(buffer));
        if (received <= 0)
        {
            break;
        }
        _write(buffer, received);
    }
    socket.close();

    // decode the response
    command = readAnsi();
    clientId = readInteger();
    queryId = readInteger();
    answerSize = readInteger();
    serverVersion = readAnsi();
    readAnsi();
    readAnsi();
    readAnsi();
    readAnsi();
    readAnsi();

    // set the success flag
    _success = true;
}

bool ServerResponse::checkReturnCode()
{
    return this->checkReturnCode(0);
}

bool ServerResponse::checkReturnCode(int nargs, ...)
{
    this->_connection->lastError = 0;
    if (!this->_success) {
        this->_connection->lastError = -100002;
        return false;
    }

    va_list args;
    auto result = true;

    if (this->getReturnCode() < 0)
    {
        va_start(args, nargs);
        result = false;
        for(auto i = 0; i < nargs; i++)
        {
            const int code = va_arg(args, int);
            if (code == returnCode)
            {
                result = true;
                break;
            }
        }
        va_end(args);
        this->_connection->lastError = this->returnCode;
    }

    return result;
}

std::string ServerResponse::getLine()
{
    std::string result;
    const auto size = _content.size();
    while (_position < size)
    {
        char c = _content.at(_position);
        _position++;

        if (c == 13)
        {
            if (_position < size)
            {
                c = _content.at(_position);
                if (c == 10)
                {
                    _position++;
                }
            }
            break;
        }

        result.push_back(c);
    }

    return result;
}

std::string ServerResponse::getRemaining()
{
    std::string result;
    const auto size = _content.size();
    if (_position < size)
    {
        const auto data = reinterpret_cast<const char*>(_content.data() + _position);
        size_t remaining = size - _position;
        while (remaining > 0) {
            const auto c = data[remaining-1];
            if (c != '\r' && c != '\n') {
                break;
            }
            remaining--;
        }
        const std::string s2(data, remaining);
        result = s2;
        _position = size;
    }

    return result;
}

int ServerResponse::getReturnCode()
{
    returnCode = readInteger();
    return returnCode;
}

std::wstring ServerResponse::readAnsi()
{
    const auto line = getLine();
    return cp1251_to_unicode(line);
}

int ServerResponse::readInteger()
{
    const auto line = getLine();
    return std::stoi(line);
}

StringList ServerResponse::readRemainingAnsiLines()
{
    const auto text = readRemainingAnsiText();
    return split(text, '\n');
}

std::wstring ServerResponse::readRemainingAnsiText()
{
    const auto line = getRemaining();
    return cp1251_to_unicode(line);
}

StringList ServerResponse::readRemainingUtfLines()
{
    // const auto text = readRemainingUtfText();
    // return split(text, '\n');

    StringList result;
    while (true) {
        const auto line = readUtf();
        if (line.empty()){
            break;
        }

        result.push_back(line);
    }

    return result;
}

std::wstring ServerResponse::readRemainingUtfText()
{
    const auto line = getRemaining();
    return fromUtf(line);
}

std::wstring ServerResponse::readUtf()
{
    const auto line = getLine();
    return fromUtf(line);
}

bool ServerResponse::success() const
{
    return this->_success;
}

void ServerResponse::_write(const BYTE *bytes, size_t size)
{
    const auto newSize = this->_content.size() + size;
    if (this->_content.capacity() < newSize) {
        this->_content.reserve(newSize);
    }

    for (size_t i = 0; i < size; i++) {
        this->_content.push_back(bytes[i]);
    }
}

}
