// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iomanip>

namespace irbis {

ClientQuery::ClientQuery(const Connection &connection, const std::string &commandCode)
{
    addAnsi(commandCode).newLine();
    addAnsi(connection.workstation).newLine();
    addAnsi(commandCode).newLine();
    add(connection.clientId).newLine();
    add(connection.queryId).newLine();
    addAnsi(connection.password).newLine();
    addAnsi(connection.username).newLine();
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
    return this->addAnsi(specification.toString());
}

ClientQuery& ClientQuery::add(const MarcRecord &record, const std::wstring &delimiter=L"\u001E")
{
    return this->addUtf(record.encode(delimiter));
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

bool ClientQuery::addFormat(const std::wstring &format)
{
    if (format.empty()) {
        this->newLine();
        return false;
    }

    const auto prepared = prepareFormat(trimStart(format));

    if (format[0] == '@') {
        this->addAnsi(format);
    } else if (format[0] == '!') {
        this->addUtf(prepared);
    } else {
        this->addUtf(std::wstring (L"!") + prepared);
    }

    this->newLine();
    return true;
}

ClientQuery& ClientQuery::addUtf(const std::wstring &text)
{
    const size_t size = text.length();
    if (!size)
    {
        return *this;
    }

    const wchar_t *src = text.c_str();
    const size_t bufSize = countUtf(src, size);
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

std::vector<BYTE> ClientQuery::encode() const
{
    std::vector<BYTE> result;
    result.reserve(_content.size() + 10);
    const auto prefix = std::to_string(_content.size());
    const auto ptr = prefix.c_str();
    for (size_t i = 0; i < prefix.length(); i++) {
        result.push_back(ptr[i]);
    }
    result.push_back(0x0a);
    result.insert(result.end(), _content.begin(), _content.end());

    return result;
}

ClientQuery& ClientQuery::newLine()
{
    _write(0x0A);
    return *this;
}

}
