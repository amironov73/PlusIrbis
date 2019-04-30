// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

Connection::Connection()
    : _connected(false),
    host(L"127.0.0.1"),
    port(6666),
    username(L""),
    password(L""),
    database(L"IBIS"),
    workstation(L"C"),
    clientId(0),
    queryId(0)
{
    socket = new Tcp4Socket();
}

bool Connection::connect()
{
    if (_connected)
    {
        return true;
    }

    clientId = 100000 + (rand() % 900000);
    queryId = 1;
    ClientQuery query (this, CommandCode::RegisterClient);
    query.addAnsi(username).newLine();
    query.addAnsi(password).newLine();
    ServerResponse response (*this, query);
    if (!response.success())
    {
        return false;
    }

    _connected = true;

    return true;
}

void Connection::disconnect()
{
    if (_connected)
    {
        _connected = false;
    }

    ClientQuery query(this, CommandCode::UnregisterClient);
    query.addAnsi(username).newLine();
    execute(query);
    _connected = false;
}

bool Connection::execute(ClientQuery &query)
{
    ServerResponse response(*this, query);
    return response.success();
}

int Connection::getMaxMfn(const std::wstring &databaseName)
{
    if (!connected())
    {
        return 0;
    }

    ClientQuery query (this, CommandCode::GetMaxMfn);
    query.addAnsi(databaseName);
    ServerResponse response(*this, query);
    if (!response.success())
    {
        return 0;
    }

    if (!response.checkReturnCode())
    {
        return 0;
    }

    const auto result = response.returnCode;

    return result;
}

bool Connection::noOp()
{
    if (!connected())
    {
        return false;
    }

    ClientQuery query(this, CommandCode::Nop);
    return execute(query);
}

NAMESPACE_IRBIS_END
