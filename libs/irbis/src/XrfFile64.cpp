// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_private.h"

#include <cassert>

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
XrfFile64::XrfFile64 (const String &fileName, DirectAccessMode mode)
{
    auto *modeString = L"r+b";
    if (mode == DirectAccessMode::ReadOnly) {
        modeString = L"rb";
    }
    this->_file = new File (fileName, modeString);
    this->_fileName = fileName;
    this->_mode = mode;
}

/// \brief Деструктор.
XrfFile64::~XrfFile64()
{
    this->close();
}

/// \brief Закрытие файла.
void XrfFile64::close()
{
    if (this->_file) {
        delete this->_file;
        this->_file = nullptr;
    }
}

/// \brief Расчёт смещения в XRF-файле для указанного MFN.
/// \param mfn MFN, для которого необходимо смещение.
/// \return Вычисленное смещение.
Offset XrfFile64::getOffset(Mfn mfn) noexcept
{
    return static_cast<Offset> (XrfRecord64::RecordSize) * static_cast<Offset> (mfn - 1);
}

/// \brief Считывание одной XRF-записи.
/// \param mfn MFN записи.
/// \return Прочитанная запись.
/// \warning Очень неэффективно! Лучше читать "пачками".
/// \throw irbis::IrbisException
XrfRecord64 XrfFile64::readRecord (Mfn mfn)
{
    assert (mfn > 0);
    std::lock_guard<std::mutex> guard (this->_mutex);
    const auto offset = XrfFile64::getOffset (mfn);
    if (this->_file->seek (offset) != offset) {
        throw IrbisException();
    }
    XrfRecord64 result;
    const auto stream = this->_file->getStream();
    if (!IO::readInt64 (stream, &result.offset)
        || !IO::readInt32 (stream, reinterpret_cast<uint32_t*> (&result.status))) {
        throw IrbisException();
    }
    return result;
}

/// \brief Сохранение одной XRF-записи.
/// \param mfn MFN записи.
/// \param record Сохраняемая запись.
/// \warning Очень неэффективно! Лучше писать "пачками".
/// \throw irbis::IrbisException
void XrfFile64::writeRecord (Mfn mfn, XrfRecord64 record)
{
    assert (mfn > 0);
    std::lock_guard<std::mutex> guard (this->_mutex);
    const auto offset = XrfFile64::getOffset (mfn);
    if (this->_file->seek(offset) != offset) {
        throw IrbisException();
    }
    const auto stream = this->_file->getStream();
    if (!IO::writeInt64(stream, record.offset)
        || !IO::writeInt32(stream, static_cast<uint32_t> (record.status))) {
        throw IrbisException();
    }
}

/// \brief Создание XRF-файла. Если файл уже существует, он усекается.
/// \param fileName Имя файла.
/// \return XRF-файл.
XrfFile64 XrfFile64::create (const String &fileName)
{
    IO::createFile (fileName);
    return { fileName, DirectAccessMode::Exclusive };
}

}
