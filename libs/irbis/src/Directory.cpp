// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#ifdef IRBIS_WINDOWS

#include <windows.h>

#else

#include <dirent.h>

#endif

namespace irbis {

/// \internal
struct Directory::Impl
{
#ifdef IRBIS_WINDOWS

    Impl (const std::string &name)
    {
        this->dirname = name + "\\*";
    }

    ~Impl()
    {
        ::FindClose (this->hFind);
    }

    std::string read()
    {
        if (started) {
            if (!::FindNextFileA (this->hFind, &ffd)) {
                return {};
            }
            return { ffd.cFileName };
        }
        else {
            this->hFind = ::FindFirstFileA (this->dirname.c_str(), &ffd);
            if (this->hFind == INVALID_HANDLE_VALUE) {
                return {};
            }
            this->started = true;
            return { ffd.cFileName };
        }
    }

    void rewind()
    {
        if (this->started) {
            ::FindClose (this->hFind);
            this->started = false;
        }
    }

    std::string dirname;
    WIN32_FIND_DATAA ffd { 0 };
    bool started { false };
    HANDLE hFind = INVALID_HANDLE_VALUE;

#else

    Impl (const std::string &name)
    {
        this->dir = ::opendir (name.c_str());
    }

    ~Impl()
    {
        ::closedir (this->dir);
    }

    std::string read() const
    {
#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD)

        struct dirent *entry;

        entry = ::readdir (dir);
        if (!entry) {
            return {};
        }

        return { entry->d_name };

#else	

        struct dirent64 *entry;

        entry = ::readdir64 (dir);
        if (!entry) {
            return {};
        }

        return { entry->d_name };

#endif
    }

    void rewind() const
    {
        ::rewinddir (dir);
    }

    DIR *dir;

#endif
};

/// \brief Конструктор.
/// \param name Имя директории.
Directory::Directory (const std::string &name)
    : _impl { new Impl { name } }
{
}

/// \brief Деструктор.
Directory::~Directory()
{
    delete this->_impl;
}
/// \brief Чтение следующего файла.
/// \return Файл либо пусто.
std::string Directory::read()
{
    return this->_impl->read();
}

/// \brief Чтение всех файлов в директории.
/// \return
std::vector <std::string> Directory::readAll()
{
    std::vector<std::string> result;
    this->rewind();
    while (true) {
        auto entry = this->read();
        if (entry.empty()) {
            break;
        }
        if (entry != "." && entry != "..") {
            result.push_back (std::move (entry));
        }
    }
    return result;
}

/// \brief Находит файл с указанным именем (с точностью до регистра символов).
/// \param name Искомое имя.
/// \return Найденный файл либо пусто.
std::string Directory::find (const std::string &name)
{
    this->rewind();
    while (true) {
        const auto entry = this->read();
        if (entry.empty()) {
            break;
        }
        if (sameString (entry, name)) {
            return { entry };
        }
    }
    return {};
}

/// \brief Перемотка в начало.
void Directory::rewind()
{
    this->_impl->rewind();
}

}
