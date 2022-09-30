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
    class ClientSocket;
    class ClientQuery;
    class Connection;
    class MarcRecord;
    class RecordField;
    class ServerResponse;
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

    class Connection final
    {
    private:
        bool _connected;
        StringList _databaseStack;
        std::mutex _mutex;

        friend class ServerResponse;

        bool _checkConnection();

    public:

        std::string  host;                     ///< Адрес сервера в виде строки.
        short        port;                     ///< Номер порта сервера.
        std::string  username;                 ///< Логин пользователя.
        std::string  password;                 ///< Пароль пользователя.
        std::string  database;                 ///< Имя текущей базы данных.
        std::string  workstation;              ///< Тип клиента.
        int          clientId;                 ///< Уникальный идентификатор клиента.
        int          queryId;                  ///< Порядковый номер команды, отсылаемой на сервер.
        int          lastError;                ///< Код ошибки, возвращённый сервером в ответ на последнюю команду.
        std::string  serverVersion;            ///< Версия сервера (получается при подключении).
        // IniFile      iniFile;                  ///< Содержимое серверного INI-файла для данного клиента.
        int          interval;                 ///< Интервал автоматического подтверждения, секунды.
        // RequestStage stage;                    ///< Этап выполнения запроса
        std::unique_ptr <ClientSocket> socket; ///< Клиентский сокет.

        Connection  ();
        ~Connection ();

        /// \brief Подключено ли к серверу?
        bool   connected() const noexcept { return this->_connected; }

        bool        connect               ();
        void        disconnect            ();
        bool        execute               (ClientQuery &query);
        int         getMaxMfn             (const std::string &databaseName);
        bool        noOp                  ();
        void        parseConnectionString (const std::string &connectionString);
        std::string popDatabase           ();
        std::string pushDatabase          (const std::string &newDatabase);
        std::string toConnectionString    () const;

//        std::vector<DatabaseInfo> listDatabases  (const IniFile &iniFile, const String &defaultFileName);
//        std::vector<DatabaseInfo> listDatabases  (const FileSpecification &specification);
//        StringList                listFiles      (const FileSpecification &specification);
//        StringList                listFiles      (const std::vector<FileSpecification> &specifications);
//        Bytes                     readBinaryFile (const FileSpecification &specification);
//        IniFile                   readIniFile    (const FileSpecification &specification);
//        MenuFile                  readMenuFile   (const FileSpecification &specification);
//        String                    readTextFile   (const FileSpecification &specification);
//        std::string               readAnsiFile   (const FileSpecification &specification);
//        StringList                readTextFiles  (std::vector<FileSpecification> &specifications);
//        StringList                readTextLines  (const FileSpecification &specification);

//        StringList               listTerms    (const String &prefix);
//        std::vector<TermPosting> readPostings (const PostingParameters &parameters);
//        std::vector<TermInfo>    readTerms    (const String &startTerm, int numberOfTerms = 100);
//        std::vector<TermInfo>    readTerms    (const TermParameters &parameters);
//        MfnList                  search       (const Search &search);
//        MfnList                  search       (const String &expression);
//        MfnList                  search       (const SearchParameters &parameters);

//        IRBIS_MAYBE_UNUSED bool                      createDatabase   (const String &databaseName, const String &description, bool readerAccess);
//        IRBIS_MAYBE_UNUSED bool                      createDictionary (const String &databaseName = L"");
//        IRBIS_MAYBE_UNUSED bool                      deleteDatabase   (const String &databaseName = L"");
//        ServerStat                getServerStat    ();
//        Version                   getServerVersion ();
//        std::vector <ProcessInfo> listProcesses    ();
//        IRBIS_MAYBE_UNUSED bool                      reloadDictionary (const String &databaseName = L"");
//        IRBIS_MAYBE_UNUSED bool                      reloadMasterFile (const String &databaseName = L"");
//        IRBIS_MAYBE_UNUSED bool                      restartServer    ();
//        IRBIS_MAYBE_UNUSED bool                      truncateDatabase (const String &databaseName = L"");
//        IRBIS_MAYBE_UNUSED bool                      unlockDatabase   (const String &databaseName = L"");
//        IRBIS_MAYBE_UNUSED bool                      unlockRecords    (const String &databaseName, const MfnList &mfnList);

    };

}

#endif //MICROIRBIS_H
