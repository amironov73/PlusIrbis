// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage

#include <cstdio>
#include <cstdlib>
#include <cstdint>
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

using Byte = std::uint8_t;
using Bytes = std::vector<Byte>;
using Char = wchar_t;
using Chars = std::vector<Char>;
using String = std::wstring;
using Flag = std::uint32_t;
using Mfn = std::uint32_t;
using MfnList = std::vector<Mfn>;
using StringList = std::vector<String>;
using SubFieldList = std::vector<SubField>;
using RecordFieldList = std::vector<RecordField>;

//=========================================================

/// \brief Результат выполнения функции (с учетом успеха/ошибки).
/// \tparam T Тип результата.
template<class T>
class Result // NOLINT(cppcoreguidelines-pro-type-member-init)
{
public:
    bool success { false }; ///< Признак успешного завершения.
    T result; ///< Результирующее значение (имеет смысл при успешном завершении).
    String errorMessage; ///< Сообщение об ошибке.

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

/// \brief Непрерывный кусок памяти.
/// \tparam T Тип элемента.
template<class T>
class Span final
{
public:
    T *ptr;        ///< Указатель на начала куска.
    size_t length; ///< Длина куска в элементах.
};

//=========================================================

/// \brief Перечислитель объектов. Абстрактный базовый класс.
/// \tparam T Тип перечисляемых объектов
///
/// Поддерживает простой перебор по некоторой коллекции.
template<class T>
class Enumerator
{
public:
    bool moveNext() = 0;   ///< Перемещает перечислитель к следующему элементу коллекции.
    T current() const = 0; ///< Возвращает элемент коллекции, соответствующий текущей позиции перечислителя.
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
    String _fileName;

public:

    explicit FileNotFoundException(const String &fileName)  // NOLINT(modernize-pass-by-value)
        : _fileName(fileName)
    {
    }
};

//=========================================================

/// \brief INI-файл.
class PLUSIRBIS_EXPORTS IniFile final
{
public:
    std::vector<IniSection> sections; ///< Секции INI-файла.

    IniFile& clear();
    bool containsSection (const String &name) const noexcept;
    IniSection& createSection(const String &name);
    bool modified() const noexcept;
    void notModified();
    ptrdiff_t getIndex(const String &name) const noexcept;
    IniSection* getSection(const String &name) const noexcept;
    const String& getValue(const String &sectionName, const String &keyName, const String &defaultValue) const noexcept;
    void parse(const StringList &lines);
    IniFile& removeSection(const String &sectionName);
    IniFile& removeValue(const String &sectionName, const String &keyName);
    IniFile& setValue(const String &sectionName, const String &keyName, const String &value);
    //void write(const std::wstring &filename, QTextCodec *encoding) const;
    //void writeModifiedOnly(QTextStream &stream) const;
};

//=========================================================

/// \brief Строка INI-файла.
///
/// Состоит из ключа и значения, разделённых символом '='.
class PLUSIRBIS_EXPORTS IniLine final
{
public:
    String key;    ///< Ключ (не должен быть пустым).
    String value;  ///< Значение (может быть пустым).

    bool modified() const noexcept;
    void notModified() noexcept;
    void setKey(const String &newKey) noexcept;
    void setValue(const String &newValue) noexcept;
    String toString() const;

private:
    bool _modified { false };
};

//=========================================================

/// \brief Секция INI-файла.
class PLUSIRBIS_EXPORTS IniSection final
{
public:
    String name; ///< Имя секции.
    std::vector<IniLine> lines; ///< Строки, входящие в секцию. \see IniLine

    IniSection& clear();
    bool containsKey(const String &key) const noexcept;
    ptrdiff_t getIndex(const String &key) const noexcept;
    IniLine* getLine(const String &key) const noexcept;
    const String& getValue(const String &key, const String &defaultValue) const noexcept;
    bool modified() const noexcept;
    void notModified();
    IniSection& removeValue(const String &key);
    IniSection& setValue(const String &key, const String &value);
    String toString() const;

    const String& operator[] (const String &index) const noexcept;
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
    std::set<Char> characters;

    /// Конструктор.
    explicit AlphabetTable(const std::vector<Byte> &bytes);

    /// Синглтон.
    static const AlphabetTable& instance();

    /// Проверка, является ли указанный символ буквой.
    bool isAlpha(const Char &c) const;

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
    virtual void send(const Byte *buffer, size_t size) = 0;

    /// \brief Получение отвера от сервера (частями).
    /// \param buffer Буфер для размещения полученного ответа.
    /// \param size Размер буфера в байтах.
    /// \return Количество полученных данных в байтах.
    /// Отрицательное число означает ошибку.
    /// 0 означает, что сервер закончил передачу данных.
    /// Положительное число означает, что приём продолжается,
    /// и вызов нужно будет повторить для получения следующей порции.
    virtual size_t receive(Byte *buffer, size_t size) = 0;
};

//=========================================================

/// \brief Клиентский запрос.
///
/// Объекты данного класса неперемещаемые.
class PLUSIRBIS_EXPORTS ClientQuery final
{
    std::vector<Byte> _content;

    void _write(const Byte *bytes, size_t size);
    void _write(Byte byte);

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
    ClientQuery& addAnsi(const String &text);
    bool addFormat(const String &format);
    ClientQuery& addUtf(const String &text);
    void dump(std::ostream &stream) const;
    Bytes encode() const;
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
    bool connected() const noexcept { return _connected; }
    void disconnect();
    bool execute(ClientQuery &query);
    String formatRecord(const String &format, Mfn mfn);
    String formatRecord(const String &format, const MarcRecord &record);
    DatabaseInfo getDatabaseInfo(const String &databaseName);
    Mfn getMaxMfn(const String &databaseName);
    GblResult globalCorrection(const GblSettings &settings);
    ServerStat getServerStat();
    Version getServerVersion();
    std::vector<UserInfo> getUserList();
    std::vector<DatabaseInfo> listDatabases(const IniFile &iniFile, const String &defaultFileName);
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
    Bytes readBinaryFile(const FileSpecification &specification);
    IniFile readIniFile(const FileSpecification &specification);
    MenuFile readMenuFile(const FileSpecification &specification);
    std::vector<TermPosting> readPostings(const PostingParameters &parameters);
    RawRecord readRawRecord(Mfn mfn);
    MarcRecord readRecord(Mfn mfn);
    MarcRecord readRecord(const String &databaseName, Mfn mfn);
    MarcRecord readRecord(const String &databaseName, Mfn mfn, int version);
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
    String toConnectionString() const;
    bool truncateDatabase(const String &databaseName);
    bool unlockDatabase(const String &databaseName);
    bool unlockRecords(const String &databaseName, const MfnList &mfnList);
    bool updateIniFile(const StringList &lines);
    bool updateUserList(const std::vector<UserInfo> &users);
    int writeRawRecord(RawRecord &record, bool lockFlag, bool actualize, bool dontParseResponse);
    int writeRecord(MarcRecord &record, bool lockFlag, bool actualize, bool dontParseResponse);
    bool writeRecords(std::vector<MarcRecord*> &records, bool lockFlag, bool actualize, bool dontParseResponse);
    void writeTextFile(const FileSpecification &specification);
};

//=========================================================

/// \brief Фабрика подключений.
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

/// \brief Информация о базе данных ИРБИС.
class PLUSIRBIS_EXPORTS DatabaseInfo final
{
public:
    String name;                      ///< Имя базы данных.
    String description;               ///< Описание базы данных в произвольной форме (может быть пустым).
    MfnList logicallyDeletedRecords;  ///< Список логически удалённых записей (может быть пустым).
    MfnList physicallyDeletedRecords; ///< Список физически удалённых записей (может быть пустым).
    MfnList nonActualizedRecords;     ///< Список неактуализированных записей (может быть пустым).
    MfnList lockedRecords;            ///< Список заблокированных записей (может быть пустым).
    Mfn maxMfn { 0 };                 ///< Максимальный MFN для базы данных.
    bool databaseLocked { false };    ///< Признак блокировки базы данных в целом.
    bool readOnly { false };          ///< База данных доступна только для чтения.

    void parse(ServerResponse &response);
    static std::vector<DatabaseInfo> parse(const MenuFile &menu);
    String toString() const;
};

//=========================================================

/// \brief Режим прямого доступа к базе данных.
enum DirectAccessMode : unsigned int
{
    Exclusive = 0u, ///< Эксклюзивный (чтение-запись).
    Shared    = 1u, ///< Разделяемый (чтение-запись).
    ReadOnly  = 2u  ///< Только чтение.
};

//=========================================================

class PLUSIRBIS_EXPORTS DirectAccess64 final
{
public:
    MstFile64 *mst;
    XrfFile64 *xrf;
    String database;

    DirectAccess64(const String &path);
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

    virtual std::vector<Byte> fromUnicode(const String &text) const = 0;
    virtual String toUnicode(const Byte *bytes, size_t count) const = 0;

    static Encoding* ansi();
    static String fromAnsi(const Byte *bytes, size_t count);
    static String fromUtf(const Byte *bytes, size_t count);
    static Bytes toAnsi(const String &text);
    static Bytes toUtf(const String &text);
    static Encoding* utf();
};

//=========================================================

class PLUSIRBIS_EXPORTS FileSpecification final
{
public:
    bool binaryFile { false };
    int path { 0 };
    String database;
    String filename;
    String content;

    FileSpecification() = default;
    FileSpecification(int path, const String &filename);
    FileSpecification(int path, const String &database, const String &filename);

    static FileSpecification parse(const std::wstring &text);
    bool verify(bool throwException) const;

    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS FoundLine final
{
public:
    Mfn mfn {0};
    String description;
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
    String command;
    String parameter1;
    String parameter2;
    String format1;
    String format2;
};

//=========================================================

class PLUSIRBIS_EXPORTS GblSettings final
{
public:
    bool actualize { false };
    bool autoin { false };
    String database;
    String fileName;
    Mfn firstRecord { 0 };
    bool formalControl { false };
    Mfn maxMfn { 0 };
    std::vector<int> mfnList;
    Mfn minMfn { 0 };
    Mfn numberOfRecords { 0 };
    String searchExpression;
    std::vector<GblStatement> statements;
};

//=========================================================

class PLUSIRBIS_EXPORTS IlfEntry final
{
public:
    String date;
    String name;
    String description;
    String data;
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

    void readLocalFile(const String &fileName);
};

//=========================================================

class PLUSIRBIS_EXPORTS IrbisDate final
{
public:
    String text;
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
    const static String IrbisDelimiter;
    const static String ShortDelimiter;
    const static String MsDosDelimiter;
    const static String UnixDelimiter;
    const static String SearchDelimiter;

    static String fromIrbisToDos(String &text);
    static String fromIrbisToUnix(String &text);
    static String fromDosToIrbis(String &text);
    static String fromDosToUnix(String &text);
    static StringList fromFullDelimiter (const String &text);
    static StringList fromShortDelimiter (const String &text);
    static String readAllAnsi(const String &filename);
    static String readAllUtf(const String &filename);
    static StringList readAnsiLines(const String &filename);
    static StringList readUtfLines(const String &filename);
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
    Mfn mfn { 0u };
    Flag status { 0u };
    unsigned int version { 0u };
    String database;
    std::list<RecordField> fields;

    RecordField& add(int tag, const String &value);
    MarcRecord clone() const;
    void decode(const StringList &lines);
    bool deleted() const noexcept;
    String encode(const String &delimiter) const;
    String fm(int tag, Char code) const noexcept;
    StringList fma(int tag, Char code) const;
    RecordField* getField(int tag, int occurrence) const noexcept;
    std::vector<RecordField*> getFields(int tag) const;
    MarcRecord& reset() noexcept;
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
    const static String StopMarker;
    const static Chars Separators;

    String fileName;
    std::list<MenuEntry> entries;

    MenuFile& add(const String &code, const String &comment);
    MenuEntry* getEntry(const String &code) const noexcept;
    MenuEntry* getEntrySensitive(const String &code) const noexcept;
    String* getValue(const String &code) const noexcept;
    String* getValueSensitive(const String &code) const noexcept;
    const String& getValue(const String &code, const String &defaultValue) const noexcept;
    const String& getValueSensitive(const String &code, const String &defaultValue) const noexcept;
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

    uint32_t ctlMfn { 0 };
    uint32_t nextMfn { 0 };
    int64_t nextPosition { 0 };
    uint32_t mftType { 0 };
    uint32_t recCnt { 0 };
    uint32_t reserv1 { 0 };
    uint32_t reserv2 { 0 };
    uint32_t blocked { 0 };

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
    String fileName;

    MstFile64(const String &fileName, DirectAccessMode mode);
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

    uint32_t mfn { 0 };
    uint32_t length { 0 };
    uint64_t previous { 0 };
    uint32_t base { 0 };
    uint32_t nvf { 0 };
    uint32_t status { 0 };
    uint32_t version { 0 };

    void read(FILE *file);
};
#pragma pack(pop)

//=========================================================

#pragma pack(push, 1)
class PLUSIRBIS_EXPORTS MstRecord64 final
{
public:
    MstRecordLeader64 leader;
    uint64_t offset { 0 };
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
    bool setUp(const String &str, const String &number);

    friend class NumberText;

public:
    String prefix;
    int64_t value { 0 };
    int length { 0 };
    bool haveValue { false };

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
    String xrf;
    String mst;
    String cnt;
    String n01;
    String n02;
    String l01;
    String l02;
    String ifp;
    String any;
    String pft;
    String ext;

    void assign (const String &path);
    void parse(const StringList &lines);
    std::map<int, String> toDictionary() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS PostingParameters final
{
public:
    StringList listOfTerms;
    String database;
    String format;
    String term;
    int firstPosting { 0 };
    int numberOfPostings { 0 };
};

//=========================================================

class PLUSIRBIS_EXPORTS ProcessInfo final
{
public:
    String number;
    String ipAddress;
    String name;
    String clientId;
    String workstation;
    String started;
    String lastCommand;
    String commandNumber;
    String processId;
    String state;

    static std::vector<ProcessInfo> parse(ServerResponse &response);
};

//=========================================================

class PLUSIRBIS_EXPORTS RawRecord final
{
public:
    Mfn mfn { 0 };
    unsigned int status { 0 };
    unsigned int version { 0 };
    StringList fields;
    String database;

    friend PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const RawRecord &record);

    String encode(const String &delimiter) const;
    void parseSingle(const StringList &lines);
    String toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordField final
{
public:
    //const static int NoTag = 0;

    int tag { 0 };
    String value;
    std::list<SubField> subfields;

    RecordField& add(Char code, const String &value = L"");
    RecordField& clear();
    RecordField clone() const;
    void decode(const String &line);
    bool empty() const noexcept;
    SubField* getFirstSubfield(Char code) const noexcept;
    String getFirstSubfieldValue(Char code) const noexcept;
    bool verify(bool throwOnError) const;
    String toString() const;

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

/// \brief Статус записи.
enum RecordStatus : unsigned int
{
    LogicallyDeleted = 1u, ///< Запись логически удалена.
    PhysicallyDeleted = 2u, ///< Запись физически удалена.
    Deleted = LogicallyDeleted | PhysicallyDeleted, ///< Запись удалена.
    Absent = 4u, ///< Запись отсутствует.
    NonActualized = 8u, ///< Не актуализирована.
    Last = 32u, ///< Последняя версия записи.
    Locked = 64u ///< Запись заблокирована.
};

//=========================================================

/// \brief Построитель запросов.
class PLUSIRBIS_EXPORTS Search final
{
    String _buffer;

public:
    static Search all();
    Search& and_(const String &text);
    Search& and_(const String &text1, const String &text2);
    Search& and_(const String &text1, const String &text2, const String &text3);
    Search& and_(const Search &item);
    Search& and_(const Search &item1, const Search &item2);
    Search& and_(const Search &item1, const Search &item2, const Search &item3);
    static Search equals(const String &prefix, const String &text);
    static Search equals(const String &prefix, const String &text1, const String &text2);
    static Search equals(const String &prefix, const String &text1, const String &text2, const String &text3);
    static bool needWrap(const String &text) noexcept;
    Search& not_(const String &text);
    Search& not_(const Search &item);
    Search& or_(const String &text);
    Search& or_(const String &text1, const String &text2);
    Search& or_(const String &text1, const String &text2, const String &text3);
    Search& or_(const Search &item);
    Search& or_(const Search &item1, const Search &item2);
    Search& or_(const Search &item1, const Search &item2, const Search &item3);
    Search& sameField(const String &text);
    Search& sameRepeat(const String &text);
    String toString() const noexcept;
    static String wrap(const String &text);
    static String wrap(const Search &item);
};

Search keyword(const String &value1);
Search author(const String &value1);
Search title(const String &value1);
Search publisher(const String &value1);
Search place(const String &value1);
Search subject(const String &value1);
Search language(const String &value1);
Search year(const String &value1);
Search magazine(const String &value1);
Search documentKind(const String &value1);
Search udc(const String &value1);
Search bbk(const String &value1);
Search rzn(const String &value1);
Search mhr(const String &value1);

//=========================================================

/// \brief Параметры для поиска записей.
class PLUSIRBIS_EXPORTS SearchParameters final
{
public:
    String searchExpression;        ///< Выражение для поиска по словарю.
    String database;                ///< Имя базы данных.
    int32_t firstRecord { 1 };          ///< Индекс первой требуемой записи.
    String formatSpecification;     ///< Формат для расформатирования записей.
    int32_t maxMfn { 0 };               ///< Максимальный MFN.
    int32_t minMfn { 0 };               ///< Минимальный MFN.
    int32_t numberOfRecords { 0 };      ///< Общее число требуемых записей.
    String sequentialSpecification; ///< Выражение для последовательного поиска.
    String filterSpecification;     ///< Выражение для локальной фильтрации.
};

//=========================================================

/// \brief Сценарий поиска.
class PLUSIRBIS_EXPORTS SearchScenario final
{
public:
    String name;              ///< Название поискового атрибута(автор, инвентарный номер и т. д.).
    String prefix;            ///< Префикс соответствующих терминов в словаре (может быть пустым).
    int dictionaryType { 0 }; ///< Тип словаря для соответствующего поиска.
    String menuName;          ///< Имя файла справочника.
    String oldFormat;         ///< Имя формата (без расширения).
    String correction;        ///< Способ корректировки по словарю.
    String truncation;        ///< Исходное положение переключателя "Усечение".
    String hint;              ///< Текст подсказки/предупреждения.
    String modByDicAuto;      ///< Параметр пока не задействован.
    String logic;             ///< Применимые логические операторы.
    String advance;           ///< Правила автоматического расширения поиска на основе авторитетного файла или тезауруса.
    String format;            ///< Имя формата показа документов.
};

//=========================================================

/// \brief Ответ сервера на клиентский запрос.
class PLUSIRBIS_EXPORTS ServerResponse final
{
    Connection *_connection;
    bool _success;
    size_t _position;
    Bytes _content;
    void _write(const Byte *bytes, size_t size);

public:
    String command;       ///< Код команды (дублирует клиентский запрос).
    int clientId;         ///< Идентификатор клиента (дублирует клиентский запрос).
    int queryId;          ///< Номер команды (дублирует клиентский запрос).
    int answerSize;       ///< Размер ответа сервера в байтах (в некоторых сценариях отсутствует).
    int returnCode;       ///< Код возврата (бывает не у всех ответов).
    String serverVersion; ///< Версия сервера (в некоторых сценариях отсутствует).

    ServerResponse(Connection &connection, ClientQuery &query);
    ServerResponse(ServerResponse &) = delete;
    ServerResponse(ServerResponse &&) = delete;
    ServerResponse& operator = (ServerResponse &) = delete;
    ServerResponse& operator = (ServerResponse &&) = delete;
    ~ServerResponse() = default;

    bool eot() const;
    bool success() const;

    bool checkReturnCode();
    bool checkReturnCode(int nargs, ...);
    std::string getLine();
    std::string getRemaining();
    int getReturnCode();
    String readAnsi();
    int readInteger();
    StringList readRemainingAnsiLines();
    String readRemainingAnsiText();
    StringList readRemainingUtfLines();
    String readRemainingUtfText();
    String readUtf();
};

//=========================================================

/// \brief Статистика работы ИРБИС-сервера.
class PLUSIRBIS_EXPORTS ServerStat final
{
public:
    std::vector<ClientInfo> runningClients; ///< Подключенные в данный момент клиенты.
    int clientCount { 0 }; ///< Число клиентов, подключенных в данный момент.
    int totalCommandCount { 0 }; ///< Общее количество команд, выполненных сервером с момента запуска.

    void parse(ServerResponse &response);
};

//=========================================================

/// \brief Подполе записи.
class PLUSIRBIS_EXPORTS SubField final
{
public:
    /// \brief Отсутствующий код подполя.
    const static Char NoCode { L'\0' };

    /// \brief Одноисмвольный код подполя.
    Char code { L'\0' };
    String value; ///< Значение подполя (может быть пустой строкой).

    SubField clone() const;
    void decode(const String &line);
    bool empty() const noexcept;
    bool verify(bool throwOnError) const;
    String toString() const;

    friend PLUSIRBIS_EXPORTS std::wostream& operator << (std::wostream &stream, const SubField &subfield);
};

//=========================================================

/// \brief Данные для метода printTable.
class PLUSIRBIS_EXPORTS TableDefinition final
{
public:
    String database;        ///< Имя базы данных.
    String table;           ///< Имя таблицы.
    StringList headers;     ///< Заголовки таблицы.
    String mode;            ///< Режим таблицы.
    String searchQuery;     ///< Поисковый запрос.
    int minMfn { 0 };       ///< Минимальный MFN.
    int maxMfn { 0 };       ///< Максимальный MFN.
    String sequentialQuery; ///< Запрос для последовательного поиска.
    MfnList mfnList;        ///< Список MFN, по которым строится таблица.
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
    void send(const Byte *buffer, size_t size) override;
    size_t receive(Byte *buffer, size_t size) override;
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
    String database;
    int numberOfTerms { 0 };
    bool reverseOrder { false };
    String startTerm;
    String format;
};

//=========================================================

class PLUSIRBIS_EXPORTS TermPosting final
{
public:
    int mfn { 0 };
    int tag { 0 };
    int occurrence { 0 };
    int count { 0 };
    String text;

    static std::vector<TermPosting> parse(const StringList &lines);
    std::wstring toString() const;
};

//=========================================================

class PLUSIRBIS_EXPORTS TextNavigator final
{
private:
    size_t _column, _length, _line, _position;
    const String &_text;

public:
    const static Char EOT;

    explicit TextNavigator(const String &text);
    TextNavigator(const TextNavigator &other);
    TextNavigator(TextNavigator&&) = delete;
    TextNavigator& operator = (const TextNavigator &) = delete;
    TextNavigator& operator = (TextNavigator &&) = delete;
    ~TextNavigator() = default;

    inline size_t column() const noexcept { return this->_column; }
    inline size_t line() const noexcept { return this->_line; }
    inline size_t length() const noexcept { return this->_length; }
    inline size_t position() const noexcept { return this->_position; }
    inline bool eot() const noexcept { return this->_position >= this->_length; }

    Char at(size_t position) const;
    Char front() const;
    Char back() const;
    Char lookAhead(ptrdiff_t distance = 1) const;
    Char lookBehind(ptrdiff_t distance = 1) const;
    TextNavigator& move(ptrdiff_t distance);
    Char peekChar() const;
    Char readChar();
    std::wstring peekString(size_t length);
    std::wstring peekTo(Char stopChar);
    std::wstring peekUntil(Char stopChar);
    std::wstring readLine();
    bool isControl() const;
    bool isDigit() const;
    bool isLetter() const;
    bool isWhitespace() const;
    std::wstring readInteger();
    std::wstring readString(size_t length);
    std::wstring readTo(Char stopChar);
    std::wstring readUntil(Char stopChar);
    std::wstring readWhile(Char goodChar);
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

bool PLUSIRBIS_EXPORTS sameChar(Char first, Char second);
bool PLUSIRBIS_EXPORTS sameString(const String &first, const String &second);

String PLUSIRBIS_EXPORTS toLower(String &text);
String PLUSIRBIS_EXPORTS toUpper(String &text);

bool PLUSIRBIS_EXPORTS contains(const String &text, const String &fragment);
bool PLUSIRBIS_EXPORTS contains(const String &text, Char c);

std::string PLUSIRBIS_EXPORTS replace(const std::string &text, const std::string &from, const std::string &to);
String PLUSIRBIS_EXPORTS replace(const String &text, const String &from, const String &to);

String PLUSIRBIS_EXPORTS trimStart(const String &text);
String PLUSIRBIS_EXPORTS trimEnd(const String &text);
String PLUSIRBIS_EXPORTS trim(const String &text);

String PLUSIRBIS_EXPORTS describeError(int errorCode);

int PLUSIRBIS_EXPORTS fastParse32(const String &text);
int PLUSIRBIS_EXPORTS fastParse32(const Char *text);
int PLUSIRBIS_EXPORTS fastParse32(const Char *text, size_t length);
int PLUSIRBIS_EXPORTS fastParse32(const std::string &text);
int PLUSIRBIS_EXPORTS fastParse32(const char *text);
int PLUSIRBIS_EXPORTS fastParse32(const char *text, size_t length);

unsigned int PLUSIRBIS_EXPORTS fastParseUnsigned32(const String &text);
unsigned int PLUSIRBIS_EXPORTS fastParseUnsigned32(const Char *text);
unsigned int PLUSIRBIS_EXPORTS fastParseUnsigned32(const Char *text, size_t length);
unsigned int PLUSIRBIS_EXPORTS fastParseUnsigned32(const std::string &text);
unsigned int PLUSIRBIS_EXPORTS fastParseUnsigned32(const char *text);
unsigned int PLUSIRBIS_EXPORTS fastParseUnsigned32(const char *text, size_t length);

PLUSIRBIS_EXPORTS const std::string& iif(const std::string &s1, const std::string &s2);
PLUSIRBIS_EXPORTS const String& iif(const String &s1, const String &s2);
PLUSIRBIS_EXPORTS const std::string& iif(const std::string &s1, const std::string &s2, const std::string &s3);
PLUSIRBIS_EXPORTS const String& iif(const String& s1, const String &s2, const String &s3);

PLUSIRBIS_EXPORTS std::wstring safeAt(const StringList &list, size_t index);

PLUSIRBIS_EXPORTS std::vector<std::string> split(const std::string &text, char delimiter);
PLUSIRBIS_EXPORTS StringList split(const std::wstring &text, Char delimiter);
PLUSIRBIS_EXPORTS std::vector<std::string> split(const std::string &text, const std::string &delimiter);
PLUSIRBIS_EXPORTS StringList split(const std::wstring &text, const std::wstring &delimiter);
PLUSIRBIS_EXPORTS StringList maxSplit(const std::wstring &text, Char separator, int count);

PLUSIRBIS_EXPORTS std::wstring cp866_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::wstring cp1251_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::wstring koi8r_to_unicode(const std::string &text);

PLUSIRBIS_EXPORTS std::string unicode_to_cp866(const std::wstring &text);
PLUSIRBIS_EXPORTS std::string unicode_to_cp1251(const std::wstring &text);
PLUSIRBIS_EXPORTS void unicode_to_cp1251(Byte *dst, const Char *src, size_t size);
PLUSIRBIS_EXPORTS std::string unicode_to_koi8r(const std::wstring &text);

PLUSIRBIS_EXPORTS Byte* toUtf(Byte *dst, const Char *src, size_t length);
PLUSIRBIS_EXPORTS Char* fromUtf(Char *dst, const Byte *src, size_t length);
PLUSIRBIS_EXPORTS size_t countUtf(const Char *src, size_t length);
PLUSIRBIS_EXPORTS size_t countUtf(const Byte *src, size_t length);
PLUSIRBIS_EXPORTS const Byte* fromUtf(const Byte *src, size_t &size, Byte stop, std::wstring &result);
PLUSIRBIS_EXPORTS Byte* toUtf(Byte *dst, const std::wstring &text);
PLUSIRBIS_EXPORTS std::wstring fromUtf(const std::string &text);
PLUSIRBIS_EXPORTS std::string toUtf(const std::wstring &text);

PLUSIRBIS_EXPORTS String removeComments(const String &text);
PLUSIRBIS_EXPORTS String prepareFormat(const String &text);

}

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif


