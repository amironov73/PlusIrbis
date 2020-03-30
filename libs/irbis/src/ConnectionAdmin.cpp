// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <random>
#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Создание на сервере базы данных с указанным именем.
/// \param databaseName Имя создаваемой базы данных.
/// \param description Описание базы данных в произвольной форме.
/// \param readerAccess Созданная база должна быть доступна АРМ "Читатель"?
/// \return Признак успешного выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::createDatabase (const String &databaseName, const String &description, bool readerAccess)
{
    assert (!databaseName.empty());
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "T");
    query.addAnsi (databaseName).newLine()
            .addAnsi (description).newLine()
            .add (readerAccess).newLine();

    return this->execute (query);
}

/// \brief Создание на сервере словаря в базе данных с заданным именем.
/// \param databaseName Имя базы данных.
/// \return Признак успешного выполенения операции.
/// \warning Операция доступна лишь администратору.
/// \warning Операция может занять много времени.
bool ConnectionAdmin::createDictionary (const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query(*this, "Z");
    const auto db = choose (databaseName, this->database);
    query.addAnsi (db).newLine();
    return this->execute (query);
}

/// \brief Удаление на сервере базы данных с заданным именем.
/// \param databaseName Имя базы данных, подлежащей удалению.
/// \return Признак успешного выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::deleteDatabase (const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "Z");
    const auto db = choose (databaseName, this->database);
    query.addAnsi (db).newLine();
    return this->execute (query);
}

/// \brief Получение статистики работы сервера ИРБИС64.
/// \return Статистика работы сервера.
ServerStat ConnectionAdmin::getServerStat()
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

    result.parse (response);
    return result;
}

/// \brief Получение версии сервера ИРБИС64.
/// \return Версия сервера.
Version ConnectionAdmin::getServerVersion()
{
    Version result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "1");
    ServerResponse response (*this, query);
    response.checkReturnCode();
    result.parse (response);

    return result;
}

/// \brief Получение списка (рабочих) процессов, выполняемых в настоящий момент на сервере ИРБИС64.
/// \return Массив серверных процессов.
/// \warning Операция доступна только администратору.
std::vector<ProcessInfo> ConnectionAdmin::listProcesses()
{
    std::vector<ProcessInfo> result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "+3");
    ServerResponse response (*this, query);
    result = ProcessInfo::parse (response);

    return result;
}

/// \brief Реорганизация словаря базы данных.
/// \param databaseName Имя базы данных.
/// \return Признак успешности выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::reloadDictionary (const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "Y");
    query.addAnsi(databaseName);
    return this->execute(query);
}

/// \brief Реорганизация мастер-файла.
/// \param databaseName Имя базы данных.
/// \return Признак успешности выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::reloadMasterFile (const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "X");
    const auto db = choose (databaseName, this->database);
    query.addAnsi (db);
    return this->execute (query);
}

/// \brief Перезапуск сервера (с сохранением подключений).
/// \return Признак успешности выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::restartServer()
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "+8");
    return this->execute (query);
}

/// \brief Опустошение базы данных.
/// \param databaseName Имя базы данных.
/// \return Признак успешности выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::truncateDatabase (const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "T");
    const auto db = choose (databaseName, this->database);
    query.addAnsi (db);
    return execute (query);
}

/// \brief Разблокирование базы данных.
/// \param databaseName Имя базы данных.
/// \return Признак успешности выполнения операции.
/// \warning Операция доступна лишь администратору.
bool ConnectionAdmin::unlockDatabase (const String &databaseName)
{
    if (!this->_checkConnection()) {
        return false;
    }

    ClientQuery query (*this, "U");
    const auto db = choose (databaseName, this->database);
    query.addAnsi (db);
    return execute (query);
}

/// \brief Разблокирование указанных записей.
/// \param databaseName Имя базы данных.
/// \param mfnList Вектор MFN.
/// \return Признак успешности выполнения операции.
bool ConnectionAdmin::unlockRecords (const String &databaseName, const MfnList &mfnList)
{
    if (!this->_checkConnection()) {
        return false;
    }

    if (mfnList.empty()) {
        return true;
    }

    ClientQuery query (*this, "Q");
    const auto db = choose (databaseName, this->database);
    query.addAnsi (db).newLine();
    for (const auto mfn : mfnList) {
        query.add (mfn).newLine();
    }

    return execute (query);
}

}
