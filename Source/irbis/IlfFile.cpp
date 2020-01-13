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

///
/// \class irbis::IlfFile
///
/// \details ILF - Архив текстовых файлов Irbis Library Files.
///
/// ILF-файлы – специфические для ИРБИС текстовые файлы,
/// содержащие независимые поименованные разделы.
///
/// Могут использоваться для хранения основных текстовых
/// ресурсов баз данных: форматов (PFT), рабочих листов (WS),
/// вложенных РЛ (WSS), справочников (MNU),
/// таблиц переформатирования (FST) и др.
///
/// При этом предлагается следующая структура имен ILF-файлов:
/// `<ИМЯ_БД>_<ТИП>.ILF`.
///
/// Например:
/// `Ibis_pft.ilf` – ILF-файл для хранения форматов БД `IBIS`.
///
/// С форума:
///
/// * Сервер ищет файлы сначала в ilf затем в директории БД.
/// * Сервер пересылает клиенту ИРБИС64 файлы по одному.
/// * Клиент ИРБИС64 кэширует скачанные файлы. Так чтобы
/// заметить изменения, например в рабочих листах,
/// нужно выполнить режим ОБНОВИТЬ КОНТЕКСТ.
/// * Распаковка ILF - это задача исключительно сервера.
///
/// Разбор содержимого типичного ILF-файла в редакторе `010 Editor`.
///
/// \image html ilf.png
///
/// Описание в формате `010 Editor`:
///
/// \verbinclude ilf.bt
///

namespace irbis {

/// \brief Чтение локального файла.
/// \param fileName Имя файла.
void IlfFile::readLocalFile(const String &fileName)
{
    throw NotImplementedException();
}

}
