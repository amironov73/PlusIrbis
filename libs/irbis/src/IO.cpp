// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#ifdef  IRBIS_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "ws2_32.lib")

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

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
    *value = (((uint64_t)buffer[1]) << 32) + ((uint64_t)buffer[0]);

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

/// \brief Превращает неправильные слэши в правильные.
/// \param path Путь к файлу.
/// \return Обработанный путь.
String& IO::convertSlashes (String &path) noexcept
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
    return path1 + L"/" + path2;
}

/// \brief Существует ли указанная директория?
/// \param path Путь к директории.
/// \return `true` если существует.
bool IO::directoryExist (const String &path)
{
    std::string narrow = irbis::wide2string(path);
    struct stat info { 0 };
    if (!::stat(narrow.c_str(), &info)) {
        return (info.st_mode & S_IFDIR) != 0;
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
        return (info.st_mode & S_IFREG) != 0;
    }
    return false;
}

/// \brief Создание директории с указанным именем.
/// \param dir Имя директории.
void IO::createDirectory (const String &dir)
{
    if (directoryExist(dir)) {
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

void IO::deleteFile (const String &path)
{
    if (!fileExist(path)) {
        return;
    }

#ifdef IRBIS_WINDOWS

    if (!::DeleteFileW(path.c_str())) {
        throw IrbisException();
    }

#else

    std::string narrow = irbis::wide2string(path);
    if (::remove(narrow.c_str()) < 0) {
        throw IrbisException();
    }

#endif
}

}
