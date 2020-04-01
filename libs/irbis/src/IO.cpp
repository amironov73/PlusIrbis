// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef  IRBIS_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <io.h>

#pragma comment (lib, "ws2_32.lib") // for ntohl/htonl

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>

#endif

/*!
    \class irbis::IO

    \details У ИРБИС64 довольно оригинальный формат записи целых чисел на диске.

    32-битные целые записываются в сетевом формате (Big Endian).
    64-битные записываются как пара 32-битных, причем сначала идет младшее,
    затем старшее.

 */

namespace irbis {

/// \brief Чтение беззнакового 32-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Куда поместить результат.
/// \return Признак успешного выполнения операции.
bool IO::readInt32 (FILE* file, uint32_t *value)
{
    uint32_t buffer;
    if (::fread ((Byte*)&buffer, 1, sizeof (uint32_t), file) != sizeof (uint32_t)) {
        return false;
    }

    *value = ntohl(buffer);
    return true;
}

/// \brief Чтение беззнакового 64-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Куда поместить результат.
/// \return Признак успешного выполнения операции.
bool IO::readInt64 (FILE* file, uint64_t *value)
{
    uint32_t buffer[2];

    if (::fread((Byte*)buffer, 1, sizeof(uint64_t), file) != sizeof(uint64_t)) {
        return false;
    }

    buffer[0] = ntohl(buffer[0]);
    buffer[1] = ntohl(buffer[1]);
    *value = (((uint64_t)buffer[1]) << 32u) + ((uint64_t)buffer[0]);

    return true;
}

/// \brief Запись беззнакового 32-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Записываемое значение.
/// \return Признак успешного выполнения операции.
bool IO::writeInt32 (FILE* file, uint32_t value)
{
    value = htonl(value);
    if (::fwrite((Byte*)&value, 1, sizeof(value), file) != sizeof(value)) {
        return false;
    }

    return true;
}

/// \brief Запись беззнакового 64-битного целого в сетевом формате.
/// \param file Файл.
/// \param value Записываемое значение.
/// \return Признак успешного выполнения операции.
bool IO::writeInt64 (FILE* file, uint64_t value)
{
    auto ptr = (uint32_t*)&value;
    uint32_t temp[2];
    temp[0] = htonl(ptr[0]);
    temp[1] = htonl(ptr[1]);
    auto buffer = (Byte*)temp;
    if (::fwrite(buffer, 1, sizeof(value), file) != sizeof(value)) {
        return false;
    }

    return true;
}

/// \brief Получение текущей директории.
/// \return Строка с полным путем текущей директории.
String IO::getCurrentDirectory()
{
#ifdef IRBIS_WINDOWS

    Char buf[FILENAME_MAX];
    memset (buf, 0, sizeof(buf));
    if (!::GetCurrentDirectoryW (FILENAME_MAX, buf)) {
        throw IrbisException();
    }
    return String(buf);

#else

    char buf [FILENAME_MAX];
    memset (buf, 0, sizeof (buf));
    if (!::getcwd (buf, sizeof (buf))) {
        throw IrbisException();
    }
    return string2wide (std::string (buf));

#endif
}

/// \brief Получение текущей директории.
/// \return Строка с полным путем текущей директории.
std::string IO::getCurrentDirectoryNarrow()
{
#ifdef IRBIS_WINDOWS

    char buf[FILENAME_MAX];
    memset (buf, 0, sizeof(buf));
    if (!::GetCurrentDirectoryA (FILENAME_MAX, buf)) {
        throw IrbisException();
    }
    return std::string(buf);

#else

    char buf [FILENAME_MAX];
    memset (buf, 0, sizeof (buf));
    if (!::getcwd (buf, sizeof (buf))) {
        throw IrbisException();
    }
    return std::string (buf);

#endif
}

/// \brief Смена текущей диретории.
/// \param dir Имя новой текущей директории.
void IO::setCurrentDirectory(const String &dir)
{
#ifdef IRBIS_WINDOWS

    if (!::SetCurrentDirectoryW (dir.c_str())) {
        throw IrbisException();
    }

#else

    auto s = wide2string (dir);
    if (::chdir (s.c_str()) < 0) {
        throw IrbisException();
    }

#endif
}

/// \brief Смена текущей диретории.
/// \param dir Имя новой текущей директории.
void IO::setCurrentDirectory(const std::string &dir)
{
#ifdef IRBIS_WINDOWS

    if (!::SetCurrentDirectoryA (dir.c_str())) {
        throw IrbisException();
    }

#else

    if (::chdir (dir.c_str()) < 0) {
        throw IrbisException();
    }

#endif
}

/// \brief Получение расширения для файла.
/// \param path Путь к файлу.
/// \return Расширение (пустое, если нет расширения).
String IO::getExtension (const String &path)
{
    if (path.empty()) {
        return String();
    }

    auto ptr = path.cend() - 1;
    if (ptr >= path.cbegin()) {
        while(true) {
            const auto c = *ptr;
            if (c == L'.') {
                if (ptr + 1 == path.cend()) {
                    return String();
                }
                break;
            }
            if (c == L'/' || c == L'\\') {
                return String();
            }
            if (ptr == path.cbegin()) {
                return String();
            }
            --ptr;
        }
    }
    return path.substr(ptr - path.cbegin());
}

/// \brief Получение расширения для файла.
/// \param path Путь к файлу.
/// \return Расширение (пустое, если нет расширения).
std::string IO::getExtension (const std::string &path)
{
    if (path.empty()) {
        return std::string();
    }

    auto ptr = path.cend() - 1;
    if (ptr >= path.cbegin()) {
        while(true) {
            const auto c = *ptr;
            if (c == '.') {
                if (ptr + 1 == path.cend()) {
                    return std::string();
                }
                break;
            }
            if (c == '/' || c == '\\') {
                return std::string();
            }
            if (ptr == path.cbegin()) {
                return std::string();
            }
            --ptr;
        }
    }
    return path.substr(ptr - path.cbegin());
}

/// \brief Получение имени файла (с расширением, если есть).
/// \param path Путь к файлу.
/// \return Имя файла (с расширением, если есть)
String IO::getFileName (const String &path)
{
    if (path.empty()) {
        return String();
    }

    auto ptr = path.cend() - 1;
    if (ptr >= path.cbegin()) {
        while(true) {
            const auto c = *ptr;
            if (c == L'/' || c == L'\\') {
                ++ptr;
                break;
            }
            if (ptr == path.cbegin()) {
                break;
            }
            --ptr;
        }
    }

    if (ptr == path.cend()) {
        return String();
    }

    return path.substr(ptr - path.cbegin());
}

/// \brief Получение имени файла (с расширением, если есть).
/// \param path Путь к файлу.
/// \return Имя файла (с расширением, если есть)
std::string IO::getFileName (const std::string &path)
{
    if (path.empty()) {
        return std::string();
    }

    auto ptr = path.cend() - 1;
    if (ptr >= path.cbegin()) {
        while(true) {
            const auto c = *ptr;
            if (c == '/' || c == '\\') {
                ++ptr;
                break;
            }
            if (ptr == path.cbegin()) {
                break;
            }
            --ptr;
        }
    }

    if (ptr == path.cend()) {
        return std::string();
    }

    return path.substr(ptr - path.cbegin());
}

/// \brief Получение директории для указанного файла.
/// \param path Путь к файлу.
/// \return Директория (пустая строка, если нет).
String IO::getDirectory (const String &path)
{
    if (path.empty()) {
        return String();
    }

    auto ptr = path.cend() - 1;
    if (ptr >= path.cbegin()) {
        while (true) {
            const auto c = *ptr;
            if (c == L'/' || c == L'\\') {
                if (ptr != path.cbegin()) {
                    --ptr;
                }
                break;
            }
            if (ptr == path.cbegin()) {
                break;
            }
            --ptr;
        }
    }

    if (ptr == path.cbegin()) {
        if (*ptr == L'/' || *ptr == L'\\') {
            return path.substr (0, 1);
        }
        return String();
    }

    return path.substr(0, ptr - path.cbegin() + 1);
}

/// \brief Получение директории для указанного файла.
/// \param path Путь к файлу.
/// \return Директория (пустая строка, если нет).
std::string IO::getDirectory (const std::string &path)
{
    if (path.empty()) {
        return std::string();
    }

    auto ptr = path.cend() - 1;
    if (ptr >= path.cbegin()) {
        while (true) {
            const auto c = *ptr;
            if (c == '/' || c == '\\') {
                if (ptr != path.cbegin()) {
                    --ptr;
                }
                break;
            }
            if (ptr == path.cbegin()) {
                break;
            }
            --ptr;
        }
    }

    if (ptr == path.cbegin()) {
        if (*ptr == '/' || *ptr == '\\') {
            return path.substr (0, 1);
        }
        return std::string();
    }

    return path.substr(0, ptr - path.cbegin() + 1);
}

/// \brief Превращает неправильные слэши в правильные.
/// \param path Путь к файлу.
/// \return Обработанный путь.
String& IO::convertSlashes (String &path) noexcept
{
    for (auto &c : path) {
#ifdef IRBIS_WINDOWS

        if (c == L'/') {
            c = L'\\';
        }

#else

        if (c == L'\\') {
            c = L'/';
        }

#endif
    }
    return path;
}

/// \brief Превращает неправильные слэши в правильные.
/// \param path Путь к файлу.
/// \return Обработанный путь.
std::string& IO::convertSlashes (std::string &path) noexcept
{
    for (auto &c : path) {
#ifdef IRBIS_WINDOWS

        if (c == '/') {
            c = '\\';
        }

#else

        if (c == '\\') {
            c = '/';
        }

#endif
    }
    return path;
}

/// \brief Склеивание пути из двух компонентов.
/// \param path1 Первый компонент.
/// \param path2 Второй компонент.
/// \return Склеенный путь.
String IO::combinePath (const String &path1, const String &path2)
{
    // TODO implement properly

    if (path1.empty()) {
        return path2;
    }
    if (path2.empty()) {
        return path1;
    }

    auto p1 = path1;
    trimTrailingSlashes(p1);

#ifdef IRBIS_WINDOWS

    return p1 + L"\\" + path2;

#else

    return p1 + L"/" + path2;

#endif
}

/// \brief Склеивание пути из двух компонентов.
/// \param path1 Первый компонент.
/// \param path2 Второй компонент.
/// \return Склеенный путь.
std::string IO::combinePath (const std::string &path1, const std::string &path2)
{
    // TODO implement properly

    if (path1.empty()) {
        return path2;
    }
    if (path2.empty()) {
        return path1;
    }

    auto p1 = path1;
    trimTrailingSlashes(p1);

#ifdef IRBIS_WINDOWS

    return p1 + "\\" + path2;

#else

    return p1 + "/" + path2;

#endif
}

/// \brief Существует ли указанная директория?
/// \param path Путь к директории.
/// \return `true` если существует.
bool IO::directoryExist (const String &path)
{
    const auto narrow = irbis::wide2string(path);
    struct stat info { 0 };
    if (!::stat(narrow.c_str(), &info)) {
        return (info.st_mode & S_IFDIR) != 0; // NOLINT(hicpp-signed-bitwise)
    }
    return false;
}

/// \brief Существует ли указанная директория?
/// \param path Путь к директории.
/// \return `true` если существует.
bool IO::directoryExist (const std::string &path)
{
    struct stat info { 0 };
    if (!::stat(path.c_str(), &info)) {
        return (info.st_mode & S_IFDIR) != 0; // NOLINT(hicpp-signed-bitwise)
    }
    return false;
}

/// \brief Существует ли указанный файл?
/// \param path Путь к файлу.
/// \return `true` если существует.
bool IO::fileExist (const String &path)
{
    std::string narrow = irbis::wide2string(path);
    struct stat info { 0 };
    if (!::stat(narrow.c_str(), &info)) {
        return (info.st_mode & S_IFREG) != 0; // NOLINT(hicpp-signed-bitwise)
    }
    return false;
}

/// \brief Существует ли указанный файл?
/// \param path Путь к файлу.
/// \return `true` если существует.
bool IO::fileExist (const std::string &path)
{
    struct stat info { 0 };
    if (!::stat(path.c_str(), &info)) {
        return (info.st_mode & S_IFREG) != 0; // NOLINT(hicpp-signed-bitwise)
    }
    return false;
}

/// \brief Создание директории с указанным именем.
/// \param dir Имя директории.
/// \param createNew Выбрасывать исключение, если директория уже существует.
void IO::createDirectory (const String &dir, bool createNew)
{
    if (IO::directoryExist(dir)) {
        if (createNew) {
            throw IrbisException();
        }
        return;
    }

#ifdef IRBIS_WINDOWS

    if (!::CreateDirectoryW(dir.c_str(), nullptr)) {
        throw IrbisException();
    }

#else

    std::string narrow = irbis::wide2string(dir);
    if (::mkdir(narrow.c_str(), 0755) < 0) {
        throw IrbisException();
    }

#endif
}

/// \brief Создание директории с указанным именем.
/// \param dir Имя директории.
/// \param createNew Выбрасывать исключение, если директория уже существует.
void IO::createDirectory (const std::string &dir, bool createNew)
{
    if (IO::directoryExist(dir)) {
        if (createNew) {
            throw IrbisException();
        }
        return;
    }

#ifdef IRBIS_WINDOWS

    if (!::CreateDirectoryA(dir.c_str(), nullptr)) {
        throw IrbisException();
    }

#else

    if (::mkdir(dir.c_str(), 0755) < 0) {
        throw IrbisException();
    }

#endif
}

/// \brief Удаление файла с указанным именем.
/// \param path Имя файла.
void IO::deleteFile (const String &path)
{
    if (!IO::fileExist(path)) {
        return;
    }

#ifdef IRBIS_WINDOWS

    if (!::DeleteFileW (path.c_str())) {
        throw IrbisException();
    }

#else

    std::string narrow = irbis::wide2string (path);
    if (::remove (narrow.c_str()) < 0) {
        throw IrbisException();
    }

#endif
}

/// \brief Удаление файла с указанным именем.
/// \param path Имя файла.
void IO::deleteFile (const std::string &path)
{
    if (!IO::fileExist(path)) {
        return;
    }

#ifdef IRBIS_WINDOWS

    if (!::DeleteFileA (path.c_str())) {
        throw IrbisException();
    }

#else

    if (::remove (path.c_str()) < 0) {
        throw IrbisException();
    }

#endif
}

/// \brief Создание файла с указанным именем.
/// \param path Имя файла.
/// \param createNew Выбрасывать исключение, если файл уже существует.
void IO::createFile (const String &path, bool createNew)
{
    if (createNew) {
        if (IO::fileExist (path)) {
            throw IrbisException();
        }
    }

#ifdef IRBIS_WINDOWS

    const auto handle = ::CreateFileW
        (
            path.c_str(),
            GENERIC_READ|GENERIC_WRITE, // NOLINT(hicpp-signed-bitwise)
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            0
        );
    if (handle == INVALID_HANDLE_VALUE) {
        throw IrbisException();
    }
    ::CloseHandle(handle);

#else

    std::string narrow = irbis::wide2string (path);
    auto fd = ::creat(narrow.c_str(), S_IREAD|S_IWRITE);
    if (fd < 0) {
        throw IrbisException();
    }
    ::close(fd);

#endif
}

/// \brief Создание файла с указанным именем.
/// \param path Имя файла.
/// \param createNew Выбрасывать исключение, если файл уже существует.
void IO::createFile (const std::string &path, bool createNew)
{
    if (createNew) {
        if (IO::fileExist (path)) {
            throw IrbisException();
        }
    }

#ifdef IRBIS_WINDOWS

    const auto handle = ::CreateFileA
        (
            path.c_str(),
            GENERIC_READ|GENERIC_WRITE, // NOLINT(hicpp-signed-bitwise)
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            0
        );
    if (handle == INVALID_HANDLE_VALUE) {
        throw IrbisException();
    }
    ::CloseHandle(handle);

#else

    auto fd = ::creat(path.c_str(), S_IREAD|S_IWRITE);
    if (fd < 0) {
        throw IrbisException();
    }
    ::close(fd);

#endif
}

/// \brief Получение временной директории.
/// \return Путь к временной директории.
String IO::getTempDirectory()
{
#ifdef IRBIS_WINDOWS

    wchar_t buffer[FILENAME_MAX];
    ::memset(buffer, 0, sizeof(buffer));
    if (!::GetTempPathW(FILENAME_MAX, buffer)) {
        throw IrbisException();
    }
    return String(buffer);

#else

    const char *result = ::getenv("TMPDIR");
    if (!result) {
        result = ::getenv("TEMPDIR");
    }
    if (!result) {
        result = ::getenv("TMP");
    }
    if (!result) {
        result = ::getenv("TEMP");
    }
    if (!result) {
        // The Filesystem Hierarchy Standard version 3.0 says:
        // The /tmp directory must be made available for programs
        // that require temporary files.
        result = "/tmp";
    }
    return irbis::string2wide(result);

#endif
}

/// \brief Получение временной директории.
/// \return Путь к временной директории.
std::string IO::getTempDirectoryNarrow()
{
#ifdef IRBIS_WINDOWS

    char buffer[FILENAME_MAX];
    ::memset(buffer, 0, sizeof(buffer));
    if (!::GetTempPathA(FILENAME_MAX, buffer)) {
        throw IrbisException();
    }
    return std::string(buffer);

#else

    const char *result = ::getenv("TMPDIR");
    if (!result) {
        result = ::getenv("TEMPDIR");
    }
    if (!result) {
        result = ::getenv("TMP");
    }
    if (!result) {
        result = ::getenv("TEMP");
    }
    if (!result) {
        // The Filesystem Hierarchy Standard version 3.0 says:
        // The /tmp directory must be made available for programs
        // that require temporary files.
        result = "/tmp";
    }
    return result;

#endif
}

/// \brief Удаляет начальные слэши в пути.
/// \param path Путь для обработки (обработка по месту).
/// \return Обработанный путь.
String& IO::trimLeadingSlashes (String &path)
{
    while (!path.empty()) {
        const auto c = path.front();
        if (c != L'/' && c != L'\\') {
            break;
        }
        path.erase(path.begin());
    }
    return path;
}

/// \brief Удаляет начальные слэши в пути.
/// \param path Путь для обработки (обработка по месту).
/// \return Обработанный путь.
std::string& IO::trimLeadingSlashes (std::string &path)
{
    while (!path.empty()) {
        const auto c = path.front();
        if (c != '/' && c != '\\') {
            break;
        }
        path.erase(path.begin());
    }
    return path;
}

/// \brief Удаляет конечные слэши в пути.
/// \param path Путь для обработки (обработка по месту).
/// \return Обработанный путь.
String& IO::trimTrailingSlashes (String &path)
{
    while (!path.empty()) {
        const auto c = path.back();
        if (c != L'/' && c != L'\\') {
            break;
        }
        path.erase(path.end() - 1);
    }
    return path;
}

/// \brief Удаляет конечные слэши в пути.
/// \param path Путь для обработки (обработка по месту).
/// \return Обработанный путь.
std::string& IO::trimTrailingSlashes (std::string &path)
{
    while (!path.empty()) {
        const auto c = path.back();
        if (c != '/' && c != '\\') {
            break;
        }
        path.erase(path.end() - 1);
    }
    return path;
}

/// \brief Получение размера файла в байтах.
/// \param path Путь к файлу.
/// \return Размер файла в байтах.
uint64_t IO::getFileSize (const String &path)
{
#ifdef IRBIS_WINDOWS

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!::GetFileAttributesExW(path.c_str(), ::GetFileExInfoStandard, &data)) {
        throw IrbisException();
    }
    LARGE_INTEGER size;
    size.HighPart = data.nFileSizeHigh;
    size.LowPart = data.nFileSizeLow;
    return size.QuadPart;

#else

    const auto narrow = irbis::wide2string (path);
    struct stat buf { 0 };
    if (::stat (narrow.c_str(), &buf) < 0) {
        throw IrbisException();
    }
    return buf.st_size;

#endif
}

/// \brief Получение размера файла в байтах.
/// \param path Путь к файлу.
/// \return Размер файла в байтах.
uint64_t IO::getFileSize (const std::string &path)
{
#ifdef IRBIS_WINDOWS

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!::GetFileAttributesExA (path.c_str(), ::GetFileExInfoStandard, &data)) {
        throw IrbisException();
    }
    LARGE_INTEGER size;
    size.HighPart = data.nFileSizeHigh;
    size.LowPart = data.nFileSizeLow;
    return size.QuadPart;

#else

    struct stat buf { 0 };
    if (::stat (path.c_str(), &buf) < 0) {
        throw IrbisException();
    }
    return buf.st_size;

#endif
}

/// \brief Удаление папки (пустой).
/// \param path Путь к папке.
/// \return Признак успешности операции.
bool IO::removeDirectory (const String &path)
{
#ifdef IRBIS_WINDOWS

    bool result = RemoveDirectoryW (path.c_str());
    return result;

#else

    const auto narrow = irbis::wide2string (path);
    int retcode = rmdir (narrow.c_str());
    return retcode == 0;

#endif
}

/// \brief Удаление папки (пустой).
/// \param path Путь к папке.
/// \return Признак успешности операции.
bool IO::removeDirectory (const std::string &path)
{
#ifdef IRBIS_WINDOWS

    bool result = RemoveDirectoryA (path.c_str());
    return result;

#else

    int retcode = rmdir (path.c_str());
    return retcode == 0;

#endif
}

}
