// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cwchar>
#include <cwctype>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
#include <ios>
#include <list>
#include <set>
#include <vector>

#define NAMESPACE_IRBIS_BEGIN namespace irbis {
#define NAMESPACE_IRBIS_END }

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

NAMESPACE_IRBIS_BEGIN

class AlphabetTable;
class BaseException;
class ChunkedBuffer;
class ClientInfo;
class ClientQuery;
class ClientSocket;
class CommandCode;
class Connection;
class ConnectionFactory;
class DatabaseInfo;
class DirectAccess64;
class EmbeddedField;
class FileSpecification;
class Format;
class IlfEntry;
class IlfFile;
class IrbisDate;
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
class RecordFieldList;
class RecordFieldListImpl;
class RecordSerializer;
class SearchParameters;
class SearchScenario;
class ServerResponse;
class ServerStat;
class StopWords;
class SubField;
class SubFieldList;
class SubFieldListImpl;
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

class PLUSIRBIS_EXPORTS NumberChunk final
{
private:
    bool setUp(const std::wstring &str, const std::wstring &number);

    friend class NumberText;

public:
    std::wstring prefix;
    long long value{0};
    int length{0};
    bool haveValue{false};

    NumberChunk() = default;
    NumberChunk(const NumberChunk &other) = default;

    int compareTo(const NumberChunk &other) const;
    constexpr bool havePrefix() const;
    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS NumberText final
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS TextNavigator final
{
private:
    size_t _column, _length, _line, _position;
    const std::wstring &_text;

public:
    const static wchar_t EOT;

    TextNavigator(const std::wstring &text);
    TextNavigator(const TextNavigator &other);
    TextNavigator& operator = (const TextNavigator &other) = delete;
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

class PLUSIRBIS_EXPORTS AlphabetTable final
{
public:
    const static std::string FileName;
    std::set<wchar_t> characters;

    AlphabetTable(const std::vector<unsigned char> &bytes /* encoding*/);

    static const AlphabetTable& instance();
    bool isAlpha(const wchar_t &c) const;
    static AlphabetTable parse(std::istream &stream);
    static AlphabetTable readLocalFile(const std::string &fileName);
    std::wstring trimText(const std::wstring &text) const;
    std::vector<std::wstring> splitWords(const std::wstring &text) const;
    bool verify(bool throwOnError) const;
};

//=========================================================

class PLUSIRBIS_EXPORTS BaseException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS MemoryChunk final
{
public:
    char *data;
    MemoryChunk *next;

    MemoryChunk(size_t size);
    MemoryChunk(const MemoryChunk &other) = delete;
    MemoryChunk(MemoryChunk &&other) = delete;
    MemoryChunk& operator = (const MemoryChunk &other) = delete;
    MemoryChunk& operator = (MemoryChunk &&other) = delete;
    ~MemoryChunk();
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

    ChunkedBuffer(size_t chunkSize = DefaultChunkSize);
    ChunkedBuffer(const ChunkedBuffer &other) = delete;
    ChunkedBuffer operator= (const ChunkedBuffer &other) = delete;
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

class PLUSIRBIS_EXPORTS ClientQuery final
{
private:
    Connection *connection;
    //QBuffer buffer;

public:
    ClientQuery(Connection *connection, const std::string &commandCode);

    ClientQuery& add(int value);
    ClientQuery& add(const FileSpecification &specification);
    ClientQuery& add(const MarcRecord &record);
    //ClientQuery& add(const RawRecord &record);
    ClientQuery& addAnsi(const std::string &text);
    ClientQuery& addAnsi(const std::wstring &text);
    ClientQuery& addAnsiNoLf(const std::wstring &text);
    ClientQuery& addLineFeed();
    ClientQuery& addUtf(const std::wstring &text);
    // QByteArray encode();
};

//=========================================================

class PLUSIRBIS_EXPORTS CommandCode final
{
public:
    const static std::string ExclusizeDatabaseLock;
    const static std::string RecordList;
    const static std::string ServerInfo;
    const static std::string DatabaseStat;
    const static std::string FormatIsoGroup;
    const static std::string UnknownCommand4;
    const static std::string GlobalConnection;
    const static std::string SaveRecordGroup;
    const static std::string Print;
    const static std::string UpdateIniFile;
    const static std::string ImportIso;
    const static std::string RegisterClient;
    const static std::string UnregisterClient;
    const static std::string ReadRecord;
    const static std::string UpdateRecord;
    const static std::string UnlockRecord;
    const static std::string ActualizeRecord;
    const static std::string FormatRecord;
    const static std::string ReadTerms;
    const static std::string ReadPostings;
    const static std::string CorrectVirtualRecord;
    const static std::string Search;
    const static std::string ReadDocument;
    const static std::string Backup;
    const static std::string Nop;
    const static std::string GetMaxMfn;
    const static std::string ReadTermsReverse;
    const static std::string UnlockRecords;
    const static std::string FullTextSearch;
    const static std::string EmptyDatabase;
    const static std::string CreateDatabase;
    const static std::string UnlockDatabase;
    const static std::string GetRecordPostings;
    const static std::string DeleteDatabase;
    const static std::string ReloadMasterFile;
    const static std::string ReloadDictionary;
    const static std::string CreateDictionary;
    const static std::string GetServerStat;
    const static std::string UnknownCommandPlus2;
    const static std::string GetProcessList;
    const static std::string UnknownCommandPlus4;
    const static std::string UnknownCommandPlus5;
    const static std::string UnknownCommandPlus6;
    const static std::string SetUserList;
    const static std::string RestartServer;
    const static std::string GetUserList;
    const static std::string ListFiles;
};

//=========================================================

class PLUSIRBIS_EXPORTS MenuEntry final
{
public:
    std::wstring code;
    std::wstring comment;

    std::wstring to_wstring() const;
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

class PLUSIRBIS_EXPORTS Version final
{
public:
    std::wstring organization; // на кого приобретен
    std::wstring version; // собственно версия, например, 64.2008.1
    int maxClients { 0 }; // максимальное количество подключений
    int connectedClients { 0 }; // текущее количество подключений

    static Version parse(ServerResponse &response);
    std::wstring to_wstring() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS Connection final
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS ConnectionFactory
{
public:
    ConnectionFactory() = default;
    ConnectionFactory(const ConnectionFactory &other) = delete;
    ConnectionFactory& operator=(const ConnectionFactory&) = delete;
    virtual ~ConnectionFactory() = default;

    virtual Connection* GetConnection();
};

//=========================================================

class PLUSIRBIS_EXPORTS Format final
{
public:
    static std::wstring removeComments(const std::wstring &text);
    static std::wstring prepareFormat(const std::wstring &text);
};

//=========================================================

class PLUSIRBIS_EXPORTS SubField final
{
public:
    constexpr static wchar_t NoCode = '\0';

    wchar_t code = L'\0';
    std::wstring value;

    constexpr bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

    friend std::wostream& operator << (std::wostream &stream, const SubField &subfield);
};

//=========================================================

class PLUSIRBIS_EXPORTS SubFieldList final
{
private:
    SubFieldListImpl *_impl;

public:
    SubFieldList();
    SubFieldList(const SubFieldList &other) = delete;
    SubFieldList(SubFieldList &&other) = delete;
    SubFieldList& operator = (const SubFieldList &other) = delete;
    SubFieldList& operator = (SubFieldList &&other) = delete;
    ~SubFieldList();

    void add(wchar_t code, const std::wstring &value);
    SubField* begin() const;
    void clear();
    SubField* end() const;
    size_t size() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordField final
{
public:
    constexpr static int NoTag = 0;

    int tag = 0;
    std::wstring value;
    std::list<SubField> subfields;

    RecordField& add(wchar_t code, const std::wstring &value = L"");
    RecordField& clear();
    constexpr bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

    friend std::wostream& operator << (std::wostream &stream, const RecordField &field);
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordFieldList final
{
private:
    RecordFieldListImpl *_impl;

public:
    RecordFieldList();
    RecordFieldList(const RecordFieldList &other) = delete;
    RecordFieldList(RecordFieldList &&other) = delete;
    RecordFieldList& operator = (const RecordFieldList &other) = delete;
    RecordFieldList& operator = (RecordFieldList &&other) = delete;
    ~RecordFieldList();
};

//=========================================================

class PLUSIRBIS_EXPORTS MarcRecord final
{
public:
    int mfn = 0;
    int status = 0;
    int version = 0;
    std::list<RecordField> fields;
    std::wstring database;

    MarcRecord& add(wchar_t code, const std::wstring &value);
    constexpr bool deleted() const;
    bool verify(bool throwOnError) const;

    friend std::wostream& operator << (std::wostream &stream, const MarcRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordSerializer {
private:
    std::wiostream &stream;

public:
    RecordSerializer(std::wiostream &stream);

    MarcRecord deserialize();
    void serialize(const MarcRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS NetworkException : public BaseException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS NotImplementedException : public BaseException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS RawRecord final
{
public:
    int mfn = 0;
    int status = 0;
    int version = 0;
    std::list<std::wstring> fields;
    std::wstring database;

    friend std::wostream& operator << (std::wostream &stream, const RawRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS ClientSocket // abstract
{
public:

    virtual ~ClientSocket();

    virtual void open();
    virtual void close();
    virtual void send(char *buffer, size_t size) = 0;
    virtual size_t receive(char *buffer, size_t size) = 0;
};

//=========================================================

class PLUSIRBIS_EXPORTS Tcp4Socket final : public ClientSocket
{
private:
    void *_impl;

public:
    std::string host;
    short port;

    Tcp4Socket(const std::string& host="localhost", short port=6666);
    ~Tcp4Socket();

    void open() override;
    void close() override;
    void send(char* buffer, size_t size) override;
    size_t receive(char* buffer, size_t size) override;
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

class PLUSIRBIS_EXPORTS VerificationException final
    : public BaseException
{
public:
};

//=========================================================

// Utilities

bool PLUSIRBIS_EXPORTS sameChar(wchar_t first, wchar_t second);
bool PLUSIRBIS_EXPORTS sameString(const std::wstring &first, const std::wstring &second);

bool PLUSIRBIS_EXPORTS contains(const std::wstring &text, const std::wstring &fragment);

int PLUSIRBIS_EXPORTS fastParse32(const std::wstring &text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text, int length);

PLUSIRBIS_EXPORTS const std::string& iif(const std::string& s1, const std::string &s2);
PLUSIRBIS_EXPORTS const std::wstring& iif(const std::wstring& s1, const std::wstring &s2);
PLUSIRBIS_EXPORTS const std::string& iif(const std::string& s1, const std::string &s2, const std::string &s3);
PLUSIRBIS_EXPORTS const std::wstring& iif(const std::wstring& s1, const std::wstring &s2, const std::wstring &s3);

PLUSIRBIS_EXPORTS std::wstring safeAt(const std::vector<std::wstring> &list, int index);

PLUSIRBIS_EXPORTS std::vector<std::wstring> maxSplit(const std::wstring &text, wchar_t separator, int count);

PLUSIRBIS_EXPORTS std::wstring cp866_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::wstring cp1251_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::wstring koi8r_to_unicode(const std::string &text);

PLUSIRBIS_EXPORTS std::string unicode_to_cp866(const std::wstring &text);
PLUSIRBIS_EXPORTS std::string unicode_to_cp1251(const std::wstring &text);
PLUSIRBIS_EXPORTS std::string unicode_to_koi8r(const std::wstring &text);

NAMESPACE_IRBIS_END

//=========================================================

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

//=========================================================
