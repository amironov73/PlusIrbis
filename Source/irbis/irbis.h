// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage

#include <chrono>
#include <string>
#include <ios>
#include <list>
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
        #define AM_EXPORT __declspec(dllexport)
        #define AM_IMPORT __declspec(dllimport)
    #elif defined(__GNUC__)
        //  GCC
        #define AM_EXPORT __attribute__((visibility("default")))
        #define AM_IMPORT
    #else
        //  do nothing and hope for the best?
        #define AM_EXPORT
        #define AM_IMPORT
        #pragma warning Unknown dynamic link import/export semantics.
    #endif

#else

    #define AM_EXPORT
    #define AM_IMPORT

#endif

//=========================================================

#ifdef PLUSIRBIS_LIBRARY

    #define PLUSIRBIS_EXPORTS AM_EXPORT

#else

    #define PLUSIRBIS_EXPORTS AM_IMPORT

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
class FileSpecification;
class Format;
class FoundLine;
class IlfEntry;
class IlfFile;
class IniFile;
class IrbisDate;
class IrbisException;
class IrbisText;
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
class PostingParameters;
class ProcessInfo;
class ProtocolText;
class RawRecord;
class RecordField;
class RecordSerializer;
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
class UserInfo;
class VerificationException;
class Version;
class XrfFile64;
class XrfRecord64;

//=========================================================

using MfnList = std::vector<int>;
using StringList = std::vector<std::wstring>;
using SubFieldList = std::vector<SubField>;
using RecordFieldList = std::vector<RecordField>;

//=========================================================

class PLUSIRBIS_EXPORTS AlphabetTable final
{
public:
    const static std::string FileName;
    std::set<wchar_t> characters;

    explicit AlphabetTable(const std::vector<unsigned char> &bytes);

    static const AlphabetTable& instance();
    bool isAlpha(const wchar_t &c) const;
    static AlphabetTable parse(std::istream &stream);
    static AlphabetTable readLocalFile(const std::string &fileName);
    std::wstring trimText(const std::wstring &text) const;
    std::vector<std::wstring> splitWords(const std::wstring &text) const;
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

    constexpr bool eof() const;
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

class PLUSIRBIS_EXPORTS ClientInfo final
{
public:
    std::wstring number;
    std::wstring ipAddress;
    std::wstring port;
    std::wstring name;
    std::wstring id;
    std::wstring workstation;
    std::wstring registered;
    std::wstring acknowledged;
    std::wstring lastCommand;
    std::wstring commandNumber;
};

//=========================================================

class PLUSIRBIS_EXPORTS ClientSocket // abstract
{
public:
    std::wstring host { L"localhost" };
    short port { 6666 };

    ClientSocket() = default;
    ClientSocket(ClientSocket &) = delete;
    ClientSocket(ClientSocket &&) = delete;
    ClientSocket& operator = (ClientSocket &) = delete;
    ClientSocket& operator = (ClientSocket &&) = delete;
    virtual ~ClientSocket() = default;

    virtual void open();
    virtual void close();
    virtual void send(const BYTE *buffer, size_t size) = 0;
    virtual size_t receive(BYTE *buffer, size_t size) = 0;
};

//=========================================================

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
    ClientQuery& add(const MarcRecord &record);
    ClientQuery& addAnsi(const std::string &text);
    ClientQuery& addAnsi(const std::wstring &text);
    ClientQuery& addUtf(const std::wstring &text);
    void dump(std::ostream &stream) const;
    std::vector<BYTE> encode() const;
    ClientQuery& newLine();
};

//=========================================================

class PLUSIRBIS_EXPORTS Connection final
{
private:
    bool _connected;
    StringList _databaseStack;

public:
    std::wstring host;
    int port;
    std::wstring username;
    std::wstring password;
    std::wstring database;
    std::wstring workstation;
    int clientId;
    int queryId;
    std::wstring serverVersion;
    ClientSocket *socket;

    Connection();
    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection& operator=(Connection&&) = delete;
    ~Connection();

    bool actualizeRecord(const std::wstring &database, int mfn);
    bool connect();
    bool createDatabase(const std::wstring &databaseName, const std::wstring &description, bool readerAccess);
    bool createDictionary(const std::wstring &databaseName);
    bool deleteDatabase(const std::wstring &databaseName);
    bool deleteRecord(int mfn);
    bool connected() const { return _connected; }
    void disconnect();
    bool execute(ClientQuery &query);
    std::wstring formatRecord(const std::wstring &format, int mfn);
    std::wstring formatRecord(const std::wstring &format, MarcRecord &record);
    DatabaseInfo getDatabaseInfo(const std::wstring &databaseName);
    int getMaxMfn(const std::wstring &databaseName);
    ServerStat getServerStat();
    Version getServerVersion();
    std::vector<UserInfo> getUserList();
    std::vector<DatabaseInfo> listDatabases(const IniFile &iniFile, const std::string &defaultFileName);
    std::vector<DatabaseInfo> listDatabases(const FileSpecification &specification);
    std::vector<std::wstring> listFiles(const FileSpecification &specification);
    std::vector<std::wstring> listFiles(const std::vector<FileSpecification> &specifications);
    std::vector<ProcessInfo> listProcesses();
    bool noOp();
    void parseConnectionString(const std::wstring &connectionString);
    std::wstring popDatabase();
    std::wstring printTable(const TableDefinition &definition);
    std::wstring pushDatabase(const std::wstring &newDatabase);
    std::vector<BYTE> readBinaryFile(const FileSpecification &specification);
    IniFile readIniFile(const FileSpecification &specification);
    std::vector<TermPosting> readPostings(const PostingParameters &parameters);
    RawRecord readRawRecord(int mfn);
    MarcRecord readRecord(int mfn);
    MarcRecord readRecord(const std::wstring &databaseName, int mfn);
    MarcRecord readRecord(const std::wstring &databaseName, int mfn, int version);
    std::vector<SearchScenario> readSearchScenario(const FileSpecification &specification);
    std::vector<TermInfo> readTerms(const TermParameters &parameters);
    std::wstring readTextFile(const FileSpecification &specification);
    StringList readTextFiles(std::vector<FileSpecification> specifications);
    bool reloadDictionary(const std::wstring &databaseName);
    bool reloadMasterFile(const std::wstring &databaseName);
    bool restartServer();
    std::wstring requireTextFile(const FileSpecification &specification);
    MfnList search(const std::wstring &expression);
    MfnList search(const SearchParameters &parameters);
    std::wstring toConnectionString() const;
    bool truncateDatabase(const std::wstring &databaseName);
    bool unlockDatabase(const std::wstring &databaseName);
    bool unlockRecords(const std::wstring &databaseName, const MfnList &mfnList);
    bool updateIniFile(StringList &lines);
    int writeRecord(MarcRecord &record, bool lockFlag, bool actualize, bool dontParseResponse);
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

    DatabaseInfo parse(ServerResponse &response);
    static std::vector<DatabaseInfo> parse(MenuFile &menu);
    std::wstring toString() const;
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

class PLUSIRBIS_EXPORTS IniFile final
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS IniLine final
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS IniSection final
{
public:
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

class PLUSIRBIS_EXPORTS IrbisException
        : public std::exception
{
public:
};

//=========================================================

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

enum IrbisPath
{
    System = 0,
    Data = 1,
    MasterFile = 2,
    InvertedFile = 3,
    ParameterFile = 10,
    FullText = 11,
    InternalResource = 12
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

class PLUSIRBIS_EXPORTS MarcRecord final
{
public:
    int mfn { 0 };
    int status { 0 };
    int version { 0 };
    std::list<RecordField> fields;
    std::wstring database;

    MarcRecord& add(wchar_t code, const std::wstring &value);
    bool deleted() const;
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
    static MenuFile parse(std::istream &stream);
    static MenuFile parseLocalFile(const std::wstring &filename /* const QTextCodec *encoding */);
};

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
    constexpr static int NoTag = 0;

    int tag { 0 };
    std::wstring value;
    std::list<SubField> subfields;

    RecordField& add(wchar_t code, const std::wstring &value = L"");
    RecordField& clear();
    bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

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

enum RecordStatus
{
    LogicallyDeleted = 1,
    PhysicallyDeleted = 2,
    Deleted = LogicallyDeleted | PhysicallyDeleted,
    Absent = 4,
    NonActualized = 8,
    Last = 32,
    Locked = 64
};

//=========================================================

class PLUSIRBIS_EXPORTS SearchParameters final
{
public:
    std::wstring database;
    int firstRecord { 1 };
    std::wstring formatSpecification;
    int maxMfn { 0 };
    int minMfn { 0 };
    int numberOfRecords { 0 };
    std::wstring searchExpression;
    std::wstring sequentialSpecification;
    std::wstring filterSpecification;
};

//=========================================================

class PLUSIRBIS_EXPORTS SearchScenario final
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS ServerResponse final
{
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

    bool success() const { return _success; }

    bool checkReturnCode(int nargs = 0, ...);
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
};

//=========================================================

class PLUSIRBIS_EXPORTS SubField final
{
public:
    constexpr static wchar_t NoCode { L'\0' };

    wchar_t code { L'\0' };
    std::wstring value;

    bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

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
    int minMfn{0};
    int maxMfn{0};
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

    constexpr size_t column() const { return _column; }
    constexpr size_t line() const { return _line; }
    constexpr size_t length() const { return _length; }
    constexpr size_t position() const { return _position; }
    constexpr bool eot() const { return _position >= _length; }

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
    std::wstring to_wstring() const;
};

//=========================================================

// Utilities

bool PLUSIRBIS_EXPORTS sameChar(wchar_t first, wchar_t second);
bool PLUSIRBIS_EXPORTS sameString(const std::wstring &first, const std::wstring &second);

std::wstring PLUSIRBIS_EXPORTS toLower(std::wstring &text);
std::wstring PLUSIRBIS_EXPORTS toUpper(std::wstring &text);

bool PLUSIRBIS_EXPORTS contains(const std::wstring &text, const std::wstring &fragment);

int PLUSIRBIS_EXPORTS fastParse32(const std::wstring &text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text, int length);

PLUSIRBIS_EXPORTS const std::string& iif(const std::string& s1, const std::string &s2);
PLUSIRBIS_EXPORTS const std::wstring& iif(const std::wstring& s1, const std::wstring &s2);
PLUSIRBIS_EXPORTS const std::string& iif(const std::string& s1, const std::string &s2, const std::string &s3);
PLUSIRBIS_EXPORTS const std::wstring& iif(const std::wstring& s1, const std::wstring &s2, const std::wstring &s3);

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
PLUSIRBIS_EXPORTS const BYTE* fromUtf(const BYTE *src, size_t &size, const BYTE stop, std::wstring &result);
PLUSIRBIS_EXPORTS BYTE* toUtf(BYTE *dst, const std::wstring &text);
PLUSIRBIS_EXPORTS std::wstring fromUtf(const std::string &text);
PLUSIRBIS_EXPORTS std::string toUtf(const std::wstring &text);

PLUSIRBIS_EXPORTS std::wstring removeComments(const std::wstring &text);
PLUSIRBIS_EXPORTS std::wstring prepareFormat(const std::wstring &text);

}

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif


