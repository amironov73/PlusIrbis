// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#ifdef IRBIS_WINDOWS

#include <winsock2.h>
#include <windows.h>

#else

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>

#endif

#pragma ide diagnostic ignored "hicpp-signed-bitwise"

namespace irbis {

#ifdef IRBIS_WINDOWS

static void DisplayError ()
{
    LPWSTR lpMsgBuf;
    DWORD dw = GetLastError();

    FormatMessageW
        (
            FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dw,
            MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR) &lpMsgBuf,
            0,
            nullptr
        );

    wprintf (L"ERROR: %ls\n", lpMsgBuf);
    LocalFree (lpMsgBuf);
}

#endif

void File::_grabHandle (File &other) noexcept
{
    this->_handle = other._handle;
#ifdef IRBIS_WINDOWS
    other._handle = INVALID_HANDLE_VALUE;
#else
    other._handle = -1;
#endif
}

/// \brief Конструктор перемещения.
/// \param other Другой файл.
File::File (File &&other) noexcept
{
    this->_grabHandle (other);
}

/// \brief Оператор перемещения.
/// \param other Другой файл.
/// \return Ссылка на себя.
File& File::operator = (File && other) noexcept
{
    this->_grabHandle (other);
    return *this;
}

/// \brief Деструктор.
File::~File() noexcept
{
    this->close();
}

/// \brief Закрывает файл.
void File::close() noexcept
{
#ifdef IRBIS_WINDOWS

    if (!::CloseHandle (this->_handle)) {
        DisplayError();
    }
    this->_handle = INVALID_HANDLE_VALUE;

#else

    if (this->_handle >= 0) {
        ::close (this->_handle);
    }
    this->_handle = -1;

#endif
}

/// \brief Создание файла с указанным именем.
/// \param fileName Имя файла.
/// \return Файл.
File File::create (const String &fileName)
{
#ifdef IRBIS_WINDOWS

   File result;
   DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;
   DWORD shareMode = 0;
   LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
   DWORD creationDisposition = CREATE_ALWAYS;
   DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
   HANDLE templateFile = nullptr;

   result._handle = ::CreateFileW (fileName.c_str(), desiredAccess, shareMode,
           securityAttributes, creationDisposition, flagsAndAttributes,
           templateFile);
   if (result._handle == INVALID_HANDLE_VALUE) {
       DisplayError();
       throw IrbisException();
   }

   return result;

#else

    File result;
    const int flags = O_RDWR | O_CREAT | O_TRUNC;
    const ::mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    const auto utfName = toUtf (fileName);

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)
    result._handle = open (utfName.c_str(), flags, mode);
#else
    result._handle = open64 (utfName.c_str(), flags, mode);
#endif

    if (result._handle < 0) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Создание файла с указанным именем.
/// \param fileName Имя файла.
/// \return Файл.
File File::create (const std::string &fileName)
{
#ifdef IRBIS_WINDOWS

   File result;
   DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;
   DWORD shareMode = 0;
   LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
   DWORD creationDisposition = CREATE_ALWAYS;
   DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
   HANDLE templateFile = nullptr;

   result._handle = ::CreateFileA (fileName.c_str(), desiredAccess, shareMode,
           securityAttributes, creationDisposition, flagsAndAttributes,
           templateFile);
   if (result._handle == INVALID_HANDLE_VALUE) {
       DisplayError();
       throw IrbisException();
   }

   return result;

#else

    File result;
    const int flags = O_RDWR | O_CREAT | O_TRUNC;
    const ::mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)
    result._handle = open (fileName.c_str(), flags, mode);
#else
    result._handle = open64 (fileName.c_str(), flags, mode);
#endif

    if (result._handle < 0) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Достигнут конец файла?
/// \return true если достигнут.
bool File::eof()
{
    return this->tell() >= this->size();
}


/// \brief Настойчивое создание файла.
/// \param fileName Имя файла.
/// \param delay Задержка между попытками.
/// \param retryLimit Количество попыток.
/// \return Файл.
File File::insistCreate (const String &fileName, int delay, int retryLimit)
{
    return Retry (delay, retryLimit).execute ([&] { return File::create (fileName); });
}

/// \brief Настойчивое создание файла.
/// \param fileName Имя файла.
/// \param delay Задержка между попытками.
/// \param retryLimit Количество попыток.
/// \return Файл.
File File::insistCreate (const std::string &fileName, int delay, int retryLimit)
{
    return Retry (delay, retryLimit).execute ([&] { return File::create (fileName); });
}

/// \brief Настойчивое открытие файла.
/// \param fileName Имя файла.
/// \param delay Задержка между попытками.
/// \param retryLimit Количество попыток.
/// \return Файл.
File File::insistOpenRead (const String &fileName, int delay, int retryLimit)
{
    return Retry (delay, retryLimit).execute ([&] { return File::openRead (fileName); });
}

/// \brief Настойчивое открытие файла.
/// \param fileName Имя файла.
/// \param delay Задержка между попытками.
/// \param retryLimit Количество попыток.
/// \return Файл.
File File::insistOpenRead (const std::string &fileName, int delay, int retryLimit)
{
    return Retry (delay, retryLimit).execute ([&] { return File::openRead (fileName); });
}

/// \brief Настойчивое открытие файла.
/// \param fileName Имя файла.
/// \param delay Задержка между попытками.
/// \param retryLimit Количество попыток.
/// \return Файл.
File File::insistOpenWrite (const String &fileName, int delay, int retryLimit)
{
    return Retry (delay, retryLimit).execute ([&] { return File::openWrite (fileName); });
}

/// \brief Настойчивое открытие файла.
/// \param fileName Имя файла.
/// \param delay Задержка между попытками.
/// \param retryLimit Количество попыток.
/// \return Файл.
File File::insistOpenWrite (const std::string &fileName, int delay, int retryLimit)
{
    return Retry (delay, retryLimit).execute ([&] { return File::openWrite (fileName); });
}

/// \brief Открытие файла для чтения.
/// \param fileName Имя файла.
/// \return Файл.
File File::openRead (const String &fileName)
{
#ifdef IRBIS_WINDOWS

    File result;
    DWORD desiredAccess = GENERIC_READ;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = nullptr;

    result._handle = ::CreateFileW (fileName.c_str(), desiredAccess, shareMode,
                                    securityAttributes, creationDisposition, flagsAndAttributes,
                                    templateFile);
    if (result._handle == INVALID_HANDLE_VALUE) {
        DisplayError();
        throw IrbisException();
    }

    return result;

#else

    File result;
    const int flags = O_RDONLY;
    const auto utfName = toUtf (fileName);

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)
    result._handle = open (utfName.c_str(), flags);
#else
    result._handle = open64 (utfName.c_str(), flags);
#endif

    if (result._handle < 0) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Открытие файла для чтения.
/// \param fileName Имя файла.
/// \return Файл.
File File::openRead (const std::string &fileName)
{
#ifdef IRBIS_WINDOWS

    File result;
    DWORD desiredAccess = GENERIC_READ;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = nullptr;

    result._handle = ::CreateFileA (fileName.c_str(), desiredAccess, shareMode,
                                    securityAttributes, creationDisposition, flagsAndAttributes,
                                    templateFile);
    if (result._handle == INVALID_HANDLE_VALUE) {
        DisplayError();
        throw IrbisException();
    }

    return result;

#else

    File result;
    const int flags = O_RDONLY;

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)
    result._handle = open (fileName.c_str(), flags);
#else
    result._handle = open64 (fileName.c_str(), flags);
#endif

    if (result._handle < 0) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Открытие файла для чтения.
/// \param fileName Имя файла.
/// \return Файл.
File File::openWrite (const String &fileName)
{
#ifdef IRBIS_WINDOWS

    File result;
    DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = nullptr;

    result._handle = ::CreateFileW (fileName.c_str(), desiredAccess, shareMode,
                                    securityAttributes, creationDisposition, flagsAndAttributes,
                                    templateFile);
    if (result._handle == INVALID_HANDLE_VALUE) {
        DisplayError();
        throw IrbisException();
    }

    return result;

#else

    File result;
    const int flags = O_RDWR;
    const auto utfName = toUtf (fileName);

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)
    result._handle = open (utfName.c_str(), flags);
#else
    result._handle = open64 (utfName.c_str(), flags);
#endif

    if (result._handle < 0) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Открытие файла для чтения.
/// \param fileName Имя файла.
/// \return Файл.
File File::openWrite (const std::string &fileName)
{
#ifdef IRBIS_WINDOWS

    File result;
    DWORD desiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = nullptr;

    result._handle = ::CreateFileA (fileName.c_str(), desiredAccess, shareMode,
                                    securityAttributes, creationDisposition, flagsAndAttributes,
                                    templateFile);
    if (result._handle == INVALID_HANDLE_VALUE) {
        DisplayError();
        throw IrbisException();
    }

    return result;

#else

    File result;
    const int flags = O_RDWR;

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)
    result._handle = open (fileName.c_str(), flags);
#else
    result._handle = open64 (fileName.c_str(), flags);
#endif

    if (result._handle < 0) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Чтение из файла.
/// \param buffer Буфер.
/// \param nbytes Количество считываемых байт.
/// \return Количество прочитанных байт.
int64_t File::read (Byte *buffer, int64_t nbytes)
{
#ifdef IRBIS_WINDOWS

    auto numberOfBytesToRead = static_cast <DWORD> (nbytes);
    DWORD numberOfBytesRead;
    if (!::ReadFile (this->_handle, buffer, numberOfBytesToRead, &numberOfBytesRead, nullptr )) {
        DisplayError();
        throw IrbisException();
    }

    return static_cast<int64_t> (numberOfBytesRead);

#else

    return ::read (this->_handle, buffer, nbytes);

#endif
}

/// \brief Чтение от текущей позиции до конца файла.
/// \return Строка, возможно, пустая.
std::string File::readAll ()
{
    std::string result;
    const auto remaining = this->size() - this->tell();
    if (remaining) {
        result.resize (remaining);
        Byte *data = reinterpret_cast<Byte*> (const_cast<char*> (result.data()));
        this->read (data, remaining);
    }
    return result;
}

/// \brief Считывание всего файла как вектора байт без учета кодировки.
/// \param fileName Имя файла.
/// \return Считанные байты в виде строки.
std::string File::readAll (const String &fileName)
{
    std::string result;
    File file = openRead (fileName);
    const auto fileSize = file.size();
    result.resize (fileSize);
    auto buffer = (Byte*) result.data();
    if (file.read (buffer, fileSize) != fileSize) {
        // TODO информативное сообщение
        throw IrbisException();
    }
    return result;
}

/// \brief Чтение одного байта из файла.
/// \return Прочитанный байт либо -1.
int File::readByte  ()
{
    Byte result;

    if (this->read (&result, 1)) {
        return result;
    }
    return -1;
}

/// \brief Чтение одного символа из файла.
/// \return Прочитанный байт либо -1.
int File::readChar  ()
{
    Byte result[2];

    if (this->read (result, 2)) {
        return (result[0] << 8u) + result[1];
    }
    return -1;
}

/// \brief Чтение беззнакового 32-битного целого в сетевом формате.
/// \return
uint32_t File::readInt32 ()
{
    uint32_t result;
    Byte *buffer = reinterpret_cast <Byte*> (&result);
    this->read (buffer, sizeof (result));
    result = ntohl (result);
    return result;
}

/// \brief Чтение беззнакового 64-битного целого в сетевом формате.
/// \return
uint64_t File::readInt64 ()
{
    uint32_t numbers [2];
    Byte *buffer = reinterpret_cast <Byte*> (&numbers);
    this->read (buffer, sizeof (numbers));
    numbers [0] = ntohl (numbers [0]);
    numbers [1] = ntohl (numbers [1]);
    uint64_t result = (((uint64_t) numbers [1]) << 32u) + ((uint64_t) numbers [0]);
    return result;
}

/// \brief Чтение строки из файла в нативной однобайтовой кодировке.
/// \return Строка (возможно, пустая).
std::string File::readNarrow ()
{
    std::string result;
    while (true) {
        int c = this->readByte();
        if (c < 0) {
            break;
        }
        else if (c == '\r') {
            c = this->readByte();
            if (c == '\n' || c < 0) {
                break;
            }
            result.push_back (static_cast <char> (c));
        }
        else if (c == '\n') {
            break;
        }
        else {
            result.push_back (static_cast <char> (c));
        }
    }
    return result;
}

/// \brief Получение текущего положения указателя файла.
/// \return Смещение от начала в байтах.
std::size_t File::tell ()
{
#ifdef IRBIS_WINDOWS

    ::LARGE_INTEGER largeInteger;
    largeInteger.QuadPart = 0;
    ::LARGE_INTEGER result;
    if (!::SetFilePointerEx (this->_handle, largeInteger, &result, FILE_CURRENT)) {
        DisplayError();
        throw IrbisException();
    }
    return static_cast<std::size_t> (result.QuadPart);

#else

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD)
    return static_cast<std::size_t> (::lseek (this->_handle, 0, SEEK_CUR));
#else
    return static_cast<std::size_t> (::lseek64 (this->_handle, 0, SEEK_CUR));
#endif

#endif
}

/// \brief Определяет размер файла.
/// \return Размер файла.
std::size_t File::size ()
{
#ifdef IRBIS_WINDOWS

    LARGE_INTEGER result;
    if (!::GetFileSizeEx (this->_handle, &result)) {
        DisplayError();
        throw IrbisException();
    }
    return static_cast <std::size_t> (result.QuadPart);

#else

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD) || defined (IRBIS_ANDROID)

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

    struct stat buf { 0 };
    if (::fstat (this->_handle, &buf) < 0) {
        throw IrbisException();
    }
    return static_cast<std::size_t> (buf.st_size);

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#else

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

    struct stat64 buf { 0 };
    if (::fstat64 (this->_handle, &buf) < 0) {
        throw IrbisException();
    }
    return static_cast<std::size_t> (buf.st_size);

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif

//#ifdef IRBIS_APPLE
//
//    off_t previous = ::lseek (this->_handle, 0, SEEK_CUR);
//    off_t result = ::lseek (this->_handle, 0, SEEK_END);
//    ::lseek (this->_handle, previous, SEEK_SET);
//    return result;
//
//#else
//
//    off64_t previous = ::lseek64 (this->_handle, 0, SEEK_CUR);
//    off64_t result = ::lseek64 (this->_handle, 0, SEEK_END);
//    ::lseek64 (this->_handle, previous, SEEK_SET);
//    return result;
//
//#endif

#endif
}

/// \brief Перемещение указателя файла.
/// \param offset Смещение от начала в байтах.
void File::seek (int64_t offset)
{
#ifdef IRBIS_WINDOWS

    ::LARGE_INTEGER largeInteger;
    largeInteger.QuadPart = offset;
    if (!::SetFilePointerEx (this->_handle, largeInteger, nullptr, FILE_BEGIN)) {
        DisplayError();
        throw IrbisException();
    }

#else

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD)
    ::lseek (this->_handle, offset, SEEK_SET);
#else
    ::lseek64 (this->_handle, offset, SEEK_SET);
#endif

#endif
}

/// \brief Перемещение в кучу.
/// \return
File* File::toHeap ()
{
    Byte *buffer = new Byte [sizeof (File)];
    std::uninitialized_fill_n (buffer, sizeof(File), 0);
    File *result = (File*) buffer;
    result->_grabHandle (*this);
    return result;
}


/// \brief Запись в файл.
/// \param buffer Буфер.
/// \param nbytes Количество записываемых байт.
int64_t File::write (const Byte* buffer, int64_t nbytes)
{
#ifdef IRBIS_WINDOWS

    auto numberOfBytesToWrite = static_cast <DWORD> (nbytes);
    DWORD numberOfBytesWritten;
    if (!::WriteFile (this->_handle, buffer, numberOfBytesToWrite, &numberOfBytesWritten, nullptr)) {
        DisplayError();
        throw IrbisException();
    }

    if (numberOfBytesWritten != nbytes) {
        DisplayError();
        throw IrbisException();
    }

    return numberOfBytesWritten;

#else

    int64_t result = ::write (this->_handle, buffer, nbytes);
    if (result != nbytes) {
        throw IrbisException();
    }
    return result;

#endif
}

/// \brief Запись беззнакового 32-битного целого в сетевом формате.
/// \param number Записываемое значение.
void File::writeInt32 (uint32_t number)
{
    number = htonl (number);
    Byte *buffer = (Byte*)(&number);
    this->write (buffer, sizeof (number));
}

/// \brief Запись беззнакового 64-битного целого в сетевом формате.
/// \param number Записываемое значение.
void File::writeInt64 (uint64_t number)
{
    uint32_t parts [2];
    parts[0] = htonl ((uint32_t)(number & 0xFFFFFFFFul));
    parts[1] = htonl ((uint32_t)((number >> 32u) & 0xFFFFFFFFul));
    Byte *buffer = (Byte*)(&parts);
    this->write (buffer, sizeof (parts));
}

}
