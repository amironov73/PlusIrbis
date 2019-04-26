// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include <iomanip>

NAMESPACE_IRBIS_BEGIN

ClientQuery::ClientQuery(Connection *connection, const std::string &commandCode)
{
    addAnsi(commandCode).newLine();
    addAnsi(connection->workstation).newLine();
    addAnsi(commandCode).newLine();
    add(connection->clientId).newLine();
    add(connection->queryId).newLine();
    addAnsi(connection->password).newLine();
    addAnsi(connection->username).newLine();
    newLine();
    newLine();
    newLine();
}

void ClientQuery::_write(const BYTE* bytes, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        _content.push_back(bytes[i]);
    }
}

void ClientQuery::_write(BYTE byte)
{
    _content.push_back(byte);
}

ClientQuery& ClientQuery::add(int value)
{
    std::string s = std::to_string(value);
    return addAnsi(s);
}

ClientQuery& ClientQuery::add(const FileSpecification &specification)
{
    return *this;
}

ClientQuery& ClientQuery::add(const MarcRecord &record)
{
    return *this;
}

ClientQuery& ClientQuery::addAnsi(const std::string &text)
{
    const size_t size = text.length();
    if (!size)
    {
        return *this;
    }

    const BYTE *ptr = reinterpret_cast<const BYTE*>(text.c_str());
    _write(ptr, size);
    return *this;
}

ClientQuery& ClientQuery::addAnsi(const std::wstring &text)
{
    const size_t size = text.length();
    if (!size)
    {
        return *this;
    }

    const wchar_t *src = text.c_str();
    BYTE *dst = new BYTE[size];
    unicode_to_cp1251(dst, src, size);
    _write(dst, size);
    delete[] dst;
    return *this;
}

ClientQuery& ClientQuery::addUtf(const std::wstring &text)
{
    const size_t size = text.length();
    if (!size)
    {
        return *this;
    }

    const wchar_t *src = text.c_str();
    const int bufSize = countUtf(src, size);
    BYTE *dst = new BYTE[bufSize];
    BYTE *end = toUtf(dst, src, size);
    _write(dst, end - dst);
    delete[] dst;
    return *this;
}

void ClientQuery::dump(std::ostream &stream) const
{
    for (auto value : _content)
    {
        stream << std::hex << std::setw(2) << value << " ";
    }
}

ClientQuery& ClientQuery::newLine()
{
    _write(0x10);
    return *this;
}

NAMESPACE_IRBIS_END
