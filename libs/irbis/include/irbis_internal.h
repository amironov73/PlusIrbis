// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef IRBIS_PRIVATE_H
#define IRBIS_PRIVATE_H

#include "irbis.h"

#include <array>
#include <memory>
#include <thread>
#include <cstring>
#include <cassert>

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

/// \brief Унифицированный итератор для контейнеров, поддерживающих обращение к элементам по индексу.
/// \tparam ContainerType Тип контейнера.
/// \tparam DataType Тип отдельного элемента в контейнере.
template <class ContainerType, class DataType>
struct RandomAccessIterator final
    : public std::iterator<std::random_access_iterator_tag, DataType>
{
    ContainerType *m_buffer;
    std::size_t  m_position;

    RandomAccessIterator () noexcept : m_buffer { nullptr }, m_position { 0 } {} ///< Конструктор.
    RandomAccessIterator (ContainerType *buffer, std::size_t position) noexcept
        : m_buffer { buffer }, m_position { position } {} ///< Конструктор.

    RandomAccessIterator             (const RandomAccessIterator&) noexcept = default; ///< Конструктор копирования.
    RandomAccessIterator             (RandomAccessIterator&&)      noexcept = default; ///< Конструктор перемещения.
    RandomAccessIterator& operator = (const RandomAccessIterator&) noexcept = default; ///< Оператор копирования.
    RandomAccessIterator& operator = (RandomAccessIterator&&)      noexcept = default; ///< Оператор перемещения.

    bool operator == (const RandomAccessIterator &other) const noexcept
    {
        return m_position == other.m_position;
    }

    bool operator != (const RandomAccessIterator &other) const noexcept
    {
        return m_position != other.m_position;
    }

    bool operator < (const RandomAccessIterator &other) const noexcept
    {
        return m_position < other.m_position;
    }

    RandomAccessIterator& operator ++ () noexcept
    {
        ++m_position;
        return *this;
    }

    RandomAccessIterator operator ++ (int) & noexcept
    {
        RandomAccessIterator copy (*this);
        ++m_position;
        return copy;
    }

    RandomAccessIterator& operator += (std::size_t n) noexcept
    {
        m_position += n;
        return *this;
    }

    RandomAccessIterator& operator -- () noexcept
    {
        --m_position;
        return *this;
    }

    RandomAccessIterator operator -- (int) & noexcept
    {
        RandomAccessIterator copy (*this);
        --m_position;
        return copy;
    }

    RandomAccessIterator& operator -= (std::size_t n) noexcept
    {
        m_position -= n;
        return *this;
    }

    DataType& operator *  () const noexcept { return (*m_buffer) [m_position]; }
    DataType& operator -> () const noexcept { return (*m_buffer) [m_position]; }

    friend RandomAccessIterator operator + (const RandomAccessIterator &left, std::size_t right) noexcept
    {
        auto result = left;
        result += right;
        return result;
    }

    friend std::ptrdiff_t operator - (const RandomAccessIterator &left, const RandomAccessIterator &right) noexcept
    {
        return static_cast<std::ptrdiff_t> (left.m_position - right.m_position);
    }
};

//=========================================================

/// \brief Буфер, накапливающий сначала данные в массиве на стеке, а при нехватке места использующий кучу.
/// \tparam StaticSize Размер массива на стеке.
template <std::size_t StaticSize = 256>
struct FastBuffer final
{
    static_assert (StaticSize >= 8, "Less is stupid");

    using iterator = RandomAccessIterator <FastBuffer <StaticSize>, Byte>;

    std::size_t m_capacity;
    std::size_t m_size;
    Byte       *m_dynamicBuffer;
    Byte        m_staticBuffer[StaticSize];

    /// \brief Конструктор.
    /// \details На момент конструирования создается только буфер в стеке.
    FastBuffer() noexcept
    {
        m_capacity = StaticSize;
        m_size = 0;
        m_dynamicBuffer = nullptr;
    };

    /// \brief Деструктор.
    ~FastBuffer()
    {
        delete []m_dynamicBuffer;
    }

    /// \brief Индексатор.
    /// \param offset Индекс необходимого элемента.
    /// \return Ссылка на элемент.
    Byte& operator [] (std::size_t offset) noexcept
    {
        assert (offset < m_size);

        if (offset < StaticSize) {
            return m_staticBuffer [offset];
        }

        return m_dynamicBuffer [offset - StaticSize];
    }

    /// \brief Ссылка на последний элемент данных.
    /// \return Ссылка.
    Byte& back () noexcept
    {
        return *(--this->end());
    }

    /// \brief Указатель на начало буфера.
    /// \return Итератор.
    iterator begin () const noexcept
    {
        return iterator { const_cast <FastBuffer <StaticSize>*> (this), 0 };
    }

    /// \brief Емкость буфера в байтах.
    /// \return Емкость.
    std::size_t capacity () const noexcept
    {
        return m_capacity;
    }

    /// \brief Очистка буфера.
    /// \details Емкость не уменьшается, динамический буфер (если он выделен) не освобождается.
    void clear () noexcept
    {
        m_size = 0;
    }

    /// \brief Контейнер пуст?
    /// \return true, если пуст.
    bool empty () const noexcept
    {
        return m_size == 0;
    }

    /// \brief Указатель за концом буфера.
    /// \return Итератор.
    iterator end () const noexcept
    {
        return iterator { const_cast <FastBuffer <StaticSize>*> (this), m_size };
    }

    /// \brief Ссылка на первый элемент данных.
    /// \return Ссылка.
    Byte& front () noexcept
    {
        return *(this->begin());
    }

    /// \brief Добавление элемента в конец буфера.
    /// \param value Добавляемый байт.
    void push_back (Byte value)
    {
        if (m_size == m_capacity) {
            const auto newSize = m_size * 2;
            this->_grow (newSize);
        }
        (*this) [m_size++] = value;
    }

    /// \brief Увеличение размера буфера, если необходимо.
    /// \param newSize Необходимый размер буфера в байтах.
    void reserve (std::size_t newSize)
    {
        this->_grow (newSize);
    }

    /// \brief Увеличение размера буфера до указанного.
    /// \param newSize Необходимый размер буфера в байтах.
    /// \details Высвобождения динамического буфера (если он выделен), не происходит.
    void resize (std::size_t newSize)
    {
        this->_grow (newSize);
        m_size = newSize;
    }

    /// \brief Размер контейнера в элементах.
    /// \return Общий размер.
    std::size_t size () const noexcept
    {
        return m_size;
    }

    /// \brief Преобразование в стандартную строку.
    /// \return Строка.
    std::string toString()
    {
        std::string result;
        result.reserve (m_size);
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

    /// \brief Преобразование в стандартный вектор.
    /// \return Вектор изо всех элементов, возможно, пустой.
    std::vector<Byte> toVector() const
    {
        std::vector <Byte> result;
        result.reserve (this->size());
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

    /// \brief Запись данных в буфер.
    /// \param data Данные для записи.
    /// \param size Размер данных в байтах.
    void write (const Byte *data, size_t size)
    {
        // TODO implement smarter
        while (size) {
            this->push_back (*data);
            ++data;
            --size;
        }
    }

private:

    /// \brief Увеличиваем емкость.
    /// \param newCapacity Требуемая емкость.
    void _grow (std::size_t newCapacity)
    {
        if (newCapacity > m_capacity) {
            auto newDynamicBuffer = new Byte [newCapacity - StaticSize];
            if (m_dynamicBuffer) {
                std::memcpy (newDynamicBuffer, m_dynamicBuffer, m_size - StaticSize);
                delete []m_dynamicBuffer;
            }
            m_dynamicBuffer = newDynamicBuffer;
            m_capacity = newCapacity;
        }
    }
};

//=========================================================

/// \brief Простой контейнер для случаев,
/// когда предполагается, что элементов будет мало - один или два.
/// \tparam T Тип элементов.
/// \tparam N Количество элементов, размещаемых на стеке. Последующие элементы размещаются в куче.
template <class T, std::size_t N = 2>
class Frugal
{
public:

    using iterator = RandomAccessIterator<Frugal<T, N>, T>;

    Frugal             ()                   = default; ///< Конструктор по умолчанию.
    Frugal             (const Frugal &)     = delete;  ///< Конструктор копирования.
    Frugal             (Frugal &&) noexcept = default; ///< Конструктор перемещения.
    ~Frugal            ()                   = default; ///< Деструктор.
    Frugal& operator = (const Frugal &)     = delete;  ///< Оператор копирования.
    Frugal& operator = (Frugal &&) noexcept = default; ///< Оператор перемещения.

    /// \brief Оператор индексирования.
    /// \return Ссылка на элемент.
    /// \warning При выходе за границы UB.
    T& operator [] (std::size_t offset)
    {
        return (offset < N)
            ? this->_static [offset]
            : this->_dynamic.get() [offset - N];
    }

    /// \brief Указатель на первый элемент.
    /// \return Указатель.
    iterator begin() noexcept { return iterator (this, 0); }

    /// \brief Указатель за последним элементом.
    /// \return Указатель.
    iterator end() noexcept { return iterator (this, this->_size); }

    /// \brief Ссылка на первый элемент.
    /// \return Ссылка.
    /// \warning Для пустого контейнера UB.
    T& front() { return (*this) [0]; }

    /// \brief Ссылка на последний элемент.
    /// \return Ссылка.
    /// \warning Для пустого контейнера UB.
    T& back() { return (*this) [this->_size - 1]; }

    /// \brief Емкость контейнера.
    /// \return Емкость контейнера в элементах.
    std::size_t capacity() const noexcept { return this->_capacity + N; }

    /// \brief Очистка контейнера.
    void clear() noexcept { this->_size = 0; }

    /// \brief Проверка, не пустой ли контейнер.
    /// \return true, если контейнер пустой.
    bool empty() const noexcept { return this->_size == 0; }

    /// \brief Текущий размер контейнера.
    /// \return Размер контейнера в элементах.
    std::size_t size() const noexcept  { return this->_size; }

    /// \brief Преобразование в стандартный список.
    /// \return Список изо всех элементов, возможно, пустой.
    std::list<T> toList()
    {
        std::list <T> result;
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

    /// \brief Преобразование в стандартный вектор.
    /// \return Вектор изо всех элементов, возможно, пустой.
    std::vector<T> toVector()
    {
        std::vector <T> result;
        result.reserve (this->size());
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

    /// \brief Помещение элемента в конец.
    /// \param value Помещаемое значение.
    void push_back (const T &value)
    {
        if (this->_size < N) {
            this->_static [this->_size] = value;
        }
        else {
            if (this->_size - N == this->_capacity) {
                this->_enlarge();
            }
            this->_dynamic [this->_size - N] = value;
        }
        ++this->_size;
    }

    /// \brief Перемещение элемента в конец.
    /// \param value Перемещаемое значение.
    void push_back (T &&value)
    {
        if (this->_size < N) {
            this->_static [this->_size] = std::move (value);
        }
        else {
            if (this->_size - N == this->_capacity) {
                this->_enlarge();
            }
            this->_dynamic.get() [this->_size - N] = std::move (value);
        }
        ++this->_size;
    }

    /// \brief Помещение элемента в конец
    /// \param args Аргументы конструктора.
    template <class... Args>
    T& emplace_back (Args&&... args)
    {
        T *place;
        if (this->_size < N) {
            place = this->_static + this->_size;
        }
        else {
            if (this->_size - N == this->_capacity) {
                this->_enlarge();
            }
            place = this->_dynamic.get() + this->_size - N;
        };
        T *value = new (place) T (std::forward <Args> (args)...);
        ++this->_size;
        return *value;
    }

private:
    std::size_t _size { 0 };
    std::size_t _capacity { 0 };
    T _static [N];
    std::unique_ptr<T[]> _dynamic {};

    void _enlarge() {
        std::size_t  oldSize = this->_capacity;
        std::size_t newSize = oldSize * 2;
        if (newSize <= sizeof (int32_t)) {
            newSize = sizeof (int32_t);
        }

        T *oldArray = this->_dynamic.get();
        T *newArray = new T [newSize];
        if (oldArray) {
            std::copy (oldArray, oldArray + oldSize, newArray);
        }
        this->_dynamic.reset (newArray);
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

/// \brief Повторение действия необходимое число раз, пока не получится.
class IRBIS_API Retry final
{
public:
    int delayInterval { 100 }; ///< Задержка между повторами, миллисекунды.
    int retryLimit    { 5 };   ///< Максимальное число повторов.

    Retry () = default;
    Retry (int delay_, int limit_) : delayInterval { delay_ }, retryLimit { limit_ } {}

    template <class TFunc, class ... TArgs>
    void action (TFunc func, TArgs ... args)
    {
        for (int i = 0; i < this->retryLimit; i++) {
            try {
                func (args ...);
                return;
            }
            catch (...) {
                this->_delay();
            }
        }
        throw std::exception();
    }

    template <class TFunc, class ... TArgs>
    auto execute (TFunc func, TArgs ... args) -> decltype(func(args...))
    {
        for (int i = 0; i < this->retryLimit; i++) {
            try {
                auto result = func (args ...);
                return result;
            }
            catch (...) {
                this->_delay();
            }
        }
        throw std::exception();
    }

private:

    void _delay()
    {
        if (this->delayInterval > 0) {
            std::this_thread::sleep_for (std::chrono::microseconds (this->delayInterval));
        }
    }
};

//=========================================================

/// \brief Простая обертка над системным файловым API.
class IRBIS_API Directory final
{
public:

    explicit Directory             (const std::string &name);
             Directory             (const Directory&)  = delete;  ///< Конструктор копирования.
             Directory             (Directory &&)      = default; ///< Конструктор перемещения.
             Directory& operator = (const Directory &) = delete;  ///< Оператор копирования.
             Directory& operator = (Directory &&)      = default; ///< Оператор перемещения.
            ~Directory             ();

    std::string               find    (const std::string &name);
    std::string               read    ();
    std::vector <std::string> readAll ();
    void                      rewind  ();

private:
    struct Impl;
    Impl *_impl;
};

//=========================================================

/// \brief Простая обертка над системным файловым API.
class IRBIS_API File final
{
public:

    File (const File &) = delete;
    File (File &&other) noexcept;
    ~File() noexcept;
    File& operator = (const File &) = delete;
    File& operator = (File && other) noexcept;

    void        close           () noexcept;
    static File create          (const String &fileName);
    static File create          (const std::string &fileName);
    bool        eof             ();
    static File insistCreate    (const String &fileName, int delay = 100, int retry = 5);
    static File insistCreate    (const std::string &fileName, int delay = 100, int retry = 5);
    static File insistOpenRead  (const String &fileName, int delay = 100, int retry = 5);
    static File insistOpenRead  (const std::string &fileName, int delay = 100, int retry = 5);
    static File insistOpenWrite (const String &fileName, int delay = 100, int retry = 5);
    static File insistOpenWrite (const std::string &fileName, int delay = 100, int retry = 5);
    static File openRead        (const String &fileName);
    static File openRead        (const std::string &fileName);
    static File openWrite       (const String &fileName);
    static File openWrite       (const std::string &fileName);
    int64_t     read            (Byte *buffer, int64_t nbytes);
    std::string readAll         ();
    static std::string readAll  (const String &fileName);
    int         readByte        ();
    int         readChar        ();
    uint32_t    readInt32       ();
    uint64_t    readInt64       ();
    std::string readNarrow      ();
    std::size_t size            ();
    std::size_t tell            ();
    void        seek            (int64_t offset);
    File*       toHeap          ();
    int64_t     write           (const Byte* buffer, int64_t nbytes);
    void        writeInt32      (uint32_t number);
    void        writeInt64      (uint64_t number);

private:
#ifdef IRBIS_WINDOWS
    void *_handle { nullptr };
#else
    int _handle { -1 };
#endif

    File() = default; ///< Скрываем конструктор.
    void _grabHandle (File &other) noexcept;
};

//=========================================================

/// \brief Буфер, состоящий из мелких блоков.
class IRBIS_API ChunkedBuffer final
{
public:

    using iterator = RandomAccessIterator <ChunkedBuffer, Byte>;

    /// \brief Блок памяти.
    class IRBIS_API Chunk final
    {
    public:
        Byte *data;
        Chunk *next;

        explicit Chunk             (const std::size_t size);
                 Chunk             (const Chunk&) = delete; ///< Конструктор копирования.
                 Chunk             (Chunk&&)      = delete; ///< Конструктор перемещения.
                 Chunk& operator = (const Chunk&) = delete; ///< Оператор копирования.
                 Chunk& operator = (Chunk&&)      = delete; ///< Оператор перемещения.
                ~Chunk();
    };

    const static std::size_t DefaultChunkSize;

    explicit ChunkedBuffer             (std::size_t chunkSize = DefaultChunkSize);
             ChunkedBuffer             (const ChunkedBuffer &) = delete;  ///< Конструктор копирования.
             ChunkedBuffer             (ChunkedBuffer &&)      = default; ///< Конструктор перемещения.
             ChunkedBuffer& operator = (const ChunkedBuffer &) = delete;  ///< Оператор копирования.
             ChunkedBuffer& operator = (ChunkedBuffer &&)      = default; ///< Оператор перемещения.
            ~ChunkedBuffer             ();

    Byte& operator [] (std::size_t index) noexcept;

    iterator    begin           () noexcept;
    iterator    current         () noexcept;
    bool        empty           () const noexcept;
    iterator    end             () noexcept;
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

// Assert

struct Tracer;

#define IRBIS_ASSERT(__x) /* nothing yet */
#define IRBIS_VERIFY(__x) (__x)

/// \brief Простой трассировщик сообщений.
struct Tracer {
    const char *filename;
    unsigned int lineNumber;

    Tracer (const char *filename_, unsigned int line_)
        : filename (filename_), lineNumber (line_) {}

    template <class ... Args>
    void operator () (const char *format, Args ... args)
    {
        printf ("%s (%d): ", filename, (static_cast<int> (lineNumber)));
        printf (format, args...);
    }
};

#define IRBIS_TRACE irbis::Tracer(__FILE__, __LINE__)

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

    #define LOG_TRACE(__message) irbis::Log::write (__message);
    #define LOG_ERROR(__message) irbis::Log::write (__message);

#else

    #define LOG_TRACE(__message)
    #define LOG_ERROR(__message)

#endif // IRBIS_DEBUG

#define LOG_ENTER LOG_TRACE(__func__)
#define LOG_LEAVE LOG_TRACE(__func__)

//=========================================================

// Handles

/// \brief Обертка для хэндла.
template <typename Traits>
class UniqueHandle
{
public:

    using Handle = typename Traits::handle;

    explicit UniqueHandle (Handle value = Traits::invalid()) noexcept
        : _value { value } {}
    ~UniqueHandle() noexcept { this->_close(); }
    UniqueHandle (const UniqueHandle&) = delete;
    UniqueHandle (UniqueHandle &&other)  noexcept
        : _value { other.release() } {}
    UniqueHandle& operator = (const UniqueHandle&) = delete;
    UniqueHandle& operator = (UniqueHandle &&other) noexcept
    {
        if (this != &other) {
            this->reset (other.release());
        }
        return *this;
    }

    explicit operator bool() const noexcept
    {
        return this->_value != Traits::invalid();
    }

    Handle get() const noexcept
    {
        return this->_value;
    }

    bool release (Handle value = Traits::invalid()) noexcept
    {
        if (value != this->_value) {
            this->_close();
            this->_value = value;
        }
        return static_cast <bool> (*this);
    };

private:

    Handle _value;

    void _close() noexcept
    {
        if (*this) {
            Traits::close (this->value);
        }
    }
};

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
IRBIS_API bool IRBIS_CALL sameString (const String &first, const String &second)           noexcept;
IRBIS_API bool IRBIS_CALL sameString (const std::string &first, const std::string &second) noexcept;
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
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
template <typename T, std::size_t N>
std::size_t size (const T (&array)[N])
{
    return N;
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

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
