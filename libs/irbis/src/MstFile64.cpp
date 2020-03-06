// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

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

MstFile64::MstFile64(const std::wstring &fileName, DirectAccessMode mode = DirectAccessMode::ReadOnly)
{
#ifdef IRBIS_WINDOWS
    this->_file = nullptr;
    _wfopen_s (&this->_file, fileName.c_str(), L"rb");
#else
    this->_file = fopen (toUtf (fileName).c_str(), "rb");
#endif
    if (!this->_file) {
        throw IrbisException();
    }

    this->fileName = fileName;
}

MstFile64::~MstFile64()
{
    if (this->_file) {
        fclose (this->_file);
        this->_file = nullptr;
    }
}

MstRecord64 MstFile64::readRecord (long position)
{
    fseek (this->_file, position, SEEK_SET);
    throw NotImplementedException();
}

}
