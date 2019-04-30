// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

struct Socket
{
};

Tcp4Socket::Tcp4Socket(const std::wstring& host_, short port_)
    : _impl(new Socket)
{
    host = host_;
    port = port_;
}

Tcp4Socket::~Tcp4Socket()
{
    delete (static_cast<Socket*>(_impl));
}

void Tcp4Socket::open()
{
}

void Tcp4Socket::close()
{
}

void Tcp4Socket::send(BYTE *buffer, size_t size)
{
}

size_t Tcp4Socket::receive(BYTE *buffer, size_t size)
{
    return 0;
}

NAMESPACE_IRBIS_END
