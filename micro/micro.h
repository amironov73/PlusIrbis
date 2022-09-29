// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef MICROIRBIS_H
#define MICROIRBIS_H

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <future>
#include <ios>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <functional>

namespace irbis
{
    class MarcRecord;
    class RecordField;
    class SubField;

    using RecordFieldList = std::vector<RecordField>;
    using StringList      = std::vector<std::string>;
    using SubFieldList    = std::vector<SubField>;

    /// \brief Статус записи.
    enum class RecordStatus : unsigned int
    {
        None = 0,                                       ///< Исходное состояние.
        LogicallyDeleted = 1,                           ///< Запись логически удалена.
        PhysicallyDeleted = 2,                          ///< Запись физически удалена.
        Deleted = LogicallyDeleted | PhysicallyDeleted, ///< Запись удалена.
        Absent = 4,                                     ///< Запись отсутствует.
        NonActualized = 8,                              ///< Не актуализирована.
        Last = 32,                                      ///< Последняя версия записи.
        Locked = 64                                     ///< Запись заблокирована.
    };

    RecordStatus operator | (RecordStatus left, RecordStatus right);
    RecordStatus operator & (RecordStatus left, RecordStatus right);

    /// \brief Подполе записи. Состоит из кода и значения.
    class SubField final
    {
        public:

        /// \brief Отсутствующий код подполя.
        const static char NoCode {L'\0'};

        char code {'\0'};  ///< Односимвольный код подполя.
        std::string value; ///< Значение подполя (может быть пустой строкой).

        explicit SubField(char code_, const std::string &value_ = "") : code(code_), value(value_) {} ///< Конструктор.
        SubField(char code_, std::string &&value_) noexcept: code(code_), value(std::move(value_)) {} ///< Конструктор.
        SubField() = default; ///< Конструктор по умолчанию.
        SubField(const SubField &) = default; ///< Конструктор копирования.
        SubField(SubField &&) noexcept = default; ///< Конструктор перемещения.
        SubField &operator = (const SubField &) = default; ///< Оператор копирования
        SubField &operator = (SubField &&) noexcept = default; ///< Оператор перемещения.
        ~SubField() = default; ///< Деструктор.

        SubField clone() const;
        void decode(const std::string &line);
        bool empty() const noexcept;
        std::string toString() const;

        friend std::ostream& operator<<(std::ostream &stream, const SubField &subfield);
        SubField &operator<<(const std::string &value_);
        SubField &operator<<(std::string &&value_);
        SubField &operator<<(const char *value_);
    };

    /// \brief Поле записи.
    class RecordField final
    {
        public:

        int tag { 0 };           ///< Метка поля.
        std::string value;       ///< Значение поля до первого разделителя.
        SubFieldList  subfields; ///< Подполя.

        explicit RecordField (int tag_, const std::string &value_ = "")
            : tag { tag_ }, value { value_ } {} ///< Конструктор.
        RecordField (int tag_, std::string &&value_) noexcept
            : tag { tag_ }, value { std::move (value_) } {} ///< Конструктор.
        RecordField (int tag_, std::initializer_list <SubField> subfields_);
        RecordField  ()                                   = default; ///< Конструктор по умолчанию.
        RecordField  (const RecordField &)                = default; ///< Конструктор копирования.
        RecordField  (RecordField &&) noexcept            = default; ///< Конструктор перемещения.
        ~RecordField ()                                   = default; ///< Деструктор.
        RecordField& operator = (const RecordField &)     = default; ///< Оператор копирования.
        RecordField& operator = (RecordField &&) noexcept = default; ///< Оператор перемещения.

        RecordField& add                   (char code, const std::string &value = "");
        RecordField& add                   (char code, std::string &&value);
        RecordField& clear                 ();
        RecordField  clone                 ()                                   const;
        void         decode                (const std::string &line);
        void         decodeBody            (const std::string &line);
        bool         empty                 ()                                   const noexcept;
        SubField*    getFirstSubfield      (char code)                          const noexcept;
        std::string  getFirstSubfieldValue (char code)                          const noexcept;
        RecordField& removeSubfield        (char code);
        RecordField& setSubfield           (char code, const std::string &newValue);
        RecordField& setSubfield           (char code, std::string &&newValue);
        std::string  toString()                                                 const;

        friend std::ostream& operator << (std::ostream &stream, const RecordField &field);

        RecordField& operator << (const SubField &subfield);
        RecordField& operator << (SubField &&subfield);
        RecordField& operator << (const std::string &body);
        RecordField& operator << (std::string &&body);
        RecordField& operator << (const char *body);
        RecordField& operator << (char code);
    };

    /// \brief Библиографическая запись. Состоит из произвольного количества полей.
    class MarcRecord final
    {
        public:

        unsigned int mfn { 0u };                    ///< MFN (порядковый номер в базе) записи.
        RecordStatus status { RecordStatus::None }; ///< Статус записи. Представляет собой набор флагов.
        unsigned int version { 0u };                ///< Номер версии записи.
        std::string database;                       ///< Имя базы данных.
        RecordFieldList fields;                     ///< Список полей.

        MarcRecord  (std::initializer_list <RecordField> fields_);
        MarcRecord  ()                              = default; ///< Конструктор по умолчанию.
        MarcRecord  (const MarcRecord &)            = default; ///< Конструктор копирования.
        MarcRecord  (MarcRecord &&)                 = default; ///< Конструктор перемещения.
        ~MarcRecord ()                              = default; ///< Деструктор.
        MarcRecord& operator = (const MarcRecord &) = default; ///< Оператор копирования.
        MarcRecord& operator = (MarcRecord &&)      = default; ///< Оператор перемещения.

        RecordField&              add         (int tag, const std::string &value = "");
        RecordField&              add         (int tag, std::string &&value);
        MarcRecord                clone       ()                                          const;
        void                      decode      (const StringList &lines);
        bool                      deleted     ()                                          const noexcept;
        std::string               encode      (const std::string &delimiter = "\u001F\u001E") const;
        std::string               fm          (int tag, char code = 0)                    const noexcept;
        StringList                fma         (int tag, char code = 0)                    const;
        RecordField*              getField    (int tag, int occurrence = 0)               const noexcept;
        std::vector<RecordField*> getFields   (int tag)                                   const;
        MarcRecord&               removeField (int tag);
        MarcRecord&               setField    (int tag, const std::string &value);
        MarcRecord&               setField    (int tag, std::string &&value);
        MarcRecord&               reset       ()                                          noexcept;

        friend std::ostream& operator << (std::ostream &stream, const MarcRecord &record);

        MarcRecord& operator << (const RecordField &field);
        MarcRecord& operator << (RecordField &&field);
        MarcRecord& operator << (const std::string &text);
        MarcRecord& operator << (int tag);
        MarcRecord& operator << (char code);
        MarcRecord& operator && (const std::string &value);
        MarcRecord& operator && (const char *value);
    };

}

#endif //MICROIRBIS_H
