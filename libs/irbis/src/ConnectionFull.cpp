// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Удаление на сервере в текущей базе данных записи по её MFN.
/// \param mfn MNF записи, подлежащей удалению.
/// \return Признак успешности выполнения операции.
///
/// Если запись уже удалена, это не считается ошибкой.
bool ConnectionFull::deleteRecord(Mfn mfn) {
    if (!this->_checkConnection()) {
        return false;
    }

    auto record = readRecord(mfn);
    if (!record.deleted()) {
        record.status = record.status | RecordStatus::LogicallyDeleted;
        this->writeRecord(record, false, true, true);
    }

    return true;
}

/// \brief Чтение записи с сервера.
/// \param mfn MFN записи.
/// \return Запись.
MarcRecord ConnectionFull::readRecord (Mfn mfn)
{
    MarcRecord result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "C");
    query.addAnsi (this->database).newLine()
            .add(mfn);

    ServerResponse response (*this, query);
    if (response.checkReturnCode (4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.decode (lines);
        result.database = this->database;
    }
    return result;
}

/// \brief Чтение записи с сервера.
/// \param databaseName Имя базы данных.
/// \param mfn MFN записи.
/// \return Запись.
MarcRecord ConnectionFull::readRecord (const String &databaseName, Mfn mfn)
{
    MarcRecord result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "C");
    query.addAnsi (databaseName).newLine()
            .add (mfn);

    ServerResponse response (*this, query);
    if (response.checkReturnCode (4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.decode(lines);
        result.database = this->database;
    }

    return result;
}

/// \brief Чтение указанной версии записи.
/// \param databaseName Имя базы данных.
/// \param mfn MFN записи.
/// \param version Номер версии.
/// \return Запись.
MarcRecord ConnectionFull::readRecord (const String &databaseName, Mfn mfn, int version)
{
    MarcRecord result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "C");
    query.addAnsi (databaseName).newLine()
            .add (mfn).newLine()
            .add (version);

    ServerResponse response (*this, query);
    if (response.checkReturnCode (4, -201, -600, -602, -603)) {
        const auto lines = response.readRemainingUtfLines();
        result.decode (lines);
        result.database = this->database;
    }

    //if (version) {
        // TODO unlockRecord
        //MfnList records { mfn };
        //this->unlockRecords (databaseName, records);
    //}

    return result;
}

/// \brief Считывание нескольких записей за один раз.
/// \param mfnList Вектор MFN.
/// \return Вектор прочитанных записей.
std::vector<MarcRecord> ConnectionFull::readRecords (const MfnList &mfnList)
{
    // TODO implement
    std::vector<MarcRecord> result;
    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

/// \brief Сохранение записи на серве.
/// \param record Запись.
/// \param lockFlag Оставить запись заблокированной.
/// \param actualize Актуализировать запись.
/// \param dontParseResponse Не разбирать ответ сервера.
/// \return Новый максимальный MFN.
int ConnectionFull::writeRecord (MarcRecord &record, bool lockFlag, bool actualize, bool dontParseResponse)
{
    if (!this->_checkConnection()) {
        return 0;
    }

    const auto db = choose (record.database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
    ClientQuery query (*this, "D");
    query.addAnsi (db).newLine();
    query.add (lockFlag).newLine();
    query.add (actualize).newLine();
    query.addUtf (record.encode (Text::IrbisDelimiter)).newLine();
    ServerResponse response(*this, query);
    if (!response.checkReturnCode()) {
        return 0;
    }

    if (!dontParseResponse) {
        record.fields.clear();
        const auto temp1 = response.readRemainingUtfLines();
        if (temp1.size() > 1) {
            StringList lines;
            lines.push_back (temp1 [0]);
            const auto temp2 = split (temp1 [1], 0x1E);
            lines.insert (std::end (lines), std::begin (temp2), std::end (temp2));
            record.decode (lines);
            record.database = this->database;
        }
    }

    return response.returnCode;
}

}
