// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_internal.h"

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
    \class irbis::XrfRecord64

    \details Из официальной документации: http://sntnarciss.ru/irbis/spravka/wtcp006002000.htm

    ```
    Каждая ссылка состоит из 3-х полей:
    Число бит Параметр
    32        XRF_LOW – младшее слово в 8 байтовом смещении на запись;
    32        XRF_HIGH– старшее слово в 8 байтовом смещении на запись;
    32        XRF_FLAGS – Индикатор записи в виде битовых флагов
              следующего содержания:
                BIT_LOG_DEL(1)  - логически удаленная запись;
                BIT_PHYS_DEL(2) - физически удаленная запись;
                BIT_ABSENT(4)  - несуществующая запись;
                BIT_NOTACT_REC(8)- неактуализированная запись;
                BIT_LOCK_REC(64)- заблокированная запись.
    ```

    \class irbis::XrfFile64

    \details Файл перекрестных ссылок XRF представляет собой
    таблицу ссылок на записи файла документов.

    Нулевая ссылка соответствует записи файла документов
    с номером 1, первая – 2  и т. д.

    \warning Объекты данного типа -- неперемещаемые и некопируемые!

 */

namespace irbis {

/// \brief Размер одной записи в XRF-файле.
const int XrfRecord64::RecordSize = sizeof(Offset) + sizeof (Mfn); //-V119

/// \brief Запись логически или физически удалена?
/// \return `true` если запись удалена.
bool XrfRecord64::deleted() const noexcept
{
    return (this->status & RecordStatus::Deleted) != RecordStatus::None;
}

/// \brief Запись заблокирована?
/// \return `true` если запись заблокирована.
bool XrfRecord64::locked() const noexcept
{
    return (this->status & RecordStatus::Locked) != RecordStatus::None;
}

/// \brief Преобразование в текстовый вид.
/// \return Строковое представление.
String XrfRecord64::toString() const
{
    return String (L", offset=") + std::to_wstring (this->offset)
           + String (L", status:=") + std::to_wstring (static_cast<uint32_t> (this->status));
}

//=========================================================

/// \brief Конструктор.
/// \param fileName Имя файла.
/// \param mode Режим доступа.
XrfFile64::XrfFile64 (const String &fileName, DirectAccessMode mode)
    : _file { File::openRead(fileName).toHeap() },
    _fileName { fileName }, _mode { mode }
{
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
XrfRecord64 XrfFile64::readRecord (Mfn mfn)
{
    assert (mfn > 0);
    std::lock_guard<std::mutex> guard (this->_mutex);
    const auto offset = XrfFile64::getOffset (mfn);
    this->_file->seek (offset);
    XrfRecord64 result;
    result.offset = this->_file->readInt64();
    result.status = (RecordStatus) this->_file->readInt32();
    return result;
}

/// \brief Сохранение одной XRF-записи.
/// \param mfn MFN записи.
/// \param record Сохраняемая запись.
/// \warning Очень неэффективно! Лучше писать "пачками".
void XrfFile64::writeRecord (Mfn mfn, XrfRecord64 record)
{
    assert (mfn > 0);
    std::lock_guard<std::mutex> guard (this->_mutex);
    const auto offset = XrfFile64::getOffset (mfn);
    this->_file->seek (offset);
    this->_file->writeInt64 (record.offset);
    this->_file->writeInt32 ((uint32_t) record.status);
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
