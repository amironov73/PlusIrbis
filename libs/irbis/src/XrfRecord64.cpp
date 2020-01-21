// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

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

 */


namespace irbis {

/// \brief Размер одной записи в XRF-файле.
const int XrfRecord64::RecordSize = sizeof(Offset) + sizeof (Mfn);

/// \brief Запись логически или физически удалена?
/// \return `true` если запись удалена.
bool XrfRecord64::deleted() const noexcept
{
    return (this->status & RecordStatus::Deleted) != 0;
}

/// \brief Запись заблокирована?
/// \return `true` если запись заблокирована.
bool XrfRecord64::locked() const noexcept
{
    return (this->status & RecordStatus::Locked) != 0;
}

/// \brief Преобразование в текстовый вид.
/// \return Строковое представление.
String XrfRecord64::toString() const
{
    return String(L"MFN=") + std::to_wstring(this->mfn)
        + String(L", offset=") + std::to_wstring(this->offset)
        + String(L", status:=") + std::to_wstring(this->status);
}

}
