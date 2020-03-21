// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <random>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Актуализация всех неактуализированных записей в указанной базе данных (если таковые имеются).
/// \param databaseName Имя базы данных.
/// \return Признак успешного выполнения операции.
bool Connection::actualizeDatabase (const String &databaseName)
{
    return this->actualizeRecord (databaseName, 0);
}

/// \brief Актуализация на сервере записи по её MFN в базе данных с заданным именем.
/// \param databaseName Имя базы данных.
/// \param mfn MFN записи, подлежащей актуализации.
/// \return Признак успешности выполения операции.
///
/// Если запись уже актуализирована, это не считается ошибкой.
bool Connection::actualizeRecord (const String &databaseName, int mfn)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "F");
    query.addAnsi (databaseName).newLine()
            .add(mfn);

    return this->execute (query);
}

// /// \brief Асинхронный вариант connect.
// /// \return Признак успешности выполнения операции.
//std::future<bool> Connection::connectAsync()
//{
//    return std::async(std::launch::async, &Connection::connect, this);
//}

// /// \brief Асинхронный вариант disconnect.
//std::future<void> Connection::disconnectAsync()
//{
//    return std::async(std::launch::async, &Connection::disconnect, this);
//}

// /// \brief Асинхронный вариант execute.
// /// \param query Клиентский запрос.
// /// \return Признак успешного выполнения запроса.
//std::future<bool> Connection::executeAsync(ClientQuery &query)
//{
//    return std::async(std::launch::async, &Connection::execute, this, std::ref(query));
//}

/// \brief Форматирование записи на сервере по её MFN.
/// \param format Спецификация формата.
/// \param mfn MFN записи, подлежащей расформатированию.
/// \return Результат расформатирования.
String Connection::formatRecord (const String &format, Mfn mfn)
{
    if (!this->_checkConnection()) {
        return L"";
    }

    const auto prepared = prepareFormat (format);
    ClientQuery query (*this, "G");
    query.addAnsi (this->database).newLine()
            .addAnsi (prepared).newLine()
            .add (1).newLine()
            .add (mfn);

    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return L"";
    }

    const auto result = response.readRemainingUtfText();
    return result;
}

/// \brief Форматирование записи на сервере по её MFN.
/// \param format Спецификация формата.
/// \param mfn MFN записи, подлежащей расформатированию.
/// \return Результат расформатирования.
std::string Connection::formatRecordLite (const std::string &format, Mfn mfn)
{
    if (!this->_checkConnection()) {
        return std::string();
    }

    // TODO prepare format
    // const auto prepared = prepareFormat (format);
    ClientQuery query (*this, "G");
    query.addAnsi (this->database).newLine()
            .addAnsi ("!").addAnsi (format).newLine()
            .add (1).newLine()
            .add (mfn);

    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return "";
    }

    const auto result = response.getRemaining();
    return result;
}

/// \brief Форматирование виртуальной записи (записи в клиентском представлении).
/// \param format Спецификация формата.
/// \param record Запись, подлежащая расформатированию.
/// \return Результат расформатирования.
String Connection::formatRecord (const String &format, const MarcRecord &record)
{
    if (!this->_checkConnection()) {
        return L"";
    }

    const auto prepared = prepareFormat (format);
    ClientQuery query (*this, "G");
    query.addAnsi (this->database).newLine()
            .addAnsi (prepared).newLine()
            .add (-2).newLine()
            .add (record, Text::UnixDelimiter);

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
DatabaseInfo Connection::getDatabaseInfo (const String &databaseName)
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
    result = UserInfo::parse (lines);

    return result;
}

/// \brief Выполнение глобальной корректировки на сервере.
/// \param settings Настройки глобальной корректировки.
/// \return Результат выполнения глобальной корректировки.
GblResult Connection::globalCorrection (const GblSettings &settings)
{
    GblResult result;

    if (!this->_checkConnection()) {
        return result;
    }

    // TODO implement
    const auto db = choose (settings.database, this->database);  // NOLINT(performance-unnecessary-copy-initialization)
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

// /// \brief Асинхронный вариант noOp.
// /// \return Признак успешности выполнения операции.
//std::future<bool> Connection::noOpAsync()
//{
//    return std::async(std::launch::async, &Connection::noOp, this);
//}

/// \brief Расформатирование таблицы в RTF.
/// \param definition Определение таблицы.
/// \return Результат расформатирования.
String Connection::printTable (const TableDefinition &definition)
{
    if (!this->_checkConnection()) {
        return L"";
    }

    ClientQuery query (*this, "L");
    query.addAnsi (definition.database).newLine()
            .addAnsi (definition.table).newLine()
            .addAnsi ("").newLine() // instead of headers
            .addAnsi (definition.mode).newLine()
            .addUtf (definition.searchQuery).newLine()
            .add (definition.minMfn).newLine()
            .add (definition.maxMfn).newLine()
            .addUtf (definition.sequentialQuery).newLine()
            .addAnsi (""); // instead of MFN list

    ServerResponse response(*this, query);
    if (!response.success()) {
        return L"";
    }
    const auto result = response.readRemainingUtfText();
    return result;
}

RawRecord Connection::readRawRecord (Mfn mfn)
{
    RawRecord result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "C");
    query.addAnsi(this->database).newLine()
            .add(mfn);

    ServerResponse response(*this, query);
    if (response.checkReturnCode (4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.parseSingle(lines);
        result.database = database;
    }

    return result;
}

std::vector<SearchScenario> Connection::readSearchScenario (const FileSpecification &specification)
{
    std::vector<SearchScenario> result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

String Connection::requireTextFile (const FileSpecification &specification)
{
    const auto result = readTextFile(specification);
    if (result.empty()) {
        throw FileNotFoundException(specification.toString());
    }

    return result;
}

bool Connection::updateIniFile (const StringList &lines)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "8");
    for (const auto &line : lines) {
        query.addAnsi (line).newLine();
    }

    return execute (query);
}

bool Connection::updateUserList (const std::vector<UserInfo> &users)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "+7");
    for (const auto &user : users) {
        query.addAnsi (user.toString()).newLine();
    }

    return this->execute(query);
}

bool Connection::writeRecords (std::vector<MarcRecord*> &records, bool lockFlag, bool actualize, bool dontParseResponse)
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
        const auto db = choose (record->database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
        query.addAnsi (db).addAnsi (Text::IrbisDelimiter)
            .addUtf (record->encode (Text::IrbisDelimiter)).newLine();
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
            record->database = choose (record->database, this->database);
            const auto recordLines = Text::fromFullDelimiter (line);
            record->decode(recordLines);
        }
    }

    return true;
}

bool Connection::writeRecords (std::vector<MarcRecord> &records, bool lockFlag, bool actualize, bool dontParseResponse)
{
    // TODO implement
    return false;
}

int Connection::writeRawRecord (RawRecord &record, bool lockFlag, bool actualize, bool dontParseResponse)
{
    if (!this->_checkConnection()) {
        return 0;
    }

    const auto db = choose (record.database, this->database);
    ClientQuery query (*this, "D");
    query.addAnsi (db).newLine();
    query.add (lockFlag).newLine();
    query.add (actualize).newLine();
    query.addUtf (record.encode(Text::IrbisDelimiter)).newLine();
    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return 0;
    }

    if (!dontParseResponse) {
        record.fields.clear();
        const auto temp1 = response.readRemainingUtfLines();
        if (temp1.size() > 1) {
            StringList lines;
            lines.push_back (temp1[0]);
            const auto temp2 = split (temp1[1], 0x1E);
            lines.insert (lines.end(), temp2.begin(), temp2.end());
            record.parseSingle (lines);
            record.database = this->database;
        }
    }

    return response.returnCode;
}

void Connection::writeTextFile (const FileSpecification &specification)
{
    ClientQuery query (*this, "L");
    query.add (specification);
    execute (query);
}

}
