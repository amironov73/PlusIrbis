// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef IRBISINTERNAL_H
#define IRBISINTERNAL_H

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
    class Connection;
    class FileSpecification;
    class MarcRecord;
    class ServerResponse;

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

    bool sameChar(char first, char second) noexcept;
    bool sameString(const std::string &first, const std::string &second) noexcept;
    char firstChar(const std::string &text) noexcept;
    std::string toLower(std::string &text) noexcept;
    std::string toUpper(std::string &text) noexcept;
    bool contains(const std::string &text, char c);
    bool contains(const std::string &text, const std::string &fragment);
    std::string replace(const std::string &text, const std::string &from, const std::string &to);
    std::string trimStart(const std::string &text);
    std::string trimEnd(const std::string &text);
    std::string trim(const std::string &text);
    int fastParse32(const std::string &text) noexcept;
    int fastParse32(const char *text) noexcept;
    int fastParse32(const char *text, std::size_t length) noexcept;
    unsigned int fastParseUnsigned32(const std::string &text) noexcept;
    unsigned int fastParseUnsigned32(const char *text) noexcept;
    unsigned int fastParseUnsigned32(const char *text, std::size_t length) noexcept;
    std::vector<std::string> split(const std::string &text, char delimiter);
    std::vector<std::string> split(const std::string &text, const std::string &delimiter);
    std::vector<std::string> maxSplit(const std::string &text, char separator, int count);

    template<typename T>
    bool isDigit(T c) { return (c >= '0') && (c <= '9'); }

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

}

#endif // IRBISINTERNAL_H
