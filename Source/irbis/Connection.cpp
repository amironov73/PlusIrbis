// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <random>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

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
          queryId(0),
          lastError(0) {
    this->socket = new Tcp4Socket();
}

Connection::~Connection() {
    this->disconnect();
    delete this->socket;
}

bool Connection::_checkConnection()
{
    if (!this->connected()) {
        this->lastError = -100003;
        return false;
    }
    // this->lastError = 0;
    return true;
}

bool Connection::actualizeDatabase(const std::wstring &databaseName)
{
    return this->actualizeRecord(databaseName, 0);
}

bool Connection::actualizeRecord(const std::wstring &databaseName, int mfn)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "F");
    query.addAnsi(databaseName).newLine()
            .add(mfn);

    return this->execute(query);
}

bool Connection::connect()
{
    if (this->connected()) {
        return true;
    }

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(100000, 900000);

    this->clientId = dis(gen);
    this->queryId = 1;
    ClientQuery query(*this, "A");
    query.addAnsi(this->username).newLine()
            .addAnsi(this->password).newLine();
    ServerResponse response(*this, query);
    if (!response.success()) {
        return false;
    }

    this->_connected = true;
    return true;
}

bool Connection::createDatabase(const std::wstring &databaseName, const std::wstring &description, bool readerAccess)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "T");
    query.addAnsi(databaseName).newLine()
            .addAnsi(description).newLine()
            .add(readerAccess);

    return this->execute(query);
}

bool Connection::createDictionary(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "Z");
    query.addAnsi(databaseName);
    return this->execute(query);
}

bool Connection::deleteDatabase(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "Z");
    query.addAnsi(databaseName);
    return this->execute(query);
}

bool Connection::deleteRecord(int mfn)
{
    if (!this->_checkConnection()) {
        return false;
    }

    auto record = readRecord(mfn);
    if (!record.deleted()) {
        record.status |= RecordStatus::LogicallyDeleted;
        this->writeRecord(record, false, true, true);
    }

    return true;
}

void Connection::disconnect()
{
    if (!this->connected()) {
        return;
    }

    ClientQuery query(*this, "B");
    query.addAnsi(username).newLine();
    this->execute(query);
    _connected = false;
}

bool Connection::execute(ClientQuery &query)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ServerResponse response(*this, query);
    return response.checkReturnCode();
}

std::wstring Connection::formatRecord(const std::wstring &format, int mfn)
{
    if (!this->_checkConnection()) {
        return L"";
    }

    const auto prepared = prepareFormat(format);
    ClientQuery query(*this, "G");
    query.addAnsi(this->database).newLine()
            .addAnsi(prepared).newLine()
            .add(1).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (!response.checkReturnCode()) {
        return L"";
    }

    const auto result = response.readRemainingUtfText();
    return result;
}

std::wstring Connection::formatRecord(const std::wstring &format, const MarcRecord &record)
{
    if (!this->_checkConnection()) {
        return L"";
    }

    const auto prepared = prepareFormat(format);
    ClientQuery query(*this, "G");
    query.addAnsi(this->database).newLine()
            .addAnsi(prepared).newLine()
            .add(-2).newLine()
            .add(record, IrbisText::UnixDelimiter);

    ServerResponse response(*this, query);
    if (!response.checkReturnCode()) {
        return L"";
    }

    const auto result = response.readRemainingUtfText();
    return result;
}

DatabaseInfo Connection::getDatabaseInfo(const std::wstring &databaseName)
{
    DatabaseInfo result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "0");
    query.addAnsi(databaseName);
    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }

    result.parse(response);
    return result;
}


int Connection::getMaxMfn(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return 0;
    }

    ClientQuery query(*this, "O");
    query.addAnsi(databaseName);
    ServerResponse response(*this, query);
    if (!response.checkReturnCode()) {
        return 0;
    }

    const auto result = response.returnCode;
    return result;
}

ServerStat Connection::getServerStat()
{
    ServerStat result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "+1");
    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }

    result.parse(response);
    return result;
}

Version Connection::getServerVersion()
{
    Version result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "1");
    ServerResponse response(*this, query);
    response.checkReturnCode();
    result.parse(response);

    return result;
}

std::vector<UserInfo> Connection::getUserList()
{
    std::vector<UserInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

GblResult Connection::globalCorrection(const GblSettings &settings)
{
    GblResult result;

    if (!this->_checkConnection()) {
        return result;
    }

    // TODO implement
    const auto db = iif (settings.database, this->database);  // NOLINT(performance-unnecessary-copy-initialization)
    ClientQuery query (*this, "5");
    query.addAnsi(db).newLine();

    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }

    const auto lines = response.readRemainingAnsiLines();
    result.parse(lines);

    return result;
}

std::vector<DatabaseInfo> Connection::listDatabases(const IniFile &iniFile, const std::string &defaultFileName)
{
    std::vector<DatabaseInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

std::vector<DatabaseInfo> Connection::listDatabases(const FileSpecification &specification)
{
    std::vector<DatabaseInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto menu = this->readMenuFile(specification);
    if (menu.entries.empty()) {
        return result;
    }

    result = DatabaseInfo::parse(menu);

    return result;
}

StringList Connection::listFiles(const FileSpecification &specification)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "!");
    query.add(specification);

    ServerResponse response(*this, query);
    const auto lines = response.readRemainingAnsiLines();
    for (const auto &line : lines) {
        auto converted = IrbisText::fromFullDelimiter(line);
        for (const auto &item : converted) {
            if (!item.empty()) {
                result.push_back(item);
            }
        }
    }

    return result;
}

StringList Connection::listFiles(const std::vector<FileSpecification> &specifications)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "!");
    for (const auto &specification : specifications) {
        query.add(specification);
    }

    ServerResponse response(*this, query);
    const auto lines = response.readRemainingAnsiLines();
    for (const auto &line : lines) {
        auto converted = IrbisText::fromFullDelimiter(line);
        for (const auto &item : converted) {
            if (!item.empty()) {
                result.push_back(item);
            }
        }
    }

    return result;
}

std::vector<ProcessInfo> Connection::listProcesses()
{
    std::vector<ProcessInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "+3");
    ServerResponse response (*this, query);
    result = ProcessInfo::parse(response);

    return result;
}

StringList Connection::listTerms(const std::wstring &prefix)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto prefixSize = prefix.size();
    std::wstring startTerm = prefix;
    std::wstring lastTerm = startTerm;
    while (true) {
        const auto terms = this->readTerms(startTerm, 512);
        if (terms.empty()) {
            break;
        }

        for (const auto &term : terms) {
            std::wstring text = term.text;
            const auto begin = text.substr(0, prefixSize);
            if (begin != prefix) {
                goto DONE;
            }
            if (text != startTerm) {
                lastTerm = text;
                text = text.substr(prefixSize);
                result.push_back(text);
            }
        }
        startTerm = lastTerm;
    }

    DONE:
    return result;
}

bool Connection::noOp() {
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "N");
    return this->execute(query);
}

void Connection::parseConnectionString(const std::wstring &connectionString) {
    const auto items = split(connectionString, L";");
    for (auto &item : items) {
        auto parts = maxSplit(item, '=', 2);
        if (parts.size() != 2) {
            throw IrbisException();
        }
        auto name = toLower(parts[0]);
        const auto value = parts[1];
        if (name.empty() || value.empty()) {
            throw IrbisException();
        }

        if (name == L"host" || name == L"server" || name == L"address") {
            this->host = value;
        } else if (name == L"port") {
            this->port = (short) fastParse32(value);
        } else if (name == L"user" || name == L"username"
                   || name == L"name" || name == L"login") {
            this->username = value;
        } else if (name == L"pwd" || name == L"password") {
            this->password = value;
        } else if (name == L"db" || name == L"catalog" || name == L"database") {
            this->database = value;
        } else if (name == L"arm" || name == L"workstation") {
            this->workstation = value.at(0);
        } else {
            throw IrbisException();
        }
    }
}

std::wstring Connection::popDatabase()
{
    const auto result = this->database;
    this->database = _databaseStack.back();
    _databaseStack.pop_back();

    return result;
}

std::wstring Connection::printTable(const TableDefinition &definition)
{
    if (!this->_checkConnection()) {
        return L"";
    }

    ClientQuery query(*this, "L");
    query.addAnsi(definition.database).newLine()
            .addAnsi(definition.table).newLine()
            .addAnsi("").newLine() // instead of headers
            .addAnsi(definition.mode).newLine()
            .addUtf(definition.searchQuery).newLine()
            .add(definition.minMfn).newLine()
            .add(definition.maxMfn).newLine()
            .addUtf(definition.sequentialQuery).newLine()
            .addAnsi(""); // instead of MFN list

    ServerResponse response(*this, query);
    const auto result = response.readRemainingUtfText();
    return result;
}

std::wstring Connection::pushDatabase(const std::wstring &newDatabase)
{
    const auto result = this->database;
    _databaseStack.push_back(newDatabase);
    this->database = newDatabase;

    return result;
}

std::vector<BYTE> Connection::readBinaryFile(const FileSpecification &specification)
{
    std::vector<BYTE> result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

IniFile Connection::readIniFile(const FileSpecification &specification)
{
    const auto lines = this->readTextLines(specification);
    IniFile result;
    result.parse(lines);

    return result;
}

MenuFile Connection::readMenuFile(const FileSpecification &specification)
{
    const auto lines = this->readTextLines(specification);
    MenuFile result;
    result.parse(lines);

    return result;
}

std::vector<TermPosting> Connection::readPostings(const PostingParameters &parameters)
{
    std::vector<TermPosting> result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto db = iif(parameters.database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
    ClientQuery query (*this, "I");
    query.addAnsi(db).newLine();
    query.add(parameters.numberOfPostings).newLine();
    query.add(parameters.firstPosting).newLine();
    query.addFormat(parameters.format);
    if (!parameters.listOfTerms.empty()) {
        query.addUtf(parameters.term).newLine();
    } else {
        for (const auto &term : parameters.listOfTerms) {
            query.addUtf(term).newLine();
        }
    }

    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }

    const auto lines = response.readRemainingUtfLines();
    result = TermPosting::parse(lines);

    return result;
}

RawRecord Connection::readRawRecord(int mfn)
{
    RawRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(this->database).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
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

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(this->database).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (response.checkReturnCode()) {
        const auto lines = response.readRemainingUtfLines();
        result.decode(lines);
        result.database = this->database;
    }

    return result;
}

MarcRecord Connection::readRecord(const std::wstring &databaseName, int mfn)
{
    MarcRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(databaseName).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (response.checkReturnCode()) {
        const auto lines = response.readRemainingUtfLines();
        result.decode(lines);
        result.database = this->database;
    }

    return result;
}

MarcRecord Connection::readRecord(const std::wstring &databaseName, int mfn, int version)
{
    MarcRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

std::vector<SearchScenario> Connection::readSearchScenario(const FileSpecification &specification)
{
    std::vector<SearchScenario> result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

std::vector<TermInfo> Connection::readTerms(const std::wstring &startTerm, int numberOfTerms = 100)
{
    auto parameters = TermParameters();
    parameters.startTerm = startTerm;
    parameters.numberOfTerms = numberOfTerms;

    return this->readTerms(parameters);
}

std::vector<TermInfo> Connection::readTerms(const TermParameters &parameters)
{
    std::vector<TermInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    const std::string command = parameters.reverseOrder ? "P" : "H";
    const auto db = iif(parameters.database, this->database);
    ClientQuery query (*this, command);
    query.addAnsi(db).newLine();
    query.addUtf(parameters.startTerm).newLine();
    query.add(parameters.numberOfTerms).newLine();
    query.addFormat(parameters.format);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }
    //if (!$response || !$response->checkReturnCode(codes_for_read_terms()))
    //    return false;

    const auto lines = response.readRemainingUtfLines();
    result = TermInfo::parse(lines);

    return result;
}

std::wstring Connection::readTextFile(const FileSpecification &specification)
{
    std::wstring result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    query.add(specification);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    result = response.readAnsi();
    result = IrbisText::fromIrbisToDos(result);

    return result;
}

StringList Connection::readTextFiles(std::vector<FileSpecification> specifications)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    for (const auto &specification : specifications) {
        query.add(specification).newLine();
    }

    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    const auto lines = response.readRemainingAnsiLines();
    for (std::wstring line : lines) {
        result.push_back(IrbisText::fromIrbisToDos(line));
    }

    return result;
}

StringList Connection::readTextLines(const FileSpecification &specification)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    query.add(specification);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    const auto text = response.readAnsi();
    result = IrbisText::fromFullDelimiter(text);

    return result;
}

bool Connection::reloadDictionary(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "Y");
    query.addAnsi(databaseName);
    return this->execute(query);
}

bool Connection::reloadMasterFile(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "X");
    query.addAnsi(databaseName);
    return this->execute(query);
}

bool Connection::restartServer()
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "+8");
    return this->execute(query);
}

std::wstring Connection::requireTextFile(const FileSpecification &specification)
{
    const auto result = readTextFile(specification);
    if (result.empty()) {
        throw FileNotFoundException(specification.toString());
    }

    return result;
}

MfnList Connection::search(const std::wstring &expression)
{
    SearchParameters parameters;
    parameters.database = database;
    parameters.searchExpression = expression;
    parameters.numberOfRecords = 0;
    parameters.firstRecord = 1;

    return this->search(parameters);
}

MfnList Connection::search(const SearchParameters &parameters)
{
    MfnList result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto &databaseName = iif(parameters.database, database);
    ClientQuery query (*this, "K");
    query.addAnsi(databaseName).newLine()
            .addUtf(parameters.searchExpression).newLine()
            .add(parameters.numberOfRecords).newLine()
            .add(parameters.firstRecord).newLine()
            .addAnsi(parameters.formatSpecification).newLine()
            .add(parameters.minMfn).newLine()
            .add(parameters.maxMfn).newLine()
            .addAnsi(parameters.sequentialSpecification);

    ServerResponse response (*this, query);
    response.checkReturnCode();
    const auto expected = response.readInteger();
    const auto batchSize = std::min(expected, 32000); // MAXPACKET
    for (auto i = 0; i < batchSize; i++) {
        const auto line = response.readAnsi();
        StringList parts = maxSplit(line, L'#', 2);
        const auto mfn = fastParse32(parts[0]);
        result.push_back(mfn);
    }

    return result;
}

std::wstring Connection::toConnectionString() const
{
    return std::wstring(L"host=") + host
           + L";port=" + std::to_wstring(port)
           + L";username=" + username
           + L";password=" + password
           + L";database" + database
           + L";arm=" + workstation
           + L";";
}

bool Connection::truncateDatabase(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "T");
    query.addAnsi(databaseName);
    return execute(query);
}

bool Connection::unlockDatabase(const std::wstring &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "U");
    query.addAnsi(databaseName);
    return execute(query);
}

bool Connection::unlockRecords(const std::wstring &databaseName, const MfnList &mfnList)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "Q");
    query.addAnsi(databaseName).newLine();
    for (const auto mfn : mfnList) {
        query.add(mfn).newLine();
    }

    return execute(query);
}

bool Connection::updateIniFile(StringList &lines)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "8");
    for (const auto &line : lines) {
        query.addAnsi(line).newLine();
    }

    return execute(query);
}

int Connection::writeRecord(MarcRecord &record, bool lockFlag, bool actualize, bool dontParseResponse)
{
    if (!this->_checkConnection()) {
        return 0;
    }

    const auto db = iif(record.database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
    ClientQuery query(*this, "D");
    query.addAnsi(db).newLine();
    query.add(lockFlag).newLine();
    query.add(actualize).newLine();
    query.addUtf(record.encode(IrbisText::IrbisDelimiter)).newLine();
    ServerResponse response(*this, query);
    if (!response.checkReturnCode()) {
        return 0;
    }

    if (!dontParseResponse) {
        record.fields.clear();
        const auto temp1 = response.readRemainingUtfLines();
        if (temp1.size() > 1) {
            StringList lines;
            lines.push_back(temp1[0]);
            const auto temp2 = split(temp1[1], 0x1E);
            lines.insert(lines.end(), temp2.begin(), temp2.end());
            record.decode(lines);
            record.database = this->database;
        }
    }

    return response.returnCode;
}

int Connection::writeRawRecord(RawRecord &record, bool lockFlag, bool actualize, bool dontParseResponse)
{
    if (!this->_checkConnection()) {
        return 0;
    }

    const auto db = iif(record.database, this->database);
    ClientQuery query(*this, "D");
    query.addAnsi(db).newLine();
    query.add(lockFlag).newLine();
    query.add(actualize).newLine();
    query.addUtf(record.encode(IrbisText::IrbisDelimiter)).newLine();
    ServerResponse response(*this, query);
    if (!response.checkReturnCode()) {
        return 0;
    }

    if (!dontParseResponse) {
        record.fields.clear();
        const auto temp1 = response.readRemainingUtfLines();
        if (temp1.size() > 1) {
            StringList lines;
            lines.push_back(temp1[0]);
            const auto temp2 = split(temp1[1], 0x1E);
            lines.insert(lines.end(), temp2.begin(), temp2.end());
            record.parseSingle(lines);
            record.database = this->database;
        }
    }

    return response.returnCode;
}

void Connection::writeTextFile(const FileSpecification &specification)
{
    ClientQuery query(*this, "L");
    query.add(specification);
    execute(query);
}

}
