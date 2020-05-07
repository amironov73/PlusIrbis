// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"

#ifdef IRBIS_WINDOWS

#include <windows.h>

#else

#include<sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif

#include <cctype>
#include <limits>
#include <iterator>

class MemoryFile;
class MemoryRegion;

/// \brief Файл, спроецированный в память.
class IRBIS_API MemoryFile final
{
public:

    explicit MemoryFile (const std::string &name);
    ~MemoryFile() noexcept;

    void        close () noexcept;
    std::size_t size  () const noexcept { return static_cast <std::size_t> (m_fileSize); }

private:
    friend class MemoryRegion;
    uint64_t m_fileSize { 0 };

#ifdef IRBIS_WINDOWS
    void *m_mappingHandle { nullptr };
#else
    int m_fileHandle { -1 };
#endif
};

/// \brief Окно с спроецированным содержимым файлом.
class IRBIS_API MemoryRegion final
{
public:

    explicit MemoryRegion (const MemoryFile &file);
    MemoryRegion (const MemoryFile &file, std::size_t offset, std::size_t size);
    ~MemoryRegion() noexcept;

    irbis::Byte* begin  () const noexcept;
    void         close  () noexcept;
    irbis::Byte* end    () const noexcept { return this->begin () + this->size(); }
    std::size_t  offset () const noexcept { return m_viewOffset; }
    std::size_t  size   () const noexcept { return m_viewSize; }

private:
    std::size_t m_viewOffset { 0 };
    std::size_t m_viewSize { 0 };

#ifdef IRBIS_WINDOWS
    void *m_viewHandle { nullptr };
#else
    void *m_pointer { nullptr };
#endif
};

MemoryFile::MemoryFile (const std::string &name)
{
#ifdef IRBIS_WINDOWS

    DWORD desiredAccess = GENERIC_READ;
    DWORD shareMode = 0;
    LPSECURITY_ATTRIBUTES securityAttributes = nullptr;
    DWORD creationDisposition = OPEN_EXISTING;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = nullptr;
    LPCSTR objectName = nullptr;

    HANDLE fileHandle = ::CreateFileA
        (
            name.c_str(),
            desiredAccess,
            shareMode,
            securityAttributes,
            creationDisposition,
            flagsAndAttributes,
            templateFile
        );
    if (fileHandle == INVALID_HANDLE_VALUE) {
        throw irbis::IrbisException();
    }

    LARGE_INTEGER size;
    GetFileSizeEx (fileHandle, &size);
    m_fileSize = size.QuadPart;

    m_mappingHandle = ::CreateFileMappingA
        (
            fileHandle,
            securityAttributes,
            PAGE_READONLY,
            0,
            0,
            objectName
        );
    CloseHandle (fileHandle);

    if (m_mappingHandle == nullptr) {
        throw irbis::IrbisException();
    }

#else

    const int flags = O_RDONLY;

#ifdef IRBIS_APPLE
    m_fileHandle = ::open (name.c_str(), flags);
#else
    m_fileHandle = ::open64 (name.c_str(), flags);
#endif

    if (m_fileHandle < 0) {
        throw irbis::IrbisException();
    }

#ifdef IRBIS_APPLE
    struct stat buf { 0 };
    if (::fstat (m_fileHandle, &buf) < 0) {
        throw irbis::IrbisException();
    }
#else
    struct stat64 buf { 0 };
    if (::fstat64 (m_fileHandle, &buf) < 0) {
        throw irbis::IrbisException();
    }
#endif

    m_fileSize = static_cast<std::size_t> (buf.st_size);


#endif
}

MemoryFile::~MemoryFile() noexcept
{
    this->close();
}

void MemoryFile::close() noexcept
{
#ifdef IRBIS_WINDOWS

    CloseHandle (m_mappingHandle);

#else

    if (m_fileHandle >= 0) {
        ::close (m_fileHandle);
    }
    m_fileHandle = -1;

#endif
}

MemoryRegion::MemoryRegion (const MemoryFile &file)
{
#ifdef IRBIS_WINDOWS

    m_viewSize = file.m_fileSize;
    m_viewHandle = ::MapViewOfFile
        (
            file.m_mappingHandle,
            FILE_MAP_READ,
            0,
            0,
            m_viewSize
        );
    if (m_viewHandle == nullptr) {
        throw irbis::IrbisException();
    }
    m_viewOffset = 0;

#else

    m_viewOffset = 0;
    m_viewSize = file.m_fileSize;
    m_pointer = mmap
        (
            nullptr,
            m_viewSize,
            PROT_READ,
            MAP_PRIVATE,
            file.m_fileHandle,
            m_viewOffset
        );

    if (m_pointer == MAP_FAILED) {
        throw irbis::IrbisException();
    }

#endif
}

MemoryRegion::MemoryRegion (const MemoryFile &file, std::size_t offset, std::size_t size)
{
#ifdef IRBIS_WINDOWS

    m_viewHandle = ::MapViewOfFile
        (
            file.m_mappingHandle,
            FILE_MAP_READ,
            static_cast <DWORD> (offset >> 32u),
            static_cast <DWORD> (offset & 0xFFFFFFFFul),
            size
        );
    if (m_viewHandle == nullptr) {
        throw irbis::IrbisException();
    }
    m_viewOffset = offset;
    m_viewSize = size;

#else

    m_pointer = mmap
        (
            nullptr,
            size,
            PROT_READ,
            MAP_PRIVATE,
            file.m_fileHandle,
            offset
        );
    m_viewOffset = offset;
    m_viewSize = size;

#endif
}

MemoryRegion::~MemoryRegion() noexcept
{
    this->close();
}

irbis::Byte* MemoryRegion::begin  () const noexcept
{
#ifdef IRBIS_WINDOWS

    return static_cast<irbis::Byte*> (m_viewHandle);

#else

    return static_cast<irbis::Byte*> (m_pointer);

#endif
}


void MemoryRegion::close () noexcept
{
#ifdef IRBIS_WINDOWS

    ::CloseHandle (m_viewHandle);

#else

    ::munmap (m_pointer, m_viewSize);

#endif
}

////////////////////////////////////////////////////////////////

void testMmap()
{
    {
        MemoryFile file ("main.cpp");
        std::cout << file.size () << std::endl;
        MemoryRegion region (file);
        auto ptr = region.begin ();
        for (auto end = region.end ();
             ptr != end;
             ++ptr) {
            std::cout << *ptr;
        }
        std::cout << std::endl;
    }

    std::cout << "=============================================" << std::endl;

    {
        MemoryFile file ("main.cpp");
        MemoryRegion region (file, 0, 100);
        auto ptr = region.begin ();
        for (auto end = region.end ();
             ptr != end;
             ++ptr) {
            std::cout << *ptr;
        }
        std::cout << std::endl;
    }
}
