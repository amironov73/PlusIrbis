#ifndef PLUSIRBIS_IRBIS_DIRECT_H
#define PLUSIRBIS_IRBIS_DIRECT_H

#include <memory>

#include "irbis.h"

namespace irbis {

//=========================================================

class  DirectAccess64;
class  File; // from irbis_private.h
struct MstControlRecord64;
struct MstDictionaryEntry64;
class  MstFile64;
class  MstRecord64;
struct MstRecordLeader64;
class  XrfFile64;
class  XrfRecord64;

//=========================================================

/// \brief Класс для прямого доступа к базе данных.
class IRBIS_API DirectAccess64 final
{
public:
    MstFile64 *mst;
    XrfFile64 *xrf;
    String database;

    DirectAccess64 (const String &parPath, const String &systemPath);
    DirectAccess64 (const DirectAccess64 &) = delete; ///< Конструктор копирования.
    DirectAccess64 (const DirectAccess64 &&) = delete; ///< Конструктор перемещения.
    DirectAccess64& operator = (const DirectAccess64 &) = delete; ///< Оператор копирования.
    DirectAccess64& operator = (const DirectAccess64 &&) = delete; ///< Оператор перемещения.
    ~DirectAccess64();

    MstRecord64 readMstRecord (Mfn mfn);
    MarcRecord readRecord     (Mfn mfn);
};

//=========================================================

/// \brief Ввод-вывод ISO 2709
class IRBIS_API Iso2709 final
{
public:
    static const int  MarkerLength = 24;
    static const char RecordDelimiter = 0x1D;
    static const char FieldDelimiter = 0x1E;
    static const char SubFieldDelimiter = 0x1F;

    static MarcRecord* readRecord (File *device, const Encoding *encoding);
    static bool writeRecord (File *device, const MarcRecord &record, const Encoding *encoding);
};

//=========================================================

#pragma pack(push, 1)
struct IRBIS_API MstControlRecord64 final
{
    const static int RecordSize;
    const static long LockFlagPosition;

    uint32_t ctlMfn       { 0 };
    uint32_t nextMfn      { 0 };
    int64_t  nextPosition { 0 };
    uint32_t mftType      { 0 };
    uint32_t recCnt       { 0 };
    uint32_t reserv1      { 0 };
    uint32_t reserv2      { 0 };
    uint32_t locked       { 0 };

    void read (File *file);
};
#pragma pack(pop)

//=========================================================

#pragma pack(push, 1)
struct IRBIS_API MstDictionaryEntry64 final
{
    const static int EntrySize;

    int32_t  tag      { 0 };
    int32_t  position { 0 };
    int32_t  length   { 0 };

    void read (File *file);
};
#pragma pack(pop)

//=========================================================

/// \brief Мастер-файл.
class IRBIS_API MstFile64 final
{
public:
    MstControlRecord64 control;
    String fileName;

    MstFile64 (const String &fileName, DirectAccessMode mode = DirectAccessMode::ReadOnly);
    MstFile64 (const MstFile64 &)              = delete;
    MstFile64 (const MstFile64 &&)             = delete;
    MstFile64& operator = (const MstFile64 &)  = delete;
    MstFile64& operator = (const MstFile64 &&) = delete;
    ~MstFile64()                               = default;

    MstRecord64 readRecord (int64_t position);

private:
    std::unique_ptr<File> _file;
};

//=========================================================

#pragma pack(push, 1)
struct IRBIS_API MstRecordLeader64 final
{
    const static int LeaderSize;

    uint32_t     mfn      { 0 };
    uint32_t     length   { 0 };
    uint64_t     previous { 0 };
    uint32_t     base     { 0 };
    uint32_t     nvf      { 0 };
    RecordStatus status   { RecordStatus::None };
    uint32_t     version  { 0 };

    void read (File *file);
};
#pragma pack(pop)

//=========================================================

#pragma pack(push, 1)
class IRBIS_API MstRecord64 final
{
public:
    MstRecordLeader64 leader;
    uint64_t offset { 0 };
    std::vector<MstDictionaryEntry64> dictionary;

    bool deleted() const;
    MarcRecord toMarcRecord() const;
};
#pragma pack(pop)

//=========================================================

/// \brief XRF-файл -- файл перекрестных ссылок.
class IRBIS_API XrfFile64 final
{
public:

    XrfFile64 (const String &fileName, DirectAccessMode mode);
    XrfFile64             (const XrfFile64 &) = delete;
    XrfFile64             (XrfFile64 &&)      = delete;
    XrfFile64& operator = (const XrfFile64 &) = delete;
    XrfFile64& operator = (XrfFile64 &&)      = delete;
    ~XrfFile64()                              = default;

    XrfRecord64 readRecord (Mfn mfn);
    void writeRecord (Mfn mfn, XrfRecord64 record);

    static XrfFile64 create (const String &fileName);

private:

    String _fileName;
    std::unique_ptr<File> _file;
    DirectAccessMode _mode;
    std::mutex _mutex;

    static Offset getOffset (Mfn mfn) noexcept;
};

//=========================================================

/// \brief Запись в XRF-файле. Содержит смещение и флаги для записи в MST-файле.
class IRBIS_API XrfRecord64 final
{
public:
    const static int RecordSize;

    Offset       offset { 0 };                  ///< Смещение записи в MST-файле.
    RecordStatus status { RecordStatus::None }; ///< Статус записи (удалена, заблокирована и т.д.).

    bool   deleted()  const noexcept;
    bool   locked()   const noexcept;
    String toString() const;
};

//=========================================================

}

#endif //PLUSIRBIS_IRBIS_DIRECT_H
