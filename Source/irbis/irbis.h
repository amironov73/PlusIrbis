// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <string>
#include <ios>
#include <list>
#include <map>
#include <set>
#include <vector>

//=========================================================

#if !defined(IRBIS_WINDOWS) && !defined (IRBIS_UNIX)

    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(_WINDOWS)

        #define IRBIS_WINDOWS

    #else

        #define IRBIS_UNIX

    #endif

#endif

//=========================================================

#ifdef IRBIS_DYNAMIC_LIBRARY

    #if defined(_MSC_VER)
        // Microsoft
        #define IRBIS_EXPORT __declspec(dllexport)
        #define IRBIS_IMPORT __declspec(dllimport)
    #elif defined(__GNUC__)
        //  GCC
        #define IRBIS_EXPORT __attribute__((visibility("default")))
        #define IRBIS_IMPORT
    #else
        //  do nothing and hope for the best?
        #define IRBIS_EXPORT
        #define IRBIS_IMPORT
        #pragma warning Unknown dynamic link import/export semantics.
    #endif

#else

    #define IRBIS_EXPORT
    #define IRBIS_IMPORT

#endif

//=========================================================

#ifdef PLUSIRBIS_LIBRARY

    #define PLUSIRBIS_EXPORTS IRBIS_EXPORT

#else

    #define PLUSIRBIS_EXPORTS IRBIS_IMPORT

#endif

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(push)
    #pragma warning(disable: 4251)
    #pragma warning(disable: 4275)

#endif

//=========================================================

#ifndef BYTE

typedef unsigned char BYTE; //-V677

#endif

//=========================================================

namespace irbis
{

class AlphabetTable;
class ChunkedBuffer;
class ClientInfo;
class ClientQuery;
class ClientSocket;
class Connection;
class ConnectionFactory;
class DatabaseInfo;
class DirectAccess64;
class EmbeddedField;
class Encoding;
class FileSpecification;
class Format;
class FoundLine;
class GblResult;
class GblSettings;
class GblStatements;
class IlfEntry;
class IlfFile;
class IniFile;
class IniLine;
class IniSection;
class IrbisDate;
class IrbisException;
class IrbisText;
class Iso2709;
class Encoding;
class MarcRecord;
class MarcRecordList;
class MemoryChunk;
class MenuEntry;
class MenuFile;
class MstControlRecord64;
class MstdDictionaryEntry64;
class MstFile64;
class MstRecord64;
class MstRecordLeader64;
class NetworkException;
class NotImplementedException;
class NumberChunk;
class NumberText;
class OptFile;
class OptLine;
class ParFile;
class PostingParameters;
class ProcessInfo;
class ProtocolText;
class RawRecord;
class RecordField;
class RecordSerializer;
class Search;
class SearchParameters;
class SearchScenario;
class ServerResponse;
class ServerStat;
class StopWords;
class SubField;
class TableDefinition;
class TermInfo;
class TermParameters;
class TermPosting;
class TextNavigator;
class TreeFile;
class TreeNode;
class UserInfo;
class VerificationException;
class Version;
class XrfFile64;
class XrfRecord64;

//=========================================================

using String = std::wstring;
using MfnList = std::vector<int>;
using StringList = std::vector<String>;
using SubFieldList = std::vector<SubField>;
using RecordFieldList = std::vector<RecordField>;

//=========================================================

/// \brief Результат выполнения функции (с учетом успеха/ошибки).
template<class T>
class Result // NOLINT(cppcoreguidelines-pro-type-member-init)
{
public:
    /// \brief Признак успешного завершения.
    bool success { false };

    /// \brief Результирующее значение (имеет смысл при успешном завершении).
    T result;

    /// \brief Сообщение об ошибке.
    String errorMessage;

    /// \brief Преобразование к логическому значению. true означает успех.
    operator bool() const // NOLINT(hicpp-explicit-conversions)
    {
        return this->success;
    }

    /// \brief Получение результирующего значения.
    operator T() const // NOLINT(hicpp-explicit-conversions)
    {
        return this->result;
    }

    /// \brief Формирование результата успешного выполнения.
    static Result Success(T value)
    {
        Result result { true, value };
        return result;
    }

    /// \brief Формирование признака ошибки.
    static Result Failure (const String &message)
    {
        Result result { false };
        result.errorMessage = message;
        return result;
    }
};

//=========================================================

/// \brief Базовое исключение для всех нештатных ситуаций.
class PLUSIRBIS_EXPORTS IrbisException
        : public std::exception
{
public:
};

//=========================================================

/// \brief Файл на сервере не найден.
class PLUSIRBIS_EXPORTS FileNotFoundException
        : public IrbisException
{
    std::wstring _fileName;

public:

    explicit FileNotFoundException(const std::wstring& fileName)  // NOLINT(modernize-pass-by-value)
        : _fileName(fileName)
    {
    }
};

//=========================================================

/// \brief INI-файл.
class PLUSIRBIS_EXPORTS IniFile final
{
public:
    std::vector<IniSection> sections;

    //IniFile();

    IniFile& clear();
    bool containsSection (const std::wstring &name) const;
    IniSection& createSection(const std::wstring &name);
    bool modified() const;
    void notModified();
    size_t getIndex(const std::wstring &name) const;
    IniSection* getSection(const std::wstring &name) const;
    const std::wstring& getValue(const std::wstring &sectionName, const std::wstring &keyName, const std::wstring &defaultValue) const;
    void parse(const StringList &lines);
    IniFile& removeSection(const std::wstring &sectionName);
    IniFile& removeValue(const std::wstring &sectionName, const std::wstring &keyName);
    IniFile& setValue(const std::wstring &sectionName, const std::wstring &keyName, const std::wstring &value);
    //void write(const std::wstring &filename, QTextCodec *encoding) const;
    //void writeModifiedOnly(QTextStream &stream) const;
};

//=========================================================

/// \brief Строка INI-файла.
class PLUSIRBIS_EXPORTS IniLine final
{
public:
    std::wstring key;
    std::wstring value;

    bool modified() const;
    void notModified();
    void setKey(const std::wstring &newKey);
    void setValue(const std::wstring &newValue);
    std::wstring toString() const;

private:
    bool _modified { false };
};

//=========================================================

/// \brief Секция INI-файла.
class PLUSIRBIS_EXPORTS IniSection final
{
public:
    /// \brief Имя секции.
    std::wstring name;

    /// \brief Строки, входящие в секцию.
    /// \see IniLine
    std::vector<IniLine> lines;

    IniSection& clear();
    bool containsKey(const std::wstring &key) const;
    size_t getIndex(const std::wstring &key) const;
    IniLine* getLine(const std::wstring &key) const;
    const std::wstring& getValue(const std::wstring &key, const std::wstring &defaultValue) const;
    bool modified() const;
    void notModified();
    IniSection& removeValue(const std::wstring &key);
    IniSection& setValue(const std::wstring &key, const std::wstring &value);
    std::wstring toString() const;

    const std::wstring& operator[] (const std::wstring &index) const;
};

//=========================================================

/// \brief Таблица алфавитных символов.
///
/// Таблица алфавитных символов используется системой ИРБИС
/// при разбиении текста на слова и представляет собой список
/// кодов символов, которые считаются алфавитными.
///
class PLUSIRBIS_EXPORTS AlphabetTable final
{
public:
    const static String FileName;
    std::set<wchar_t> characters;

    /// Конструктор.
    explicit AlphabetTable(const std::vector<BYTE> &bytes);

    /// Синглтон.
    static const AlphabetTable& instance();

    /// Проверка, является ли указанный символ буквой.
    bool isAlpha(const wchar_t &c) const;

    /// Парсинг потока.
    static AlphabetTable parse(std::istream &stream);

    /// Чтение локального файла.
    static AlphabetTable readLocalFile(const String &fileName);

    /// Удаление пробелов в начале и в конце строки.
    String trimText(const String &text) const;

    /// Разбиение текста на слова.
    StringList splitWords(const String &text) const;

    /// Верификация таблицы.
    bool verify(bool throwOnError) const;
};

//=========================================================

class PLUSIRBIS_EXPORTS ChunkedBuffer final
{
private:
    MemoryChunk *_first, *_current, *_last;
    size_t _chunkSize, _position, _read;

    bool _advance();
    void _appendChunk();

public:
    const static size_t DefaultChunkSize;

    explicit ChunkedBuffer(size_t chunkSize = DefaultChunkSize);
    ChunkedBuffer(const ChunkedBuffer &) = delete;
    ChunkedBuffer(ChunkedBuffer &&) = delete;
    ChunkedBuffer operator = (const ChunkedBuffer &) = delete;
    ChunkedBuffer operator = (ChunkedBuffer &&) = delete;
    ~ChunkedBuffer();

    bool eof() const;
    int peek();
    size_t read(char *buffer, size_t offset, size_t count);
    int readByte();
    //std::wstring readLine(QTextCodec *codec);
    void rewind();
    void write(const char *buffer, size_t offset, size_t count);
    //void write(const std::wstring &text, QTextCodec *codec);
    void writeByte(char value);
};

//=========================================================

/// \brief Информация о подключенном клиенте.
class PLUSIRBIS_EXPORTS ClientInfo final
{
public:
    String number;        ///< Порядковый номер.
    String ipAddress;     ///< Адрес клиента.
    String port;          ///< Порт клиента.
    String name;          ///< Логин пользователя.
    String id;            ///< Идентификатор клиентской программы (просто уникальное число).
    String workstation;   ///< Тип клиентского АРМ.
    String registered;    ///< Момент подключения к серверу.
    String acknowledged;  ///< Последнее подтверждение, посланное серверу.
    String lastCommand;   ///< Последняя команда, посланная серверу.
    String commandNumber; ///< Порядковый номер последней программы.
};

//=========================================================

/// \brief Абстрактный клиентский сокет.
///
/// Наследники обязательно должны переопределить методы send и receive.
/// Объекты данного класса неперемещаемые.
class PLUSIRBIS_EXPORTS ClientSocket // abstract
{
public:
    String host { L"localhost" }; ///< Адрес сервера в виде строки.
    short port { 6666 }; ///< Номер порта сервера.

    ClientSocket() = default;
    ClientSocket(ClientSocket &) = delete;
    ClientSocket(ClientSocket &&) = delete;
    ClientSocket& operator = (ClientSocket &) = delete;
    ClientSocket& operator = (ClientSocket &&) = delete;
    virtual ~ClientSocket() = default;

    virtual void open();
    virtual void close();

    /// \brief Отсылка данных на сервер.
    /// \param buffer Указатель на буфер с данными.
    /// \param size Размер данных в байтах.
    virtual void send(const BYTE *buffer, size_t size) = 0;

    /// \brief Получение отвера от сервера (частями).
    /// \param buffer Буфер для размещения полученного ответа.
    /// \param size Размер буфера в байтах.
    /// \return Количество полученных данных в байтах.
    /// Отрицательное число означает ошибку.
    /// 0 означает, что сервер закончил передачу данных.
    /// Положительное число означает, что приём продолжается,
    /// и вызов нужно будет повторить для получения следующей порции.
    virtual size_t receive(BYTE *buffer, size_t size) = 0;
};

//=========================================================

/// \brief Клиентский запрос.
///
/// Объекты данного класса неперемещаемые.
class PLUSIRBIS_EXPORTS ClientQuery final
{
    std::vector<BYTE> _content;

    void _write(const BYTE *bytes, size_t size);
    void _write(BYTE byte);

public:
    ClientQuery(const Connection &connection, const std::string &commandCode);
    ClientQuery(ClientQuery &) = delete;
    ClientQuery(ClientQuery &&) = delete;
    ClientQuery& operator = (ClientQuery &) = delete;
    ClientQuery& operator = (ClientQuery &&) = delete;
    ~ClientQuery() = default;

    ClientQuery& add(int value);
    ClientQuery& add(const FileSpecification &specification);
    ClientQuery& add(const MarcRecord &record, const std::wstring &delimiter);
    ClientQuery& addAnsi(const std::string &text);
    ClientQuery& addAnsi(const std::wstring &text);
    bool addFormat(const std::wstring &format);
    ClientQuery& addUtf(const std::wstring &text);
    void dump(std::ostream &stream) const;
    std::vector<BYTE> encode() const;
    ClientQuery& newLine();
};

//=========================================================

/// \brief Подключение к серверу ИРБИС64.
///
/// Объекты данного типа неперемещаемые.
class PLUSIRBIS_EXPORTS Connection final
{
private:
    bool _connected;
    StringList _databaseStack;

    bool _checkConnection();

public:
    String host;          ///< Адрес сервера в виде строки.
    short port;           ///< Номер порта сервера.
    String username;      ///< Логин пользователя.
    String password;      ///< Пароль пользователя.
    String database;      ///< Имя текущей базы данных.
    String workstation;   ///< Тип клиента.
    int clientId;         ///< Уникальный идентификатор клиента.
    int queryId;          ///< Порядковый номер команды, отсылаемой на сервер.
    int lastError;        ///< Код ошибки, возвращённый сервером в ответ на последнюю команду.
    String serverVersion; ///< Версия сервера (получается при подключении).
    IniFile iniFile;      ///< Содержимое серверного INI-файла для данного клиента.
    int interval { 0 };   ///< Интервал автоматического подтверждения.
    ClientSocket *socket; ///< Клиентский сокет.

    Connection();
    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection& operator=(Connection&&) = delete;
    ~Connection();

    bool actualizeDatabase(const String &databaseName);
    bool actualizeRecord(const String &databaseName, int mfn);
    bool connect();
    bool createDatabase(const String &databaseName, const String &description, bool readerAccess);
    bool createDictionary(const String &databaseName);
    bool deleteDatabase(const String &databaseName);
    bool deleteRecord(int mfn);
    bool connected() const { return _connected; }
    void disconnect();
    bool execute(ClientQuery &query);
    String formatRecord(const String &format, int mfn);
    String formatRecord(const String &format, const MarcRecord &record);
    DatabaseInfo getDatabaseInfo(const std::wstring &databaseName);
    int getMaxMfn(const String &databaseName);
    GblResult globalCorrection(const GblSettings &settings);
    ServerStat getServerStat();
    Version getServerVersion();
    std::vector<UserInfo> getUserList();
    std::vector<DatabaseInfo> listDatabases(const IniFile &iniFile, const std::string &defaultFileName);
    std::vector<DatabaseInfo> listDatabases(const FileSpecification &specification);
    StringList listFiles(const FileSpecification &specification);
    StringList listFiles(const std::vector<FileSpecification> &specifications);
    std::vector<ProcessInfo> listProcesses();
    StringList listTerms(const String &prefix);
    bool noOp();
    void parseConnectionString(const String &connectionString);
    String popDatabase();
    String printTable(const TableDefinition &definition);
    String pushDatabase(const String &newDatabase);
    std::vector<BYTE> readBinaryFile(const FileSpecification &specification);
    IniFile readIniFile(const FileSpecification &specification);
    MenuFile readMenuFile(const FileSpecification &specification);
    std::vector<TermPosting> readPostings(const PostingParameters &parameters);
    RawRecord readRawRecord(int mfn);
    MarcRecord readRecord(int mfn);
    MarcRecord readRecord(const String &databaseName, int mfn);
    MarcRecord readRecord(const String &databaseName, int mfn, int version);
    std::vector<SearchScenario> readSearchScenario(const FileSpecification &specification);
    std::vector<TermInfo> readTerms(const String &startTerm, int numberOfTerms);
    std::vector<TermInfo> readTerms(const TermParameters &parameters);
    String readTextFile(const FileSpecification &specification);
    StringList readTextFiles(std::vector<FileSpecification> specifications);
    StringList  readTextLines(const FileSpecification &specification);
    bool reloadDictionary(const String &databaseName);
    bool reloadMasterFile(const String &databaseName);
    bool restartServer();
    String requireTextFile(const FileSpecification &specification);
    MfnList search(const Search &search);
    MfnList search(const String &expression);
    MfnList search(const SearchParameters &parameters);
    std::wstring toConnectionString() const;
    bool truncateDatabase(const String &databaseName);
    bool unlockDatabase(const String &databaseName);
    bool unlockRecords(const String &databaseName, const MfnList &mfnList);
    bool updateIniFile(StringList &lines);
    bool updateUserList(const std::vector<UserInfo> &users);
    int writeRawRecord(RawRecord &record, bool lockFlag, bool actualize, bool dontParseResponse);
    int writeRecord(MarcRecord &record, bool lockFlag, bool actualize, bool dontParseResponse);
    bool writeRecords(std::vector<MarcRecord*> &records, bool lockFlag, bool actualize, bool dontParseResponse);
    void writeTextFile(const FileSpecification &specification);
};

//=========================================================

class PLUSIRBIS_EXPORTS ConnectionFactory
{
public:
    ConnectionFactory() = default;
    ConnectionFactory(const ConnectionFactory &) = delete;
    ConnectionFactory(ConnectionFactory&&) = delete;
    ConnectionFactory& operator=(const ConnectionFactory&) = delete;
    ConnectionFactory& operator=(ConnectionFactory&&) = delete;
    virtual ~ConnectionFactory() = default;

    virtual Connection* GetConnection();
};

//=========================================================

class PLUSIRBIS_EXPORTS DatabaseInfo final
{
public:
    MfnList logicallyDeletedRecords;
    MfnList physicallyDeletedRecords;
    MfnList nonActualizedRecords;
    MfnList lockedRecords;
    std::wstring name;
    std::wstring description;
    int maxMfn { 0 };
    bool databaseLocked { false };
    bool readOnly { false };

    void parse(ServerResponse &response);
    static std::vector<DatabaseInfo> parse(const MenuFile &menu);
    std::wstring toString() const;
};

//=========================================================

enum DirectAccessMode : unsigned int
{
    Exclusive = 0u,
    Shared    = 1u,
    ReadOnly  = 2u
};

//=========================================================

class PLUSIRBIS_EXPORTS DirectAccess64 final
{
public:
    MstFile64 *mst;
    XrfFile64 *xrf;
    std::wstring database;

    DirectAccess64(const std::wstring &path);
    DirectAccess64(const DirectAccess64 &) = delete;
    DirectAccess64(const DirectAccess64 &&) = delete;
    DirectAccess64& operator = (const DirectAccess64 &) = delete;
    DirectAccess64& operator = (const DirectAccess64 &&) = delete;
    ~DirectAccess64();

    MstRecord64 readRawRecord(unsigned int mfn);
    MarcRecord readRecord(unsigned int mfn);
};

//=========================================================

class PLUSIRBIS_EXPORTS EmbeddedField final
{
public:

    const static char DefaultCode;

    static RecordFieldList getEmbeddedFields(const RecordField &field, char sign = DefaultCode);
};

//=========================================================

class PLUSIRBIS_EXPORTS Encoding // abstract
{
    static Encoding *_ansi;
    static Encoding *_utf;

public:

    virtual std::vector<BYTE> fromUnicode(const std::wstring &text) const = 0;
    virtual std::wstring toUnicode(const BYTE *bytes, size_t count) const = 0;

    static Encoding* ansi();
    static std::wstring fromAnsi(const BYTE *bytes, size_t count);
    static std::wstring fromUtf(const BYTE *bytes, size_t count);
    static std::vector<BYTE> toAnsi(const std::wstring &text);
    static std::vector<BYTE> toUtf(const std::wstring &text);
    static Encoding* utf();
};

//=========================================================

class PLUSIRBIS_EXPORTS FileSpecification final
{
public:
    bool binaryFile { false };
    int path { 0 };
    std::wstring database;
    std::wstring filename;
    std::wstring content;

    FileSpecification() = default;
    FileSpecification(int path, const std::wstring &filename);
    FileSpecification(int path, const std::wstring &database, const std::wstring &filename);

    static FileSpecification parse(const std::wstring &text);
    bool verify(bool throwException) const;

    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS FoundLine final
{
public:
    int mfn {0};
    std::wstring description;
};

//=========================================================

class PLUSIRBIS_EXPORTS GblResult final
{
public:

    void parse(const StringList &lines);
};

//=========================================================

class PLUSIRBIS_EXPORTS GblStatement final
{
public:
    std::wstring command;
    std::wstring parameter1;
    std::wstring parameter2;
    std::wstring format1;
    std::wstring format2;
};

//=========================================================

class PLUSIRBIS_EXPORTS GblSettings final
{
public:
    bool actualize { false };
    bool autoin { false };
    std::wstring database;
    std::wstring fileName;
    int firstRecord { 0 };
    bool formalControl { false };
    int maxMfn { 0 };
    std::vector<int> mfnList;
    int minMfn { 0 };
    int numberOfRecords { 0 };
    std::wstring searchExpression;
    std::vector<GblStatement> statements;
};

//=========================================================

class PLUSIRBIS_EXPORTS IlfEntry final
{
public:
    std::wstring date;
    std::wstring name;
    std::wstring description;
    std::wstring data;
    int position { 0 };
    int number { 0 };
    int dataLength { 0 };
    short flags { 0 };
    bool deleted { false };
};

//=========================================================

class PLUSIRBIS_EXPORTS IlfFile final
{
public:
    const static std::string MagicString;

    std::vector<IlfEntry> entries;
    int unknown1 { 0 };
    int slotCount { 0 };
    int entryCount { 0 };
    int writeCount { 0 };
    int deleteCount { 0 };

    IlfFile();

    void readLocalFile(const std::wstring &fileName);
};

//=========================================================

class PLUSIRBIS_EXPORTS IrbisDate final
{
public:
    std::wstring text;
    std::chrono::system_clock::time_point date;

    explicit IrbisDate(const std::wstring &text);
    explicit IrbisDate(std::chrono::system_clock::time_point date);

    static std::chrono::system_clock::time_point convert(const std::wstring &text);
    static std::wstring convert(const std::chrono::system_clock::time_point &date);
    static std::wstring today();
    static IrbisDate* safeParse(const std::wstring &text);
};

//=========================================================

enum IrbisPath : unsigned int
{
    System = 0u,
    Data = 1u,
    MasterFile = 2u,
    InvertedFile = 3u,
    ParameterFile = 10u,
    FullText = 11u,
    InternalResource = 12u
};

//=========================================================

class PLUSIRBIS_EXPORTS IrbisText final
{
public:
    const static char CrLf[];
    const static char Lf[];
    const static std::wstring IrbisDelimiter;
    const static std::wstring ShortDelimiter;
    const static std::wstring MsDosDelimiter;
    const static std::wstring UnixDelimiter;
    const static std::wstring SearchDelimiter;

    static std::wstring fromIrbisToDos(std::wstring &text);
    static std::wstring fromIrbisToUnix(std::wstring &text);
    static std::wstring fromDosToIrbis(std::wstring &text);
    static std::wstring fromDosToUnix(std::wstring &text);
    static StringList fromFullDelimiter (const std::wstring &text);
    static StringList fromShortDelimiter (const std::wstring &text);
    static std::wstring readAllAnsi(const std::wstring &filename);
    static std::wstring readAllUtf(const std::wstring &filename);
    static StringList readAnsiLines(const std::wstring &filename);
    static StringList readUtfLines(const std::wstring &filename);
};

//=========================================================

class PLUSIRBIS_EXPORTS Iso2709 final
{
public:
    static const int MarkerLength = 24;
    static const char RecordDelimiter = 0x1D;
    static const char FieldDelimiter = 0x1E;
    static const char SubFieldDelimiter = 0x1F;

    static MarcRecord* readRecord(FILE *device, const Encoding *encoding);
    static void writeRecord(FILE *device, const MarcRecord &record, const Encoding *encoding);
};

//=========================================================

class PLUSIRBIS_EXPORTS MarcRecord final
{
public:
    unsigned int mfn { 0u };
    unsigned int status { 0u };
    unsigned int version { 0u };
    std::wstring database;
    std::list<RecordField> fields;

    MarcRecord& add(wchar_t code, const std::wstring &value);
    MarcRecord clone() const;
    void decode(const StringList &lines);
    bool deleted() const;
    std::wstring encode(const std::wstring &delimiter) const;
    std::wstring fm(int tag, wchar_t code) const;
    StringList fma(int tag, wchar_t code) const;
    RecordField* getField(int tag, int occurrence) const;
    std::vector<RecordField*> getFields(int tag) const;
    MarcRecord& reset();
    bool verify(bool throwOnError) const;

    friend PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const MarcRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS MemoryChunk final
{
public:
    char *data;
    MemoryChunk *next;

    explicit MemoryChunk(size_t size);
    MemoryChunk(const MemoryChunk &other) = delete;
    MemoryChunk(MemoryChunk &&other) = delete;
    MemoryChunk& operator = (const MemoryChunk &other) = delete;
    MemoryChunk& operator = (MemoryChunk &&other) = delete;
    ~MemoryChunk();
};

//=========================================================

class PLUSIRBIS_EXPORTS MenuEntry final
{
public:
    std::wstring code;
    std::wstring comment;

    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS MenuFile final
{
public:
    const static std::wstring StopMarker;
    const static std::vector<wchar_t> Separators;

    std::string fileName;
    std::list<MenuEntry> entries;

    MenuFile& add(const std::wstring &code, const std::wstring &comment);
    MenuEntry* getEntry(const std::wstring &code) const;
    MenuEntry* getEntrySensitive(const std::wstring &code) const;
    std::wstring* getValue(const std::wstring &code) const;
    std::wstring* getValueSensitive(const std::wstring &code) const;
    const std::wstring& getValue(const std::wstring &code, const std::wstring &defaultValue) const;
    const std::wstring& getValueSensitive(const std::wstring &code, const std::wstring &defaultValue) const;
    void parse(std::istream &stream);
    void parse(const StringList &lines);
    void parseLocalFile(const std::wstring &filename /* const QTextCodec *encoding */);
};

//=========================================================

#pragma pack(push, 1)
class PLUSIRBIS_EXPORTS MstControlRecord64 final
{
public:
    const static int RecordSize;
    const static long LockFlagPosition;

    int ctlMfn { 0 };
    int nextMfn { 0 };
    long nextPosition { 0 };
    int mftType { 0 };
    int recCnt { 0 };
    int reserv1 { 0 };
    int reserv2 { 0 };
    int blocked { 0 };

    void read(FILE *file);
};
#pragma pack(pop)

//=========================================================

#pragma pack(push, 1)
class PLUSIRBIS_EXPORTS MstDictionaryEntry64 final
{
public:
    const static int EntrySize;

    int tag { 0 };
    int position { 0 };
    int length { 0 };
    std::wstring text;

    void read(FILE *file);
};
#pragma pack(pop)

//=========================================================

class PLUSIRBIS_EXPORTS MstFile64 final
{
    FILE *_file;

public:
    MstControlRecord64 control;
    std::wstring fileName;

    MstFile64(const std::wstring &fileName, DirectAccessMode mode);
    MstFile64(const MstFile64 &) = delete;
    MstFile64(const MstFile64 &&) = delete;
    MstFile64& operator = (const MstFile64 &) = delete;
    MstFile64& operator = (const MstFile64 &&) = delete;
    ~MstFile64();

    MstRecord64 readRecord(long position);
};

//=========================================================

#pragma pack(push, 1)
class PLUSIRBIS_EXPORTS MstRecordLeader64 final
{
public:
    const static int LeaderSize;

    unsigned int mfn { 0 };
    unsigned int length { 0 };
    long previous { 0 };
    unsigned int base { 0 };
    unsigned int nvf { 0 };
    unsigned int status { 0 };
    unsigned int version { 0 };

    void read(FILE *file);
};
#pragma pack(pop)

//=========================================================

#pragma pack(push, 1)
class PLUSIRBIS_EXPORTS MstRecord64 final
{
public:
    MstRecordLeader64 leader;
    long offset { 0 };
    std::vector<MstDictionaryEntry64> dictionary;

    bool deleted() const;
    MarcRecord toMarcRecord() const;
};
#pragma pack(pop)

//=========================================================

class PLUSIRBIS_EXPORTS NetworkException final
    : public IrbisException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS NotImplementedException final
    : public IrbisException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS NumberChunk final
{
    bool setUp(const std::wstring &str, const std::wstring &number);

    friend class NumberText;

public:
    std::wstring prefix;
    long long value{0};
    int length{0};
    bool haveValue{false};

    NumberChunk() = default;
    NumberChunk(const NumberChunk &) = delete;
    NumberChunk(NumberChunk &&) = delete;
    NumberChunk& operator = (const NumberChunk &) = delete;
    NumberChunk& operator = (NumberChunk &&) = delete;
    ~NumberChunk() = default;

    int compareTo(const NumberChunk &other) const;
    bool havePrefix() const;
    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS NumberText final
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS OptFile final
{
public:
    std::vector<OptLine> lines;

    void parse(const StringList &text);
};

//=========================================================

class PLUSIRBIS_EXPORTS OptLine final
{
public:
    std::wstring key;
    std::wstring value;
};

//=========================================================

class PLUSIRBIS_EXPORTS ParFile final
{
public:
    std::wstring xrf;
    std::wstring mst;
    std::wstring cnt;
    std::wstring n01;
    std::wstring n02;
    std::wstring l01;
    std::wstring l02;
    std::wstring ifp;
    std::wstring any;
    std::wstring pft;
    std::wstring ext;

    void assign (const std::wstring &path);
    void parse(const StringList &lines);
    std::map<int, std::wstring> toDictionary() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS PostingParameters final
{
public:
    StringList listOfTerms;
    std::wstring database;
    std::wstring format;
    std::wstring term;
    int firstPosting { 0 };
    int numberOfPostings { 0 };
};

//=========================================================

class PLUSIRBIS_EXPORTS ProcessInfo final
{
public:
    std::wstring number;
    std::wstring ipAddress;
    std::wstring name;
    std::wstring clientId;
    std::wstring workstation;
    std::wstring started;
    std::wstring lastCommand;
    std::wstring commandNumber;
    std::wstring processId;
    std::wstring state;

    static std::vector<ProcessInfo> parse(ServerResponse &response);
};

//=========================================================

class PLUSIRBIS_EXPORTS RawRecord final
{
public:
    int mfn { 0 };
    unsigned int status { 0 };
    unsigned int version { 0 };
    StringList fields;
    std::wstring database;

    friend PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const RawRecord &record);

    std::wstring encode(const std::wstring &delimiter) const;
    void parseSingle(const StringList &lines);
    std::wstring to_wstring() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordField final
{
public:
    const static int NoTag = 0;

    int tag { 0 };
    std::wstring value;
    std::list<SubField> subfields;

    RecordField& add(wchar_t code, const std::wstring &value = L"");
    RecordField& clear();
    RecordField clone() const;
    void decode(const std::wstring &line);
    bool empty() const;
    SubField* getFirstSubfield(wchar_t code) const;
    std::wstring getFirstSubfieldValue(wchar_t code) const;
    bool verify(bool throwOnError) const;
    std::wstring toString() const;

    friend PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const RecordField &field);
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordSerializer
{
    std::wiostream &stream;

public:
    explicit RecordSerializer(std::wiostream &stream);

    MarcRecord deserialize();
    void serialize(const MarcRecord &record);
};

//=========================================================

enum RecordStatus : unsigned int
{
    LogicallyDeleted = 1u,
    PhysicallyDeleted = 2u,
    Deleted = LogicallyDeleted | PhysicallyDeleted,
    Absent = 4u,
    NonActualized = 8u,
    Last = 32u,
    Locked = 64u
};

//=========================================================

class PLUSIRBIS_EXPORTS Search final
{
    std::wstring _buffer;

public:
    static Search all();
    Search& and_(const std::wstring &text);
    Search& and_(const std::wstring &text1, const std::wstring &text2);
    Search& and_(const std::wstring &text1, const std::wstring &text2, const std::wstring &text3);
    Search& and_(const Search &item);
    Search& and_(const Search &item1, const Search &item2);
    Search& and_(const Search &item1, const Search &item2, const Search &item3);
    static Search equals(const std::wstring &prefix, const std::wstring &text);
    static Search equals(const std::wstring &prefix, const std::wstring &text1, const std::wstring &text2);
    static Search equals(const std::wstring &prefix, const std::wstring &text1, const std::wstring &text2, const std::wstring &text3);
    static bool needWrap(const std::wstring &text);
    Search& not_(const std::wstring &text);
    Search& not_(const Search &item);
    Search& or_(const std::wstring &text);
    Search& or_(const std::wstring &text1, const std::wstring &text2);
    Search& or_(const std::wstring &text1, const std::wstring &text2, const std::wstring &text3);
    Search& or_(const Search &item);
    Search& or_(const Search &item1, const Search &item2);
    Search& or_(const Search &item1, const Search &item2, const Search &item3);
    Search& sameField(const std::wstring &text);
    Search& sameRepeat(const std::wstring &text);
    std::wstring toString() const;
    static std::wstring wrap(const std::wstring &text);
    static std::wstring wrap(const Search &item);
};

Search keyword(const std::wstring &value1);
Search author(const std::wstring &value1);
Search title(const std::wstring &value1);
Search publisher(const std::wstring &value1);
Search place(const std::wstring &value1);
Search subject(const std::wstring &value1);
Search language(const std::wstring &value1);
Search year(const std::wstring &value1);
Search magazine(const std::wstring &value1);
Search documentKind(const std::wstring &value1);
Search udc(const std::wstring &value1);
Search bbk(const std::wstring &value1);
Search rzn(const std::wstring &value1);
Search mhr(const std::wstring &value1);

//=========================================================

class PLUSIRBIS_EXPORTS SearchParameters final
{
public:
    std::wstring database;
    std::wstring searchExpression;
    int firstRecord { 1 };
    std::wstring formatSpecification;
    int maxMfn { 0 };
    int minMfn { 0 };
    int numberOfRecords { 0 };
    std::wstring sequentialSpecification;
    std::wstring filterSpecification;
};

//=========================================================

class PLUSIRBIS_EXPORTS SearchScenario final
{
public:
    std::wstring name;
    std::wstring prefix;
    int dictionaryType { 0 };
    std::wstring menuName;
    std::wstring oldFormat;
    std::wstring correction;
    std::wstring truncation;
    std::wstring hint;
    std::wstring modByDicAuto;
    std::wstring logic;
    std::wstring advance;
    std::wstring format;
};

//=========================================================

class PLUSIRBIS_EXPORTS ServerResponse final
{
    Connection *_connection;
    bool _success;
    size_t _position;
    std::vector<BYTE> _content;
    void _write(const BYTE *bytes, size_t size);

public:
    std::wstring command;
    int clientId;
    int queryId;
    int answerSize;
    int returnCode;
    std::wstring serverVersion;

    ServerResponse(Connection &connection, ClientQuery &query);
    ServerResponse(ServerResponse &) = delete;
    ServerResponse(ServerResponse &&) = delete;
    ServerResponse& operator = (ServerResponse &) = delete;
    ServerResponse& operator = (ServerResponse &&) = delete;
    ~ServerResponse() = default;

    bool success() const;

    bool checkReturnCode();
    bool checkReturnCode(int nargs, ...);
    std::string getLine();
    std::string getRemaining();
    int getReturnCode();
    std::wstring readAnsi();
    int readInteger();
    StringList readRemainingAnsiLines();
    std::wstring readRemainingAnsiText();
    StringList readRemainingUtfLines();
    std::wstring readRemainingUtfText();
    std::wstring readUtf();
};

//=========================================================

class PLUSIRBIS_EXPORTS ServerStat final
{
public:
    std::vector<ClientInfo> runningClients;
    int clientCount { 0 };
    int totalCommandCount { 0 };

    void parse(ServerResponse &response);
};

//=========================================================

class PLUSIRBIS_EXPORTS SubField final
{
public:
    const static wchar_t NoCode { L'\0' };

    wchar_t code { L'\0' };
    std::wstring value;

    SubField clone() const;
    void decode(const std::wstring &line);
    bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring toString() const;

    friend PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const SubField &subfield);
};

//=========================================================

class PLUSIRBIS_EXPORTS TableDefinition final
{
public:
    std::wstring database;
    std::wstring table;
    StringList headers;
    std::wstring mode;
    std::wstring searchQuery;
    int minMfn { 0 };
    int maxMfn { 0 };
    std::wstring sequentialQuery;
    MfnList mfnList;
};

//=========================================================

class PLUSIRBIS_EXPORTS Tcp4Socket final
    : public ClientSocket
{
    void *_impl;

public:
    explicit Tcp4Socket(const std::wstring& host=L"localhost", short port=6666);
    Tcp4Socket(const Tcp4Socket &) = delete;
    Tcp4Socket(Tcp4Socket &&) = delete;
    Tcp4Socket& operator = (const Tcp4Socket &) = delete;
    Tcp4Socket& operator = (Tcp4Socket &&) = delete;
    ~Tcp4Socket() override;

    void open() override;
    void close() override;
    void send(const BYTE *buffer, size_t size) override;
    size_t receive(BYTE *buffer, size_t size) override;
};

//=========================================================

class PLUSIRBIS_EXPORTS TermInfo final
{
public:
    int count { 0 };
    std::wstring text;

    static std::vector<TermInfo> parse(const StringList &lines);
    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS TermParameters final
{
public:
    std::wstring database;
    int numberOfTerms { 0 };
    bool reverseOrder { false };
    std::wstring startTerm;
    std::wstring format;
};

//=========================================================

class PLUSIRBIS_EXPORTS TermPosting final
{
public:
    int mfn { 0 };
    int tag { 0 };
    int occurrence { 0 };
    int count { 0 };
    std::wstring text;

    static std::vector<TermPosting> parse(const StringList &lines);
    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS TextNavigator final
{
private:
    size_t _column, _length, _line, _position;
    const std::wstring &_text;

public:
    const static wchar_t EOT;

    explicit TextNavigator(const std::wstring &text);
    TextNavigator(const TextNavigator &other);
    TextNavigator(TextNavigator&&) = delete;
    TextNavigator& operator = (const TextNavigator &) = delete;
    TextNavigator& operator = (TextNavigator &&) = delete;
    ~TextNavigator() = default;

    size_t column() const { return _column; }
    size_t line() const { return _line; }
    size_t length() const { return _length; }
    size_t position() const { return _position; }
    bool eot() const { return _position >= _length; }

    wchar_t at(size_t position) const;
    wchar_t front() const;
    wchar_t back() const;
    wchar_t lookAhead(ptrdiff_t distance = 1) const;
    wchar_t lookBehind(ptrdiff_t distance = 1) const;
    TextNavigator& move(ptrdiff_t distance);
    wchar_t peekChar() const;
    wchar_t readChar();
    std::wstring peekString(size_t length);
    std::wstring peekTo(wchar_t stopChar);
    std::wstring peekUntil(wchar_t stopChar);
    std::wstring readLine();
    bool isControl() const;
    bool isDigit() const;
    bool isLetter() const;
    bool isWhitespace() const;
    std::wstring readInteger();
    std::wstring readString(size_t length);
    std::wstring readTo(wchar_t stopChar);
    std::wstring readUntil(wchar_t stopChar);
    std::wstring readWhile(wchar_t goodChar);
    std::wstring readWord();
    std::wstring recentText(ptrdiff_t length) const;
    std::wstring remainingText() const;
    TextNavigator& skipWhitespace();
    TextNavigator& skipPunctuation();
    std::wstring substr(size_t offset, size_t length) const;
};

//=========================================================

class PLUSIRBIS_EXPORTS TreeFile final
{
public:
    std::vector<TreeNode> roots;

    void parse(const StringList &lines);
};

//=========================================================

class PLUSIRBIS_EXPORTS TreeNode final
{
public:
    std::wstring value;
    std::vector<TreeNode> children;
    int level { 0 };

    TreeNode& add(const std::wstring &name);
};

//=========================================================

class PLUSIRBIS_EXPORTS UserInfo final
{
public:
    std::wstring number;
    std::wstring name;
    std::wstring password;
    std::wstring cataloger;
    std::wstring reader;
    std::wstring circulation;
    std::wstring acquisitions;
    std::wstring provision;
    std::wstring administrator;

    String toString() const;
    static std::vector<UserInfo> parse(const StringList &lines);
};

//=========================================================

class PLUSIRBIS_EXPORTS VerificationException final
    : public IrbisException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS Version final
{
public:
    std::wstring organization; // на кого приобретен
    std::wstring version; // собственно версия, например, 64.2008.1
    int maxClients { 0 }; // максимальное количество подключений
    int connectedClients { 0 }; // текущее количество подключений

    void parse(ServerResponse &response);
    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS XrfFile64 final
{
    std::wstring _fileName;
    FILE *_file;

    static unsigned long long int getOffset(unsigned int mfn);

public:

    XrfFile64(const std::wstring &fileName, DirectAccessMode mode);
    XrfFile64(const XrfFile64 &) = delete;
    XrfFile64(const XrfFile64 &&) = delete;
    XrfFile64& operator = (const XrfFile64 &) = delete;
    XrfFile64& operator = (const XrfFile64 &&) = delete;
    ~XrfFile64();

    XrfRecord64 readRecord(unsigned int mfn);
};

//=========================================================

class PLUSIRBIS_EXPORTS XrfRecord64 final
{
public:
    const static int RecordSize;

    unsigned int mfn { 0 };
    long long offset { 0LL };
    unsigned int status { 0 };

    bool deleted() const;
    bool locked() const;
    std::wstring toString() const;
};

//=========================================================

// Utilities

bool PLUSIRBIS_EXPORTS sameChar(wchar_t first, wchar_t second);
bool PLUSIRBIS_EXPORTS sameString(const String &first, const String &second);

String PLUSIRBIS_EXPORTS toLower(String &text);
String PLUSIRBIS_EXPORTS toUpper(String &text);

bool PLUSIRBIS_EXPORTS contains(const String &text, const String &fragment);
bool PLUSIRBIS_EXPORTS contains(const String &text, wchar_t c);

std::string PLUSIRBIS_EXPORTS replace(const std::string &text, const std::string &from, const std::string &to);
String PLUSIRBIS_EXPORTS replace(const String &text, const String &from, const String &to);

String PLUSIRBIS_EXPORTS trimStart(const String &text);
String PLUSIRBIS_EXPORTS trimEnd(const String &text);
String trim(const String &text);

String PLUSIRBIS_EXPORTS describeError(int errorCode);

int PLUSIRBIS_EXPORTS fastParse32(const String &text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text, size_t length);
int PLUSIRBIS_EXPORTS fastParse32(const std::string &text);
int PLUSIRBIS_EXPORTS fastParse32(const char *text);
int PLUSIRBIS_EXPORTS fastParse32(const char *text, size_t length);

PLUSIRBIS_EXPORTS const std::string& iif(const std::string &s1, const std::string &s2);
PLUSIRBIS_EXPORTS const String& iif(const String &s1, const String &s2);
PLUSIRBIS_EXPORTS const std::string& iif(const std::string &s1, const std::string &s2, const std::string &s3);
PLUSIRBIS_EXPORTS const String& iif(const String& s1, const String &s2, const String &s3);

PLUSIRBIS_EXPORTS std::wstring safeAt(const StringList &list, size_t index);

PLUSIRBIS_EXPORTS std::vector<std::string> split(const std::string &text, char delimiter);
PLUSIRBIS_EXPORTS StringList split(const std::wstring &text, wchar_t delimiter);
PLUSIRBIS_EXPORTS std::vector<std::string> split(const std::string &text, const std::string &delimiter);
PLUSIRBIS_EXPORTS StringList split(const std::wstring &text, const std::wstring &delimiter);
PLUSIRBIS_EXPORTS StringList maxSplit(const std::wstring &text, wchar_t separator, int count);

PLUSIRBIS_EXPORTS std::wstring cp866_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::wstring cp1251_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::wstring koi8r_to_unicode(const std::string &text);

PLUSIRBIS_EXPORTS std::string unicode_to_cp866(const std::wstring &text);
PLUSIRBIS_EXPORTS std::string unicode_to_cp1251(const std::wstring &text);
PLUSIRBIS_EXPORTS void unicode_to_cp1251(BYTE *dst, const wchar_t *src, size_t size);
PLUSIRBIS_EXPORTS std::string unicode_to_koi8r(const std::wstring &text);

PLUSIRBIS_EXPORTS BYTE* toUtf(BYTE *dst, const wchar_t *src, size_t length);
PLUSIRBIS_EXPORTS wchar_t* fromUtf(wchar_t *dst, const BYTE *src, size_t length);
PLUSIRBIS_EXPORTS size_t countUtf(const wchar_t *src, size_t length);
PLUSIRBIS_EXPORTS size_t countUtf(const BYTE *src, size_t length);
PLUSIRBIS_EXPORTS const BYTE* fromUtf(const BYTE *src, size_t &size, BYTE stop, std::wstring &result);
PLUSIRBIS_EXPORTS BYTE* toUtf(BYTE *dst, const std::wstring &text);
PLUSIRBIS_EXPORTS std::wstring fromUtf(const std::string &text);
PLUSIRBIS_EXPORTS std::string toUtf(const std::wstring &text);

PLUSIRBIS_EXPORTS String removeComments(const String &text);
PLUSIRBIS_EXPORTS String prepareFormat(const String &text);

}

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif


