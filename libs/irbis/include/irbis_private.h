// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef IRBIS_PRIVATE_H
#define IRBIS_PRIVATE_H

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(push)
    #pragma warning(disable: 4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #pragma warning(disable: 4275) // non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'

#endif

namespace irbis
{

//=========================================================

/// \brief Простая обертка над файлом.
class PLUSIRBIS_EXPORTS File final
{
public:

    File (FILE *stream);
    File (const char *name, const char *mode);
    File (const std::string &name, const std::string &mode);
    File (const String &name, const String &mode);
    File (const File &other) = delete;
    File (File &&other) = delete;
    File& operator = (const File &other) = delete;
    File& operator = (File &&other) = delete;
    ~File();

    FILE* getStream() const noexcept { return this->_stream; }

    std::size_t read (Byte *buffer, std::size_t count);
    int64_t seek (int64_t position);
    int64_t tell();
    std::size_t write (const Byte *buffer, std::size_t count);

private:

    FILE *_stream;
};

//=========================================================

class PLUSIRBIS_EXPORTS ChunkedBuffer final
{
private:
    MemoryChunk *_first, *_current, *_last;
    std::size_t _chunkSize, _position, _read;

    bool _advance();
    void _appendChunk();

public:
    const static std::size_t DefaultChunkSize;

    explicit ChunkedBuffer(std::size_t chunkSize = DefaultChunkSize);
    ChunkedBuffer(const ChunkedBuffer &) = delete;
    ChunkedBuffer(ChunkedBuffer &&) = delete;
    ChunkedBuffer operator = (const ChunkedBuffer &) = delete;
    ChunkedBuffer operator = (ChunkedBuffer &&) = delete;
    ~ChunkedBuffer();

    bool eof() const;
    int peek();
    std::size_t read(char *buffer, std::size_t offset, std::size_t count);
    int readByte();
    //std::wstring readLine(QTextCodec *codec);
    void rewind();
    void write(const char *buffer, std::size_t offset, std::size_t count);
    //void write(const std::wstring &text, QTextCodec *codec);
    void writeByte(char value);
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
    virtual void send(const Byte *buffer, std::size_t size) = 0;

    /// \brief Получение отвера от сервера (частями).
    /// \param buffer Буфер для размещения полученного ответа.
    /// \param size Размер буфера в байтах.
    /// \return Количество полученных данных в байтах.
    /// Отрицательное число означает ошибку.
    /// 0 означает, что сервер закончил передачу данных.
    /// Положительное число означает, что приём продолжается,
    /// и вызов нужно будет повторить для получения следующей порции.
    virtual std::size_t receive(Byte *buffer, std::size_t size) = 0;
};

//=========================================================

/// \brief Клиентский запрос.
///
/// Объекты данного класса неперемещаемые.
class PLUSIRBIS_EXPORTS ClientQuery final
{
    std::vector<Byte> _content;

    void _write(const Byte *bytes, std::size_t size);
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

/// \brief Утилиты для ввода-вывода.
class PLUSIRBIS_EXPORTS IO final
{
public:

    IO() = delete;

    static bool readInt32 (FILE* file, uint32_t *value);
    static bool readInt64 (FILE* file, uint64_t *value);
    static bool writeInt32 (FILE* file, uint32_t value);
    static bool writeInt64 (FILE* file, uint64_t value);
    static void createDirectory (const String &dir, bool createNew = false);
    static void createDirectory (const std::string &dir, bool createNew = false);
    static String getCurrentDirectory();
    static std::string getCurrentDirectoryNarrow();
    static void setCurrentDirectory (const String &dir);
    static void setCurrentDirectory (const std::string &dir);
    static String getExtension (const String &path);
    static std::string getExtension (const std::string &path);
    static String getFileName (const String &path);
    static std::string getFileName (const std::string &path);
    static String getDirectory (const String &path);
    static std::string getDirectory (const std::string &path);
    static String& convertSlashes (String &path) noexcept;
    static std::string& convertSlashes (std::string &path) noexcept;
    static String combinePath (const String &path1, const String &path2);
    static std::string combinePath (const std::string &path1, const std::string &path2);
    static bool directoryExist (const String &path);
    static bool directoryExist (const std::string &path);
    static bool fileExist (const String &path);
    static bool fileExist (const std::string &path);
    static void deleteFile (const String &path);
    static void deleteFile (const std::string &path);
    static void createFile (const String &path, bool createNew = false);
    static void createFile (const std::string &path, bool createNew = false);
    static String getTempDirectory();
    static std::string getTempDirectoryNarrow();
    static String& trimLeadingSlashes (String &path);
    static std::string& trimLeadingSlashes (std::string &path);
    static String& trimTrailingSlashes (String &path);
    static std::string& trimTrailingSlashes (std::string &path);
    static uint64_t getFileSize (const String &path);
    static uint64_t getFileSize (const std::string &path);
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

class PLUSIRBIS_EXPORTS MemoryChunk final
{
public:
    char *data;
    MemoryChunk *next;

    explicit MemoryChunk(std::size_t size);
    MemoryChunk(const MemoryChunk &other) = delete;
    MemoryChunk(MemoryChunk &&other) = delete;
    MemoryChunk& operator = (const MemoryChunk &other) = delete;
    MemoryChunk& operator = (MemoryChunk &&other) = delete;
    ~MemoryChunk();
};

//=========================================================

/// \brief Ответ сервера на клиентский запрос.
class PLUSIRBIS_EXPORTS ServerResponse final
{
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

private:
    Connection *_connection;
    bool _success;
    std::size_t _position;
    Bytes _content;
    void _write(const Byte *bytes, std::size_t size);
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
    void send(const Byte *buffer, std::size_t size) override;
    std::size_t receive(Byte *buffer, std::size_t size) override;
};

//=========================================================

// Utilities

PLUSIRBIS_EXPORTS bool sameChar(Char first, Char second) noexcept;
PLUSIRBIS_EXPORTS bool sameString(const String &first, const String &second) noexcept;

PLUSIRBIS_EXPORTS String toLower(String &text) noexcept;
PLUSIRBIS_EXPORTS String toUpper(String &text) noexcept ;

PLUSIRBIS_EXPORTS bool contains(const String &text, const String &fragment);
PLUSIRBIS_EXPORTS bool contains(const String &text, Char c);

PLUSIRBIS_EXPORTS std::string replace(const std::string &text, const std::string &from, const std::string &to);
PLUSIRBIS_EXPORTS String replace(const String &text, const String &from, const String &to);

PLUSIRBIS_EXPORTS String trimStart(const String &text);
PLUSIRBIS_EXPORTS String trimEnd(const String &text);
PLUSIRBIS_EXPORTS String trim(const String &text);

PLUSIRBIS_EXPORTS int fastParse32(const String &text);
PLUSIRBIS_EXPORTS int fastParse32(CharSpan text);
PLUSIRBIS_EXPORTS int fastParse32(WideSpan text);
PLUSIRBIS_EXPORTS int fastParse32(const Char *text);
PLUSIRBIS_EXPORTS int fastParse32(const Char *text, std::size_t length);
PLUSIRBIS_EXPORTS int fastParse32(const std::string &text);
PLUSIRBIS_EXPORTS int fastParse32(const char *text);
PLUSIRBIS_EXPORTS int fastParse32(const char *text, std::size_t length);

PLUSIRBIS_EXPORTS unsigned int fastParseUnsigned32(const String &text);
PLUSIRBIS_EXPORTS unsigned int fastParseUnsigned32(const Char *text);
PLUSIRBIS_EXPORTS unsigned int fastParseUnsigned32(const Char *text, std::size_t length);
PLUSIRBIS_EXPORTS unsigned int fastParseUnsigned32(const std::string &text);
PLUSIRBIS_EXPORTS unsigned int fastParseUnsigned32(const char *text);
PLUSIRBIS_EXPORTS unsigned int fastParseUnsigned32(const char *text, std::size_t length);

PLUSIRBIS_EXPORTS const std::string& iif(const std::string &s1, const std::string &s2);
PLUSIRBIS_EXPORTS const String& iif(const String &s1, const String &s2);
PLUSIRBIS_EXPORTS const std::string& iif(const std::string &s1, const std::string &s2, const std::string &s3);
PLUSIRBIS_EXPORTS const String& iif(const String& s1, const String &s2, const String &s3);

PLUSIRBIS_EXPORTS std::wstring safeAt(const StringList &list, std::size_t index);

PLUSIRBIS_EXPORTS std::vector<std::string> split(const std::string &text, char delimiter);
PLUSIRBIS_EXPORTS StringList split(const String &text, Char delimiter);
PLUSIRBIS_EXPORTS std::vector<std::string> split(const std::string &text, const std::string &delimiter);
PLUSIRBIS_EXPORTS StringList split(const String &text, const String &delimiter);
PLUSIRBIS_EXPORTS StringList maxSplit(const String &text, Char separator, int count);

PLUSIRBIS_EXPORTS String cp866_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS String cp1251_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS String koi8r_to_unicode(const std::string &text);

PLUSIRBIS_EXPORTS std::string unicode_to_cp866(const String &text);
PLUSIRBIS_EXPORTS std::string unicode_to_cp1251(const String &text);
PLUSIRBIS_EXPORTS void unicode_to_cp1251(Byte *dst, const Char *src, std::size_t size);
PLUSIRBIS_EXPORTS std::string unicode_to_koi8r(const String &text);

PLUSIRBIS_EXPORTS std::string narrow(const String &wide, const std::locale &loc);
PLUSIRBIS_EXPORTS String widen(const std::string &str, const std::locale &loc);
PLUSIRBIS_EXPORTS String new_cp1251_to_unicode(const std::string &text);
PLUSIRBIS_EXPORTS std::string new_unicode_to_cp1251(const String &text);
PLUSIRBIS_EXPORTS std::string new_toUtf(const String &text);
PLUSIRBIS_EXPORTS String new_fromUtf(const std::string &text);

PLUSIRBIS_EXPORTS Byte* toUtf (Byte *dst, const Char *src, std::size_t length);
PLUSIRBIS_EXPORTS Char* fromUtf (Char *dst, const Byte *src, std::size_t length);
PLUSIRBIS_EXPORTS std::size_t countUtf (const Char *src, std::size_t length);
PLUSIRBIS_EXPORTS std::size_t countUtf (const Byte *src, std::size_t length);
PLUSIRBIS_EXPORTS const Byte* fromUtf (const Byte *src, std::size_t &size, Byte stop, String &result);
PLUSIRBIS_EXPORTS Byte* toUtf (Byte *dst, const String &text);
PLUSIRBIS_EXPORTS String fromUtf (const std::string &text);
PLUSIRBIS_EXPORTS std::string toUtf (const String &text);
PLUSIRBIS_EXPORTS String fromUtf (ByteSpan span);

PLUSIRBIS_EXPORTS String removeComments (const String &text);
PLUSIRBIS_EXPORTS String prepareFormat (const String &text);

}

//=========================================================

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif

// IRBIS_PRIVATE_H
#endif
