// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef IRBIS_PRIVATE_H
#define IRBIS_PRIVATE_H

#include "irbis.h"

#include <array>

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(push)
    #pragma warning(disable: 4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #pragma warning(disable: 4275) // non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'

#endif

// ReSharper disable CommentTypo
// ReSharper disable IdentifierTypo

namespace irbis
{

//=========================================================

class File;

//=========================================================

/// \brief Простой контейнер для случаев,
/// когда предполагается, что элементов будет мало - один или два.
template <class T>
class Frugal
{
public:

    Frugal() = default; ///< Конструктор по умолчанию.
    Frugal (const Frugal &) = delete; ///< Конструктор копирования.
    Frugal (Frugal &&)      = delete; ///< Конструктор перемещения
    Frugal& operator = (const Frugal &) = delete; ///< Оператор копирования.
    Frugal& operator = (Frugal &&)      = delete; ///< Оператор перемещения.
    ~Frugal() ///< Деструктор
    {
        if (this->_array) {
            delete[] this->_array;
        }
    }

    /// \brief Итератор.
    class iterator : public std::iterator<std::input_iterator_tag, T>
    {
    public:
        iterator(Frugal<T> &container, std::size_t pos)
                : _container (container), _position (pos) {}

        bool operator == (const iterator &other) { return this->_position == other._position; }
        bool operator != (const iterator &other) { return this->_position != other._position; }
        iterator& operator ++() { ++this->_position; return *this; }
        T& operator *() { return this->_container[this->_position]; }
        T& operator ->() { return this->_container[this->_position]; }

    private:
        Frugal<T> &_container;
        std::size_t _position;
    };

    /// \brief Оператор индексирования.
    T& operator [] (std::size_t offset)
    {
        switch (offset) {
            case 0: return this->_first;
            case 1: return this->_second;
            default: return this->_array[offset - 2];
        }
    }

    /// \brief Указатель на первый элемент.
    iterator begin() { return iterator (*this, 0); }

    /// \brief Указатель за последним элементом.
    iterator end() { return iterator (*this, this->_size); }

    /// \brief Ссылка на первый элемент.
    T& front() { return (*this)[0]; }

    /// \brief Ссылка на последний элемент.
    T& back() { return (*this)[this->_size - 1]; }

    /// \brief Емкость контейнера.
    std::size_t capacity() const noexcept { return this->_capacity + 2; }

    /// \brief Очистка контейнера.
    void clear() noexcept { this->_size = 0; }

    /// \brief Проверка, не пустой ли контейнер.
    bool empty() const noexcept { return this->_size == 0; }

    /// \brief Текущий размер контейнера.
    std::size_t size() const noexcept  { return this->_size; }

    /// \brief Преобразование в стандартный вектор.
    std::vector<T> toVector()
    {
        std::vector<T> result;
        result.reserve (this->size());
        result.insert (std::end(result), std::begin(*this), std::end(*this));
        return result;
    }

    /// \brief Помещение элемента в конец.
    void push_back (const T &value)
    {
        switch (this->_size) {
            case 0: this->_first = value; break;
            case 1: this->_second = value; break;
            default:
                if (this->_size - 2 == this->_capacity) {
                    this->_enlarge();
                }
                this->_array[this->_size - 2] = value;
                break;
        }
        ++this->_size;
    }

    /// \brief Помещение элемента в конец.
    void push_back (T &&value)
    {
        switch (this->_size) {
            case 0: this->_first = std::move (value); break;
            case 1: this->_second = std::move (value); break;
            default:
                if (this->_size - 2 == this->_capacity) {
                    this->_enlarge();
                }
                this->_array[this->_size - 2] = std::move (value);
                break;
        }
        ++this->_size;
    }

    /// \brief Помещение элемента в конец
    template <class... Args>
    T& emplace_back (Args&&... args)
    {
        T * place;
        switch (this->_size) {
            case 0: place = &this->_first; break;
            case 1: place = &this->_second; break;
            default:
                if (this->_size - 2 == this->_capacity) {
                    this->_enlarge();
                }
                place = this->_array + this->_size - 2;
                break;
        }
        T *value = new (place) T(std::forward<Args> (args)...);
        ++this->_size;
        return *value;
    }

private:
    std::size_t _size { 0 };
    std::size_t _capacity { 0 };
    T *_array { nullptr };
    T _first {};
    T _second {};

    void _enlarge() {
        std::size_t  oldSize = this->_capacity;
        std::size_t newSize = oldSize * 2;
        if (newSize <= 4) {
            newSize = 4;
        }

        T *newArray = new T[newSize];
        if (this->_array) {
            std::copy (this->_array, this->_array + oldSize, newArray);
            delete[] this->_array;
        }

        this->_array = newArray;
        this->_capacity = newSize;
    }
};

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

/// \brief Буфер, состоящий из мелких блоков.
class IRBIS_API ChunkedBuffer final
{
public:

    /// \brief Блок памяти.
    class IRBIS_API Chunk final
    {
    public:
        Byte *data;
        Chunk *next;

        explicit Chunk (std::size_t size);
        Chunk (const Chunk &other)             = delete; ///< Конструктор копирования.
        Chunk (Chunk &&other)                  = delete; ///< Конструктор перемещения.
        Chunk& operator = (const Chunk &other) = delete; ///< Оператор копирования.
        Chunk& operator = (Chunk &&other)      = delete; ///< Оператор перемещения.
        ~Chunk();
    };

    const static std::size_t DefaultChunkSize;

    explicit ChunkedBuffer (std::size_t chunkSize = DefaultChunkSize);
    ChunkedBuffer (const ChunkedBuffer &)            = delete; ///< Конструктор копирования.
    ChunkedBuffer (ChunkedBuffer &&)                 = delete; ///< Конструктор перемещения.
    ChunkedBuffer operator = (const ChunkedBuffer &) = delete; ///< Оператор копирования.
    ChunkedBuffer operator = (ChunkedBuffer &&)      = delete; ///< Оператор перемещения.
    ~ChunkedBuffer();

    bool        empty           () const noexcept;
    bool        eof             () const noexcept;
    int         peek            ();
    std::size_t position        () const noexcept;
    std::size_t read            (Byte *buffer, std::size_t offset, std::size_t count);
    int         readByte        ();
    Bytes       readLine        ();
    String      readLine        (Encoding *encoding);
    Bytes       readRemaining   () const;
    std::size_t remainingLength () const noexcept;
    void        rewind          ();
    std::size_t size            () const noexcept;
    Bytes       toBytes         () const;
    void        write           (const Byte *buffer, std::size_t offset, std::size_t count);
    void        writeByte       (Byte value);
    void        writeLine       (const String &text, Encoding *encoding);

private:
    Chunk *_first, *_current, *_last;
    std::size_t _chunkSize, _position, _read;

    bool _advance();
    void _appendChunk();
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

    ClientQuery& add       (int value);
    ClientQuery& add       (const FileSpecification &specification);
    ClientQuery& add       (const MarcRecord &record, const std::wstring &delimiter);
    ClientQuery& addAnsi   (const std::string &text);
    ClientQuery& addAnsi   (const String &text);
    bool         addFormat (const String &format);
    ClientQuery& addUtf    (const String &text);
    void         dump      (std::ostream &stream) const;
    Bytes        encode    () const;
    ClientQuery& newLine   ();
};

//=========================================================

/// \brief Блок памяти из пула.
struct IRBIS_API MemoryBlock final
{
    void *pointer    { nullptr }; ///< Указатель на кусок памяти.
    std::size_t size { 0 };       ///< Размер блока.
};

/// \brief Пул памяти.
class IRBIS_API MemoryPool final
{
public:
    static MemoryPool& instance();
    MemoryBlock rent (std::size_t size);
    void release (MemoryBlock block);
    void reset ();

private:
    std::list<MemoryBlock> _used;
    std::list<MemoryBlock> _free;
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
    ServerResponse (ServerResponse &)              = delete;  ///< Конструктор копирования.
    ServerResponse (ServerResponse &&)             = delete;  ///< Конструктор перемещения.
    ~ServerResponse ()                             = default; ///< Деструктор.
    ServerResponse& operator = (ServerResponse &)  = delete;  ///< Оператор копирования.
    ServerResponse& operator = (ServerResponse &&) = delete;  ///< Оператор перемещения.

    bool                     checkReturnCode        ();
    bool                     checkReturnCode        (int nargs, ...);
    static ServerResponse*   emptyResonse           ();
    bool                     eot                    () const;
    std::string              getLine                ();
    std::string              getRemaining           ();
    int                      getReturnCode          ();
    String                   readAnsi               ();
    int                      readInteger            ();
    StringList               readRemainingAnsiLines ();
    String                   readRemainingAnsiText  ();
    StringList               readRemainingUtfLines  ();
    std::vector<std::string> readRemainingLinesUtf  ();
    String                   readRemainingUtfText   ();
    String                   readUtf                ();
    bool                     success                () const;

private:
    ConnectionBase *_connection;
    bool _success;
    std::size_t _position;
    Bytes _content;

    ServerResponse() = default; ///< Конструктор по умолчанию (для тестов)

    void _write(const Byte *bytes, std::size_t size);
};

//=========================================================

/// \brief Клиентский сокет, работающий по TCP/IP v4.
class IRBIS_API Tcp4Socket final
    : public ClientSocket
{
    struct TcpInternals;
    std::unique_ptr<TcpInternals> _impl;

public:
    explicit Tcp4Socket (const String &host=L"localhost", short port=6666); ///< Конструктор
    Tcp4Socket (const Tcp4Socket &) = delete; ///< Конструктор копирования.
    Tcp4Socket (Tcp4Socket &&)      = delete; ///< Конструктор перемещения.
    ~Tcp4Socket()                   override; ///< Деструктор.
    Tcp4Socket& operator = (const Tcp4Socket &) = delete; ///< Оператор коприрования.
    Tcp4Socket& operator = (Tcp4Socket &&)      = delete; ///< Оператор перемещения.

    void open () override;
    void close () override;
    void send (const Byte *buffer, std::size_t size) override;
    std::size_t receive(Byte *buffer, std::size_t size) override;
};

//=========================================================

/// \brief Навигация по диапазону байт.
class IRBIS_API ByteNavigator final
{
public:
    const static int EOT;

    ByteNavigator () noexcept;
    explicit ByteNavigator (ByteSpan data) noexcept;
    ByteNavigator (const ByteNavigator &)             = default; ///< Конструктор копирования.
    ByteNavigator (ByteNavigator &&)                  = default; ///< Конструктор перемещения.
    ByteNavigator& operator = (const ByteNavigator &) = default; ///< Оператор копирования.
    ByteNavigator& operator = (ByteNavigator &&)      = default; ///< Оператор присваивания.

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

    Char           at              (std::size_t position)        const noexcept;
    Char           back            ()                            const noexcept;
    Char*          begin           ()                            const noexcept;
    const Char*    cbegin          ()                            const noexcept;
    const Char*    ccurrent        ()                            const noexcept;
    const Char*    cend            ()                            const noexcept;
    std::size_t    column          ()                            const noexcept;
    Char*          current         ()                            const noexcept;
    Char*          end             ()                            const noexcept;
    bool           eot             ()                            const noexcept;
    WideSpan       extractInteger  ()                                  noexcept;
    Char           front           ()                            const noexcept;
    bool           isControl       ()                            const noexcept;
    bool           isDigit         ()                            const noexcept;
    bool           isLetter        ()                            const noexcept;
    bool           isWhitespace    ()                            const noexcept;
    std::size_t    length          ()                            const noexcept;
    std::size_t    line            ()                            const noexcept;
    Char           lookAhead       (std::ptrdiff_t distance = 1) const noexcept;
    Char           lookBehind      (std::ptrdiff_t distance = 1) const noexcept;
    TextNavigator& move            (std::ptrdiff_t distance)           noexcept;
    Char           peekChar        ()                            const noexcept;
    Char           peekCharNoCrLf  ()                            const noexcept;
    WideSpan       peekString      (std::size_t length)          const noexcept;
    WideSpan       peekTo          (Char stopChar)               const noexcept;
    WideSpan       peekUntil       (Char stopChar)               const noexcept;
    std::size_t    position        ()                            const noexcept;
    Char           readChar        ()                                  noexcept;
    Char           readCharNoCrLf  ()                                  noexcept;
    WideSpan       readLine        ()                                  noexcept;
    WideSpan       readInteger     ()                                  noexcept;
    WideSpan       readString      (std::size_t length)                noexcept;
    WideSpan       readTo          (Char stopChar)                     noexcept;
    WideSpan       readUntil       (Char stopChar)                     noexcept;
    WideSpan       readWhile       (Char goodChar)                     noexcept;
    WideSpan       readWord        ()                                  noexcept;
    WideSpan       recentText      (std::ptrdiff_t length)       const noexcept;
    WideSpan       remainingText   ()                            const noexcept;
    TextNavigator& skipNonWord     ()                                  noexcept;
    TextNavigator& skipPunctuation ()                                  noexcept;
    TextNavigator& skipWhitespace  ()                                  noexcept;
    WideSpan       substr          (std::ptrdiff_t offset, std::size_t length) const noexcept;

private:
    std::size_t _column, _length, _line, _position;
    const Char *_text;
};

//=========================================================

// Logging

/// \brief Поддержка логирования.
class IRBIS_API Log
{
public:
    static void write (const String &text);
    static void write (const std::string &text);
    static void write (const char *text);
};

#ifdef IRBIS_DEBUG

    #define LOG_TRACE(__message) irbis::Log::write(__message);
    #define LOG_ERROR(__message) irbis::Log::write(__message);

#else

    #define LOG_TRACE(__message)
    #define LOG_ERROR(__message)

#endif // IRBIS_DEBUG

#define LOG_ENTER LOG_TRACE(__func__)
#define LOG_LEAVE LOG_TRACE(__func__)

//=========================================================

// Utilities

/// \brief Выбор первой непустой строки из произвольного количества.
template<class T> T choose (T first) { return first; }

/// \brief Выбор первой непустой строки из произвольного количества.
template<class T1, class... T2>
T1 choose (const T1& first, const T2&... others)
{
    if (!first.empty()) {
        return first;
    }
    return choose (others...);
}

/// \brief Является ли данный элемент одним из перечисленных?
template<class T> bool oneOf (T left, T right) { return left == right; }

/// \brief Является ли данный элемент одним из перечисленных?
template<class T1, class... T2>
bool oneOf (const T1& first, const T1& second, const T2& ... others)
{
    if (oneOf (first, second)) {
        return true;
    }
    return oneOf (first, others...);
}

/// \brief Безопасное извлечение элемента из контейнера.
template<typename T1, typename T2>
typename T1::value_type safeAt (const T1& container, T2 index)
{
    if (index >= container.size()) {
        return typename T1::value_type();
    }
    return container[index];
}

IRBIS_API bool IRBIS_CALL sameChar   (Char first, Char second) noexcept;
IRBIS_API bool IRBIS_CALL sameString (const String &first, const String &second) noexcept;
IRBIS_API Char IRBIS_CALL firstChar  (const String &text)      noexcept;
IRBIS_API char IRBIS_CALL firstChar  (const std::string &text) noexcept;

IRBIS_API String      IRBIS_CALL toLower (String &text)      noexcept;
IRBIS_API std::string IRBIS_CALL toLower (std::string &text) noexcept;
IRBIS_API String      IRBIS_CALL toUpper (String &text)      noexcept;
IRBIS_API std::string IRBIS_CALL toUpper (std::string &text) noexcept;

IRBIS_API bool contains (const String &text, const String &fragment);
IRBIS_API bool contains (const String &text, Char c);

IRBIS_API std::string replace (const std::string &text, const std::string &from, const std::string &to);
IRBIS_API String      replace (const String &text, const String &from, const String &to);

IRBIS_API String trimStart (const String &text);
IRBIS_API String trimEnd   (const String &text);
IRBIS_API String trim      (const String &text);

IRBIS_API int IRBIS_CALL fastParse32 (const String &text)                   noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (CharSpan text)                        noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (WideSpan text)                        noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (const Char *text)                     noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (const Char *text, std::size_t length) noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (const std::string &text)              noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (const char *text)                     noexcept;
IRBIS_API int IRBIS_CALL fastParse32 (const char *text, std::size_t length) noexcept;

IRBIS_API unsigned int IRBIS_CALL fastParseUnsigned32 (const String &text)                   noexcept;
IRBIS_API unsigned int IRBIS_CALL fastParseUnsigned32 (const Char *text)                     noexcept;
IRBIS_API unsigned int IRBIS_CALL fastParseUnsigned32 (const Char *text, std::size_t length) noexcept;
IRBIS_API unsigned int IRBIS_CALL fastParseUnsigned32 (const std::string &text)              noexcept;
IRBIS_API unsigned int IRBIS_CALL fastParseUnsigned32 (const char *text)                     noexcept;
IRBIS_API unsigned int IRBIS_CALL fastParseUnsigned32 (const char *text, std::size_t length) noexcept;

IRBIS_API int64_t IRBIS_CALL fastParse64 (const String &text)      noexcept;
IRBIS_API int64_t IRBIS_CALL fastParse64 (const Char *text)        noexcept;
IRBIS_API int64_t IRBIS_CALL fastParse64 (const std::string &text) noexcept;
IRBIS_API int64_t IRBIS_CALL fastParse64 (const char *text)        noexcept;

IRBIS_API std::vector<std::string> IRBIS_CALL split    (const std::string &text, char delimiter);
IRBIS_API StringList               IRBIS_CALL split    (const String &text, Char delimiter);
IRBIS_API std::vector<std::string> IRBIS_CALL split    (const std::string &text, const std::string &delimiter);
IRBIS_API StringList               IRBIS_CALL split    (const String &text, const String &delimiter);
IRBIS_API std::vector<std::string> IRBIS_CALL maxSplit (const std::string &text, char separator, int count);
IRBIS_API StringList               IRBIS_CALL maxSplit (const String &text, Char separator, int count);

IRBIS_API String cp866_to_unicode  (const std::string &text);
IRBIS_API String cp1251_to_unicode (const std::string &text);
IRBIS_API String koi8r_to_unicode  (const std::string &text);

IRBIS_API std::string unicode_to_cp866  (const String &text);
IRBIS_API std::string unicode_to_cp1251 (const String &text);
IRBIS_API void        unicode_to_cp1251 (Byte *dst, const Char *src, std::size_t size);
IRBIS_API std::string unicode_to_koi8r  (const String &text);

IRBIS_API std::string narrow                (const String &wide, const std::locale &loc);
IRBIS_API String      widen                 (const std::string &str, const std::locale &loc);
IRBIS_API String      new_cp1251_to_unicode (const std::string &text);
IRBIS_API std::string new_unicode_to_cp1251 (const String &text);
IRBIS_API std::string new_toUtf             (const String &text);
IRBIS_API String      new_fromUtf           (const std::string &text);

IRBIS_API Byte*       IRBIS_CALL toUtf    (Byte *dst, const Char *src, std::size_t length) noexcept;
IRBIS_API Char*       IRBIS_CALL fromUtf  (Char *dst, const Byte *src, std::size_t length) noexcept;
IRBIS_API std::size_t IRBIS_CALL countUtf (const Char *src, std::size_t length)            noexcept;
IRBIS_API std::size_t IRBIS_CALL countUtf (const Byte *src, std::size_t length)            noexcept;
IRBIS_API const Byte* IRBIS_CALL fromUtf  (const Byte *src, std::size_t &size, Byte stop, String &result);
IRBIS_API Byte*       IRBIS_CALL toUtf    (Byte *dst, const String &text);
IRBIS_API String      IRBIS_CALL fromUtf  (const std::string &text);
IRBIS_API std::string IRBIS_CALL toUtf    (const String &text);
IRBIS_API String      IRBIS_CALL fromUtf  (ByteSpan span);

IRBIS_API String IRBIS_CALL removeComments (const String &text);
IRBIS_API String IRBIS_CALL prepareFormat  (const String &text);

template<typename T>
bool isDigit(T c)  { return (c >= '0') && (c <= '9'); }

//=========================================================
// borrow from C++14

template<typename T, typename ... Args>
std::unique_ptr<T> makeUnique (Args&& ... args)
{ return std::unique_ptr<T> (new T (std::forward<Args>(args)...)); }

//=========================================================
// borrow from C++17

/// \brief Вычисляет размер обычного сишного массива.
/// \tparam T Тип элементов массива.
/// \tparam N Размер массива в элементах.
/// \param array Собственно массив.
/// \return Размер массива в элементах.
/// \warning Не работает для массивов нулевой или переменной длины!
template <typename T, std::size_t N>
std::size_t size (const T (&array)[N])
{
    return N;
}

/// \brief Вычисляет размер вектора.
/// \tparam T Тип элементов вектора.
/// \param v Собственно вектор.
/// \return Размер вектора в элементах.
template <typename T>
std::size_t size (const std::vector<T> &v)
{
    return v.size();
}

/// \brief Вычисляет размер массива.
/// \tparam T Тип элементов массива.
/// \tparam N Размер массива в элементах.
/// \param v Собственно массив.
/// \return Размер массива в элементах.
template <typename T, std::size_t N>
std::size_t size (const std::array<T, N> &a)
{
    return N;
}

/// \brief Вычисляет размер списка.
/// \tparam T Тип элементов списка.
/// \param v Собственно список.
/// \return Размер списка в элементах.
template <typename T>
std::size_t size (const std::list<T> &l)
{
    return l.size();
}

/// \brief Вычисляет знак числа.
/// \tparam T Тип числа (целое, длинное и т. д.).
/// \param value Значение числа.
/// \return -1, 0 или 1.
template <typename T>
int sign (T value)
{
    return (T(0) < value) - (value < T(0));
}

}

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif

// IRBIS_PRIVATE_H
#endif
