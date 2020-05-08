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

struct IRBIS_API System
{
    static std::size_t pageAligned (std::size_t offset);
    static std::size_t pageSize();
    static std::size_t allocationGranularity();
};

/// \brief Выравниваем на границу страницы.
/// \param offset Смещение, которое должно быть выравнено.
/// \return Результат выравнивания.
std::size_t System::pageAligned (std::size_t offset)
{
    const auto pageSize = System::pageSize();
    return offset / pageSize * pageSize;
}

/// \brief Размер страницы памяти.
/// \return Размер страницы в байтах.
std::size_t System::pageSize()
{
    static std::size_t _pageSize = 0;

    if (_pageSize == 0) {
#ifdef IRBIS_WINDOWS

        SYSTEM_INFO SystemInfo;
        //GetSystemInfo (&SystemInfo);
        GetNativeSystemInfo (&SystemInfo);
        _pageSize = SystemInfo.dwPageSize;
#else

        _pageSize = sysconf (_SC_PAGE_SIZE);

#endif
    }

    return _pageSize;
}

std::size_t System::allocationGranularity()
{
    static std::size_t _granularity = 0;

    if (_granularity == 0) {
#ifdef IRBIS_WINDOWS

        SYSTEM_INFO SystemInfo;
        GetNativeSystemInfo (&SystemInfo);
        _granularity = SystemInfo.dwAllocationGranularity;
#else

        _granularity = sysconf (_SC_PAGE_SIZE);

#endif
    }

    return _granularity;
}

void testSystem()
{
    std::cout << "Granularity=" << System::allocationGranularity() << std::endl;
    std::cout << "Page size=" << System::pageSize() << std::endl;
    std::cout << "Aligned=" << System::pageAligned (152931) << std::endl;
}
