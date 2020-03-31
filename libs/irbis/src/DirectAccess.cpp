// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_private.h"

namespace irbis {

/// \brief Конструктор.
/// \param parPath Путь до PAR-файла.
/// \param systemPath Системный путь.
DirectAccess64::DirectAccess64 (const String &parPath, const String &systemPath)
{
    const auto par = ParFile::readLocalFile (parPath);
    auto databaseName = IO::getFileName (parPath);
    databaseName = databaseName.substr (0, databaseName.size() - 4);
    auto mstPath = IO::combinePath (systemPath, par.mst);
    mstPath = IO::combinePath (mstPath, databaseName + L".mst");
    IO::convertSlashes (mstPath);
    if (!IO::fileExist (mstPath)) {
        throw IrbisException();
    }

    auto xrfPath = IO::combinePath (systemPath, par.xrf);
    xrfPath = IO::combinePath (xrfPath, databaseName + L".xrf");
    IO::convertSlashes (xrfPath);
    if (!IO::fileExist (xrfPath)) {
        throw IrbisException();
    }

    this->mst = new MstFile64 (mstPath, DirectAccessMode::ReadOnly);
    this->xrf = new XrfFile64 (xrfPath, DirectAccessMode::ReadOnly);
}

/// \brief Деструктор.
DirectAccess64::~DirectAccess64()
{
    delete this->mst;
    this->mst = nullptr;
    delete this->xrf;
    this->xrf = nullptr;
}

/// \brief Чтение сырой записи.
MstRecord64 DirectAccess64::readMstRecord (Mfn mfn)
{
    MstRecord64 result;

    return result;
}

MarcRecord DirectAccess64::readRecord (Mfn mfn)
{
    const auto xrf_ = this->xrf->readRecord (mfn);
    const auto mst_ = this->mst->readRecord (xrf_.offset);
    return mst_.toMarcRecord();
}

}
