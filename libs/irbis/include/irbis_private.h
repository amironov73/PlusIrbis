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

/// \brief Простая освобождалка памяти.
template <typename T>
class IRBIS_API PointerGuard final
{
public:
    PointerGuard (T *ptr, bool needFree = true) : _pointer(ptr), _needFree(needFree) {}; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    PointerGuard (const PointerGuard &other) = delete;
    PointerGuard (PointerGuard &&other) = delete;
    PointerGuard& operator = (const PointerGuard &other) = delete;
    PointerGuard& operator = (PointerGuard &&other) = delete;
    ~PointerGuard() { if (this->_needFree) delete this->_pointer; }

    /// \brief Получение "сырого" указателя.
    /// \return "Сырой" указатель.
    T* pointer() { return this->_pointer; }

    /// \brief Доступ по индексу.
    /// \param offset Индекс.
    /// \return Ссылка на значение по индексу.
    T& operator[] (std::ptrdiff_t offset) { return this->_pointer[offset]; }

private:
    T* _pointer;
    bool _needFree;
};

//=========================================================

/// \brief Простая обертка над файлом.
class IRBIS_API File final
{
public:

    File (FILE *stream); // NOLINT(google-explicit-constructor)
    File (const char *name, const char *mode);
    File (const std::string &name, const std::string &mode);
    File (const String &name, const String &mode);
    File (const File &other) = delete;
    File (File &&other) = delete;
    File& operator = (const File &other) = delete;
    File& operator = (File &&other) = delete;
    ~File();

    FILE* getStream() const noexcept;
    bool eof() const;

    std::size_t read (Byte *buffer, std::size_t count);
    int64_t seek (int64_t position);
    int64_t tell();
    std::size_t write (const Byte *buffer, std::size_t count);
    int readChar ();
    std::string readLine ();
    static std::string readAllBytes (const String &fileName);

private:

    FILE *_stream;
};

//=========================================================

class IRBIS_API ChunkedBuffer final
{
private:
    MemoryChunk *_first, *_current, *_last;
    std::size_t _chunkSize, _position, _read;

    bool _advance();
    void _appendChunk();

public:
    const static std::size_t DefaultChunkSize;

    explicit ChunkedBuffer (std::size_t chunkSize = DefaultChunkSize);
    ChunkedBuffer (const ChunkedBuffer &) = delete;
    ChunkedBuffer (ChunkedBuffer &&) = delete;
    ChunkedBuffer operator = (const ChunkedBuffer &) = delete;
    ChunkedBuffer operator = (ChunkedBuffer &&) = delete;
    ~ChunkedBuffer();

    bool eof() const;
    int peek();
    std::size_t read (char *buffer, std::size_t offset, std::size_t count);
    int readByte();
    //std::wstring readLine(QTextCodec *codec);
    void rewind();
    void write (const char *buffer, std::size_t offset, std::size_t count);
    //void write(const std::wstring &text, QTextCodec *codec);
    void writeByte (char value);
};

//=========================================================

/// \brief Абстрактный клиентский сокет.
///
/// Наследники обязательно должны переопределить методы send и receive.
/// Объекты данного класса неперемещаемые.
class IRBIS_API ClientSocket // abstract
{
public:
    String host { L"localhost" }; ///< Адрес сервера в виде строки.
    short port { 6666 }; ///< Номер порта сервера.

    ClientSocket() = default;
    ClientSocket (ClientSocket &) = delete;
    ClientSocket (ClientSocket &&) = delete;
    ClientSocket& operator = (ClientSocket &) = delete;
    ClientSocket& operator = (ClientSocket &&) = delete;
    virtual ~ClientSocket() = default;

    virtual void open();
    virtual void close();

    /// \brief Отсылка данных на сервер.
    /// \param buffer Указатель на буфер с данными.
    /// \param size Размер данных в байтах.
    virtual void send (const Byte *buffer, std::size_t size) = 0;

    /// \brief Получение отвера от сервера (частями).
    /// \param buffer Буфер для размещения полученного ответа.
    /// \param size Размер буфера в байтах.
    /// \return Количество полученных данных в байтах.
    /// Отрицательное число означает ошибку.
    /// 0 означает, что сервер закончил передачу данных.
    /// Положительное число означает, что приём продолжается,
    /// и вызов нужно будет повторить для получения следующей порции.
    virtual std::size_t receive (Byte *buffer, std::size_t size) = 0;
};

//=========================================================

/// \brief Клиентский запрос.
///
/// Объекты данного класса неперемещаемые.
class IRBIS_API ClientQuery final
{
    std::vector<Byte> _content;

    void _write (const Byte *bytes, std::size_t size);
    void _write (Byte byte);

public:
    ClientQuery (const ConnectionBase &connection, const std::string &commandCode);
    ClientQuery (ClientQuery &) = delete;
    ClientQuery (ClientQuery &&) = delete;
    ClientQuery& operator = (ClientQuery &) = delete;
    ClientQuery& operator = (ClientQuery &&) = delete;
    ~ClientQuery() = default;

    ClientQuery& add (int value);
    ClientQuery& add (const FileSpecification &specification);
    ClientQuery& add (const MarcRecord &record, const std::wstring &delimiter);
    ClientQuery& addAnsi (const std::string &text);
    ClientQuery& addAnsi (const String &text);
    bool addFormat (const String &format);
    ClientQuery& addUtf (const String &text);
    void dump (std::ostream &stream) const;
    Bytes encode() const;
    ClientQuery& newLine();
};

//=========================================================

/// \brief Утилиты для ввода-вывода.
class IRBIS_API IO final
{
public:

    IO() = delete; // it's a static class

    static String       combinePath               (const String &path1, const String &path2);
    static std::string  combinePath               (const std::string &path1, const std::string &path2);
    static String&      convertSlashes            (String &path)      noexcept;
    static std::string& convertSlashes            (std::string &path) noexcept;
    static void         createDirectory           (const String &dir, bool createNew = false);
    static void         createDirectory           (const std::string &dir, bool createNew = false);
    static void         createFile                (const String &path, bool createNew = false);
    static void         createFile                (const std::string &path, bool createNew = false);
    static void         deleteFile                (const String &path);
    static void         deleteFile                (const std::string &path);
    static bool         directoryExist            (const String &path);
    static bool         directoryExist            (const std::string &path);
    static bool         fileExist                 (const String &path);
    static bool         fileExist                 (const std::string &path);
    static String       getCurrentDirectory       ();
    static std::string  getCurrentDirectoryNarrow ();
    static String       getDirectory              (const String &path);
    static std::string  getDirectory              (const std::string &path);
    static String       getExtension              (const String &path);
    static std::string  getExtension              (const std::string &path);
    static String       getFileName               (const String &path);
    static std::string  getFileName               (const std::string &path);
    static uint64_t     getFileSize               (const String &path);
    static uint64_t     getFileSize               (const std::string &path);
    static String       getTempDirectory          ();
    static std::string  getTempDirectoryNarrow    ();
    static bool         readInt32                 (FILE* file, uint32_t *value);
    static bool         readInt64                 (FILE* file, uint64_t *value);
    static bool         removeDirectory           (const String &path);
    static bool         removeDirectory           (const std::string &path);
    static void         setCurrentDirectory       (const String &dir);
    static void         setCurrentDirectory       (const std::string &dir);
    static String&      trimLeadingSlashes        (String &path);
    static std::string& trimLeadingSlashes        (std::string &path);
    static String&      trimTrailingSlashes       (String &path);
    static std::string& trimTrailingSlashes       (std::string &path);
    static bool         writeInt32                (FILE* file, uint32_t value);
    static bool         writeInt64                (FILE* file, uint64_t value);
};

//=========================================================

/// \brief Специфичная для ИРБИС работа с текстом.
class IRBIS_API Text final
{
public:
    const static char CrLf[];
    const static char Lf[];
    const static String IrbisDelimiter;
    const static String ShortDelimiter;
    const static String MsDosDelimiter;
    const static String UnixDelimiter;
    const static String SearchDelimiter;

    static String fromIrbisToDos (String &text);
    static std::string fromIrbisToUnix (std::string &text);
    static String fromIrbisToUnix (String &text);
    static String fromDosToIrbis (String &text);
    static String fromUnixToIrbis (String &text);
    static String fromDosToUnix (String &text);
    static String fromUnixToDos (String &text);
    static StringList fromFullDelimiter (const String &text);
    static StringList fromShortDelimiter (const String &text);
    static String readAllAnsi (const String &filename);
    static String readAllUtf (const String &filename);
    static StringList readAnsiLines (const String &filename);
    static StringList readUtfLines (const String &filename);
};

//=========================================================

class IRBIS_API MemoryChunk final
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
class IRBIS_API ServerResponse final
{
public:
    String command;       ///< Код команды (дублирует клиентский запрос).
    int clientId;         ///< Идентификатор клиента (дублирует клиентский запрос).
    int queryId;          ///< Номер команды (дублирует клиентский запрос).
    int answerSize;       ///< Размер ответа сервера в байтах (в некоторых сценариях отсутствует).
    int returnCode;       ///< Код возврата (бывает не у всех ответов).
    String serverVersion; ///< Версия сервера (в некоторых сценариях отсутствует).

    ServerResponse (ConnectionBase &connection, ClientQuery &query);
    ServerResponse (ServerResponse &) = delete;
    ServerResponse (ServerResponse &&) = delete;
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
    std::vector<std::string> readRemainingLinesUtf();
    String readRemainingUtfText();
    String readUtf();

private:
    ConnectionBase *_connection;
    bool _success;
    std::size_t _position;
    Bytes _content;
    void _write(const Byte *bytes, std::size_t size);
};

//=========================================================

class IRBIS_API Tcp4Socket final
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

/// \brief Навигация по диапазону байт.
class IRBIS_API ByteNavigator final
{
public:
    const static int EOT;

    ByteNavigator() noexcept;
    ByteNavigator (ByteSpan data) noexcept; // NOLINT(google-explicit-constructor)
    ByteNavigator (const ByteNavigator &other)             = default;
    ByteNavigator (ByteNavigator &&other)                  = default;
    ByteNavigator& operator = (const ByteNavigator &other) = default;
    ByteNavigator& operator = (ByteNavigator &&other)      = default;

    int         back()     const noexcept;
    Byte*       begin()    const noexcept;
    const Byte* cbegin()   const noexcept;
    const Byte* ccurrent() const noexcept;
    const Byte* cend()     const noexcept;
    Byte*       current()  const noexcept;
    Byte*       end()      const noexcept;
    bool        eot()      const noexcept;
    int         front()    const noexcept;
    std::size_t position() const noexcept;
    std::size_t size()     const noexcept;

    int            at              (std::size_t position)        const noexcept;
    ByteNavigator& fill            (Byte value)                        noexcept;
    int            find            (Byte value)                  const noexcept;
    int            find            (ByteSpan array)              const noexcept;
    bool           isControl       ()                            const noexcept;
    bool           isDigit         ()                            const noexcept;
    bool           isLetter        ()                            const noexcept;
    bool           isWhitespace    ()                            const noexcept;
    int            lookAhead       (std::ptrdiff_t distance = 1) const noexcept;
    int            lookBehind      (std::ptrdiff_t distance = 1) const noexcept;
    ByteNavigator& move            (std::ptrdiff_t distance)           noexcept;
    int            peekByte        ()                            const noexcept;
    ByteSpan       peekString      (std::size_t length)          const noexcept;
    ByteSpan       peekTo          (Byte stop)                   const noexcept;
    ByteSpan       peekUntil       (Byte stop)                   const noexcept;
    int            peekUtf         ()                            const noexcept;
    int            peekWideBE      ()                            const noexcept;
    int            peekWideLE      ()                            const noexcept;
    int            readByte        ()                                  noexcept;
    ByteSpan       readInteger     ()                                  noexcept;
    ByteSpan       readLine        ()                                  noexcept;
    ByteSpan       readString      (std::size_t length)                noexcept;
    ByteSpan       readTo          (Byte stopByte)                     noexcept;
    ByteSpan       readUntil       (Byte stopByte)                     noexcept;
    int            readUtf         ()                                  noexcept;
    ByteSpan       readWhile       (Byte goodByte)                     noexcept;
    int            readWideBE      ()                                  noexcept;
    int            readWideLE      ()                                  noexcept;
    ByteSpan       readWord        ()                                  noexcept;
    ByteSpan       recent          (std::ptrdiff_t length)       const noexcept;
    ByteSpan       remaining       ()                            const noexcept;
    ByteNavigator& skipWhitespace  ()                                  noexcept;
    ByteNavigator& skipPunctuation ()                                  noexcept;
    ByteNavigator& skipLine        ()                                  noexcept;
    ByteSpan       slice           (std::ptrdiff_t offset, std::size_t length) const noexcept;
    bool           writeByte       (Byte byte)                         noexcept;
    bool           writeUtf        (unsigned int c)                    noexcept;
    bool           writeUtf        (WideSpan text)                     noexcept;
    bool           writeWideLE     (Char c)                            noexcept;
    bool           writeWideBE     (Char c)                            noexcept;
    bool           writeWideLE     (WideSpan text)                     noexcept;
    bool           writeWideBE     (WideSpan text)                     noexcept;

private:
    ByteSpan _data;
    std::size_t _position { 0u };
};

//=========================================================

/// \brief Навигация по тексту.
class IRBIS_API TextNavigator final
{
public:
    const static Char EOT;

    TextNavigator (const Char* text, std::size_t length) noexcept;
    TextNavigator (WideSpan text) noexcept; // NOLINT(google-explicit-constructor)
    TextNavigator (const String &text) noexcept; // NOLINT(google-explicit-constructor)
    TextNavigator (const TextNavigator &other) noexcept;
    TextNavigator (TextNavigator&&) = delete;
    TextNavigator& operator = (const TextNavigator &) = delete;
    TextNavigator& operator = (TextNavigator &&) = delete;
    ~TextNavigator() = default;

    std::size_t column()   const noexcept;
    std::size_t line()     const noexcept;
    std::size_t length()   const noexcept;
    std::size_t position() const noexcept;
    Char* begin()          const noexcept;
    const Char* cbegin()   const noexcept;
    Char* current()        const noexcept;
    const Char* ccurrent() const noexcept;
    Char* end()            const noexcept;
    const Char* cend()     const noexcept;
    bool eot()             const noexcept;
    Char front()           const noexcept;
    Char back()            const noexcept;

    Char at (std::size_t position) const noexcept;
    Char lookAhead (std::ptrdiff_t distance = 1) const noexcept;
    Char lookBehind (std::ptrdiff_t distance = 1) const noexcept;
    TextNavigator& move (std::ptrdiff_t distance) noexcept;
    Char peekChar() const noexcept;
    Char readChar() noexcept;
    WideSpan peekString (std::size_t length) const noexcept;
    WideSpan peekTo (Char stopChar) const noexcept;
    WideSpan peekUntil (Char stopChar) const noexcept;
    WideSpan readLine() noexcept;
    bool isControl() const noexcept;
    bool isDigit() const noexcept;
    bool isLetter() const noexcept;
    bool isWhitespace() const noexcept;
    WideSpan readInteger() noexcept;
    WideSpan readString (std::size_t length) noexcept;
    WideSpan readTo (Char stopChar) noexcept;
    WideSpan readUntil (Char stopChar) noexcept;
    WideSpan readWhile (Char goodChar) noexcept;
    WideSpan readWord () noexcept;
    WideSpan recentText (std::ptrdiff_t length) const noexcept;
    WideSpan remainingText() const noexcept;
    TextNavigator& skipWhitespace() noexcept;
    TextNavigator& skipPunctuation() noexcept;
    WideSpan substr (std::ptrdiff_t offset, std::size_t length) const noexcept;

private:
    std::size_t _column, _length, _line, _position;
    const Char *_text;
};

//=========================================================

// Utilities

IRBIS_API bool sameChar   (Char first, Char second) noexcept;
IRBIS_API bool sameString (const String &first, const String &second) noexcept;

IRBIS_API String toLower      (String &text)      noexcept;
IRBIS_API std::string toLower (std::string &text) noexcept;
IRBIS_API String toUpper      (String &text)      noexcept;
IRBIS_API std::string toUpper (std::string &text) noexcept;

IRBIS_API bool contains (const String &text, const String &fragment);
IRBIS_API bool contains (const String &text, Char c);

IRBIS_API std::string replace (const std::string &text, const std::string &from, const std::string &to);
IRBIS_API String replace      (const String &text, const String &from, const String &to);

IRBIS_API String trimStart (const String &text);
IRBIS_API String trimEnd   (const String &text);
IRBIS_API String trim      (const String &text);

IRBIS_API int fastParse32 (const String &text);
IRBIS_API int fastParse32 (CharSpan text);
IRBIS_API int fastParse32 (WideSpan text);
IRBIS_API int fastParse32 (const Char *text);
IRBIS_API int fastParse32 (const Char *text, std::size_t length);
IRBIS_API int fastParse32 (const std::string &text);
IRBIS_API int fastParse32 (const char *text);
IRBIS_API int fastParse32 (const char *text, std::size_t length);

IRBIS_API unsigned int fastParseUnsigned32 (const String &text);
IRBIS_API unsigned int fastParseUnsigned32 (const Char *text);
IRBIS_API unsigned int fastParseUnsigned32 (const Char *text, std::size_t length);
IRBIS_API unsigned int fastParseUnsigned32 (const std::string &text);
IRBIS_API unsigned int fastParseUnsigned32 (const char *text);
IRBIS_API unsigned int fastParseUnsigned32 (const char *text, std::size_t length);

IRBIS_API const std::string& iif(const std::string &s1, const std::string &s2);
IRBIS_API const String& iif(const String &s1, const String &s2);
IRBIS_API const std::string& iif(const std::string &s1, const std::string &s2, const std::string &s3);
IRBIS_API const String& iif(const String& s1, const String &s2, const String &s3);

IRBIS_API std::string safeAt (const std::vector<std::string> &list, std::size_t index);
IRBIS_API String safeAt (const StringList &list, std::size_t index);

IRBIS_API std::vector<std::string> split(const std::string &text, char delimiter);
IRBIS_API StringList split(const String &text, Char delimiter);
IRBIS_API std::vector<std::string> split(const std::string &text, const std::string &delimiter);
IRBIS_API StringList split(const String &text, const String &delimiter);
IRBIS_API std::vector<std::string> maxSplit(const std::string &text, char separator, int count);
IRBIS_API StringList maxSplit(const String &text, Char separator, int count);

IRBIS_API String cp866_to_unicode(const std::string &text);
IRBIS_API String cp1251_to_unicode(const std::string &text);
IRBIS_API String koi8r_to_unicode(const std::string &text);

IRBIS_API std::string unicode_to_cp866(const String &text);
IRBIS_API std::string unicode_to_cp1251(const String &text);
IRBIS_API void unicode_to_cp1251(Byte *dst, const Char *src, std::size_t size);
IRBIS_API std::string unicode_to_koi8r(const String &text);

IRBIS_API std::string narrow(const String &wide, const std::locale &loc);
IRBIS_API String widen(const std::string &str, const std::locale &loc);
IRBIS_API String new_cp1251_to_unicode(const std::string &text);
IRBIS_API std::string new_unicode_to_cp1251(const String &text);
IRBIS_API std::string new_toUtf(const String &text);
IRBIS_API String new_fromUtf(const std::string &text);

IRBIS_API Byte* toUtf (Byte *dst, const Char *src, std::size_t length);
IRBIS_API Char* fromUtf (Char *dst, const Byte *src, std::size_t length);
IRBIS_API std::size_t countUtf (const Char *src, std::size_t length);
IRBIS_API std::size_t countUtf (const Byte *src, std::size_t length);
IRBIS_API const Byte* fromUtf (const Byte *src, std::size_t &size, Byte stop, String &result);
IRBIS_API Byte* toUtf (Byte *dst, const String &text);
IRBIS_API String fromUtf (const std::string &text);
IRBIS_API std::string toUtf (const String &text);
IRBIS_API String fromUtf (ByteSpan span);

IRBIS_API String removeComments (const String &text);
IRBIS_API String prepareFormat (const String &text);

template<typename T>
bool isDigit(T c)  { return (c >= '0') && (c <= '9'); }

}

//=========================================================

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif

// IRBIS_PRIVATE_H
#endif
