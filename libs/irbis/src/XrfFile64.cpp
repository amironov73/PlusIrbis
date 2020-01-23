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

/*!
    \class irbis::XrfFile64

    \details Файл перекрестных ссылок XRF представляет собой
    таблицу ссылок на записи файла документов.

    Нулевая ссылка соответствует записи файла документов
    с номером 1, первая – 2  и т. д.

    \warning Объекты данного типа -- неперемещаемые и некопируемые!

 */

namespace irbis {

/// \brief Конструктор.
/// \param fileName Имя файла.
/// \param mode Режим доступа.
XrfFile64::XrfFile64(const String &fileName, DirectAccessMode mode)
{
    // TODO implement properly
    auto ansiName = unicode_to_cp1251(fileName);
    this->_file = fopen(ansiName.c_str(), "rb");
    if (!this->_file) {
        // TODO proper exception with message
        throw IrbisException();
    }
    this->_fileName = fileName;
    this->_mode = mode;
}

/// \brief Деструктор.
XrfFile64::~XrfFile64()
{
    if (this->_file) {
        fclose(this->_file);
        this->_file = nullptr;
    }
}

Offset XrfFile64::getOffset(Mfn mfn) noexcept
{
    return static_cast<Offset>(XrfRecord64::RecordSize) * static_cast<Offset>(mfn - 1);
}

XrfRecord64 XrfFile64::readRecord(Mfn mfn)
{
    std::lock_guard<std::mutex> guard(this->_mutex);

    const auto offset = XrfFile64::getOffset(mfn);
    // TODO handle the truncation
    ::fseek(this->_file, static_cast<long>(offset), SEEK_SET);
    XrfRecord64 result;
    result.mfn = mfn;
    if (!IO::readInt64(this->_file, &result.offset)
        || !IO::readInt32(this->_file, &result.status)) {
        throw IrbisException();
    }

    return result;
}

}

