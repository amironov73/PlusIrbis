// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_direct.h"
#include "irbis_internal.h"

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
    \file Mst.cpp

    Работа с мастер-файлом напрямую.

    \class irbis::MstControlRecord64
    \brief Первая запись в файле документов - управляющая.
    \details Она формируется в момент создания базы данных
    и поддерживается автоматически. Ее содержание следующее:

    ```
    Число бит Параметр
    32        CTLMFN – резерв;
    32        NXTMFN –номер записи файла документов,
              назначаемый для следующей записи,
              создаваемой в базе данных;
    32        NXT_LOW – младшее слово смещения на свободное место
              в файле; (всегда указывает на конец файла MST)
    32        NXT_HIGH – старшее слово смещения на свободное
              место в файле
    32        MFTYPE – резерв;
    32        RECCNT – резерв;
    32        MFCXX1 – резерв;
    32        MFCXX2 – резерв;
    32        MFCXX3 – индикатор блокировки базы данных
              (0 – нет, >0 – да).
    ```

    \var irbis::MstControlRecord64::RecordSize
    \brief Размер управляющей записи.

    \var irbis::MstControlRecord64::LockFlagPosition
    \brief Позиция индикатора блокировки базы данных
    в управляющей записи.

    \var irbis::MstControlRecord64::ctlMfn
    \brief Резерв.

    \var irbis::MstControlRecord64::nextMfn
    \brief Номер записи файла документов, назначаемый
    для следующей записи, создаваемой в базе данных.

    \var irbis::MstControlRecord64::nextPosition
    \brief Смещение свободного места в файле;
    не обязательно указывает на конец файла MST,
    т. к. пара мегабайт может быть захвачена авансом,
    чтобы избежать фрагментации файла документов.

    \var irbis::MstControlRecord64::mftType
    \brief Резерв.

    \var irbis::MstControlRecord64::recCnt
    \brief Резерв.

    \var irbis::MstControlRecord64::reserv1
    \brief Резерв.

    \var irbis::MstControlRecord64::reserv2
    \brief Резерв.

    \var irbis::MstControlRecord64::locked
    \brief Индикатор блокировки базы данных в целом.

    \class irbis::MstRecordLeader64
    \brief Лидер записи.

    \var irbis::MstRecordLeader64::mfn
    \brief Номер записи в файле документов.

    \var irbis::MstRecordLeader64::length
    \brief Длина записи в байтах.
    \details В официальной документации сказано,
    что всегда четное число, но по факту это не так.

    \var irbis::MstRecordLeader64::previous
    \brief Ссылка на предыдущую версию записи.

    \var irbis::MstRecordLeader64::base
    \brief Смещение (базовый адрес) полей переменной длины
    (это общая часть лидера и справочника записи в байтах).

    \var irbis::MstRecordLeader64::nvf
    \brief Число полей в записи (т. е. число входов
    в справочнике).

    \var irbis::MstRecordLeader64::status
    \brief Статус записи (логически удаленная и т. п.).

    \var irbis::MstRecordLeader64::version
    \brief Номер версии записи.

    \class irbis::MstDictionaryEntry64
    \brief Элемент справочника MST-записи, описывающий
    поле переменной длины.

    \var irbis::MstDictionaryEntry64::tag
    \brief Метка поля.

    \var irbis::MstDictionaryEntry64::position
    \brief Смещение данных в байтах.

    \var irbis::MstDictionaryEntry64::length
    \brief Длина данных в байтах.

 */

namespace irbis {

/// \brief Длина элемента справочника MST-записи.
const int MstDictionaryEntry64::EntrySize = 12;

//=========================================================

/// \brief Считывание управляющей записи с диска.
/// \param file
void MstControlRecord64::read (File *file) {
    //fread (this, sizeof(MstControlRecord64), 1, file);
    throw NotImplementedException();
}

//=========================================================

void MstDictionaryEntry64::read (File *file) {
    //fread (this, sizeof(MstDictionaryEntry64), 1, file);
    throw NotImplementedException();
}

//=========================================================

/// \brief Конструктор.
/// \param fileName
/// \param mode
MstFile64::MstFile64 (const String &fileName, DirectAccessMode mode = DirectAccessMode::ReadOnly)
{
    this->_file = new File (fileName, L"rb");
    this->fileName = fileName;
}

/// \brief Деструктор.
MstFile64::~MstFile64()
{
    if (this->_file) {
        delete this->_file;
        this->_file = nullptr;
    }
}

/// \brief Чтение записи.
/// \param position Позиция
/// \return
MstRecord64 MstFile64::readRecord (int64_t position)
{
    this->_file->seek(position);
    //fseek (this->_file, position, SEEK_SET);
    throw NotImplementedException();
}

//=========================================================

/// \brief Запись удалена?
/// \return true если удалена.
bool MstRecord64::deleted() const
{
    return (this->leader.status & RecordStatus::Deleted) != RecordStatus::None;
}

/// \brief Превращение в полноценную запись.
/// \return
MarcRecord MstRecord64::toMarcRecord() const
{
    MarcRecord result;
    result.status = RecordStatus::None; // TODO obtain the status
    result.version = 0; // TODO obtain the version
    //result.fields.reserve(dictionary.size());
    for (const auto &entry : dictionary) {
        RecordField field;
        field.tag = entry.tag;
        // field.parse(entry.text); // TODO parse the field
        result.fields.push_back(field);
    }

    return result;
}

//=========================================================

/// \brief Чтение лидера с диска.
/// \param file Файл.
void MstRecordLeader64::read (File *file)
{
    //fread (this, sizeof(MstRecordLeader64), 1, file);
    throw NotImplementedException();
}

}