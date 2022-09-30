// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"
#include "micro.h"

#include <iostream>
#include <iomanip>
#include <random>

namespace irbis
{
    /// \brief Единственный конструктор для данного класса.
    Connection::Connection()
        : _connected    { false },
          host          { "127.0.0.1" },
          port          { 6666 },
          username      { },
          password      { },
          database      { "IBIS" },
          workstation   { "C" },
          clientId      { 0 },
          queryId       { 0 },
          lastError     { 0 },
          serverVersion { },
          interval      { 600 }//,
          //stage         { RequestStage::None },
          //socket        { new Tcp4Socket }
    {
        // пустое тело конструктора
    }

    /// \brief Деструктор.
    ///
    /// \details Если необходимо, выполняет отключение от сервера.
    Connection::~Connection()
    {
        this->disconnect();
        // this->socket.release();
    }

    /// \brief Проверка, подключены ли мы к серверу.
    /// \return `false` если не подключены.
    ///
    /// Большинство операций нет смысла начинать,
    /// если не установлено подключение к серверу.
    bool Connection::_checkConnection()
    {
        if (!this->connected()) {
            this->lastError = -100003;
            return false;
        }
        // this->lastError = 0;
        return true;
    }

    /// \brief Подключение к серверу.
    /// \return Признак успешности выполнения операции.
    /// \warning Подключение некоторыми типами клиентов увеличивает на сервере счетчик использованных лицензий!
    ///
    /// Повторные попытки подключения с помощью того же объекта Connection игнорируются.
    /// При этом возвращается true.
    bool Connection::connect()
    {
        if (this->connected()) {
            return true;
        }

        this->lastError = 0;

        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen (rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis (100000, 900000);

        AGAIN: this->clientId = dis (gen);
        this->queryId = 1;
        ClientQuery query (*this, "A");
        query.addAnsi (this->username).newLine()
            .addAnsi (this->password);

        try
        {
            ServerResponse response (*this, query);
            if (!response.success()) {
                return false;
            }

            response.getReturnCode();
            if (response.returnCode == -3337) {
                // клиент с данным идентификатором уже зарегистрирован
                goto AGAIN;
            }

            if (response.returnCode < 0) {
                return false;
            }

            this->_connected = true;
            this->serverVersion = response.serverVersion;
            this->interval = response.readInteger();
            const auto lines = response.readRemainingAnsiLines();
            //this->iniFile.parse(lines);
        }
        catch (...) {
            this->lastError = -100002;
            return false;
        }

        return true;
    }

    /// \brief Отключение от сервера.
    ///
    /// Повторные попытки отключения уже отключенного экземпляра клиента игнорируются.
    /// Если при подключении был увеличен счётчик использованных лицензий, он соответственно уменьшается.
    void Connection::disconnect()
    {
        if (!this->connected()) {
            return;
        }

        ClientQuery query (*this, "B");
        query.addAnsi (this->username).newLine();
        this->execute (query);
        this->_connected = false;
    }

    /// \brief Результат исполнения запроса на сервере.
    /// \param query Полностью сформированный клиентский запрос.
    /// \return Признак успешного выполнения запроса.
    /// \warning Должно быть установлено подключение к серверу!
    ///
    /// Если метод вернул false, конкретный код ошибки находится в lastError.
    bool Connection::execute (ClientQuery &query)
    {
        if (!this->_checkConnection()) {
            return false;
        }

        bool result = false;

        try {
            ServerResponse response (*this, query);
            result = response.checkReturnCode();
        }
        catch (...) {
            // Do nothing
        }

        return result;
    }

    /// \brief Получение максимального MFN для базы данных с указанным именем.
    /// \param databaseName Имя базы данных.
    /// \return Максимальный MFN. 0 или отрицательное число означают ошибку.
    int Connection::getMaxMfn (const std::string &databaseName)
    {
        if (!this->_checkConnection()) {
            return 0;
        }

        ClientQuery query (*this, "O");
        query.addAnsi (databaseName);
        ServerResponse response (*this, query);
        if (!response.checkReturnCode()) {
            return 0;
        }

        const auto result = response.returnCode;
        return result;
    }

    /// \brief Пустая операция (не требует от сервера никаких действий).
    /// \return Признак успешности выполнения операции.
    ///
    /// Используется для подтверждения подключения клиента
    /// при длительном бездействии пользователя.
    bool Connection::noOp() {
        if (!this->_checkConnection()) {
            return false;
        }

        ClientQuery query (*this, "N");
        auto result = this->execute (query);
        return result;
    }

    /// \brief Разбор строки подключения.
    /// \param connectionString Строка подключения.
    /// \throw IrbisException Ошибка в структуре строки подключения.
    void Connection::parseConnectionString (const std::string &connectionString)
    {
        const auto items = split (connectionString, ";");
        for (auto &item : items) {
            auto parts = maxSplit (item, '=', 2);
            if (parts.size() != 2) {
                throw std::exception();
            }
            auto name = toLower (parts[0]);
            const auto value = parts[1];
            if (name.empty() || value.empty()) {
                throw std::exception();
            }
            if (name == "host" || name == "server" || name == "address") {
                this->host = value;
            } else if (name == "port") {
                this->port = static_cast<short > (fastParse32 (value));
            } else if (name == "user" || name == "username"
                       || name == "name" || name == "login") {
                this->username = value;
            } else if (name == "pwd" || name == "password") {
                this->password = value;
            } else if (name == "db" || name == "catalog" || name == "database") {
                this->database = value;
            } else if (name == "arm" || name == "workstation") {
                this->workstation = value.at (0);
            } else {
                throw std::exception();
            }
        }
    }

    /// \brief Возврат к предыдущей базе данных.
    /// \return Имя базы данных, использовавшейся до возврата.
    std::string Connection::popDatabase()
    {
        auto result = this->database;
        this->database = _databaseStack.back();
        _databaseStack.pop_back();
        return result;
    }

    /// \brief Запоминание имени текущей базы данных для последующего возврата к ней.
    /// \param newDatabase Имя базы данных, устанавливаемой в качестве текущей.
    /// \return Имя базы данных, использовавшейся в качестве текущей.
    std::string Connection::pushDatabase (const std::string &newDatabase)
    {
        auto result = this->database;
        _databaseStack.push_back(newDatabase);
        this->database = newDatabase;
        return result;
    }

    /// \brief Получение актуальной строки подключения.
    /// \return Строка подключения
    std::string Connection::toConnectionString() const
    {
        // Нет смысла логировать
        return std::string("host=") + this->host
               + ";port="          + std::to_string (this->port)
               + ";username="      + this->username
               + ";password="      + this->password
               + ";database"       + this->database
               + ";arm="           + this->workstation
               + ";";
    }

}
