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
    class ClientSocketImpl;
    class ClientQuery;
    class Connection;
    class Encoding;
    class FileSpecification;
    class MarcRecord;
    class RecordField;
    class ServerResponse;
    class SubField;
    class Version;

    using Byte            = std::uint8_t;
    using Bytes           = std::vector<Byte>;
    using Mfn             = std::uint32_t;
    using MfnList         = std::vector<Mfn>;
    using RecordFieldList = std::vector<RecordField>;
    using StringList      = std::vector<std::string>;
    using SubFieldList    = std::vector<SubField>;

    //================================================================

    /// \brief Выбор первой непустой строки из произвольного количества.
    template<class T>
    T choose(T first) { return first; }

    /// \brief Выбор первой непустой строки из произвольного количества.
    template<class T1, class... T2>
    T1 choose(const T1 &first, const T2 &... others)
    {
        if (!first.empty()) {
            return first;
        }
        return choose(others...);
    }

    /// \brief Является ли данный элемент одним из перечисленных?
    template<class T>
    bool oneOf(T left, T right) { return left == right; }

    /// \brief Является ли данный элемент одним из перечисленных?
    template<class T1, class... T2>
    bool oneOf(const T1 &first, const T1 &second, const T2 &... others)
    {
        if (oneOf(first, second)) {
            return true;
        }
        return oneOf(first, others...);
    }

    /// \brief Безопасное извлечение элемента из контейнера.
    template<typename T1>
    typename T1::value_type safeAt(const T1 &container, std::size_t index)
    {
        if (index >= container.size()) {
            return typename T1::value_type();
        }
        return container[index];
    }

    bool sameChar (char first, char second) noexcept;
    bool sameString (const std::string &first, const std::string &second) noexcept;
    char firstChar (const std::string &text) noexcept;
    std::string toLower (std::string &text) noexcept;
    std::string toUpper (std::string &text) noexcept;
    bool contains (const std::string &text, char c);
    bool contains (const std::string &text, const std::string &fragment);
    std::string replace (const std::string &text, const std::string &from, const std::string &to);
    std::string trimStart (const std::string &text);
    std::string trimEnd (const std::string &text);
    std::string trim (const std::string &text);
    int fastParse32 (const std::string &text) noexcept;
    int fastParse32 (const char *text) noexcept;
    int fastParse32 (const char *text, std::size_t length) noexcept;
    unsigned int fastParseUnsigned32 (const std::string &text) noexcept;
    unsigned int fastParseUnsigned32 (const char *text) noexcept;
    unsigned int fastParseUnsigned32 (const char *text, std::size_t length) noexcept;
    std::vector<std::string> split (const std::string &text, char delimiter);
    std::vector<std::string> split (const std::string &text, const std::string &delimiter);
    std::vector<std::string> maxSplit (const std::string &text, char separator, int count);

    template<typename T>
    bool isDigit(T c) { return (c >= '0') && (c <= '9'); }

    //================================================================

    /// \brief Строка INI-файла.
    ///
    /// Состоит из ключа и значения, разделённых символом '='.
    class IniLine final
    {
        public:

        std::string key;    ///< Ключ (не должен быть пустым).
        std::string value;  ///< Значение (может быть пустым).

        IniLine() = default;
        IniLine (const std::string &key_, const std::string &value_)
            : key (key_), value (value_) {}
        IniLine (std::string &&key_, std::string &&value_)
            : key (std::move (key_)), value (std::move (value_)) {}

        bool        modified    ()                             const noexcept;
        void        notModified ()                             noexcept;
        void        setKey      (const std::string &newKey);
        void        setKey      (std::string &&newKey)         noexcept;
        void        setValue    (const std::string &newValue);
        void        setValue    (std::string &&newValue)       noexcept;
        std::string toString    ()                             const;

        private:

        bool _modified { false };
    };

    /// \brief Секция INI-файла.
    class IniSection final
    {
        public:

        std::string name;           ///< Имя секции.
        std::vector<IniLine> lines; ///< Строки, входящие в секцию. \see IniLine

        IniSection&    clear       ();
        bool           containsKey (const std::string &key) const noexcept;
        std::ptrdiff_t getIndex    (const std::string &key) const noexcept;
        IniLine*       getLine     (const std::string &key) const noexcept;
        std::string    getValue    (const std::string &key, const std::string &defaultValue) const noexcept;
        bool           modified    ()                       const noexcept;
        void           notModified ();
        IniSection&    removeValue (const std::string &key);
        IniSection&    setValue    (const std::string &key, const std::string &value);
        std::string    toString    ()                       const;

        std::string operator[]     (const std::string &index) const noexcept;
    };

    /// \brief INI-файл.
    class IniFile final
    {
        public:

        std::vector<IniSection> sections; ///< Секции INI-файла.

        IniFile&       clear           ();
        bool           containsSection (const std::string &name) const noexcept;
        IniSection&    createSection   (const std::string &name);
        bool           modified        () const noexcept;
        void           notModified     ();
        std::ptrdiff_t getIndex        (const std::string &name) const noexcept;
        IniSection*    getSection      (const std::string &name) const noexcept;
        std::string    getValue        (const std::string &sectionName, const std::string &keyName, const std::string &defaultValue = "") const noexcept;
        void           parse           (const StringList &lines);
        IniFile&       removeSection   (const std::string &sectionName);
        IniFile&       removeValue     (const std::string &sectionName, const std::string &keyName);
        IniFile&       setValue        (const std::string &sectionName, const std::string &keyName, const std::string &value);
        std::string    toString        () const;
    };

    //================================================================

    /// \brief Клиентский запрос.
    ///
    /// Объекты данного класса неперемещаемые.
    class ClientQuery final
    {
        private:

        std::vector<unsigned char> _content;

        void _write (const unsigned char *bytes, std::size_t size);
        void _write (unsigned char byte);

        public:

        ClientQuery (const Connection &connection, const std::string &commandCode);
        ClientQuery (ClientQuery &) = delete;
        ClientQuery (ClientQuery &&) = delete;
        ClientQuery& operator = (ClientQuery &) = delete;
        ClientQuery& operator = (ClientQuery &&) = delete;
        ~ClientQuery() = default;

        ClientQuery&               add       (int value);
        ClientQuery&               add       (const FileSpecification &specification);
        // ClientQuery&               add       (const MarcRecord &record, const std::wstring &delimiter);
        ClientQuery&               addAnsi   (const std::string &text);
        ClientQuery&               addUtf    (const std::string &text);
        void                       dump      (std::ostream &stream) const;
        std::vector<unsigned char> encode    () const;
        ClientQuery&               newLine   ();
    };

    /// \brief Ответ сервера на клиентский запрос.
    class ServerResponse final
    {
        public:

        std::string command;       ///< Код команды (дублирует клиентский запрос).
        int clientId;              ///< Идентификатор клиента (дублирует клиентский запрос).
        int queryId;               ///< Номер команды (дублирует клиентский запрос).
        int answerSize;            ///< Размер ответа сервера в байтах (в некоторых сценариях отсутствует).
        int returnCode;            ///< Код возврата (бывает не у всех ответов).
        std::string serverVersion; ///< Версия сервера (в некоторых сценариях отсутствует).

        ServerResponse (Connection &connection, ClientQuery &query);
        ServerResponse (ServerResponse &)              = delete;  ///< Конструктор копирования.
        ServerResponse (ServerResponse &&)             = delete;  ///< Конструктор перемещения.
        ~ServerResponse ()                             = default; ///< Деструктор.
        ServerResponse& operator = (ServerResponse &)  = delete;  ///< Оператор копирования.
        ServerResponse& operator = (ServerResponse &&) = delete;  ///< Оператор перемещения.

        bool                     checkReturnCode        ();
        bool                     checkReturnCode        (int nargs, ...);
        static ServerResponse*   emptyResonse           ();
        bool                     eot                    () const;
        std::string              getLine                ();
        std::string              getRemaining           ();
        int                      getReturnCode          ();
        std::string              readAnsi               ();
        int                      readInteger            ();
        std::vector<std::string> readRemainingAnsiLines ();
        std::string              readRemainingAnsiText  ();
        std::vector<std::string> readRemainingUtfLines  ();
        std::vector<std::string> readRemainingLinesUtf  ();
        std::string              readRemainingUtfText   ();
        std::string              readUtf                ();
        bool                     success                () const;

    private:

        Connection *_connection;
        bool _success;
        std::size_t _position;
        std::vector<unsigned char> _content;

        ServerResponse() = default; ///< Конструктор по умолчанию (для тестов)

        void _write (const unsigned char *bytes, std::size_t size);
    };

    /// \brief Абстрактный клиентский сокет.
    ///
    class ClientSocket
    {
    public:
        std::string host { "localhost" }; ///< Адрес сервера в виде строки.
        short port { 6666 }; ///< Номер порта сервера.

        explicit ClientSocket (const std::string &host="localhost", short port=6666); ///< Конструктор
        ClientSocket (const ClientSocket &) = delete; ///< Конструктор копирования.
        ClientSocket (ClientSocket &&)      = delete; ///< Конструктор перемещения.
        ~ClientSocket(); ///< Деструктор.
        ClientSocket& operator = (const ClientSocket &) = delete; ///< Оператор коприрования.
        ClientSocket& operator = (ClientSocket &&)      = delete; ///< Оператор перемещения.

        /// \brief Открытие сокета.
        void open();

        /// \brief Закрытие сокета.
        void close();

        /// \brief Отсылка данных на сервер.
        /// \param buffer Указатель на буфер с данными.
        /// \param size Размер данных в байтах.
        void send (const unsigned char *buffer, std::size_t size);

        /// \brief Получение отвера от сервера (частями).
        /// \param buffer Буфер для размещения полученного ответа.
        /// \param size Размер буфера в байтах.
        /// \return Количество полученных данных в байтах.
        /// Отрицательное число означает ошибку.
        /// 0 означает, что сервер закончил передачу данных.
        /// Положительное число означает, что приём продолжается,
        /// и вызов нужно будет повторить для получения следующей порции.
        std::size_t receive (unsigned char *buffer, std::size_t size);

    private:

        std::unique_ptr<ClientSocketImpl> _impl;
    };

    //================================================================

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

    /// \brief Этапы выполнения запроса к серверу.
    enum class RequestStage
    {
        None,           ///< В состоянии простоя.
        BuildPackage,   ///< Формирование пакета запроса.
        NameResolution, ///< Разрешение имени сервера.
        Connecting,     ///< Установка подключения к серверу.
        Sending,        ///< Передача пакета запроса на сервер.
        Waiting,        ///< Ожидание ответа от сервера.
        Receiving,      ///< Получение ответного пакета от сервера.
        Shutdown,       ///< Отключение от сервера.
        ParseHeader,    ///< Разбор заголовка ответа.
        ParseBody,      ///< Разбор тела ответа (если есть).
        Error           ///< Ошибка.
    };

    /// \brief Информация о базе данных ИРБИС.
    class DatabaseInfo final
    {
        public:
        std::string name;                 ///< Имя базы данных.
        std::string description;          ///< Описание базы данных в произвольной форме (может быть пустым).
        MfnList logicallyDeletedRecords;  ///< Список логически удалённых записей (может быть пустым).
        MfnList physicallyDeletedRecords; ///< Список физически удалённых записей (может быть пустым).
        MfnList nonActualizedRecords;     ///< Список неактуализированных записей (может быть пустым).
        MfnList lockedRecords;            ///< Список заблокированных записей (может быть пустым).
        int maxMfn { 0 };                 ///< Максимальный MFN для базы данных.
        bool databaseLocked { false };    ///< Признак блокировки базы данных в целом.
        bool readOnly { false };          ///< База данных доступна только для чтения.

        void        parse   (ServerResponse &response);
        std::string toString()                          const;

        // static std::vector<DatabaseInfo> parse (const MenuFile &menu);
    };

    /// \brief Простая абстракция кодировки символов.
    class Encoding // abstract
    {
        static Encoding *_ansi;
        static Encoding *_utf;

        public:

        Encoding() = default;
        virtual ~Encoding() = default;

        virtual Bytes        fromUnicode   (const std::wstring &text)             const = 0; ///< Преобразование в UNICODE.
        virtual std::size_t  getSize       (const std::wstring &text)             const = 0; ///< Подсчет размера текста в данной кодировке.
        virtual std::wstring toUnicode     (const Byte *bytes, std::size_t count) const = 0; ///< Преобразование из UNICODE.

        static Encoding*   ansi     ();
        static std::string fromAnsi (const Byte *bytes, std::size_t count);
        static std::string fromUtf  (const Byte *bytes, std::size_t count);
        static Bytes       toAnsi   (const std::string &text);
        static Bytes       toUtf    (const std::string &text);
        static Encoding*   utf      ();
    };

    /// \brief Информация о версии ИРБИС-сервера.
    class Version final
    {
        public:

        std::string organization;      ///< На какое юридическое лицо приобретен сервер.
        std::string version;           ///< Собственно версия сервера. Например, 64.2008.1.
        int    maxClients       { 0 }; ///< Максимальное количество одновременных подключений.
        int    connectedClients { 0 }; ///< Текущее количество подключений.

        Version             ()                = default; ///< Конструктор по умолчанию.
        Version             (const Version &) = default; ///< Конструктор копирования.
        Version             (Version &&)      = default; ///< Конструктор перемещения.
        ~Version            ()                = default; ///< Деструктор.
        Version& operator = (const Version &) = default; ///< Оператор копирования.
        Version& operator = (Version &&)      = default; ///< Оператор перемещения.

        void parse (ServerResponse &response);
        std::string toString() const;
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
        IniFile      iniFile;                  ///< Содержимое серверного INI-файла для данного клиента.
        int          interval;                 ///< Интервал автоматического подтверждения, секунды.
        RequestStage stage;                    ///< Этап выполнения запроса
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

        //bool                     deleteRecord (int mfn);
        MarcRecord               readRecord   (int mfn);
        //MarcRecord               readRecord   (const std::string &databaseName, int mfn);
        //MarcRecord               readRecord   (const std::string &databaseName, int mfn, int version);
        //std::vector <MarcRecord> readRecords  (const std::vector<int> &mfnList);
        //int                      writeRecord  (MarcRecord &record, bool lockFlag = false, bool actualize = true, bool dontParseResponse = false);


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
        Version                   getServerVersion ();
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
