// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*

    https://docs.microsoft.com/en-us/windows/win32/memory/creating-a-view-within-a-file

    If you want to view a portion of the file that does not start at the beginning
    of the file, you must create a file mapping object. This object is the size
    of the portion of the file you want to view plus the offset into the file.
    For example, if you want to view the 1 kilobyte (1K) that begins 131,072 bytes
    (128K) into the file, you must create a file mapping object of at least
    132,096 bytes (129K) in size. The view starts 131,072 bytes (128K) into the
    file and extend for at least 1,024 bytes. This example assumes a file
    allocation granularity of 64K.

    File allocation granularity affects where a map view can start. A map view
    must start at an offset into the file that is a multiple of the file allocation
    granularity. So the data you want to view may be the file offset modulo
    the allocation granularity into the view. The size of the view is the offset
    of the data modulo the allocation granularity, plus the size of the data that
    you want to examine.

    For example, suppose that the GetSystemInfo function indicates an allocation
    granularity of 64K. To examine 1K of data that is 138,240 bytes (135K) into
    the file, do the following:

    * Create a file mapping object of at least 139,264 bytes (136K) in size.

    * Create a file view that starts at a file offset that is the largest multiple
      of the file allocation granularity less than the offset you require. In this case,
      the file view starts at offset 131,072 (128K) into the file. The view is 139264 bytes
      (136K) minus 131,072 bytes (128K), or 8,192 bytes (8K), in size.
      
    * Create a pointer offset 7K into the view to access the 1K in which you are interested.

    If the data you want straddles a file allocation granularity boundary, you could make
    the view larger than the file allocation granularity. This avoids breaking the data
    into pieces.

 */

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
struct MemoryIterator;
class MemoryRegion;

/// \brief Файл, спроецированный в память.
class IRBIS_API MemoryFile final
{
public:

    MemoryFile() noexcept = default;
    explicit MemoryFile (const std::string &fileName);
    MemoryFile (const MemoryFile&) = delete;
    MemoryFile (MemoryFile &&other) noexcept;
    MemoryFile& operator = (const MemoryFile&) = delete;
    MemoryFile& operator = (MemoryFile &&other) noexcept;
    ~MemoryFile() noexcept;

    void        close () noexcept;
    bool        open  (const std::string &fileName);
    std::size_t size  () const noexcept { return static_cast <std::size_t> (m_fileSize); }
    MemoryRegion view ();
    MemoryRegion view (std::size_t offset, std::size_t size);

private:
    friend class MemoryRegion;
    uint64_t m_fileSize { 0 };

#ifdef IRBIS_WINDOWS
    void *m_mappingHandle { INVALID_HANDLE_VALUE };
#else
    int m_fileHandle { -1 };
#endif
};

/// \brief Окно с спроецированным содержимым файлом.
class IRBIS_API MemoryRegion final
{
public:

    MemoryRegion() = default;
    explicit MemoryRegion    (MemoryFile *file);
    MemoryRegion             (MemoryFile *file, std::size_t offset, std::size_t size);
    MemoryRegion             (const MemoryRegion&) = delete;
    MemoryRegion             (MemoryRegion &&other) noexcept;
    MemoryRegion& operator = (const MemoryRegion&) = delete;
    MemoryRegion& operator = (MemoryRegion &&other) noexcept;
    ~MemoryRegion() noexcept;

    MemoryIterator begin   () const noexcept;
    void           close   () noexcept;
    MemoryIterator end     () const noexcept;
    void           move    (std::size_t newOffset);
    std::size_t    offset  () const noexcept { return m_viewOffset; }
    irbis::Byte*   pointer () const noexcept { return m_pointer; };
    std::size_t    size    () const noexcept { return m_viewSize; }

    irbis::Byte& operator[] (std::size_t offset);

private:
    MemoryFile *m_file { nullptr };
    std::size_t m_viewOffset { 0 };
    std::size_t m_viewSize { 0 };
    irbis::Byte *m_pointer { nullptr };

    void _init();
};

struct IRBIS_API MemoryIterator final
    : public std::iterator<std::random_access_iterator_tag, irbis::Byte>
{
    MemoryRegion *m_region;
    std::size_t   m_position;

    MemoryIterator () noexcept : m_region { nullptr }, m_position { 0 } {}
    MemoryIterator (MemoryRegion *region, std::size_t position) noexcept
        : m_region { region }, m_position { position } {}

    MemoryIterator             (const MemoryIterator&) noexcept = default;
    MemoryIterator             (MemoryIterator&&)      noexcept = default;
    MemoryIterator& operator = (const MemoryIterator&) noexcept = default;
    MemoryIterator& operator = (MemoryIterator&&)      noexcept = default;

    irbis::Byte* data() const noexcept { return m_region->pointer() + m_position; }

    bool operator == (const MemoryIterator &other) const noexcept
    {
        return m_position == other.m_position;
    }

    bool operator != (const MemoryIterator &other) const noexcept
    {
        return m_position != other.m_position;
    }

    bool operator < (const MemoryIterator &other) const noexcept
    {
        return m_position < other.m_position;
    }

    MemoryIterator& operator ++ () noexcept
    {
        ++m_position;
        return *this;
    }

    MemoryIterator operator ++ (int) & noexcept
    {
        MemoryIterator copy (*this);
        ++m_position;
        return copy;
    }

    MemoryIterator& operator += (std::size_t n) noexcept
    {
        m_position += n;
        return *this;
    }

    MemoryIterator& operator -- () noexcept
    {
        --m_position;
        return *this;
    }

    MemoryIterator operator -- (int) & noexcept
    {
        MemoryIterator copy (*this);
        --m_position;
        return copy;
    }

    MemoryIterator& operator -= (std::size_t n) noexcept
    {
        m_position -= n;
        return *this;
    }

    irbis::Byte& operator * () const noexcept { return (*m_region) [m_position]; }

    friend MemoryIterator operator + (const MemoryIterator &left, std::size_t right) noexcept
    {
        auto result = left;
        result += right;
        return result;
    }

    friend std::ptrdiff_t operator - (const MemoryIterator &left, const MemoryIterator &right) noexcept
    {
        return static_cast<std::ptrdiff_t> (left.m_position - right.m_position);
    }
};

//////////////////////////////////////////////////////////////////////

/// \brief Конструктор
/// \param fileName Имя файла.
MemoryFile::MemoryFile (const std::string &fileName)
{
    if (!this->open (fileName)) {
        throw irbis::IrbisException();
    }
}

/// \brief Конструктор перемещения.
MemoryFile::MemoryFile (MemoryFile &&other) noexcept
{
    m_fileSize = other.m_fileSize;

#ifdef IRBIS_WINDOWS

    m_mappingHandle = other.m_mappingHandle;
    other.m_mappingHandle = INVALID_HANDLE_VALUE;

#else

    m_fileHandle = other.m_fileHandle;
    other.m_fileHandle = -1;

#endif
}

/// \brief Оператор перемещения.
MemoryFile& MemoryFile::operator = (MemoryFile &&other) noexcept
{
    m_fileSize = other.m_fileSize;

#ifdef IRBIS_WINDOWS

    m_mappingHandle = other.m_mappingHandle;
    other.m_mappingHandle = INVALID_HANDLE_VALUE;

#else

    m_fileHandle = other.m_fileHandle;
    other.m_fileHandle = -1;

#endif

    return *this;
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

bool MemoryFile::open (const std::string &fileName)
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
            fileName.c_str(),
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

    // В дальнейшем файловый дескриптор нам не нужен.
    CloseHandle (fileHandle);

    if (m_mappingHandle == nullptr) {
        return false;
    }

#else

    const int flags = O_RDONLY;

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD)
    m_fileHandle = ::open (fileName.c_str(), flags);
#else
    m_fileHandle = ::open64 (fileName.c_str(), flags);
#endif

    if (m_fileHandle < 0) {
        return false;
    }

#if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD)
    struct stat buf { 0 };
    if (::fstat (m_fileHandle, &buf) < 0) {
        return false;
    }
#else
    struct stat64 buf { 0 };
    if (::fstat64 (m_fileHandle, &buf) < 0) {
        return false;
    }
#endif

    m_fileSize = static_cast<std::size_t> (buf.st_size);

#endif

    return true;
}

MemoryRegion MemoryFile::view ()
{
    return MemoryRegion (this);
}

MemoryRegion MemoryFile::view (std::size_t offset, std::size_t size)
{
    return MemoryRegion (this, offset, size);
}

//////////////////////////////////////////////////////////////////////

MemoryRegion::MemoryRegion (MemoryFile *file)
    : m_file { file }, m_viewOffset { 0 }, m_viewSize { file->m_fileSize }
{
    this->_init();
}

MemoryRegion::MemoryRegion (MemoryFile *file, std::size_t offset, std::size_t size)
    : m_file { file }, m_viewOffset { offset }, m_viewSize { size }
{
    this->_init();
}

MemoryRegion::~MemoryRegion() noexcept
{
    this->close();
}

MemoryRegion::MemoryRegion (MemoryRegion &&other) noexcept
{
    m_file = other.m_file;
    other.m_file = nullptr;
    m_viewSize = other.m_viewSize;
    m_viewOffset = other.m_viewOffset;
    m_pointer = other.m_pointer;
    other.m_pointer = nullptr;
}

MemoryRegion& MemoryRegion::operator = (MemoryRegion &&other) noexcept
{
    return *this;
}

irbis::Byte& MemoryRegion::operator[] (std::size_t offset)
{
    return *(this->pointer() + offset);
}

MemoryIterator MemoryRegion::begin  () const noexcept
{
    auto noconst = const_cast <MemoryRegion*> (this);
    return { noconst, 0 };
}


void MemoryRegion::close () noexcept
{
#ifdef IRBIS_WINDOWS

    ::UnmapViewOfFile (m_pointer);

#else

    ::munmap (m_pointer, m_viewSize);

#endif
}

MemoryIterator MemoryRegion::end () const noexcept
{
    auto noconst = const_cast <MemoryRegion*> (this);
    return { noconst, this->size() };
}

void MemoryRegion::_init()
{
#ifdef IRBIS_WINDOWS

    m_pointer = (irbis::Byte*) ::MapViewOfFile
        (
            m_file->m_mappingHandle,
            FILE_MAP_READ,
            static_cast <DWORD> (m_viewOffset >> 32u),
            static_cast <DWORD> (m_viewOffset & 0xFFFFFFFFul),
            m_viewSize
        );
    if (m_pointer == nullptr) {
        throw irbis::IrbisException();
    }

#else

    #if defined (IRBIS_APPLE) || defined (IRBIS_FREEBSD)
    m_pointer = (irbis::Byte*) mmap
        (
            nullptr,
            m_viewSize,
            PROT_READ,
            MAP_PRIVATE,
            m_file->m_fileHandle,
            m_viewOffset
        );
#else
    m_pointer = (irbis::Byte*) mmap64
        (
            nullptr,
            m_viewSize,
            PROT_READ,
            MAP_PRIVATE,
            m_file->m_fileHandle,
            m_viewOffset
        );
#endif

    if (m_pointer == MAP_FAILED) {
        throw irbis::IrbisException();
    }

#endif
}

void MemoryRegion::move (std::size_t newOffset)
{
    this->close();
    m_viewOffset = newOffset;
    this->_init();
}


//////////////////////////////////////////////////////////////////////

void testMmap()
{
    {
        MemoryFile file ("main.cpp");
        std::cout << file.size () << std::endl;
        MemoryRegion region = file.view();
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
        MemoryRegion region = file.view (0, 100);
        auto ptr = region.begin ();
        for (auto end = region.end ();
             ptr != end;
             ++ptr) {
            std::cout << *ptr;
        }
        std::cout << std::endl;
    }
}
