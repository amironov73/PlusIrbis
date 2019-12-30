// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

DirectAccess64::DirectAccess64(const String &path)
{
    this->mst = new MstFile64(L"", DirectAccessMode::ReadOnly);
    this->xrf = new XrfFile64(L"", DirectAccessMode::ReadOnly);
    throw NotImplementedException();
}

DirectAccess64::~DirectAccess64()
{
    delete this->mst;
    this->mst = nullptr;
    delete this->xrf;
    this->xrf = nullptr;
}

MstRecord64 DirectAccess64::readRawRecord(unsigned int mfn)
{
    // TODO implement
    MstRecord64 result;
    return result;
}

MarcRecord DirectAccess64::readRecord(unsigned int mfn)
{
    // TODO implement
    MarcRecord result;
    return result;
}

}
