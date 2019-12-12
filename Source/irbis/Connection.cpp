// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <random>

namespace irbis {

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
    this->socket = new Tcp4Socket();
}

Connection::~Connection()
{
    delete this->socket;
}


bool Connection::actualizeRecord(const std::wstring &database, int mfn)
{
    if (!connected()) {
        return false;
    }

    ClientQuery query(*this, "F");
    query.addAnsi(database).newLine()
            .add(mfn);

    ServerResponse response (*this, query);
    return response.success();

}

bool Connection::connect()
{
    if (_connected)
    {
        return true;
    }

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(100000, 900000);

    clientId = dis(gen);
    queryId = 1;
    ClientQuery query (*this, "A");
    query.addAnsi(username).newLine();
    query.addAnsi(password).newLine();
    ServerResponse response (*this, query);
    if (!response.success()) {
        return false;
    }

    _connected = true;

    return true;
}

bool Connection::createDatabase(const std::wstring &databaseName, const std::wstring &description, bool readerAccess)
{
    if (!connected()) {
        return false;
    }

    ClientQuery query (*this, "T");
    query.addAnsi(databaseName).newLine()
            .addAnsi(description).newLine()
            .add(readerAccess);

    ServerResponse response (*this, query);
    return response.success();
}

bool Connection::createDictionary(const std::wstring &databaseName)
{
    if (!connected()) {
        return false;
    }

    ClientQuery query (*this, "Z");
    query.addAnsi(databaseName);

    ServerResponse response (*this, query);
    return response.success();
}

bool Connection::deleteDatabase(const std::wstring &databaseName)
{
    if (!connected()) {
        return false;
    }

    ClientQuery query (*this, "Z");
    query.addAnsi(databaseName);

    ServerResponse response (*this, query);
    return response.success();
}

bool Connection::deleteRecord(int mfn)
{
    if (!connected()) {
        return false;
    }

    MarcRecord record = readRecord(mfn);
    if (!record.deleted()) {
        record.status |= RecordStatus::LogicallyDeleted;
        writeRecord(record, false, true, true);
    }

    return true;
}

void Connection::disconnect()
{
    if (!_connected) {
        return;
    }

    ClientQuery query(*this, "B");
    query.addAnsi(username).newLine();
    execute(query);
    _connected = false;
}

bool Connection::execute(ClientQuery &query)
{
    ServerResponse response(*this, query);
    return response.success();
}

std::wstring Connection::formatRecord(const std::wstring &format, int mfn)
{
    if (!connected()) {
        return L"";
    }

    const auto prepared = prepareFormat(format);
    ClientQuery query (*this, "G");
    query.addAnsi(database).newLine()
            .addAnsi(prepared).newLine()
            .add(1).newLine()
            .add(mfn);

    ServerResponse response (*this, query);
    if (!response.success()) {
        return L"";
    }

    response.getReturnCode();
    const auto result = response.readRemainingUtfText();
    return result;
}

std::wstring Connection::formatRecord(const std::wstring &format, MarcRecord &record)
{
    if (!connected()) {
        return L"";
    }

    auto prepared = prepareFormat(format);
    ClientQuery query (*this, "G");
    query.addAnsi(database).newLine()
            .addAnsi(prepared).newLine()
            .add(-2).newLine()
            .add(record);

    ServerResponse response (*this, query);
    if (!response.success()) {
        return L"";
    }

    const auto result = response.readRemainingUtfText();
    return result;
}

DatabaseInfo Connection::getDatabaseInfo(const std::wstring &databaseName)
{
    DatabaseInfo result;

    if (!connected()) {
        return result;
    }

    return result;
}


int Connection::getMaxMfn(const std::wstring &databaseName)
{
    if (!connected()) {
        return 0;
    }

    ClientQuery query (*this, "O");
    query.addAnsi(databaseName);
    ServerResponse response(*this, query);
    if (!response.success())  {
        return 0;
    }

    if (!response.checkReturnCode())
    {
        return 0;
    }

    const auto result = response.returnCode;
    return result;
}

ServerStat Connection::getServerStat()
{
    ServerStat result;

    if (!connected()) {
        return result;
    }

    return result;
}

Version Connection::getServerVersion()
{
    Version result;

    if (!connected()) {
        return result;
    }

    ClientQuery query(*this, "1");
    ServerResponse response (*this, query);
    response.checkReturnCode();
    result.parse(response);

    return result;
}

std::vector<DatabaseInfo> Connection::listDatabases(const IniFile &iniFile, const std::string &defaultFileName)
{
    std::vector<DatabaseInfo> result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<DatabaseInfo> Connection::listDatabases(const FileSpecification &specification)
{
    std::vector<DatabaseInfo> result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<std::wstring> Connection::listFiles(const FileSpecification &specification)
{
    std::vector<std::wstring> result;

    if (!connected()) {
        return result;
    }

    ClientQuery query(*this, "!");
    query.add(specification);

    ServerResponse response (*this, query);
    std::vector<std::wstring> lines = response.readRemainingAnsiLines();
    for (auto line : lines) {
        //std::vector<std::wstring> converted = IrbisText::fromFullDelimiter(line);
        //result.append(converted);
    }

    return result;
}

std::vector<std::wstring> Connection::listFiles(const std::vector<FileSpecification> &specifications)
{
    std::vector<std::wstring> result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<ProcessInfo> Connection::listProcesses()
{
    std::vector<ProcessInfo> result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<UserInfo> Connection::getUserList()
{
    std::vector<UserInfo> result;

    if (!connected()) {
        return result;

    }

    return result;
}

bool Connection::noOp()
{
    if (!connected())
    {
        return false;
    }

    ClientQuery query(*this, "N");
    return execute(query);
}

void Connection::parseConnectionString(const std::wstring &connectionString)
{
    throw NotImplementedException();
}

std::wstring Connection::popDatabase()
{
    throw NotImplementedException();
}

std::wstring Connection::printTable(const TableDefinition &definition)
{
    throw NotImplementedException();
}

std::wstring Connection::pushDatabase(const std::wstring &newDatabase)
{
    throw NotImplementedException();
}

std::vector<BYTE> Connection::readBinaryFile(const FileSpecification &specification)
{
    std::vector<BYTE> result;

    if (!connected()) {
        return result;
    }

    return result;
}

IniFile Connection::readIniFile(const FileSpecification &specification)
{
    IniFile result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<TermPosting> Connection::readPostings(const PostingParameters &parameters)
{
    std::vector<TermPosting> result;

    if (!connected()) {
        return result;
    }

    return result;
}

RawRecord Connection::readRawRecord(int mfn)
{
    RawRecord result;

    if (!connected()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(database).newLine()
        .add(mfn);

    ServerResponse response (*this, query);
    if (response.checkReturnCode()) {
        const auto lines = response.readRemainingUtfLines();
        result.parseSingle(lines);
        result.database = database;
    }

    return result;
}

MarcRecord Connection::readRecord(int mfn)
{
    MarcRecord result;

    if (!connected()) {
        return result;
    }

    return result;
}

MarcRecord Connection::readRecord(const std::wstring &databaseName, int mfn)
{
    MarcRecord result;

    if (!connected()) {
        return result;
    }

    return result;
}

MarcRecord Connection::readRecord(const std::wstring &databaseName, int mfn, int version)
{
    MarcRecord result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<SearchScenario> Connection::readSearchScenario(const FileSpecification &specification)
{
    std::vector<SearchScenario> result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::vector<TermInfo> Connection::readTerms(const TermParameters &parameters)
{
    std::vector<TermInfo> result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::wstring Connection::readTextFile(const FileSpecification &specification)
{
    if (!connected()) {
        return L"";
    }

    return L"";
}

StringList Connection::readTextFiles(std::vector<FileSpecification> specifications)
{
    StringList result;

    if (!connected()) {
        return result;
    }

    return result;
}

bool Connection::reloadDictionary(const std::wstring &databaseName)
{
    if (!connected()) {
        return false;
    }

    return false;
}

bool Connection::reloadMasterFile(const std::wstring &databaseName)
{
    if (!connected()) {
        return false;
    }

    return false;
}

bool Connection::restartServer()
{
    if (!connected()) {
        return false;
    }

    return false;
}

std::wstring Connection::requireTextFile(const FileSpecification &specification)
{
    throw NotImplementedException();
}

MfnList Connection::search(const std::wstring &expression)
{
    MfnList result;

    if (!connected()) {
        return result;
    }

    return result;
}

MfnList Connection::search(const SearchParameters &parameters)
{
    MfnList result;

    if (!connected()) {
        return result;
    }

    return result;
}

std::wstring Connection::toConnectionString()
{
    throw NotImplementedException();
}

bool Connection::truncateDatabase(const std::wstring &databaseName)
{
    if (!connected()) {
        return false;
    }

    return false;
}

bool Connection::unlockDatabase(const std::wstring &databaseName)
{
    if (!connected()) {
        return false;
    }

    return false;
}

bool Connection::unlockRecords(const std::wstring &databaseName, const std::vector<int> &mfnList)
{
    if (!connected()) {
        return false;
    }

    return false;
}

bool Connection::updateIniFile(std::vector<std::wstring> &lines)
{
    if (!connected()) {
        return false;
    }

    return false;
}

int Connection::writeRecord(MarcRecord &record, bool lockFlag, bool actualize, bool dontParseResponse)
{
    if (!connected()) {
        return 0;
    }

    return 0;
}

void Connection::writeTextFile(const FileSpecification &specification)
{
    throw NotImplementedException();
}

}
