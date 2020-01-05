// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <random>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Единственный конструктор для данного класса.
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

/// \brief Деструктор для данного класса.
///
/// Если необходимо, выполняет отключение от сервера.
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

/// \brief Актуализация всех неактуализированных записей в указанной базе данных (если таковые имеются).
/// \param databaseName Имя базы данных.
/// \return Признак успешного выполнения операции.
bool Connection::actualizeDatabase(const String &databaseName)
{
    return this->actualizeRecord(databaseName, 0);
}

/// \brief Актуализация на сервере записи по её MFN в базе данных с заданным именем.
/// \param databaseName Имя базы данных.
/// \param mfn MFN записи, подлежащей актуализации.
/// \return Признак успешности выполения операции.
///
/// Если запись уже актуализирована, это не считается ошибкой.
bool Connection::actualizeRecord(const String &databaseName, int mfn)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "F");
    query.addAnsi(databaseName).newLine()
            .add(mfn);

    return this->execute(query);
}

/// \brief Подключение к серверу.
/// \return Признак успешности выполнения операции.
/// \warning Подключение некоторыми типами клиентов увеличивает на сервере счетчик использованных лицензий!
///
/// Повторные попытки подключения с помощью того же объекта Connection игнорируются.
/// При этом возвращается true.
bool Connection::connect()
{
    if (this->connected()) {
        return true;
    }

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(100000, 900000);

    AGAIN: this->clientId = dis(gen);
    this->queryId = 1;
    ClientQuery query(*this, "A");
    query.addAnsi(this->username).newLine()
            .addAnsi(this->password);
    ServerResponse response(*this, query);
    if (!response.success()) {
        return false;
    }

    response.getReturnCode();
    if (response.returnCode == -3337) {
        // клиент с данным идентификатором уже зарегистрирован
        goto AGAIN;
    }

    if (response.returnCode < 0) {
        return false;
    }

    this->_connected = true;
    this->serverVersion = response.serverVersion;
    this->interval = response.readInteger();
    const auto lines = response.readRemainingAnsiLines();
    this->iniFile.parse(lines);
    return true;
}

/// \brief Создание на сервере базы данных с указанным именем.
/// \param databaseName Имя создаваемой базы данных.
/// \param description Описание базы данных в произвольной форме.
/// \param readerAccess Созданная база должна быть доступна АРМ "Читатель"?
/// \return Признак успешного выполнения операции.
/// \warning Операция доступна лишь администратору.
bool Connection::createDatabase(const String &databaseName, const String &description, bool readerAccess)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "T");
    query.addAnsi(databaseName).newLine()
            .addAnsi(description).newLine()
            .add(readerAccess).newLine();

    return this->execute(query);
}

/// \brief Создание на сервере словаря в базе данных с заданным именем.
/// \param databaseName Имя базы данных.
/// \return Признак успешного выполенения операции.
/// \warning Операция доступна лишь администратору.
/// \warning Операция может занять много времени.
bool Connection::createDictionary(const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "Z");
    query.addAnsi(databaseName).newLine();
    return this->execute(query);
}

/// \brief Удаление на сервере базы данных с заданным именем.
/// \param databaseName Имя базы данных, подлежащей удалению.
/// \return Признак успешного выполнения операции.
/// \warning Операция доступна лишь администратору.
bool Connection::deleteDatabase(const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "Z");
    query.addAnsi(databaseName).newLine();
    return this->execute(query);
}

/// \brief Удаление на сервере в текущей базе данных записи по её MFN.
/// \param mfn MNF записи, подлежащей удалению.
/// \return Признак успешности выполнения операции.
///
/// Если запись уже удалена, это не считается ошибкой.
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

/// \brief Отключение от сервера.
///
/// Повторные попытки отключения уже отключенного экземпляра клиента игнорируются.
/// Если при подключении был увеличен счётчик использованных лицензий, он соответственно уменьшается.
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

/// \brief Результат исполнения запроса на сервере.
/// \param query Полностью сформированный клиентский запрос.
/// \return Признак успешного выполнения запроса.
/// \warning Должно быть установлено подключение к серверу!
///
/// Если метод вернул false, конкретный код ошибки находится в lastError.
bool Connection::execute(ClientQuery &query)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ServerResponse response(*this, query);
    return response.checkReturnCode();
}

/// \brief Форматирование записи на сервере по её MFN.
/// \param format Спецификация формата.
/// \param mfn MFN записи, подлежащей расформатированию.
/// \return Результат расформатирования.
String Connection::formatRecord(const String &format, Mfn mfn)
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

/// \brief Форматирование виртуальной записи (записи в клиентском представлении).
/// \param format Спецификация формата.
/// \param record Запись, подлежащая расформатированию.
/// \return Результат расформатирования.
String Connection::formatRecord(const String &format, const MarcRecord &record)
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

/// \brief Получение информации о базе данных с указанным именем.
/// \param databaseName Имя базы данных.
/// \return Информация о базе данных.
DatabaseInfo Connection::getDatabaseInfo(const String &databaseName)
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

/// \brief Получение максимального MFN для базы данных с указанным именем.
/// \param databaseName Имя базы данных.
/// \return Максимальный MFN. 0 или отрицательное число означают ошибку.
Mfn Connection::getMaxMfn(const String &databaseName)
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

/// \brief Получение статистики работы сервера ИРБИС64.
/// \return Статистика работы сервера.
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

/// \brief Получение версии сервера ИРБИС64.
/// \return Версия сервера.
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

/// \brief Получение списка пользователей, имеющих (потенциальный) доступ к серверу ИРБИС64.
/// \return Список пользователей.
/// \warning Операция доступна лишь администратору.
std::vector<UserInfo> Connection::getUserList()
{
    std::vector<UserInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "+9");
    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }

    const auto lines = response.readRemainingAnsiLines();
    result = UserInfo::parse(lines);

    return result;
}

/// \brief Выполнение глобальной корректировки на сервере.
/// \param settings Настройки глобальной корректировки.
/// \return Результат выполнения глобальной корректировки.
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

std::vector<DatabaseInfo> Connection::listDatabases(const IniFile &theIni, const String &defaultFileName)
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

/// \brief Получение списка файлов на сервере.
/// \param specification Спецификация файла.
/// \return Список файлов.
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

/// \brief Получение списка файлов на сервере.
/// \param specifications Вектор спецификаций файлов.
/// \return Список файлов.
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

/// \brief Получение списка (рабочих) процессов, выполняемых в настоящий момент на сервере ИРБИС64.
/// \return Массив серверных процессов.
/// \warning Операция доступна только администратору.
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

/// \brief Получение списка терминов в текущей базе данных с указанным префиксом.
/// \param prefix Префикс, для которого строится список терминов (например, "T=")
/// \return Термины, очищенные от префикса.
/// \warning Для больших баз данных выполнение операции может потребовать
/// много времени и занять слишком много оперативной памяти.
StringList Connection::listTerms(const String &prefix)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto prefixSize = prefix.size();
    String startTerm = prefix;
    String lastTerm = startTerm;
    while (true) {
        const auto terms = this->readTerms(startTerm, 512);
        if (terms.empty()) {
            break;
        }

        for (const auto &term : terms) {
            String text = term.text;
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

/// \brief Пустая операция (не требует от сервера никаких действий).
/// \return Признак успешности выполнения операции.
///
/// Используется для подтверждения подключения клиента
/// при длительном бездействии пользователя.
bool Connection::noOp() {
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "N");
    return this->execute(query);
}

/// \brief Разбор строки подключения.
/// \param connectionString Строка подключения.
/// \throw IrbisException Ошибка в структуре строки подключения.
void Connection::parseConnectionString(const String &connectionString) {
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

/// \brief Возврат к предыдущей базе данных.
/// \return Имя базы данных, использовавшейся до возврата.
String Connection::popDatabase()
{
    const auto result = this->database;
    this->database = _databaseStack.back();
    _databaseStack.pop_back();

    return result;
}

/// \brief Расформатирование таблицы в RTF.
/// \param definition Определение таблицы.
/// \return Результат расформатирования.
String Connection::printTable(const TableDefinition &definition)
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
    if (!response.success()) {
        return L"";
    }
    const auto result = response.readRemainingUtfText();
    return result;
}

/// \brief Запоминание имени текущей базы данных для последующего возврата к ней.
/// \param newDatabase Имя базы данных, устанавливаемой в качестве текущей.
/// \return Имя базы данных, использовавшейся в качестве текущей.
String Connection::pushDatabase(const String &newDatabase)
{
    const auto result = this->database;
    _databaseStack.push_back(newDatabase);
    this->database = newDatabase;

    return result;
}

Bytes Connection::readBinaryFile(const FileSpecification &specification)
{
    Bytes result;

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

RawRecord Connection::readRawRecord(Mfn mfn)
{
    RawRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(this->database).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (response.checkReturnCode(4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.parseSingle(lines);
        result.database = database;
    }

    return result;
}

MarcRecord Connection::readRecord(Mfn mfn)
{
    MarcRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(this->database).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (response.checkReturnCode(4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.decode(lines);
        result.database = this->database;
    }

    return result;
}

MarcRecord Connection::readRecord(const String &databaseName, Mfn mfn)
{
    MarcRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(databaseName).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (response.checkReturnCode(4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.decode(lines);
        result.database = this->database;
    }

    return result;
}

MarcRecord Connection::readRecord(const String &databaseName, Mfn mfn, int version)
{
    MarcRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(databaseName).newLine()
            .add(mfn).newLine()
            .add(version);

    ServerResponse response(*this, query);
    if (response.checkReturnCode(4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.decode(lines);
        result.database = this->database;
    }

    if (version) {
        // TODO unlockRecord
        MfnList records { mfn };
        this->unlockRecords(databaseName, records);
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

std::vector<TermInfo> Connection::readTerms(const String &startTerm, int numberOfTerms = 100)
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
    if (!response.checkReturnCode(3, -202, -203, -204)) {
        return result;
    }

    const auto lines = response.readRemainingUtfLines();
    result = TermInfo::parse(lines);

    return result;
}

String Connection::readTextFile(const FileSpecification &specification)
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

bool Connection::reloadDictionary(const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "Y");
    query.addAnsi(databaseName);
    return this->execute(query);
}

bool Connection::reloadMasterFile(const String &databaseName)
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

String Connection::requireTextFile(const FileSpecification &specification)
{
    const auto result = readTextFile(specification);
    if (result.empty()) {
        throw FileNotFoundException(specification.toString());
    }

    return result;
}

MfnList Connection::search(const Search &search)
{
    SearchParameters parameters { this->database, search.toString() };
    parameters.numberOfRecords = 0;
    parameters.firstRecord = 1;

    return this->search(parameters);
}

MfnList Connection::search(const String &expression)
{
    SearchParameters parameters { expression, this->database };
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

    const auto &databaseName = iif(parameters.database, this->database);
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
    if (!response.checkReturnCode()) {
        return result;
    }
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

String Connection::toConnectionString() const
{
    return String(L"host=") + this->host
           + L";port=" + std::to_wstring(this->port)
           + L";username=" + this->username
           + L";password=" + this->password
           + L";database" + this->database
           + L";arm=" + this->workstation
           + L";";
}

bool Connection::truncateDatabase(const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "T");
    query.addAnsi(databaseName);
    return execute(query);
}

bool Connection::unlockDatabase(const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "U");
    query.addAnsi(databaseName);
    return execute(query);
}

bool Connection::unlockRecords(const String &databaseName, const MfnList &mfnList)
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

bool Connection::updateIniFile(const StringList &lines)
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

bool Connection::updateUserList(const std::vector<UserInfo> &users)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "+7");
    for (const auto &user : users) {
        query.addAnsi(user.toString()).newLine();
    }

    return this->execute(query);
}

int Connection::writeRecord(MarcRecord &record, bool lockFlag = false, bool actualize = true, bool dontParseResponse = false)
{
    if (!this->_checkConnection()) {
        return 0;
    }

    const auto db = iif (record.database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
    ClientQuery query (*this, "D");
    query.addAnsi (db).newLine();
    query.add (lockFlag).newLine();
    query.add (actualize).newLine();
    query.addUtf (record.encode(IrbisText::IrbisDelimiter)).newLine();
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

bool Connection::writeRecords(std::vector<MarcRecord*> &records, bool lockFlag, bool actualize, bool dontParseResponse)
{
    if (!this->_checkConnection()) {
        return false;
    }

    if (records.empty()) {
        return true;
    }

    if (records.size() == 1) {
        this->writeRecord (*records[0], lockFlag, actualize, dontParseResponse);
        return true;
    }

    ClientQuery query (*this, "6");
    query.add (lockFlag).newLine();
    query.add (actualize).newLine();
    for (const auto record : records) {
        const auto db = iif (record->database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
        query.addAnsi (db).addAnsi (IrbisText::IrbisDelimiter)
            .addUtf (record->encode(IrbisText::IrbisDelimiter)).newLine();
    }
    ServerResponse response (*this, query);
    if (!response.success()) {
        return false;
    }

    response.getReturnCode();

    if (!dontParseResponse) {
        const auto lines = response.readRemainingUtfLines();
        for (auto i = 0; i < lines.size(); i++) {
            const auto &line = lines[i];
            if (line.empty()) {
                continue;
            }

            auto record = records[i];
            record->fields.clear();
            record->database = iif (record->database, this->database);
            const auto recordLines = IrbisText::fromFullDelimiter(line);
            record->decode(recordLines);
        }
    }

    return true;
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
