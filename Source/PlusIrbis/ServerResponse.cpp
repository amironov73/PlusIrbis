// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

ServerResponse::ServerResponse(Connection &connection)
{
    returnCode = 0;

    // TODO read the answer
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
}

bool ServerResponse::checkReturnCode()
{
    return true;
}

std::string ServerResponse::getLine()
{
    return "";
}

int ServerResponse::getReturnCode()
{
    return 0;
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

std::vector<std::wstring> ServerResponse::readRemainingAnsiLines()
{
    return std::vector<std::wstring>();
}

std::wstring ServerResponse::readRemainingAnsiText()
{
    return L"";
}

std::vector<std::wstring> readRemainingUtfLines()
{
    return std::vector<std::wstring>();
}

std::wstring readRemainingUtfText()
{
    return L"";
}

std::wstring ServerResponse::readUtf()
{
    const auto line = getLine();
    return fromUtf(line);
}

NAMESPACE_IRBIS_END
