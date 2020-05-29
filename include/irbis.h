// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef IRBIS_H
#define IRBIS_H

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
// ReSharper disable CommentTypo

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <future>
#include <ios>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <functional>

//=========================================================

#if !defined(IRBIS_WINDOWS) && !defined (IRBIS_UNIX)

    #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(_WINDOWS)

        #define IRBIS_WINDOWS

    #else

        #define IRBIS_UNIX

        #if defined(__FreeBSD__)

           #define IRBIS_FREEBSD

	    #endif

        #if defined(__ANDROID__)

            #define IRBIS_ANDROID

        #endif

        #if defined(__APPLE__) || defined(__APPLE_CC__) || defined (__OSX__)

            #define IRBIS_APPLE

        #endif

    #endif

#endif

//=========================================================

#ifdef IRBIS_DYNAMIC_LIBRARY

    #if defined(_MSC_VER)
        // Microsoft
        #define IRBIS_EXPORT __declspec(dllexport)
        #define IRBIS_IMPORT __declspec(dllimport)
        #define IRBIS_HIDDEN
    #elif defined(__GNUC__)
        //  GCC
        #define IRBIS_EXPORT __attribute__((visibility("default")))
        #define IRBIS_IMPORT
        #define IRBIS_HIDDEN __attribute__((visibility("hidden")))
    #else
        //  do nothing and hope for the best?
        #define IRBIS_EXPORT
        #define IRBIS_IMPORT
        #define IRBIS_HIDDEN
        #pragma warning Unknown dynamic link import/export semantics.
    #endif

#else

    #define IRBIS_EXPORT
    #define IRBIS_IMPORT
    #define IRBIS_HIDDEN

#endif

//=========================================================

#ifndef IRBIS_API

    #ifdef PLUSIRBIS_LIBRARY

        #define IRBIS_API IRBIS_EXPORT

    #else

        #define IRBIS_API IRBIS_IMPORT

    #endif // PLUSIRBIS_LIBRARY

#endif // IRBIS_API

#if !defined(IRBIS_DEBUG)

    #ifdef _DEBUG
        #define IRBIS_DEBUG
    #endif // _DEBUG

#endif // IRBIS_DEBUG

// для функций: int __fastcall func (int arg).
#ifndef IRBIS_CALL

    #if defined(_MSC_VER)
        // Microsoft
        #define IRBIS_CALL __fastcall
    #elif defined(__GNUC__)
        //  GCC
        #define IRBIS_CALL
    #else
        //  do nothing and hope for the best?
        #define IRBIS_CALL
    #endif

#endif // IRBIS_CALL

// для функций: __declspec(naked) int func (int arg).
#ifndef IRBIS_NAKED

    #if defined(_MSC_VER)
        // Microsoft
        #define IRBIS_NAKED __declspec(naked)
    #elif defined(__GNUC__)
        //  GCC
        #define IRBIS_NAKED
    #else
        //  do nothing and hope for the best?
        #define IRBIS_NAKED
    #endif

#endif // IRBIS_NAKED

// для функций: void __declspec(nothrow) func (int arg).
#ifndef IRBIS_NOTHROW

    #if defined(_MSC_VER)
        // Microsoft
        #define IRBIS_NOTHROW __declspec(nothrow)
    #elif defined(__GNUC__)
        //  GCC
        #define IRBIS_NOTHROW
    #else
        //  do nothing and hope for the best?
        #define IRBIS_NOTHROW
    #endif

#endif // IRBIS_NOTHROW

// для аргументов функций: int func (char* __restrict ptr1, int* __restrict ptr2).
#ifndef IRBIS_RESTRICT

    #if defined(_MSC_VER)
        // Microsoft
        #define IRBIS_RESTRICT __restrict
    #elif defined(__GNUC__)
        //  GCC
        #define IRBIS_RESTRICT __restrict__
    #else
        //  do nothing and hope for the best?
        #define IRBIS_RESTRICT
    #endif

#endif // IRBIS_RESTRICT

// для функций: int func () noexcept.
#ifndef IRBIS_NOEXCEPT

    #define IRBIS_NOEXCEPT noexcept

#endif // IRBIS_NOEXCEPT

// для функций [[nodiscard]]
#ifndef IRBIS_NODISCARD

    #define IRBIS_NODISCARD

#endif

// для функций: [[maybe_unused]]
#ifndef IRBIS_MAYBE_UNUSED

    #define IRBIS_MAYBE_UNUSED

#endif

// из C++20: [[likely]], [[unlikely]]
#ifndef IRBIS_LIKELY

    #define IRBIS_LIKELY
    #define IRBIS_UNLIKELY

#endif

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(push)
    #pragma warning(disable: 4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #pragma warning(disable: 4275) // non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'

#endif

//=========================================================

namespace irbis
{

class  AlphabetTable;
class  ChunkedBuffer;
class  ClientInfo;
class  ClientQuery;
class  ClientSocket;
class  Connection;
class  ConnectionFactory;
class  DatabaseInfo;
class  Date;
class  Ean13;
class  Ean8;
class  EmbeddedField;
class  Encoding;
class  Exemplar;
class  FileSpecification;
class  Format;
class  FoundLine;
class  GblResult;
class  GblSettings;
class  GblStatements;
class  IlfEntry;
class  IlfFile;
class  IniFile;
class  IniLine;
class  IniSection;
class  IrbisException;
class  Isbn;
class  Iso2709;
class  LiteField;
class  LiteRecord;
class  LiteSubField;
class  MarcRecord;
class  MarcRecordList;
class  MenuEntry;
class  MenuFile;
class  NetworkException;
class  NotImplementedException;
struct NumberText;
class  OptFile;
class  OptLine;
class  ParFile;
class  PhantomField;
class  PhantomRecord;
class  PhantomSubField;
class  PostingParameters;
class  ProcessInfo;
class  ProtocolText;
class  RawRecord;
class  RecordField;
class  RecordSerializer;
class  Search;
class  SearchParameters;
class  SearchScenario;
class  ServerResponse;
class  ServerStat;
class  StopWords;
class  SubField;
class  TableDefinition;
class  TermInfo;
class  TermParameters;
class  TermPosting;
class  Text;
class  TextNavigator;
class  TreeFile;
class  TreeNode;
class  Upc12;
class  UserInfo;
class  VerificationException;
class  Version;

//=========================================================

using Byte            = std::uint8_t;
using Bytes           = std::vector<Byte>;
using Char            = wchar_t;
using Chars           = std::vector<Char>;
using String          = std::wstring;
using Flag            = std::uint32_t;
using Mfn             = std::uint32_t;
using MfnList         = std::vector<Mfn>;
using Offset          = std::uint64_t;
using StringList      = std::vector<String>;
using SubFieldList    = std::vector<SubField>;
using RecordFieldList = std::vector<RecordField>;

//=========================================================

/// \brief Обертка над указателем, который не может быть равен `nullptr`.
/// \tparam T Тип указателя.
template <class T>
struct NotNull final
{
    T *m_ptr;

    NotNull ()                           = delete;  ///< Конструктор по умолчанию.
    NotNull (const NotNull&)             = default; ///< Конструктор копирования.
    NotNull& operator = (const NotNull&) = default; ///< Оператор копирования.

    /// \brief Универсальный конструктор.
    /// \param ptr Указатель.
    template <typename U>
    explicit NotNull (U&& ptr) : m_ptr (std::forward <U> (ptr))
    {
        if (m_ptr == nullptr) {
            throw std::exception();
        }
    }

    /// \brief Универсальный оператор присваивания.
    /// \tparam U Тип присваиваемого значения.
    /// \param ptr Присваиваемое значения.
    /// \return this.
    template <typename U>
    NotNull& operator = (U&& ptr)
    {
        m_ptr = std::forward <U> (ptr);
        if (m_ptr == nullptr) {
            throw std::exception();
        }
        return *this;
    }

    T& operator*  () const { return *m_ptr; }
    T* operator-> () const { return m_ptr; }

    NotNull              (std::nullptr_t)   = delete;
    NotNull& operator =  (std::nullptr_t)   = delete;
    NotNull& operator ++ ()                 = delete;
    NotNull& operator -- ()                 = delete;
    NotNull  operator ++ (int)              = delete;
    NotNull  operator -- (int)              = delete;
    NotNull& operator += (std::ptrdiff_t)   = delete;
    NotNull& operator -= (std::ptrdiff_t)   = delete;
    void operator [] (std::ptrdiff_t) const = delete;
};

template <class T>
std::ostream& operator << (std::ostream &os, const NotNull <T> &val)
{
    os << *val;
    return os;
}

template <class T, class U>
bool operator== (const NotNull <T> &left, const NotNull <U> &right)
{
    return left.m_ptr == right.m_ptr;
}

template <class T, class U>
bool operator != (const NotNull <T> &left, const NotNull<U> &right)
{
    return left.m_ptr != right.m_ptr;
}

template <class T, class U>
bool operator < (const NotNull <T> &left, const NotNull <U> &right)
{
    return left.m_ptr < right.m_ptr;
}

template <class T, class U>
bool operator <= (const NotNull <T> &left, const NotNull <U> &right)
{
    return left.m_ptr <= right.m_ptr;
}

template <class T, class U>
bool operator > (const NotNull <T> &left, const NotNull <U> &right)
{
    return left.m_ptr > right.m_ptr;
}

template <class T, class U>
bool operator >= (const NotNull <T> &left, const NotNull <U> &right)
{
    return left.m_ptr >= right.m_ptr;
}

//=========================================================

/// \brief Результат выполнения функции (с учетом успеха/ошибки).
/// \tparam T Тип результата.
template <class T>
class Result // NOLINT(cppcoreguidelines-pro-type-member-init)
{
public:
    T result;    ///< Результирующее значение (имеет смысл при успешном завершении).
    bool success { false }; ///< Признак успешного завершения.
    String errorMessage;    ///< Сообщение об ошибке.

    /// \brief Преобразование к логическому значению.
    /// \return true означает успех.
    explicit operator bool() const noexcept
    {
        return this->success;
    }

    /// \brief Получение результирующего значения.
    explicit operator T() const
    {
        return this->result;
    }

    /// \brief Формирование результата успешного выполнения.
    static Result Success (T &&value)
    {
        Result result;
        result.success = true;
        result.result = value;
        return result;
    }

    /// \brief Формирование признака ошибки.
    /// \param message Сообщение об ошибке.
    /// \return Результат.
    static Result Failure (const String &message)
    {
        Result result;
        result.success = false; //-V1048
        result.errorMessage = message;
        return result;
    }

    T resultOr (T (*func) ()) { return this->success ? this->result : func(); }
};

template<class T, class... Args>
Result<T> makeResult (Args&&... args)
{
    return Result<T>::Success ({std::forward<Args> (args)...});
}

//=========================================================

/// \brief Опциональное значение.
/// \tparam T Тип элемента.
template<class T>
class Optional
{
public:
    T value; ///< Хранимое значение (если есть).
    bool hasValue { false }; ///< Содержит ли значение?

    Optional () = default;
    explicit Optional (T value_) noexcept : value { value_ }, hasValue { true } {}
    Optional (const Optional<T> &other)
        : value (other.value), hasValue (other.hasValue) {}
    Optional (Optional<T> &&other) noexcept
        : value (std::move (other.value)), hasValue (other.hasValue) { other.reset(); }
    Optional& operator = (const Optional<T> &other)
        { if (this != &other) { this->value = other.value; this->hasValue = other.value; }
            return *this; }
    Optional& operator = (const T &newValue) noexcept
        { this->value = newValue; this->hasValue = true; return *this; }
    Optional& operator = (std::nullptr_t) noexcept
        { this->hasValue = false; return *this; }
    Optional& operator = (Optional<T> &&other) noexcept
        { this->value = std::move (other.value); this->hasValue = other.hasValue;
          other.reset(); return *this; }

    constexpr explicit operator bool() const noexcept { return this->hasValue; }
    constexpr T operator *() const noexcept { return this->value; }
    void reset() noexcept { this->hasValue = false; this->value = T(); }
    constexpr T valueOr (const T&& alternative) const noexcept { return this->hasValue ? this->value : alternative; }
    T valueOr (T (*func) ()) const { return this->hasValue ? this->value : func(); }
};

/// \brief Вывод в поток
template <class T>
std::ostream& operator << (std::ostream &stream, const Optional<T> &value)
{
    if (value.hasValue) {
        stream << value.value;
    }
    return stream;
}

using OptionalString = Optional<String>;

//=========================================================

// Монада Maybe

// предварительное объявление для функции maybe
template <class T> struct Maybe;

/// \brief Создание монады.
template <class T>
Maybe<T> maybe (T *context) noexcept
{
    return Maybe<T> { context };
}

/// \brief Создание монады.
template <class T>
Maybe<T> maybe (Optional <T*> optional) noexcept
{
    return Maybe<T> { optional.value };
}

/// \brief Преобразование в опциональное значение.
template <class T>
Optional<T*> optional (Maybe<T> maybe) noexcept
{
    return Optional<T*> (maybe.context);
}

/// \brief Шаблон монады.
template <class T>
struct Maybe final
{
    T *context; ///< Контекст (хранимый указатель).

    /// Конструктор.
    explicit Maybe (T *context_) noexcept
            : context { context_ } {}
    Maybe (const Maybe<T>&) = default;
    Maybe (Maybe<T> &&other) noexcept
        : context (other.context) { other.context = nullptr; }
    Maybe<T>& operator = (const Maybe<T>&) = default;
    Maybe<T>& operator = (Maybe<T> &&other) noexcept
    {
        if (&other != this) {
            this->context = other.context;
            other.context = nullptr;
        }
        return *this;
    }

    /// \brief Обращение к члену класса.
    template <class Func>
    auto operator & (Func func) -> decltype (maybe (func (this->context)))
    {
        using ReturnType = typename std::result_of <Func(T*)>::type;
        using WithoutPointer = typename std::remove_pointer <ReturnType>::type;
        if (this->context) {
            return maybe (func (this->context));
        }
        return maybe <WithoutPointer> (nullptr);
    }

    /// \brief Вызов функции.
    template <class Func>
    Maybe<T>& operator | (Func func)
    {
        if (this->context) {
            func (context);
        }
        return *this;
    }
};

/// \brief Вывод в поток
template <class T>
std::ostream& operator << (std::ostream &stream, const Maybe<T> &value)
{
    if (value.context) {
        stream << *(value.context);
    }
    return stream;
}

//=========================================================

/// \brief Непрерывный кусок памяти.
/// \tparam T Тип элемента.
template<class T>
class Span final
{
public:
    T *ptr { nullptr };  ///< Указатель на начала куска.
    size_t length { 0 }; ///< Длина куска в элементах.

    constexpr Span() noexcept : ptr { nullptr }, length  { 0 } {}; ///< Конструктор по умолчанию.
    constexpr Span (T *ptr_, std::size_t length_) noexcept : ptr { ptr_ }, length { length_ } {} ///< Конструктор.
    Span (const T *ptr_, std::size_t length_) noexcept
        : ptr (const_cast <T*> (ptr_)), length (length_) {} ///< Конструктор.
    explicit Span (const std::vector<T> &vec) noexcept
        : ptr (const_cast <T*> (vec.data())), length (vec.size()) {} ///< Конструктор.
    explicit Span (const std::basic_string<T> &str) noexcept
        : ptr(const_cast<T*> (str.data())), length (str.size()) {} ///< Конструктор.

    /// \brief Спан из ASCIIZ-строки.
    /// \param ptr Указатель на начало строки.
    /// \return Полученный спан.
    static Span<T> fromString (const T *ptr) noexcept
    {
        std::size_t length = 0;
        const T *p = ptr;
        while (*p) {
            ++p;
            ++length;
        }
        return Span<T> (const_cast <T*> (ptr), length);
    }

    /// \brief Сырой указатель на данные.
    /// \return Сырой указатель.
    T* data() const noexcept { return this->ptr; }

    /// \brief Константный сырой указатель на данные.
    /// \return Сырой указатель.
    const T* cdata() const noexcept { return this->ptr; }

    /// \brief Пустой спан?
    /// \return `true` если пустой.
    constexpr bool empty() const noexcept { return this->length == 0; }

    /// \brief Длина данных в элементах.
    /// \return Длина.
    constexpr std::size_t size() const noexcept { return this->length; }

    /// \brief Оператор индексирования.
    /// \param offset Смещение.
    /// \return Ссылка на элемент с указанным смещением.
    T& operator[](std::ptrdiff_t offset) const noexcept { return this->ptr[offset]; }

    /// \brief Содержит ли спан указанный элемент.
    /// \param val Искомое значение.
    /// \return `true` если содержит.
    bool contains (const T val) const noexcept
    {
        for(const auto item : *this) {
            if (item == val) {
                return true;
            }
        }
        return false;
    }

    /// \brief Заполнение спана указанным значением.
    /// \param val Значение для заполнения.
    /// \return this.
    Span<T>& fill (const T val) noexcept
    {
        for(std::size_t ofs = 0; ofs < this->length; ++ofs) {
            this->ptr[ofs] = val;
        }
        return *this;
    }

    /// \brief Поиск указанного значения в спане с начала.
    /// \param val Искомое значение.
    /// \return Индекс первого вхождения найденного элемента,
    /// либо отрицательное значение.
    std::ptrdiff_t indexOf (const T val) const noexcept
    {
        for (std::size_t i = 0; i < this->length; ++i) {
            if (this->ptr[i] == val) {
                return static_cast <std::ptrdiff_t > (i);
            }
        }
        return -1;
    }

    /// \brief Поиск указанного значения в спане с конца.
    /// \param val Искомое значение.
    /// \return Индекс последнего вхождения найденного элемента,
    /// либо отрицательное значение.
    std::ptrdiff_t lastIndexOf (const T val) const noexcept
    {
        for (auto i = static_cast <std::ptrdiff_t> (this->length - 1); i >= 0; --i) {
            if (this->ptr[i] == val) {
                return i;
            }
        }
        return -1;
    }

    /// \brief Срез спана.
    /// \param start Смещение начала спана.
    /// \param length_ Длина среза.
    /// \return Срез.
    ///
    /// Копирования данных не происходит.
    Span<T> slice (std::ptrdiff_t start, std::ptrdiff_t length_=-1) const noexcept
    {
        if (length_ == -1) {
            length_ = this->length - start;
        }

        if (length_ < 0) {
            length_ = 0;
        }

        return Span<T> (this->ptr + start, length_);
    }

    /// \brief Сырой указатель на начало спана.
    /// \return Указатель.
    T* begin() const noexcept { return &this->ptr[0]; }

    /// \brief Константный сырой указатель на начало спана.
    /// \return Указатель.
    const T* cbegin() const noexcept { return &this->ptr[0]; }

    /// \brief Сырой указатель за концом спана.
    /// \return Указатель.
    T* end() const noexcept { return &this->ptr[this->length]; }

    /// \brief Константный сырой указатель за концом спана.
    /// \return Указатель.
    const T* cend() const noexcept { return &this->ptr[this->length]; }

    /// \brief Конверсия в строку.
    /// \return Строка.
    /// \warning Копирует данные из спана!
    std::basic_string<T> toString() const
    {
        std::basic_string<T> result;
        result.reserve(this->length);
        for (std::size_t ofs = 0; ofs < this->length; ofs++) {
            result.push_back (this->ptr[ofs]);
        }
        return result;
    }

    /// \brief Конверсия в вектор.
    /// \return Вектор.
    /// \warning Копирует данные из спана!
    std::vector<T> toVector() const
    {
        std::vector<T> result;
        result.reserve(this->length);
        for (std::size_t ofs = 0; ofs < this->length; ofs++) {
            result.push_back(this->ptr[ofs]);
        }

        return result;
    }

    /// \brief Совпадает ли начало спана с другим спаном?
    /// \param prefix Префикс.
    /// \return `true` если совпадает.
    bool startsWith (const Span<T> prefix) const noexcept //-V801
    {
        if (prefix.size() > this->size()) {
            return false;
        }

        auto p1 = this->cbegin();
        auto p2 = prefix.cbegin(), e2 = prefix.cend();

        while (p2 < e2) {
            if (*p1 != *p2) {
                return false;
            }
            ++p1; ++p2;
        }

        return true;
    }

    /// \brief Совпадает ли конец спана с другим спаном?
    /// \param suffix Суффикс.
    /// \return `true` если совпадает.
    bool endsWith (const Span<T> suffix) const noexcept //-V801
    {
        if (suffix.size() > this->size()) {
            return false;
        }

        auto p1 = this->cend() - suffix.size();
        auto p2 = suffix.cbegin(), e2 = suffix.cend();

        while (p2 < e2) {
            if (*p1 != *p2) {
                return false;
            }
            ++p1; ++p2;
        }

        return true;
    }

    /// \brief Поэлементное сравнение двух диапазонов.
    /// \param other Диапазон для сравнения.
    /// \return Результат сравнения:
    /// 0 если все элементы совпадают.
    int compare (const Span<T> other) const //-V801
    {
        auto size = this->size();
        if (other.size() < size) {
            size = other.size();
        }

        auto p1 = this->cbegin(), e2 = this->cbegin() + size;
        auto p2 = other.cbegin();

        while (p1 < e2) {
            const int result = *p1 - *p2;
            if (result != 0) {
                return result;
            }
            ++p1; ++p2;
        }

        if (this->size() == other.size()) {
            return 0;
        }

        if (this->size() > other.size()) {
            return 1;
        }

        return -1;
    }

    /// \brief Разбиение текста на фрагменты.
    /// \param delimiter Символ-разделитель.
    /// \return Вектор спанов.
    ///
    /// Пустые фрагменты не включаются в результат.
    std::vector<Span<T>> split (T delimiter) const
    {
        std::vector<Span<T>> result;
        auto start = this->cbegin(), current = start, end = this->cend();

        while (start < end) {
            while (current < end) {
                if (*current == delimiter) {
                    break;
                }
                ++current;
            }
            if (current != start) {
                result.emplace_back (start, current - start);
            }
            start = ++current;
        }

        return result;
    }

    /// \brief Разбиение текста на фрагменты.
    /// \param delimiter Символ-разделитель.
    /// \param nelem Максимальное количество элементов
    /// \return Вектор спанов.
    ///
    /// Пустые фрагменты не включаются в результат.
    std::vector<Span<T>> split (T delimiter, int nelem) const
    {
        std::vector<Span<T>> result;
        auto start = this->cbegin(), current = start, end = this->cend();

        --nelem;
        while (nelem > 0 && start < end) {
            while (current < end) {
                if (*current == delimiter) {
                    break;
                }
                ++current;
            }
            if (current != start) {
                result.emplace_back (start, current - start);
            }
            start = ++current;
            --nelem;
        }
        if (nelem <= 0 && start < end) {
            result.emplace_back (start, end - start);
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор спана как целого числа.
    /// \return Результат разбора.
    ///
    /// \warning Мусор на входе -- мусор на выходе!
    int32_t parseInt32() const noexcept
    {
        int32_t result = 0;
        auto p = this->cbegin(), e = this->cend();
        while (p < e) {
            result = result * 10 + (*p - '0');
            ++p;
        }

        return result;
    }

    /// \brief Быстрый и грязный разбор спана как целого числа.
    /// \return Результат разбора.
    ///
    /// \warning Мусор на входе -- мусор на выходе!
    int64_t parseInt64() const noexcept
    {
        int64_t result = 0;
        auto p = this->cbegin(), e = this->cend();
        while (p < e) {
            result = result * 10 + (*p - '0');
            ++p;
        }

        return result;
    }
};

template <class T>
inline bool operator == (const Span<T> left, const Span<T> right) //-V801
{
    return left.compare (right) == 0;
}

template <class T>
inline bool operator == (const Span<T> left, const std::basic_string<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) == 0;
}

template <class T>
inline bool operator == (const Span<T> left, const std::vector<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) == 0;
}

template <class T>
inline bool operator != (const Span<T> left, const Span<T> right) //-V801
{
    return left.compare (right) != 0;
}

template <class T>
inline bool operator != (const Span<T> left, const std::basic_string<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) != 0;
}

template <class T>
inline bool operator != (const Span<T> left, const std::vector<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) != 0;
}

template <class T>
inline bool operator < (const Span<T> left, const Span<T> right) //-V801
{
    return left.compare (right) < 0;
}

template <class T>
inline bool operator < (const Span<T> left, const std::basic_string<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) < 0;
}

template <class T>
inline bool operator < (const Span<T> left, const std::vector<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) < 0;
}

template <class T>
inline bool operator <= (const Span<T> left, const Span<T> right) //-V801
{
    return left.compare (right) <= 0;
}

template <class T>
inline bool operator <= (const Span<T> left, const std::basic_string<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) <= 0;
}

template <class T>
inline bool operator <= (const Span<T> left, const std::vector<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) <= 0;
}

template <class T>
inline bool operator > (const Span<T> left, const Span<T> right) //-V801
{
    return left.compare (right) > 0;
}

template <class T>
inline bool operator > (const Span<T> left, const std::basic_string<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) > 0;
}

template <class T>
inline bool operator > (const Span<T> left, const std::vector<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) > 0;
}

template <class T>
inline bool operator >= (const Span<T> left, const Span<T> right) //-V801
{
    return left.compare (right) >= 0;
}

template <class T>
inline bool operator >= (const Span<T> left, const std::basic_string<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) >= 0;
}

template <class T>
inline bool operator >= (const Span<T> left, const std::vector<T> &right) //-V801
{
    return left.compare (Span<T> (right.data(), right.size())) >= 0;
}

using CharSpan = Span<char>;
using WideSpan = Span<Char>;
using ByteSpan = Span<Byte>;

IRBIS_API bool     IRBIS_CALL sameString (CharSpan first, CharSpan second);
IRBIS_API bool     IRBIS_CALL sameString (WideSpan first, WideSpan second);
IRBIS_API CharSpan IRBIS_CALL trimStart  (CharSpan text);
IRBIS_API WideSpan IRBIS_CALL trimStart  (WideSpan text);
IRBIS_API CharSpan IRBIS_CALL trimEnd    (CharSpan text);
IRBIS_API WideSpan IRBIS_CALL trimEnd    (WideSpan text);
IRBIS_API CharSpan IRBIS_CALL trim       (CharSpan text);
IRBIS_API WideSpan IRBIS_CALL trim       (WideSpan text);
IRBIS_API CharSpan IRBIS_CALL toUpper    (CharSpan text);
IRBIS_API WideSpan IRBIS_CALL toUpper    (WideSpan text);
IRBIS_API CharSpan IRBIS_CALL toLower    (CharSpan text);
IRBIS_API WideSpan IRBIS_CALL toLower    (WideSpan text);

//=========================================================

/// \brief Данные, нарезанные ломтями.
template <class T>
class ChunkedData final
{
private:
    /// \brief Ломоть данных.
    struct Chunk final
    {
        T *begin;
        T *end;
    };

public:

    /// \brief Итератор.
    struct Iterator final
            : public std::iterator<std::bidirectional_iterator_tag, T>
    {

        constexpr Iterator (ChunkedData<T> *data, std::size_t index, T *currentIterator) noexcept
            : _data { data }, _index { index }, _currentIterator { currentIterator } {}

        bool operator == (const Iterator &other) const noexcept
        {
            return this->_currentIterator == other._currentIterator;
        }

        bool operator != (const Iterator &other) const noexcept
        {
            return this->_currentIterator != other._currentIterator;
        }

        /// \brief Преинкремент
        /// \return
        Iterator& operator ++ ()
        {
            ++this->_currentIterator;
            if (this->_currentIterator == this->_data->_chunks [this->_index].end) {
                ++this->_index;
                if (this->_index == this->_data->_chunks.size()) {
                    this->_currentIterator = this->_data->_chunks [this->_index - 1].end;
                }
                else {
                    this->_currentIterator = this->_data->_chunks [this->_index].begin;
                }
            }
            return *this;
        }

        /// \brief Постинкремент
        /// \return
        Iterator operator ++ (int)
        {
            Iterator copy (this->_data, this->_index, this->_currentIterator);

            ++this->_currentIterator;
            if (this->_currentIterator == this->_data->_chunks [this->_index].end) {
                ++this->_index;
                if (this->_index == this->_data->_chunks.size()) {
                    this->_currentIterator = this->_data->_chunks [this->_index - 1].end;
                }
                else {
                    this->_currentIterator = this->_data->_chunks [this->_index].begin;
                }
            }

            return copy;
        }

        /// \brief Предекремент
        /// \return
        Iterator& operator -- ()
        {
            if (this->_index >= this->_data->_chunks.size()) {
                this->_index = this->_data->_chunks.size() - 1;
                this->_currentIterator = this->_data->_chunks [this->_index].end;
            }
            if (this->_currentIterator == this->_data->_chunks [this->_index].begin) {
                --this->_index;
                this->_currentIterator = this->_data->_chunks [this->_index].end;
            }
            --this->_currentIterator;
            return *this;
        }

        /// \brief Постинкремент
        Iterator operator -- (int)
        {
            Iterator copy (this->_data, this->_index, this->_currentIterator);

            if (this->_index >= this->_data->_chunks.size()) {
                this->_index = this->_data->_chunks.size() - 1;
                this->_currentIterator = this->_data->_chunks [this->_index].end;
            }
            if (this->_currentIterator == this->_data->_chunks [this->_index].begin) {
                --this->_index;
                this->_currentIterator = this->_data->_chunks [this->_index].end;
            }
            --this->_currentIterator;

            return copy;
        }

        T& operator *  () const { return *(this->_currentIterator); } ///< Оператор разыменования.
        T& operator -> () const { return *(this->_currentIterator); } ///< Оператор доступа к члену.

    private:
        ChunkedData *_data;
        std::size_t _index;
        T *_currentIterator;
    };

    /// \brief Оператор индексирования.
    /// \param offset Смещение.
    /// \return Ссылка на полученный элемент.
    T& operator[] (std::size_t offset)
    {
        for (const auto &chunk : this->_chunks) {
            const auto length = static_cast <std::size_t> (chunk.end - chunk.begin);
            if (offset < length) {
                return chunk.begin [offset];
            }
            offset -= length;
        }
        // Вышли за пределы.
        throw std::exception();
    }

    /// \brief Сложение двух контейнеров.
    /// \param left
    /// \param right
    /// \return
    friend ChunkedData<T> operator + (const ChunkedData<T> &left, const ChunkedData<T> &right)
    {
        ChunkedData<T> result (left);
        return (result += right);
    }

    /// \brief Дописывание в конец другого контейнера.
    /// \param other
    /// \return
    ChunkedData<T> operator += (const ChunkedData &other)
    {
        for (const auto &chunk : other._chunks) {
            this->_chunks.push_back (chunk);
        }
        return *this;
    }

    /// \brief Добавление ломтя данных.
    /// \param begin Указатель на начало.
    /// \param end Указатель за концом.
    void append (T *begin, T *end)
    {
        if (begin != end) {
            Chunk chunk;
            chunk.begin = begin;
            chunk.end = end;
            this->_chunks.push_back (chunk);
        }
    }

    /// \brief Добавление ломтя данных.
    /// \param begin Указатель на начало.
    /// \param length Размер ломтя в элементах.
    void append (T *begin, std::size_t length)
    {
        if (length != 0) {
            Chunk chunk;
            chunk.begin = begin;
            chunk.end = begin + length;
            this->_chunks.push_back (chunk);
        }
    }

    /// \brief Добавление ломтя-спана.
    /// \param span Спан.
    void append (Span<T> span)
    {
        if (!span.empty()) {
            Chunk chunk;
            chunk.begin = span.ptr;
            chunk.end = span.ptr + span.size();
            this->_chunks.push_back (chunk);
        }
    }

    /// \brief Добавление строки.
    /// \param text Добавляемый текст.
    void append (const std::basic_string<T> &text)
    {
        if (!text.empty()) {
            Chunk chunk;
            chunk.begin = const_cast<T*> (text.data());
            chunk.end = chunk.begin + text.size();
            this->_chunks.push_back (chunk);
        }
    }

    /// \brief Добавление вектора.
    /// \param vector Добавляемый вектор.
    void append (const std::vector<T> &vector)
    {
        if (!vector.empty()) {
            Chunk chunk;
            chunk.begin = vector.data();
            chunk.end = chunk.begin + vector.size();
            this->_chunks.push_back (chunk);
        }
    }

    /// \brief Ссылка на последний элемент данных.
    /// \return Ссылка.
    T& back() noexcept
    {
        return this->_chunks.back().end[-1];
    }

    /// \brief Итератор на начало данных.
    /// \return Итератор.
    Iterator begin () const noexcept
    {
        auto This = const_cast <ChunkedData<T>*> (this);
        if (This->_chunks.empty()) {
            return Iterator (This, 0, nullptr);
        }
        return Iterator (This, 0, This->_chunks[0].begin);
    }

    /// \brief Контейнер пуст?
    /// \return true если пуст.
    bool empty () const noexcept
    {
        return this->_chunks.empty();
    }

    /// \brief Итератор за концом данных.
    /// \return Итератор.
    Iterator end () const noexcept
    {
        auto This = const_cast <ChunkedData<T>*> (this);
        if (This->_chunks.empty()) {
            return Iterator (This, 0, nullptr);
        }
        auto last = This->_chunks.size() - 1;
        return Iterator (This, last, This->_chunks [last].end);
    }

    /// \brief Ссылка на первый элемент данных.
    /// \return Ссылка.
    T& front() noexcept
    {
        return *this->_chunks[0].begin;
    }

    /// \brief Общий размер контейнера.
    /// \return Размер контейнера в элементах.
    std::size_t size () const noexcept
    {
        std::size_t result = 0;
        for (const auto &chunk : this->_chunks) {
            //result += std::distance(chunk.end, chunk.begin);
            result += chunk.end - chunk.begin;
        }
        return result;
    }

    /// \brief Выделение поддиапазона данных.
    /// \param offset Смещение.
    /// \param length Длина отрезка.
    /// \return Выделенные данные.
    ChunkedData<T> slice (std::size_t offset, std::size_t length) const
    {
        ChunkedData<T> result;
        for (const auto &chunk : this->_chunks) {
            if (!length) {
                break;
            }
            const auto chunkSize = static_cast<std::size_t> (chunk.end - chunk.begin);
            if (offset < chunkSize) {
                auto extent = length;
                if (chunkSize < extent + offset) {
                    extent = chunkSize - offset;
                }
                result.append (chunk.begin + offset, extent);
                length -= extent;
                offset = 0;
            }
            else {
                offset -= chunkSize;
            }
        }
        return result;
    }

    /// \brief Преобразование в стандартную строку.
    /// \return Строка.
    std::basic_string<T> toString()
    {
        std::basic_string<T> result;
        result.reserve (this->size());
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

    /// \brief Преобразование в стандартный вектор.
    /// \return Вектор изо всех элементов, возможно, пустой.
    std::vector<T> toVector()
    {
        std::vector<T> result;
        result.reserve (this->size());
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

private:
    friend struct Chunk;
    std::vector<Chunk> _chunks;
};

using ChunkedBytes  = ChunkedData <Byte>;
using ChunkedString = ChunkedData <char>;
using ChunkedWide   = ChunkedData <Char>;

//=========================================================

/// \brief Данные, рассованные по контейнерам.
/// \tparam DataType Тип данных.
/// \tparam IteratorType Тип итератора.
template <class DataType, class IteratorType>
class JoinedData final
{
private:

    /// \brief Пара итераторов.
    struct Pair final
    {
        IteratorType m_begin; ///< Начало данных.
        IteratorType m_end;   ///< Прямо за концом данных.

        Pair () = default;
        constexpr Pair (IteratorType begin, IteratorType end) noexcept
            : m_begin { begin }, m_end { end } {}
        ~Pair() = default;

        std::size_t size() const noexcept
        {
            return std::distance (m_begin, m_end);
        }
    };

    std::vector <Pair> m_data {};

public:

    /// \brief Итератор.
    struct iterator final
        : public std::iterator<std::bidirectional_iterator_tag, DataType>
    {
        JoinedData *m_joiner;
        std::size_t m_offset;

        constexpr iterator() : m_joiner { nullptr }, m_offset { 0 } {}
        constexpr iterator (JoinedData *joiner, std::size_t offset)
            : m_joiner { joiner }, m_offset { offset } {}

        iterator (const iterator&) = default;
        iterator (iterator&&) noexcept = default;
        iterator& operator = (const iterator&) = default;
        iterator& operator = (iterator&&) noexcept = default;

        bool operator == (const iterator &other) const noexcept
        {
            return m_offset == other.m_offset;
        }

        bool operator != (const iterator &other) const noexcept
        {
            return m_offset != other.m_offset;
        }

        iterator& operator ++ () noexcept
        {
            ++m_offset;
            return *this;
        }

        iterator operator ++ (int) noexcept
        {
            iterator copy (*this);
            ++m_offset;
            return copy;
        }

        iterator& operator += (std::size_t n) noexcept
        {
            m_offset += n;
            return *this;
        }

        iterator& operator -- () noexcept
        {
            --m_offset;
            return *this;
        }

        iterator operator -- (int) & noexcept
        {
            iterator copy (*this);
            --m_offset;
            return copy;
        }

        iterator& operator -= (std::size_t n) noexcept
        {
            m_offset -= n;
            return *this;
        }

        DataType& operator *  () const noexcept { return *((*m_joiner) [m_offset]); }
        DataType& operator -> () const noexcept { return *((*m_joiner) [m_offset]); }

        friend iterator operator + (const iterator &left, std::size_t right) noexcept
        {
            auto result = left;
            result += right;
            return result;
        }

        friend std::ptrdiff_t operator - (const iterator &left, const iterator &right) noexcept
        {
            return static_cast<std::ptrdiff_t> (left.m_offset - right.m_offset);
        }
    };

    JoinedData () = default; ///< Конструктор по умолчанию.

    /// \brief Конструктор со списком.
    JoinedData (std::initializer_list<IteratorType> pairs)
    {
        const auto end = std::end (pairs);
        auto it = std::begin (pairs);
        while (it != end) {
            const auto begin_ = *it;
            ++it;
            if (it != end) {
                const auto end_ = *it;
                ++it;
                this->append (begin_, end_);
            }
        }
    }

    ~JoinedData () = default; ///< Деструктор.

    IteratorType operator [] (std::size_t offset) const
    {
        for (const auto &item : m_data) {
            const auto itemSize = item.size();
            if (offset < itemSize) {
                return std::next (item.m_begin, offset);
            }
            offset -= itemSize;
        }
        return m_data.back().m_end;
    }

    /// \brief Сложение двух контейнеров.
    /// \param left
    /// \param right
    /// \return
    friend JoinedData operator + (const JoinedData &left, const JoinedData &right)
    {
        JoinedData result (left);
        return (result += right);
    }

    /// \brief Дописывание в конец другого контейнера.
    /// \param other
    /// \return
    JoinedData operator += (const JoinedData &other)
    {
        for (const auto &data : other.m_data) {
            m_data.push_back (data);
        }
        return *this;
    }

    JoinedData& append (IteratorType begin, IteratorType end)
    {
        if (begin != end) {
            m_data.emplace_back (begin, end);
        }
        return *this;
    }

    JoinedData& append (Span<DataType> span)
    {
        return append (span.begin(), span.end());
    }

    template <class Container>
    JoinedData& append (Container &&container)
    {
        return append (std::begin (container), std::end (container));
    }

    /// \brief Ссылка на последний элемент данных.
    /// \return Ссылка.
    DataType& back() noexcept
    {
        return *(--this->end());
    }

    iterator begin () const noexcept
    {
        return iterator (const_cast<JoinedData*>(this), 0);
    }

    /// \brief Контейнер пуст?
    /// \return true, если пуст.
    bool empty () const noexcept
    {
        return m_data.empty();
    }

    iterator end () const noexcept
    {
        auto This = const_cast<JoinedData*>(this);
        iterator result (This, this->size());
        return result;
    }

    /// \brief Ссылка на первый элемент данных.
    /// \return Ссылка.
    DataType& front() noexcept
    {
        return *(this->begin());
    }

    /// \brief Размер контейнера в элементах.
    /// \return Общий размер.
    std::size_t size() const noexcept
    {
        std::size_t result = 0;
        for (const auto &item : m_data) {
            result += item.size();
        }
        return result;
    }

    JoinedData slice (std::size_t offset, std::size_t length) const
    {
        JoinedData<DataType, IteratorType> result;
        result.m_data.reserve (m_data.size());
        for (const auto &item : m_data) {
            if (!length) {
                break;
            }
            const auto itemSize = item.size();
            if (offset < itemSize) {
                auto extent = length;
                if (itemSize < extent + offset) {
                    extent = itemSize - offset;
                }
                auto end_ = item.m_end;
                if ((offset + extent) < itemSize) {
                    end_ = std::next (item.m_begin, offset + extent);
                }
                result.append (std::next (item.m_begin, offset), end_);
                length -= extent;
                offset = 0;
            }
            else {
                offset -= itemSize;
            }
        }
        return result;
    }

    /// \brief Преобразование в стандартную строку.
    /// \return Строка.
    std::basic_string<DataType> toString()
    {
        std::basic_string<DataType> result;
        result.reserve (this->size());
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

    /// \brief Преобразование в стандартный вектор.
    /// \return Вектор изо всех элементов, возможно, пустой.
    std::vector<DataType> toVector() const
    {
        std::vector<DataType> result;
        result.reserve (this->size());
        result.insert (std::end (result), std::begin (*this), std::end (*this));
        return result;
    }

};

//=========================================================

/// \brief Перечислитель объектов. Абстрактный базовый класс.
/// \tparam T Тип перечисляемых объектов
///
/// Поддерживает простой перебор по некоторой коллекции.
template <class T>
class Enumerator
{
public:
    bool moveNext() = 0;   ///< Перемещает перечислитель к следующему элементу коллекции.
    T current() const = 0; ///< Возвращает элемент коллекции, соответствующий текущей позиции перечислителя.
};

//=========================================================

/// \brief Диапазон, задаваемый двумя итераторами.
/// \tparam IteratorType Тип итератора.
template<class IteratorType>
struct Range
{
    IteratorType m_begin;   // Начало диапазона (включается).
    IteratorType m_current; // Текущая позиция.
    IteratorType m_end;     // Конец диапазона (не включается).

    constexpr Range (IteratorType begin, IteratorType end) noexcept
        : m_begin { begin }, m_current { begin }, m_end { end } {}
    Range             (const Range &) noexcept = default;
    Range             (Range &&)      noexcept = default;
    Range& operator = (const Range &) noexcept = default;
    Range& operator = (Range &&)      noexcept = default;
    ~Range            ()                       = default;

    /// \brief Начало диапазона (включительно).
    IteratorType begin() const noexcept { return this->m_begin; }

    /// \brief Текущая позиция итератора (может указывать на конец).
    IteratorType current() const noexcept { return this->m_current; }

    /// \brief Пустой диапазон?
    /// \return true, если пустой.
    bool empty() const noexcept { return !(this->m_begin != this->m_end); }

    /// \brief Конец диапазона (не включается).
    IteratorType end() const noexcept { return this->m_end; }

    /// \brief Переход к следующему элементу.
    /// \return true, если был выполнен шаг вперед.
    bool moveNext()
    {
        if (this->m_current != this->m_end) {
            ++this->m_current;
            return this->m_current != this->m_end;
        }
        return false;
    }

    /// \brief Возврат к начальной позиции.
    void reset() noexcept { this->m_current = this->m_begin; }

    /// \brief Значение в текущей позиции.
    auto value() -> decltype (*(this->current())) { return *(this->current()); }
};

template<class T>
Range<typename T::iterator> makeRange (T& value)
{
    return Range <typename T::iterator> (std::begin (value), std::end (value));
}

//=========================================================

/// \brief Реализация идиомы PImpl, не требующая динамической аллокации.
/// \tparam HiddenData Тип скрываемых данных (интерфейс).
/// \tparam DataSize Размер скрываемых данных, байты.
/// \tparam DataAlignment Выравнивание скрываемых данных, байты.
/// \details См. выступление Антона Полухина
/// "C++ трюки из Такси" на CoreHard Autumn 2019
/// https://www.youtube.com/watch?v=_AkF8SpUV3k&t=3s
template <class HiddenData, std::size_t DataSize,
    std::size_t DataAlignment = std::alignment_of <HiddenData>::value>
class FastPImpl
{
public:

    /// \brief Конструктор.
    /// \tparam Args Тип данных.
    /// \param args Данные для инициализации.
    template <class ... Args>
    explicit FastPImpl (Args&& ... args)
    {
        validate <sizeof (HiddenData), alignof (HiddenData)>();
        new (this->ptr()) HiddenData (args...);
    }

    /// \brief Конструктор копирования.
    /// \param other Объект, подлежащий копированию.
    FastPImpl (const FastPImpl &other)
    {
        auto &another = const_cast <FastPImpl&> (other);
        const auto &otherData = *(another.ptr());
        new (this->ptr()) HiddenData (otherData);
    }

    /// \brief Оператор копирования.
    /// \param other Объект, подлежащий копированию.
    /// \return this.
    FastPImpl& operator = (const FastPImpl &other)
    {
        auto another = const_cast <FastPImpl*> (&other);
        *(this->ptr()) = *(another->ptr());
        return *this;
    }

    /// \brief Конструктор перемещения.
    /// \param other Объект, из которого происходит перемещение.
    FastPImpl (FastPImpl &&other) noexcept
    {
        validate <sizeof (HiddenData), alignof (HiddenData)>();
        *this->ptr() = std::move (*other.ptr());
    }

    /// \brief Оператор перемещения.
    /// \param other Объект, их которого происходит перемещение.
    /// \return this.
    FastPImpl& operator = (FastPImpl &&other) noexcept
    {
        validate <sizeof (HiddenData), alignof (HiddenData)>();
        *this->ptr() = std::move (*other.ptr());
        return *this;
    }

    /// \brief Деструктор.
    ~FastPImpl()
    {
        validate <sizeof (HiddenData), alignof (HiddenData)>();
        this->ptr()-> ~HiddenData();
    }

    HiddenData* operator -> () noexcept { return this->ptr();  } ///< Обращение к членам.
    HiddenData& operator *  () noexcept { return *this->ptr(); } ///< Получение значения.

    /// \brief Получение указателя на объект.
    /// \return
    HiddenData* ptr() noexcept
    {
        return reinterpret_cast <HiddenData*> (&m_data);
    }

private:

    /// \brief Валидация.
    /// \tparam ActualSize Актуальный размер данных.
    /// \tparam ActualAlignment Актулаьное выравнивание.
    template <std::size_t ActualSize, std::size_t ActualAlignment>
    void validate ()
    {
        static_assert (DataSize >= ActualSize,
            "DataSize and ActualSize mismatch");
        static_assert (DataAlignment == ActualAlignment,
            "DataAlignment and ActualAlignment mismatch");
    }

    /// \brief Буфер для данных.
    typename std::aligned_storage <DataSize, DataAlignment>::type m_data;
};

//=========================================================

/// \brief Базовое исключение для всех нештатных ситуаций.
class IRBIS_API IrbisException
        : public std::exception
{
public:
};

//=========================================================

/// \brief Файл на сервере не найден.
class IRBIS_API FileNotFoundException
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

/// \brief Штрих-код длиной 13 символов.
class IRBIS_API Ean13 final
{
public:
    static char computeCheckDigit (CharSpan text) noexcept;
    static Char computeCheckDigit (WideSpan text) noexcept;
    static bool checkControlDigit (CharSpan text) noexcept;
    static bool checkControlDigit (WideSpan text) noexcept;
};

//=========================================================

/// \brief Штрих-код длиной 8 символов.
class IRBIS_API Ean8 final
{
public:
    static char computeCheckDigit (CharSpan text) noexcept;
    static Char computeCheckDigit (WideSpan text) noexcept;
    static bool checkControlDigit (CharSpan text) noexcept;
    static bool checkControlDigit (WideSpan text) noexcept;
};

//=========================================================

/// \brief INI-файл.
class IRBIS_API IniFile final
{
public:
    std::vector<IniSection> sections; ///< Секции INI-файла.

    IniFile&       clear           ();
    bool           containsSection (const String &name) const noexcept;
    IniSection&    createSection   (const String &name);
    bool           modified        () const noexcept;
    void           notModified     ();
    std::ptrdiff_t getIndex        (const String &name) const noexcept;
    IniSection*    getSection      (const String &name) const noexcept;
    String         getValue        (const String &sectionName, const String &keyName, const String &defaultValue = L"") const noexcept;
    void           parse           (const StringList &lines);
    IniFile&       removeSection   (const String &sectionName);
    IniFile&       removeValue     (const String &sectionName, const String &keyName);
    IniFile&       setValue        (const String &sectionName, const String &keyName, const String &value);
    String         toString        () const;
    //void write(const std::wstring &filename, QTextCodec *encoding) const;
    //void writeModifiedOnly(QTextStream &stream) const;
};

//=========================================================

/// \brief Строка INI-файла.
///
/// Состоит из ключа и значения, разделённых символом '='.
class IRBIS_API IniLine final
{
public:
    String key;    ///< Ключ (не должен быть пустым).
    String value;  ///< Значение (может быть пустым).

    IniLine() = default;
    IniLine (const String &key_, const String &value_) : key (key_), value (value_) {}
    IniLine (String &&key_, String &&value_) : key (std::move (key_)), value (std::move (value_)) {}

    bool   modified    ()                       const noexcept;
    void   notModified ()                       noexcept;
    void   setKey      (const String &newKey);
    void   setKey      (String &&newKey)        noexcept;
    void   setValue    (const String &newValue);
    void   setValue    (String &&newValue)      noexcept;
    String toString    ()                       const;

private:
    bool _modified { false };
};

//=========================================================

/// \brief Секция INI-файла.
class IRBIS_API IniSection final
{
public:
    String name;                ///< Имя секции.
    std::vector<IniLine> lines; ///< Строки, входящие в секцию. \see IniLine

    IniSection&    clear       ();
    bool           containsKey (const String &key) const noexcept;
    std::ptrdiff_t getIndex    (const String &key) const noexcept;
    IniLine*       getLine     (const String &key) const noexcept;
    String         getValue    (const String &key, const String &defaultValue) const noexcept;
    bool           modified    ()                  const noexcept;
    void           notModified ();
    IniSection&    removeValue (const String &key);
    IniSection&    setValue    (const String &key, const String &value);
    String         toString    ()                  const;

    String operator[] (const String &index) const noexcept;
};

//=========================================================

/// \brief Таблица алфавитных символов.
class IRBIS_API AlphabetTable final
{
public:
    const static String FileName;
    std::set<Char> characters;

    static const AlphabetTable& instance();
    static AlphabetTable readLocalFile (const String &fileName);

    bool       isAlpha    (const Char &c)            const;
    void       parse      (const StringList &lines);
    void       setup      (const Bytes &bytes);
    StringList splitWords (const String &text)       const;
    String     trimText   (const String &text)       const;
    bool       verify     (bool throwOnError)        const;
};

//=========================================================

/// \brief Информация о подключенном клиенте.
class IRBIS_API ClientInfo final
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

/// \brief Этапы выполнения запроса к серверу.
enum class RequestStage
{
    None,           ///< В состоянии простоя.
    BuildPackage,   ///< Формирование пакета запроса.
    NameResolution, ///< Разрешение имени сервера.
    Connecting,     ///< Установка подключения к серверу.
    Sending,        ///< Передача пакета запроса на сервер.
    Waiting,        ///< Ожидание ответа от сервера.
    Receiving,      ///< Получение ответного пакета от сервера.
    Shutdown,       ///< Отключение от сервера.
    ParseHeader,    ///< Разбор заголовка ответа.
    ParseBody,      ///< Разбор тела ответа (если есть).
    Error           ///< Ошибка.
};

//=========================================================

/// \brief Базовые функции подключения
class IRBIS_API ConnectionBase
{
private:
    bool _connected;
    StringList _databaseStack;
    std::mutex _mutex;

    friend class ServerResponse;

protected:

    bool _checkConnection();

public:

    String       host;                     ///< Адрес сервера в виде строки.
    short        port;                     ///< Номер порта сервера.
    String       username;                 ///< Логин пользователя.
    String       password;                 ///< Пароль пользователя.
    String       database;                 ///< Имя текущей базы данных.
    String       workstation;              ///< Тип клиента.
    int          clientId;                 ///< Уникальный идентификатор клиента.
    int          queryId;                  ///< Порядковый номер команды, отсылаемой на сервер.
    int          lastError;                ///< Код ошибки, возвращённый сервером в ответ на последнюю команду.
    String       serverVersion;            ///< Версия сервера (получается при подключении).
    IniFile      iniFile;                  ///< Содержимое серверного INI-файла для данного клиента.
    int          interval;                 ///< Интервал автоматического подтверждения, секунды.
    RequestStage stage;                    ///< Этап выполнения запроса
    std::unique_ptr <ClientSocket> socket; ///< Клиентский сокет.

    ConnectionBase  ();
    ~ConnectionBase ();

    /// \brief Подключено ли к серверу?
    bool   connected() const noexcept { return this->_connected; }

    IRBIS_MAYBE_UNUSED bool   connect               ();
                       void   disconnect            ();
    IRBIS_MAYBE_UNUSED bool   execute               (ClientQuery &query);
                       Mfn    getMaxMfn             (const String &databaseName);
    IRBIS_MAYBE_UNUSED bool   noOp                  ();
                       void   parseConnectionString (const std::string &connectionString);
                       void   parseConnectionString (const String &connectionString);
                       String popDatabase           ();
                       String pushDatabase          (const String &newDatabase);
                       String toConnectionString    () const;
};

/// \brief Функции работы с контекстом
class IRBIS_API ConnectionContext : public virtual ConnectionBase
{
public:
    std::vector<DatabaseInfo> listDatabases  (const IniFile &iniFile, const String &defaultFileName);
    std::vector<DatabaseInfo> listDatabases  (const FileSpecification &specification);
    StringList                listFiles      (const FileSpecification &specification);
    StringList                listFiles      (const std::vector<FileSpecification> &specifications);
    Bytes                     readBinaryFile (const FileSpecification &specification);
    IniFile                   readIniFile    (const FileSpecification &specification);
    MenuFile                  readMenuFile   (const FileSpecification &specification);
    String                    readTextFile   (const FileSpecification &specification);
    std::string               readAnsiFile   (const FileSpecification &specification);
    StringList                readTextFiles  (std::vector<FileSpecification> &specifications);
    StringList                readTextLines  (const FileSpecification &specification);
};

/// \brief Функции работы с поисковым индексом
class IRBIS_API ConnectionSearch : public virtual ConnectionBase
{
public:
    StringList               listTerms    (const String &prefix);
    std::vector<TermPosting> readPostings (const PostingParameters &parameters);
    std::vector<TermInfo>    readTerms    (const String &startTerm, int numberOfTerms = 100);
    std::vector<TermInfo>    readTerms    (const TermParameters &parameters);
    MfnList                  search       (const Search &search);
    MfnList                  search       (const String &expression);
    MfnList                  search       (const SearchParameters &parameters);
};

/// \brief Администраторские функции
class IRBIS_API ConnectionAdmin : public virtual ConnectionBase
{
public:
    IRBIS_MAYBE_UNUSED bool                      createDatabase   (const String &databaseName, const String &description, bool readerAccess);
    IRBIS_MAYBE_UNUSED bool                      createDictionary (const String &databaseName = L"");
    IRBIS_MAYBE_UNUSED bool                      deleteDatabase   (const String &databaseName = L"");
                       ServerStat                getServerStat    ();
                       Version                   getServerVersion ();
                       std::vector <ProcessInfo> listProcesses    ();
    IRBIS_MAYBE_UNUSED bool                      reloadDictionary (const String &databaseName = L"");
    IRBIS_MAYBE_UNUSED bool                      reloadMasterFile (const String &databaseName = L"");
    IRBIS_MAYBE_UNUSED bool                      restartServer    ();
    IRBIS_MAYBE_UNUSED bool                      truncateDatabase (const String &databaseName = L"");
    IRBIS_MAYBE_UNUSED bool                      unlockDatabase   (const String &databaseName = L"");
    IRBIS_MAYBE_UNUSED bool                      unlockRecords    (const String &databaseName, const MfnList &mfnList);
};

/// \brief Функции работы с фантомными записями
class IRBIS_API ConnectionPhantom : public virtual ConnectionBase
{
public:
                       PhantomRecord readPhantomRecord  (Mfn mfn);
    IRBIS_MAYBE_UNUSED int           writePhantomRecord (PhantomRecord &record);
};

/// \brief Облегченные функции работы с записями
class IRBIS_API ConnectionLite : public virtual ConnectionBase
{
public:
    LiteRecord readLiteRecord (Mfn mfn);
};

/// \brief Полноценные функции работы с записями
class IRBIS_API ConnectionFull : public virtual ConnectionBase
{
public:
    IRBIS_MAYBE_UNUSED bool                     deleteRecord (Mfn mfn);
                       MarcRecord               readRecord   (Mfn mfn);
                       MarcRecord               readRecord   (const String &databaseName, Mfn mfn);
                       MarcRecord               readRecord   (const String &databaseName, Mfn mfn, int version);
                       std::vector <MarcRecord> readRecords  (const MfnList &mfnList);
    IRBIS_MAYBE_UNUSED int                      writeRecord  (MarcRecord &record, bool lockFlag = false, bool actualize = true, bool dontParseResponse = false);
};

/// \brief Подключение к серверу ИРБИС64.
///
/// Объекты данного типа неперемещаемые.
class IRBIS_API Connection final :
    public virtual ConnectionContext,
    public virtual ConnectionSearch,
    public virtual ConnectionAdmin,
    public virtual ConnectionPhantom,
    public virtual ConnectionLite,
    public virtual ConnectionFull
{
public:

    Connection  ()                             = default; ///< Конструктор по умолчанию.
    Connection  (const Connection&)            = delete;  ///< Конструктор копирования.
    Connection  (Connection&&)                 = delete;  ///< Конструктор перемещения.
    ~Connection ()                             = default; ///< Деструктор.
    Connection& operator = (const Connection&) = delete;  ///< Оператор копирования.
    Connection& operator = (Connection&&)      = delete;  ///< Оператор перемещения.

    bool actualizeDatabase (const String &databaseName);
    bool actualizeRecord (const String &databaseName, int mfn);
    std::future<bool> connectAsync();
    std::future<void> disconnectAsync();
    std::future<bool> executeAsync(ClientQuery &query);
    String formatRecord (const String &format, Mfn mfn);
    std::string formatRecordLite (const std::string &format, Mfn mfn);
    String formatRecord (const String &format, const MarcRecord &record);
    DatabaseInfo getDatabaseInfo (const String &databaseName);
    GblResult globalCorrection (const GblSettings &settings);
    std::vector<UserInfo> getUserList();
    std::future<bool> noOpAsync();
    String printTable (const TableDefinition &definition);
    RawRecord readRawRecord (Mfn mfn);
    std::vector<SearchScenario> readSearchScenario (const FileSpecification &specification);
    String requireTextFile (const FileSpecification &specification);
    bool updateIniFile (const StringList &lines);
    bool updateUserList (const std::vector<UserInfo> &users);
    int writeRawRecord (RawRecord &record, bool lockFlag, bool actualize, bool dontParseResponse);
    bool writeRecords (std::vector<MarcRecord*> &records, bool lockFlag, bool actualize, bool dontParseResponse);
    bool writeRecords (std::vector<MarcRecord> &records, bool lockFlag, bool actualize, bool dontParseResponse);
    void writeTextFile (const FileSpecification &specification);
};

//=========================================================

/// \brief Фабрика подключений.
class IRBIS_API ConnectionFactory
{
public:
    ConnectionFactory             ()                          = default; ///< Конструктор по умолчанию.
    ConnectionFactory             (const ConnectionFactory &) = delete;  ///< Копирующий конструктор.
    ConnectionFactory             (ConnectionFactory&&)       = delete;  ///< Перемещающий конструктор.
    virtual ~ConnectionFactory    ()                          = default; ///< Деструктор.
    ConnectionFactory& operator = (const ConnectionFactory&)  = delete;  ///< Оператор копирования.
    ConnectionFactory& operator = (ConnectionFactory&&)       = delete;  ///< Оператор перемещения.

    virtual Connection* GetConnection();
};

//=========================================================

/// \brief Информация о базе данных ИРБИС.
class IRBIS_API DatabaseInfo final
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

    void   parse   (ServerResponse &response);
    String toString()                        const;

    static std::vector<DatabaseInfo> parse (const MenuFile &menu);
};

//=========================================================

/// \brief Режим прямого доступа к базе данных.
enum DirectAccessMode : uint32_t
{
    Exclusive = 0u, ///< Эксклюзивный (чтение-запись).
    Shared    = 1u, ///< Разделяемый (чтение-запись).
    ReadOnly  = 2u  ///< Только чтение.
};

//=========================================================

/// \brief Встроенное поле.
class IRBIS_API EmbeddedField final
{
public:
    const static Char DefaultCode;

    static RecordFieldList getEmbeddedFields (const RecordField &field, Char sign = DefaultCode);
};

//=========================================================

/// \brief Простая абстракция кодировки символов.
class IRBIS_API Encoding // abstract
{
    static Encoding *_ansi;
    static Encoding *_utf;

public:
    Encoding() = default;
    virtual ~Encoding() = default;

    virtual Bytes       fromUnicode   (const String &text)                   const = 0; ///< Преобразование в UNICODE.
    virtual std::size_t getSize       (const String &text)                   const = 0; ///< Подсчет размера текста в данной кодировке.
    virtual String      toUnicode     (const Byte *bytes, std::size_t count) const = 0; ///< Преобразование из UNICODE.

    static Encoding* ansi     ();
    static String    fromAnsi (const Byte *bytes, std::size_t count);
    static String    fromUtf  (const Byte *bytes, std::size_t count);
    static Bytes     toAnsi   (const String &text);
    static Bytes     toUtf    (const String &text);
    static Encoding* utf      ();
};

class IRBIS_API Cp1251Encoding final
    : public Encoding
{
public:

    Bytes       fromUnicode (const String &text)                   const override;
    String      toUnicode   (const Byte *bytes, std::size_t count) const override;
    std::size_t getSize     (const String &text)                   const override;
};

class IRBIS_API Utf8Encoding final
    : public Encoding
{
public:

    Bytes       fromUnicode (const String &text)                   const override;
    String      toUnicode   (const Byte *bytes, std::size_t count) const override;
    std::size_t getSize     (const String &text)                   const override;
};

//=========================================================

/// \brief Коды (поле 900).
class IRBIS_API Codes final
{
public:
    static const int TAG = 900; ///< Метка поля для кодов.

    String type;
    String kind;
    String character1;
    String character2;
    String character3;
    String character4;
    String character5;
    String character6;
    String purpose1;
    String purpose2;
    String purpose3;
    String restrictions;

    RecordField *field { nullptr };

    void applyTo (RecordField &field_)       const;
    void parse   (const RecordField &field_);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Сведения об экземпляре документа (поле 910).
class IRBIS_API Exemplar final
{
public:
    static const int TAG = 910; ///< Метка поля для экземпляра.

    String status;
    String number;
    String date;
    String place;
    String collection;
    String shelf;
    String price;
    String barcode;
    String amount;
    String purpose;
    String coefficient;
    String offBalance;
    String ksuNumber1;
    String actNumber1;
    String channel;
    String onHand;
    String actNumber2;
    String writeOff;
    String completion;
    String actNumber3;
    String moving;
    String newPlace;
    String checkedDate;
    String checkedAmount;
    String realPlace;
    String bindingIndex;
    String bindingNumber;

    RecordField *field { nullptr };

    void applyTo (RecordField &field_) const;
    void parse (const RecordField &field_);
    static std::vector<Exemplar> parse (const MarcRecord &record);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Сведения о посещении/книговыдаче (поле 40).
class IRBIS_API Visit final
{
public:
    static const int TAG = 40; ///< Метка поля для посещения/книговыдачи.

    String database;
    String index;
    String inventory;
    String barcode;
    String sigla;
    String given;
    String department;
    String expected;
    String returned;
    String prolong;
    String lost;
    String description;
    String responsible;
    String timeIn;
    String timeOut;
    String count;

    RecordField *field { nullptr };

    void applyTo    (RecordField &field_)       const;
    bool isVisit    ()                          const noexcept;
    bool isReturned ()                          const noexcept;
    void parse      (const RecordField &field_);

    static std::vector<Visit> parse (const MarcRecord &record);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Адрес читателя, поле 13 в базе RDR.
class IRBIS_API Address final
{
public:
    static const int TAG = 13;

    String postcode;
    String country;
    String city;
    String street;
    String building;
    String entrance;
    String apartment;
    String additional;

    RecordField *field { nullptr };

    void applyTo (RecordField &field_) const;
    void parse (const RecordField &field_);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Информация о регистрации/перерегистрации читателя.
class IRBIS_API Registration final
{
public:
    String date;
    String chair;
    String number;
    String reason;

    RecordField *field { nullptr };

    void applyTo (RecordField &field_) const;
    void parse (const RecordField &field_);
    static std::vector<Registration> parse (const MarcRecord &record, int tag);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Сведения о читателе (запись в базе RDR).
class IRBIS_API Reader final
{
public:
    String       familyName;
    String       firstName;
    String       patronymic;
    String       birth;
    String       ticket;
    String       gender;
    String       category;
    Address      address;
    String       workplace;
    String       education;
    String       email;
    String       phone;
    Registration registration;
    String       enabled;
    String       disabled;
    String       right;
    String       remarks;
    String       photo;
    String       password;
    std::vector<Visit> visits;
    std::vector<Registration> enrollment;
    std::vector<Registration> registrations;

    Mfn mfn { 0 };
    MarcRecord *record { nullptr };

    void   applyTo  (MarcRecord &record_)        const;
    String fullName ()                           const;
    void   parse    (const MarcRecord &record_);

private:
    String fm (int tag, Char code=0) const noexcept;
};

//=========================================================

/// \brief Информация об авторе.
class IRBIS_API Author final
{
public:

    String familyName;
    String initials;
    String fullName;
    String canBeInverted;
    String postfix;
    String appendix;
    String number;
    String dates;
    String variant;
    String workplace;

    RecordField *field { nullptr };

    void applyTo (RecordField &field_) const;
    void parse (const RecordField &field_);
    static std::vector<Author> parse (const MarcRecord &record, int tag);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Информация о заглавии, поле 200.
class IRBIS_API Title final
{
public:
    static const int TAG = 200; ///< Метка поля заглавия.

    String number;
    String title;
    String specific;
    String general;
    String subtitle;
    String first;
    String other;

    RecordField *field { nullptr };

    void applyTo (RecordField &field_) const;
    String fullTitle() const;
    void parse (const RecordField &field_);

private:
    String fm (Char code) const noexcept;
};

//=========================================================

/// \brief Самая общая информация о книге.
class IRBIS_API BookInfo final
{
public:

    String description;
    String firstAuthor;
    String character;
    String title;
    String type;
    String kind;
    String worksheet;

    MarcRecord *record { nullptr };

    explicit BookInfo (MarcRecord *record_);

    int                   amount();
    StringList            authors();
    bool                  electronic();
    std::vector<Exemplar> exemplars();
    bool                  foreign();
    StringList            languages();
    String                link();
    int                   pages();
    float                 price();
    StringList            publishers();
    int                   usage();
    String                volume();
    int                   year();

private:
    String fm (int tag, Char code=0) const;
};

//=========================================================

/// \brief Код пути к ресурсу.
enum class IrbisPath : unsigned int
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

/// \brief Спецификация имени файла.
class IRBIS_API FileSpecification final
{
public:
    bool binaryFile { false };            ///< Признак двоичного файла.
    IrbisPath path { IrbisPath::System }; ///< Код пути.
    String database;                      ///< Имя базы данных (если необходимо).
    String filename;                      ///< Имя файла (обязательно).
    String content;                       ///< Содержимое файла (если необходимо).

    FileSpecification() = default;                                                ///< Конструктор по умолчанию.
    FileSpecification (IrbisPath path, const String &filename);
    FileSpecification (IrbisPath path, const String &database, const String &filename);
    FileSpecification (const FileSpecification &other) = default;                 ///< Конструктор копирования.
    FileSpecification (FileSpecification &&other) noexcept = default;             ///< Конструктор перемещения.
    FileSpecification& operator = (const FileSpecification &other)  = default;    ///< Оператор копирования.
    FileSpecification& operator = (FileSpecification &&other) noexcept = default; ///< Оператор перемещения.
    ~FileSpecification() = default;                                               ///< Деструктор.

    static FileSpecification parse (const String &text);
    bool verify (bool throwException) const;
    String toString() const;
};

//=========================================================

/// \brief Одна строка из списка найденных записей.
class IRBIS_API FoundLine final
{
public:
    Mfn mfn { 0 };       ///< MFN найденной записи.
    String description;  ///< Опциональный текст, например, результат расформатирования записи.

    FoundLine  ()                 = default; ///< Конструктор по умолчанию.
    FoundLine  (const FoundLine&) = default; ///< Конструктор копирования.
    FoundLine  (FoundLine&&)      = default; ///< Конструктор перемещения.
    ~FoundLine ()                 = default; ///< Деструктор.
    FoundLine  (Mfn mfn_, const String &description_) : mfn(mfn_), description(description_) {} ///< Конструктор
    FoundLine& operator = (const FoundLine&) = default; ///< Оператор копирования
    FoundLine& operator = (FoundLine&&)      = default; ///< Оператор перемещения

    void parse (const String &line);
};

//=========================================================

/// \brief Параметр глобальной корректировки.
class IRBIS_API GblParameter final
{
public:
    String title;
    String value;
};

//=========================================================

/// \brief Результат выполнения глобальной корректировки.
class IRBIS_API GblResult final
{
public:

    void parse (const StringList &lines);
};

//=========================================================

/// \brief Оператор глобальной корректировки.
class IRBIS_API GblStatement final
{
public:
    String command;    ///< Команда, например, ADD или DEL.
    String parameter1; ///< Первый параметр, как правило, спецификация поля/подполя.
    String parameter2; ///< Второй параметр, как правило, спецификация повторения.
    String format1;    ///< Первый формат, например, выражение для замены.
    String format2;    ///< Второй формат, например, заменяющее выражение.

    GblStatement             ()                     = default; ///< Конструктор по умолчанию.
    GblStatement             (const GblStatement &) = default; ///< Конструктор копирования.
    GblStatement             (GblStatement &&)      = default; ///< Конструктор перемешения.
    ~GblStatement            ()                     = default; ///< Деструктор.
    GblStatement& operator = (const GblStatement &) = default; ///< Оператор копирования.
    GblStatement& operator = (GblStatement &&)      = default; ///< Оператор перемещения.
};

//=========================================================

/// \brief Настройки для глобальной корректировки.
class IRBIS_API GblSettings final
{
public:
    bool                      actualize        { false }; ///< Надо ли актуализировать записи?
    bool                      autoin           { false }; ///< Надо ли запускать `autoin.gbl`?
    String                    database;                   ///< Имя базы данных.
    String                    fileName;                   ///< Имя файла глобальной корректировки.
    Mfn                       firstRecord      { 0 };     ///< Mfn первой записи.
    bool                      formalControl    { false }; ///< Применять формальный контроль?
    Mfn                       maxMfn           { 0 };     ///< Максимальный MFN.
    MfnList                   mfnList;                    ///< Список MFN для обработки.
    Mfn                       minMfn           { 0 };     ///< Минимальный MFN. 0 означает "все записи в базе".
    Mfn                       numberOfRecords  { 0 };     ///< Количество записей, подлежащих обработке.
    String                    searchExpression;           ///< Выражение для прямого поиска по словарю.
    String                    sequentialSearch;           ///< Выражение для последовательного поиска.
    std::vector<GblStatement> statements;                 ///< Вектор операторов.
    std::vector<GblParameter> parameters;                 ///< Вектор параметров.

    GblSettings             ()                    = default; ///< Конструктор по умолчанию.
    GblSettings             (const GblSettings &) = default; ///< Конструктор копирования.
    GblSettings             (GblSettings &&)      = default; ///< Конструктор перемещения.
    ~GblSettings            ()                    = default; ///< Деструктор.
    GblSettings& operator = (const GblSettings &) = default; ///< Оператор копирования.
    GblSettings& operator = (GblSettings &&)      = default; ///< Оператор перемещения.
};

//=========================================================

/// \brief Запись в ILF-файле. Хранит информацию об одном ресурсе.
class IRBIS_API IlfEntry final
{
public:
    String   date;               ///< Дата создания.
    String   name;               ///< Имя ресурса.
    String   description;        ///< Описание в произвольной форме.
    String   data;               ///< Собственно данные.
    uint32_t position   { 0 };   ///< Смещение данных от начала файла, байты.
    uint32_t number     { 0 };   ///< Порядковый номер ресурса.
    uint32_t dataLength { 0 };   ///< Длина данных в байтах.
    short    flags      { 0 };   ///< Флаги.
    bool     deleted { false };  ///< Признак удаленной записи.
};

//=========================================================

/// \brief Обертка над ILF-файлом.
class IRBIS_API IlfFile final
{
public:
    const static std::string MagicString; ///< Магическая строка в заголовке файла.

    std::vector<IlfEntry> entries; ///< Вектор записей.
    uint32_t unknown1    { 0 };    ///< Неизвестно.
    uint32_t slotCount   { 0 };    ///< Количество слотов для хранения записей.
    uint32_t entryCount  { 0 };    ///< Количество записей.
    uint32_t writeCount  { 0 };    ///< Количество модификаций записей.
    uint32_t deleteCount { 0 };    ///< Количество удаленных записей.

    void readLocalFile (const String &fileName);
};

//=========================================================

/// \brief Специфичная для ИРБИС дата.
class IRBIS_API Date final
{
public:
    String text;          ///< Текстовое представление в формате YYYYMMDD.
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif
    struct tm date; ///< Разбиение даты на компоненты.
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

    Date() = default;                               ///< Конструктор перемещения.
    Date (int year, int month, int day);
    explicit Date (const String &text_);
    explicit Date (const struct tm *date_);
    Date (const Date &other) = default;              ///< Конструктор копирования.
    Date (Date &&other) = default;                   ///< Конструктор перемещения.
    Date& operator = (const Date &other) = default; ///< Оператор копирования.
    Date& operator = (Date &&other) = default;      ///< Оператор перемещения.
    ~Date() = default;                              ///< Деструктор.

    static String convert(const struct tm *date);
    static bool parse(const String &text, struct tm *date);
    static Date today();
};

//=========================================================

/// \brief Международный стандартный книжный номер.
class IRBIS_API Isbn final
{
public:
    static bool check978 (WideSpan isbn) noexcept;
    static bool checkControlDigit (WideSpan isbn) noexcept;
};

//=========================================================

/// \brief Статус записи.
enum class IRBIS_API RecordStatus : unsigned int
{
    None = 0,                                       ///< Исходное состояние.
    LogicallyDeleted = 1,                           ///< Запись логически удалена.
    PhysicallyDeleted = 2,                          ///< Запись физически удалена.
    Deleted = LogicallyDeleted | PhysicallyDeleted, ///< Запись удалена.
    Absent = 4,                                     ///< Запись отсутствует.
    NonActualized = 8,                              ///< Не актуализирована.
    Last = 32,                                      ///< Последняя версия записи.
    Locked = 64                                     ///< Запись заблокирована.
};

IRBIS_API RecordStatus operator | (RecordStatus left, RecordStatus right);
IRBIS_API RecordStatus operator & (RecordStatus left, RecordStatus right);

//=========================================================

/// \brief Библиографическая запись. Состоит из произвольного количества полей.
class IRBIS_API MarcRecord final
{
public:
    Mfn mfn { 0u };                             ///< MFN (порядковый номер в базе) записи.
    RecordStatus status { RecordStatus::None }; ///< Статус записи. Представляет собой набор флагов.
    unsigned int version { 0u };                ///< Номер версии записи.
    String database;                            ///< Имя базы данных.
    std::list<RecordField> fields;              ///< Список полей.

    MarcRecord  (std::initializer_list <RecordField> fields_);
    MarcRecord  ()                              = default; ///< Конструктор по умолчанию.
    MarcRecord  (const MarcRecord &)            = default; ///< Конструктор копирования.
    MarcRecord  (MarcRecord &&)                 = default; ///< Конструктор перемещения.
    ~MarcRecord ()                              = default; ///< Деструктор.
    MarcRecord& operator = (const MarcRecord &) = default; ///< Оператор копирования.
    MarcRecord& operator = (MarcRecord &&)      = default; ///< Оператор перемещения.

    RecordField&              add         (int tag, const String &value = L"");
    RecordField&              add         (int tag, String &&value);
    MarcRecord                clone       ()                                          const;
    void                      decode      (const StringList &lines);
    bool                      deleted     ()                                          const noexcept;
    String                    encode      (const String &delimiter = L"\u001F\u001E") const;
    String                    fm          (int tag, Char code = 0)                    const noexcept;
    StringList                fma         (int tag, Char code = 0)                    const;
    RecordField*              getField    (int tag, int occurrence = 0)               const noexcept;
    std::vector<RecordField*> getFields   (int tag)                                   const;
    MarcRecord&               removeField (int tag);
    MarcRecord&               setField    (int tag, const String &value);
    MarcRecord&               setField    (int tag, String &&value);
    MarcRecord&               reset       ()                                          noexcept;
    bool                      verify      (bool throwOnError)                         const;

    friend IRBIS_API std::wostream& operator << (std::wostream &stream, const MarcRecord &record);

    MarcRecord& operator << (const RecordField &field);
    MarcRecord& operator << (RecordField &&field);
    MarcRecord& operator << (const String &text);
    MarcRecord& operator << (const std::string &text);
    MarcRecord& operator << (int tag);
    MarcRecord& operator << (char code);
    MarcRecord& operator << (Char code);
    MarcRecord& operator && (const String &value);
    MarcRecord& operator && (const Char *value);
};

//=========================================================

/// \brief Пара строк в MNU-файле.
class IRBIS_API MenuEntry final
{
public:
    String code;    ///< Первая строка: содержит код.
    String comment; ///< Вторая строка: содержит комментарий.

    MenuEntry (const String &code_, const String &comment_) : code (code_), comment (comment_) {} ///< Конструктор.
    MenuEntry (String &&code_, String &&comment_) : code (std::move (code_)), comment (std::move (comment_)) {} ///< Конструктор.
    MenuEntry             ()                  = default; ///< Конструктор по умолчанию.
    MenuEntry             (const MenuEntry &) = default; ///< Конструктор копирования.
    MenuEntry             (MenuEntry &&)      = default; ///< Конструктор перемещения.
    ~MenuEntry            ()                  = default; ///< Деструктор.
    MenuEntry& operator = (const MenuEntry &) = default; ///< Оператор копирования.
    MenuEntry& operator = (MenuEntry &&)      = default; ///< Оператор перемещения.

    String toString() const;
};

//=========================================================

/// \brief Обертка над MNU-файлом.
class IRBIS_API MenuFile final
{
public:
    const static String StopMarker;
    const static Chars Separators;

    String fileName;               ///< Имя MNU-файла (опционально).
    std::list<MenuEntry> entries;  ///< Элементы меню.

    MenuFile             ()                 = default; ///< Конструктор по умолчанию.
    MenuFile             (const MenuFile &) = default; ///< Конструктор копирования.
    MenuFile             (MenuFile &&)      = default; ///< Конструктор пермещения.
    ~MenuFile            ()                 = default; ///< Деструктор.
    MenuFile& operator = (const MenuFile &) = default; ///< Оператор копирования.
    MenuFile& operator = (MenuFile &&)      = default; ///< Оператор перемещения.

    MenuFile& add                (const String &code, const String &comment);
    MenuFile& add                (String &&code, String &&comment);
    MenuEntry* getEntry          (const String &code) const;
    MenuEntry* getEntrySensitive (const String &code) const;
    String getValue              (const String &code) const;
    String getValueSensitive     (const String &code) const;
    String getValue              (const String &code, const String &defaultValue) const;
    String getValueSensitive     (const String &code, const String &defaultValue) const;
    void parse                   (const StringList &lines);
    void parseLocalFile          (const String &filename);
};

//=========================================================

class IRBIS_API NetworkException final
    : public IrbisException
{
public:
};

//=========================================================

class IRBIS_API NotImplementedException final
    : public IrbisException
{
public:
};

//=========================================================

/// \brief Текст, содержащий фрагменты с числами.
struct IRBIS_API NumberText final
{
    /// \brief Фрагмент текста.
    struct Chunk
    {
        String  prefix    {};        ///< Префикс (может быть пустым).
        int64_t value     { 0 };     ///< Число.
        int     length    { 0 };     ///< Длина (используется при дополнении нулями).
        bool    haveValue { false }; ///< Есть ли значение?

        int    compareTo   (const Chunk &other) const noexcept;
        bool   havePrefix  ()                   const noexcept;
        bool   setUp       (const String &str, const String &number);
        String toString    ()                   const;
    };

    std::vector<Chunk> chunks;

    Chunk&      append    ();
    NumberText& append    (const String &prefix);
    NumberText& append    (int64_t value_, int length = 0);
    NumberText& append    (const String &prefix_, int64_t value_, int length_ = 0);
    Chunk&      back      () const noexcept;
    int         compareTo (const NumberText &other) const;
    bool        empty     () const noexcept;
    Chunk&      front     () const noexcept;
    NumberText& increment (std::ptrdiff_t index = -1, int64_t delta = 1);
    std::size_t lastIndex () const noexcept;
    std::size_t size      () const noexcept;
    String      toString  () const;

    static NumberText parse (const String &text);
};

//=========================================================

/// \brief Обёртка над OPT-файлом.
class IRBIS_API OptFile final
{
public:
    int tag { 0 };              ///< Метка поля, используемая для хранения рабочего листа.
    int length { 0 };           ///< Длина ключа (шаблона) в символах.
    std::vector<OptLine> lines; ///< Строки с ключами и рабочими листами

    void parse (const StringList &text);
};

//=========================================================

/// \brief Строка OPT-файла.
class IRBIS_API OptLine final
{
public:
    String key;   ///< Ключ (шаблон).
    String value; ///< Имя соответствующего рабочего листа.
};

//=========================================================

/// \brief Обертка над PAR-файлом.
class IRBIS_API ParFile final
{
public:
    String xrf; ///< Путь к файлу XRF.
    String mst; ///< Путь к файлу MST.
    String cnt; ///< Путь к файлу CNT.
    String n01; ///< Путь к файлу N01.
    String n02; ///< Путь к файлу N02. В ИРБИС64 не используется.
    String l01; ///< Путь к файлу L01.
    String l02; ///< Путь к файлу L02. В ИРБИС64 не используется.
    String ifp; ///< Путь к файлу IFP.
    String any; ///< Путь к файлу ANY. В ИРБИС64 не используется.
    String pft; ///< Путь к файлам PFT.
    String ext; ///< Расположение внешних объектов (поле 951).

    void assign (const String &path);
    void parse (const StringList &lines);
    static ParFile readLocalFile (const String &path);
    std::map<int, String> toDictionary() const;
};

//=========================================================

/// \brief Невладеющее подполе.
class IRBIS_API PhantomSubField final
{
public:

    /// \brief Одноисмвольный код подполя.
    Byte code { '\0' };
    ByteSpan value; ///< Значение подполя (может быть пустым).

    PhantomSubField() = default;
    PhantomSubField (Byte code_, ByteSpan value_) : code(code_), value(value_) {}
    PhantomSubField (const PhantomSubField &other) = default;
    PhantomSubField (PhantomSubField &&other) = default;
    PhantomSubField& operator = (const PhantomSubField &other) = default;
    PhantomSubField& operator = (PhantomSubField &&other) = default;
    ~PhantomSubField() = default;

    PhantomSubField clone() const;
    void decode (ByteSpan line);
    bool empty() const noexcept;
    bool verify (bool throwOnError) const;
    std::string toString() const;

    SubField materialize() const;
};

//=========================================================

/// \brief Невладеющее поле.
class IRBIS_API PhantomField final
{
public:

    int tag { 0 };
    ByteSpan value;
    std::list<PhantomSubField> subfields;

    PhantomField() = default;
    PhantomField (int tag_, ByteSpan value_) : tag(tag_), value(value_) {}
    PhantomField (const PhantomField &other) = default;
    PhantomField (PhantomField &&other) = default;
    PhantomField& operator = (const PhantomField &other) = default;
    PhantomField& operator = (PhantomField &&other) = default;
    ~PhantomField() = default;

    PhantomField& add (Byte code, ByteSpan value);
    PhantomField& clear();
    PhantomField clone() const;
    void decode (ByteSpan line);
    bool empty() const noexcept;
    PhantomSubField* getFirstSubfield (Byte code) const noexcept;
    ByteSpan getFirstSubfieldValue (Byte code) const noexcept;
    PhantomField& removeSubfield (Byte code);
    PhantomField& setSubfield (Byte code, ByteSpan newValue);
    bool verify (bool throwOnError) const;
    std::string toString() const;

    RecordField materialize() const;
};

//=========================================================

/// \brief Невладеющая запись.
class IRBIS_API PhantomRecord final
{
public:
    Mfn mfn { 0u };                             ///< MFN (порядковый номер в базе) записи.
    RecordStatus status { RecordStatus::None }; ///< Статус записи. Представляет собой набор флагов.
    unsigned int version { 0u };                ///< Номер версии записи.
    std::list<PhantomField> fields;             ///< Список полей.
    std::string database;                       ///< База данных.

    PhantomRecord() = default;                                        ///< Конструктор по умолчанию.
    PhantomRecord (const PhantomRecord &other) = default;             ///< Конструктор копирования.
    PhantomRecord (PhantomRecord &&other) = default;                  ///< Конструктор перемещения.
    ~PhantomRecord() = default;                                       ///< Деструктор.
    PhantomRecord& operator = (const PhantomRecord &other) = default; ///< Оператор копирования.
    PhantomRecord& operator = (PhantomRecord &&other) = default;      ///< Оператор перемещения.

    PhantomField& add (int tag, ByteSpan value);
    PhantomRecord clone() const;
    void decode (const std::vector<ByteSpan> &lines);
    bool deleted() const noexcept;
    void encode (ByteSpan buffer, ByteSpan delimiter) const;
    std::string fm (int tag, Byte code = 0) const noexcept;
    std::vector<std::string> fma (int tag, char code = 0) const;
    PhantomField* getField (int tag, int occurrence = 0) const noexcept;
    std::vector<PhantomField*> getFields (int tag) const;
    PhantomRecord& reset() noexcept;
    bool verify (bool throwOnError) const;

    MarcRecord materialize() const;
};

//=========================================================

/// \brief Параметры выборки постингов.
class IRBIS_API PostingParameters final
{
public:
    StringList listOfTerms;            ///< Перечень термов.
    String     database;               ///< Имя базы данных.
    String     format;                 ///< Формат.
    String     term;                   ///< Терм (взаимоисключающе с `listOfTerms`).
    int        firstPosting     { 0 }; ///< Номер первого постинга.
    int        numberOfPostings { 0 }; ///< Требуемое число постингов.
};

//=========================================================

/// \brief Информация о процессе на ИРБИС-сервере.
class IRBIS_API ProcessInfo final
{
public:
    String number;        ///< Просто порядковый номер процесса.
    String ipAddress;     ///< С каким клиентом взаимодействует.
    String name;          ///< Логин оператора.
    String clientId;      ///< Идентификатор клиента.
    String workstation;   ///< Тип АРМ.
    String started;       ///< Время запуска.
    String lastCommand;   ///< Последняя выполненная (или выполняемая) команда.
    String commandNumber; ///< Порядковый номер последней команды.
    String processId;     ///< Идентификатор процесса.
    String state;         ///< Состояние.

    static std::vector<ProcessInfo> parse (ServerResponse &response);
};

//=========================================================

/// \brief Запись с нераскодированными полями.
class IRBIS_API RawRecord final
{
public:
    Mfn          mfn     { 0 }; ///< MFN записи.
    unsigned int status  { 0 }; ///< Статус записи.
    unsigned int version { 0 }; ///< Версия записи.
    StringList   fields;        ///< Поля записи.
    String       database;      ///< Имя базы данных.


    String encode      (const String &delimiter = L"\u001F\u001E") const;
    void   parseSingle (const StringList &lines);
    String toString    ()                                          const;

    friend IRBIS_API std::wostream& operator << (std::wostream &stream, const RawRecord &record);
};

//=========================================================

/// \brief Поле записи.
class IRBIS_API RecordField final
{
public:
    //const static int NoTag = 0;

    int tag { 0 };           ///< Метка поля.
    String value;            ///< Значение поля до первого разделителя.
    SubFieldList  subfields; ///< Подполя.

    explicit RecordField (int tag_, const String &value_ = L"")
        : tag { tag_ }, value { value_ } {} ///< Конструктор.
    RecordField (int tag_, String &&value_) noexcept
        : tag { tag_ }, value { std::move (value_) } {} ///< Конструктор.
    RecordField (int tag_, std::initializer_list <SubField> subfields_);
    RecordField  ()                                   = default; ///< Конструктор по умолчанию.
    RecordField  (const RecordField &)                = default; ///< Конструктор копирования.
    RecordField  (RecordField &&) noexcept            = default; ///< Конструктор перемещения.
    ~RecordField ()                                   = default; ///< Деструктор.
    RecordField& operator = (const RecordField &)     = default; ///< Оператор копирования.
    RecordField& operator = (RecordField &&) noexcept = default; ///< Оператор перемещения.

                       RecordField& _                     (const char *value);
                       RecordField& _                     (const wchar_t *value);
    IRBIS_MAYBE_UNUSED RecordField& add                   (Char code, const String &value = L"");
    IRBIS_MAYBE_UNUSED RecordField& add                   (Char code, String &&value);
    IRBIS_MAYBE_UNUSED RecordField& clear                 ();
                       RecordField  clone                 ()                                   const;
                       void         decode                (const String &line);
                       void         decodeBody            (const String &line);
                       bool         empty                 ()                                   const noexcept;
                       SubField*    getFirstSubfield      (Char code)                          const noexcept;
                       String       getFirstSubfieldValue (Char code)                          const noexcept;
                       RecordField& removeSubfield        (Char code);
                       RecordField& setSubfield           (Char code, const String &newValue);
                       RecordField& setSubfield           (Char code, String &&newValue);
                       bool         verify                (bool throwOnError)                  const;
                       String       toString()                                                 const;

    friend IRBIS_API std::wostream& operator << (std::wostream &stream, const RecordField &field);

    RecordField& operator << (const SubField &subfield);
    RecordField& operator << (SubField &&subfield);
    RecordField& operator << (const String &body);
    RecordField& operator << (String &&body);
    RecordField& operator << (const Char *body);
    RecordField& operator << (const std::string &body);
    RecordField& operator << (const char *text);
    RecordField& operator << (char code);
    RecordField& operator << (Char code);
};

RecordField operator "" _field (unsigned long long int tag);
RecordField operator "" _field (const char *text, std::size_t size);
RecordField operator "" _field (const wchar_t *text, std::size_t size);

//=========================================================

class IRBIS_API RecordSerializer
{
    std::wiostream &stream;

public:
    explicit RecordSerializer(std::wiostream &stream);

    MarcRecord deserialize();
    void serialize (const MarcRecord &record);
};

//=========================================================

/// \brief Построитель запросов.
class IRBIS_API Search final
{
public:
    static Search all();
    Search& and_ (const String &text);
    Search& and_ (const String &text1, const String &text2);
    Search& and_ (const String &text1, const String &text2, const String &text3);
    Search& and_ (const Search &item);
    Search& and_ (const Search &item1, const Search &item2);
    Search& and_ (const Search &item1, const Search &item2, const Search &item3);
    static Search equals (const String &prefix, const String &text);
    static Search equals (const String &prefix, const String &text1, const String &text2);
    static Search equals (const String &prefix, const String &text1, const String &text2, const String &text3);
    static bool needWrap (const String &text) noexcept;
    Search& not_ (const String &text);
    Search& not_ (const Search &item);
    Search& or_ (const String &text);
    Search& or_ (const String &text1, const String &text2);
    Search& or_ (const String &text1, const String &text2, const String &text3);
    Search& or_ (const Search &item);
    Search& or_ (const Search &item1, const Search &item2);
    Search& or_ (const Search &item1, const Search &item2, const Search &item3);
    Search& sameField (const String &text);
    Search& sameRepeat (const String &text);
    String toString() const noexcept;
    static String wrap (const String &text);
    static String wrap (const Search &item);

private:
    String _buffer;
};

Search keyword      (const String &value1);
Search author       (const String &value1);
Search title        (const String &value1);
Search publisher    (const String &value1);
Search place        (const String &value1);
Search subject      (const String &value1);
Search language     (const String &value1);
Search year         (const String &value1);
Search magazine     (const String &value1);
Search documentKind (const String &value1);
Search udc          (const String &value1);
Search bbk          (const String &value1);
Search rzn          (const String &value1);
Search mhr          (const String &value1);

//=========================================================

/// \brief Параметры для поиска записей.
class IRBIS_API SearchParameters final
{
public:
    String searchExpression;        ///< Выражение для поиска по словарю.
    String database;                ///< Имя базы данных.
    Mfn firstRecord { 1 };          ///< Индекс первой требуемой записи.
    String formatSpecification;     ///< Формат для расформатирования записей.
    Mfn maxMfn { 0 };               ///< Максимальный MFN.
    Mfn minMfn { 0 };               ///< Минимальный MFN.
    Mfn numberOfRecords { 0 };      ///< Общее число требуемых записей.
    String sequentialSpecification; ///< Выражение для последовательного поиска.
    String filterSpecification;     ///< Выражение для локальной фильтрации.
};

//=========================================================

/// \brief Сценарий поиска.
class IRBIS_API SearchScenario final
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

/// \brief Статистика работы ИРБИС-сервера.
class IRBIS_API ServerStat final
{
public:
    std::vector<ClientInfo> runningClients; ///< Подключенные в данный момент клиенты.
    int clientCount { 0 };                  ///< Число клиентов, подключенных в данный момент.
    int totalCommandCount { 0 };            ///< Общее количество команд, выполненных сервером с момента запуска.

    void parse (ServerResponse &response);
};

//=========================================================

/// \brief Подполе записи. Состоит из кода и значения.
class IRBIS_API SubField final
{
public:
    /// \brief Отсутствующий код подполя.
    const static Char NoCode { L'\0' };

    Char code { L'\0' }; ///< Односимвольный код подполя.
    String value;        ///< Значение подполя (может быть пустой строкой).

    explicit SubField (Char code_, const String &value_ = L"") : code (code_), value (value_) {} ///< Конструктор.
             SubField (Char code_, String &&value_) noexcept : code (code_), value (std::move (value_)) {} ///< Конструктор.
             SubField ()                                 = default; ///< Конструктор по умолчанию.
             SubField (const SubField &)                 = default; ///< Конструктор копирования.
             SubField  (SubField &&) noexcept            = default; ///< Конструктор перемещения.
             SubField& operator = (const SubField &)     = default; ///< Оператор копирования
             SubField& operator = (SubField &&) noexcept = default; ///< Оператор перемещения.
            ~SubField             ()                     = default; ///< Деструктор.

    SubField  clone    ()                    const;
    void      decode   (const String &line);
    bool      empty    ()                    const noexcept;
    String    toString ()                    const;
    bool      verify   (bool throwOnError)   const;

    friend IRBIS_API std::wostream& operator << (std::wostream &stream, const SubField &subfield);

    SubField& operator << (const String &value_);
    SubField& operator << (String &&value_);
    SubField& operator << (const Char *value_);
    SubField& operator << (const std::string &value_);
    SubField& operator << (const char *value_);
};

SubField operator "" _sub (char code);
SubField operator "" _sub (const char *text, std::size_t size);
SubField operator "" _sub (const wchar_t *text, std::size_t size);

//=========================================================

/// \brief Данные для метода printTable.
class IRBIS_API TableDefinition final
{
public:
    String     database;        ///< Имя базы данных.
    String     table;           ///< Имя таблицы.
    StringList headers;         ///< Заголовки таблицы.
    String     mode;            ///< Режим таблицы.
    String     searchQuery;     ///< Поисковый запрос.
    int        minMfn { 0 };    ///< Минимальный MFN.
    int        maxMfn { 0 };    ///< Максимальный MFN.
    String     sequentialQuery; ///< Запрос для последовательного поиска.
    MfnList    mfnList;         ///< Список MFN, по которым строится таблица.
};

//=========================================================

/// \brief Информация о термине поискового словаря.
class IRBIS_API TermInfo final
{
public:
    int count   { 0 }; ///< Количество ссылок на данный термин.
    String text {};    ///< Значение поискового термина.

    TermInfo             () = default;                              ///< Конструктор по умолчанию.
    TermInfo             (int count_, const String &text_) : count (count_), text (text_) {} ///< Конструктор.
    TermInfo             (int count_, String &&text_) noexcept : count (count_), text (std::move (text_)) {} ///< Конструктор.
    TermInfo             (const TermInfo &)              = default; ///< Конструктор копирования.
    TermInfo             (TermInfo &&) noexcept          = default; ///< Конструктор перемещения.
    TermInfo& operator = (const TermInfo &other)         = default; ///< Оператор копирования.
    TermInfo& operator = (TermInfo &&other) noexcept     = default; ///< Оператор перемещения.
    ~TermInfo            ()                              = default; ///< Деструктор.

    static std::vector<TermInfo> parse (const StringList &lines);
    String toString() const;
};

//=========================================================

/// \brief Параметры извлечения терминов из поискового словаря.
class IRBIS_API TermParameters final
{
public:
    String database;              ///< Имя базы данных.
    Mfn numberOfTerms { 0 };      ///< Количество извлекаемых терминов.
    bool reverseOrder { false };  ///< Выдавать термины в обратном порядке?
    String startTerm;             ///< Стартовый термин.
    String format;                ///< Спецификация формата (опционально).
};

//=========================================================

/// \brief Постинг термина.
class IRBIS_API TermPosting final
{
public:
    Mfn    mfn        { 0 };
    Mfn    tag        { 0 };
    Mfn    occurrence { 0 };
    Mfn    count      { 0 };
    String text       {   };

    static std::vector<TermPosting> parse (const StringList &lines);
    String toString() const;
};

//=========================================================

/// \brief Обертка над TRE-файлом.
class IRBIS_API TreeFile final
{
public:
    std::vector<TreeNode> roots; ///< Корневые элементы.

    void parse (const StringList &lines);
};

//=========================================================

/// \brief Узел дерева TRE-файла.
class IRBIS_API TreeNode final
{
public:
    String value; ///< Значение, хранящееся в узле.
    std::vector<TreeNode> children; ///< Дочерние узлы.
    int level { 0 }; ///< Уровень вложенности узла (отслеживается автоматически).

    explicit TreeNode    (const String &value_) : value (value_) {} ///< Конструктор.
    explicit TreeNode    (String &&value_) noexcept : value (std::move (value_)) {} ///< Конструктор.
             TreeNode             ()                     = default; ///< Конструктор по умолчанию.
             TreeNode             (const TreeNode &)     = default; ///< Конструктор копирования.
             TreeNode             (TreeNode &&) noexcept = default; ///< Конструктор перемещения.
             TreeNode& operator = (const TreeNode &)     = default; ///< Оператор копирования.
             TreeNode& operator = (TreeNode &&) noexcept = default; ///< Оператор перемещения.
            ~TreeNode             ()                     = default; ///< Деструктор.

    IRBIS_MAYBE_UNUSED TreeNode& add (const String &name);
    IRBIS_MAYBE_UNUSED TreeNode& add (String &&name);
};

//=========================================================

/// \brief Информация о зарегистрированном пользователе системы (по данным client_m.mnu).
class IRBIS_API UserInfo final
{
public:
    String number;        ///< Номер по порядку в списке.
    String name;          ///< Логин.
    String password;      ///< Пароль.
    String cataloger;     ///< Доступность АРМ Каталогизатор.
    String reader;        ///< Доступность АРМ Читатель.
    String circulation;   ///< Доступность АРМ Книговыдача.
    String acquisitions;  ///< Доступность АРМ Комплектатор.
    String provision;     ///< Доступность АРМ Книгообеспеченность.
    String administrator; ///< Доступность АРМ Администратор.

    UserInfo             ()                     = default; ///< Конструктор по умолчанию.
    UserInfo             (const UserInfo &)     = default; ///< Конструктор копирования.
    UserInfo             (UserInfo &&) noexcept = default; ///< Конструктор перемещения.
    ~UserInfo            ()                     = default; ///< Деструктор.
    UserInfo& operator = (const UserInfo &)     = default; ///< Оператор копирования.
    UserInfo& operator = (UserInfo &&) noexcept = default; ///< Оператор перемещения.

    String toString() const;
    static std::vector<UserInfo> parse(const StringList &lines);
};

//=========================================================

/// \brief UTF-версия RecordField.
class IRBIS_API LiteField final
{
public:
    int                     tag { 0 }; ///< Метка поля.
    std::string             value;     ///< Значение до первого разделителя.
    std::list<LiteSubField> subfields; ///< Подполя.

             LiteField             ()                                = default; ///< Конструктор по умолчанию.
    explicit LiteField             (int tag_, const std::string &value_ = "") : tag (tag_), value (value_) {}
    explicit LiteField             (int tag_, std::string &&value_ = "") noexcept : tag (tag_), value (std::move (value_)) {}
             LiteField             (const LiteField &other)          = default; ///< Конструктор копирования.
             LiteField             (LiteField &&other) noexcept      = default; ///< Конструктор перемещения.
             LiteField& operator = (const LiteField &other)          = default; ///< Оператор копирования.
             LiteField& operator = (LiteField &&other)      noexcept = default; ///< Оператор перемещения.
            ~LiteField             ()                                = default; ///< Деструктор.

    IRBIS_MAYBE_UNUSED LiteField&    add (char code, const std::string &value = "");
    IRBIS_MAYBE_UNUSED LiteField&    clear()                                 noexcept;
                       LiteField     clone()                           const;
                       void          decode (const std::string &line);
                       bool          empty()                           const noexcept;
                       LiteSubField* getFirstSubfield (char code)      const noexcept;
                       std::string   getFirstSubfieldValue (char code) const noexcept;
                       RecordField   materialize()                     const;
    IRBIS_MAYBE_UNUSED LiteField&    removeSubfield (char code);
    IRBIS_MAYBE_UNUSED LiteField&    setSubfield (char code, const std::string &newValue);
                       std::string   toString()                        const;
                       bool          verify (bool throwOnError)        const;
};

//=========================================================

/// \brief UTF-версия MarcRecord.
class IRBIS_API LiteRecord final
{
public:
    Mfn                  mfn     { 0u };                 ///< MFN (порядковый номер в базе) записи.
    RecordStatus         status  { RecordStatus::None }; ///< Статус записи. Представляет собой набор флагов.
    unsigned int         version { 0u };                 ///< Номер версии записи.
    std::list<LiteField> fields;                         ///< Список полей.
    std::string          database;                       ///< База данных.

    LiteRecord             ()                                 = default; ///< Конструктор по умолчанию.
    LiteRecord             (const LiteRecord &other)          = default; ///< Конструктор копирования.
    LiteRecord             (LiteRecord &&other)      noexcept = default; ///< Конструктор перемещения.
    LiteRecord& operator = (const LiteRecord &other)          = default; ///< Оператор копирования.
    LiteRecord& operator = (LiteRecord &&other)      noexcept = default; ///< Оператор перемещения.
    ~LiteRecord            ()                                 = default; ///< Деструктор.

    IRBIS_MAYBE_UNUSED LiteField&               add         (int tag, const std::string &value);
                       LiteRecord               clone       ()                                          const;
                       void                     decode      (const std::vector<std::string> &lines);
                       bool                     deleted     ()                                          const noexcept;
                       std::string              encode      (const std::string &delimiter = "\x1F\x1E") const;
                       std::string              fm          (int tag, char code = 0)                    const noexcept;
                       std::vector<std::string> fma         (int tag, char code = 0)                    const;
                       LiteField*               getField    (int tag, int occurrence = 0)               const noexcept;
                       std::vector<LiteField*>  getFields   (int tag)                                   const;
                       MarcRecord               materialize ()                                          const;
    IRBIS_MAYBE_UNUSED LiteRecord&              reset       ()                                                noexcept;
    IRBIS_MAYBE_UNUSED bool                     verify      (bool throwOnError)                         const;

};

//=========================================================

/// \brief UTF-версия SubField.
class IRBIS_API LiteSubField final
{
public:
    /// \brief Одноисмвольный код подполя.
    char code { '\0' };
    std::string value; ///< Значение подполя (может быть пустым).

    LiteSubField() = default;
    explicit LiteSubField (char code_, const std::string &value_ = "") : code(code_), value(value_) {}
    LiteSubField (const LiteSubField &other) = default;
    LiteSubField (LiteSubField &&other) = default;
    LiteSubField& operator = (const LiteSubField &other) = default;
    LiteSubField& operator = (LiteSubField &&other) = default;
    ~LiteSubField() = default;

    LiteSubField clone() const;
    void decode (const std::string &line);
    bool empty() const noexcept;
    bool verify (bool throwOnError) const;
    std::string toString() const;

    SubField materialize() const;
};

//=========================================================

class IRBIS_API VerificationException final
    : public IrbisException
{
public:
};

//=========================================================

/// \brief Американский штрих-код для товаров.
class IRBIS_API Upc12 final
{
public:
    static char computeCheckDigit (CharSpan text);
    static Char computeCheckDigit (WideSpan text);
    static bool checkControlDigit (CharSpan text);
    static bool checkControlDigit (WideSpan text);
};

//=========================================================

/// \brief Информация о версии ИРБИС-сервера.
class IRBIS_API Version final
{
public:
    String organization;           ///< На какое юридическое лицо приобретен сервер.
    String version;                ///< Собственно версия сервера. Например, 64.2008.1.
    int    maxClients       { 0 }; ///< Максимальное количество одновременных подключений.
    int    connectedClients { 0 }; ///< Текущее количество подключений.

    Version             ()                = default; ///< Конструктор по умолчанию.
    Version             (const Version &) = default; ///< Конструктор копирования.
    Version             (Version &&)      = default; ///< Конструктор перемещения.
    ~Version            ()                = default; ///< Деструктор.
    Version& operator = (const Version &) = default; ///< Оператор копирования.
    Version& operator = (Version &&)      = default; ///< Оператор перемещения.

    void parse (ServerResponse &response);
    String toString() const;
};

//=========================================================

// Utilities

IRBIS_API uint32_t    IRBIS_CALL libraryVersion() noexcept;
IRBIS_API std::string IRBIS_CALL libraryVersionString();

IRBIS_API std::string IRBIS_CALL wide2string (const String &s);
IRBIS_API String      IRBIS_CALL string2wide (const std::string &s);
IRBIS_API std::string IRBIS_CALL toUtf       (const std::string &s);
IRBIS_API std::string IRBIS_CALL toAnsi      (const std::string &s);

IRBIS_API bool        IRBIS_CALL isWindows() noexcept;

IRBIS_API String      IRBIS_CALL describeError (int errorCode);

}

//=========================================================

#if defined(_MSC_VER)

    #pragma warning(pop)

#endif

// IRBIS_H
#endif
