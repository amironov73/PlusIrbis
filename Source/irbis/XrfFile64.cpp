// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <sys/stat.h>
#include <fcntl.h>

#ifdef IRBIS_WINDOWS
#include <cwchar>
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

XrfFile64::XrfFile64(const std::wstring &fileName, DirectAccessMode mode)
{
    throw NotImplementedException();
}

XrfFile64::~XrfFile64()
{
    // TODO implement
}

unsigned long long int XrfFile64::getOffset(unsigned int mfn)
{
    return static_cast<unsigned long long int>(XrfRecord64::RecordSize) * static_cast<unsigned long long int>(mfn - 1);
}

XrfRecord64 XrfFile64::readRecord(unsigned int mfn)
{
    // TODO lock

    const auto offset = getOffset(mfn);
    // TODO handle truncation
    fseek(this->_file, static_cast<long>(offset), SEEK_SET);
    XrfRecord64 result;
    result.mfn = mfn;
    //result.offset = read64bit(_stream);
    //_stream >> result.status;

    return result;
}

}

