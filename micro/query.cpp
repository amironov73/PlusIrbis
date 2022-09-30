// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "internal.h"
#include "micro.h"

#include <iostream>
#include <iomanip>

namespace irbis
{
    /// \brief Конструктор клиентского запроса.
    /// \param connection Подключение, для которого формируется запрос.
    /// \param commandCode Код команды.
    ClientQuery::ClientQuery
        (
            const Connection &connection,
            const std::string &commandCode
        )
    {
        this->addAnsi (commandCode)             .newLine();
        this->addAnsi (connection.workstation)  .newLine();
        this->addAnsi (commandCode)             .newLine();
        this->add     (connection.clientId)    .newLine();
        this->add     (connection.queryId)     .newLine();
        this->addAnsi (connection.password)     .newLine();
        this->addAnsi (connection.username)     .newLine();
        this->                                       newLine();
        this->                                       newLine();
        this->                                       newLine();
    }

    /// \brief Добавление информации к запросу.
    /// \param bytes Массив с информацией.
    /// \param size Количество байт.
    void ClientQuery::_write (const unsigned char *bytes, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i++) {
            this->_content.push_back (bytes[i]);
        }
    }

    /// \brief Добавление байта к запросу.
    /// \param byte Добавляемый байт.
    void ClientQuery::_write (unsigned char byte)
    {
        _content.push_back (byte);
    }

    /// \brief Добавление целого числа к запросу.
    /// \param value Добавляемое значение.
    /// \return this.
    ClientQuery& ClientQuery::add (int value)
    {
        const auto s = std::to_string (value);
        return this->addAnsi (s);
    }

    /// \brief Добавление файловой спецификации к запросу.
    /// \param specification Спецификация файла.
    /// \return this.
    ClientQuery& ClientQuery::add (const FileSpecification &specification)
    {
        // return this->addAnsi (specification.toString());
        return *this;
    }

    /// \brief Добавление строки в кодировке ANSI.
    /// \param text Добавляемый текст.
    /// \return `this`.
    /// \warning Предполагается, что переданный текст в кодировке ANSI!
    ClientQuery& ClientQuery::addAnsi (const std::string &text)
    {
        const std::size_t size = text.length();
        if (!size) {
            return *this;
        }

        const auto *ptr = reinterpret_cast<const unsigned char*> (text.c_str());
        this->_write (ptr, size);
        return *this;
    }

    /// \brief Добавление строки в формате UTF-8.
    /// \param text Добавляемый текст.
    /// \return this.
    ClientQuery& ClientQuery::addUtf (const std::string &text)
    {
        return *this;

//        const auto size = text.length();
//        if (!size) {
//            return *this;
//        }
//
//        const auto src = text.c_str();
//        const auto bufSize = countUtf (src, size);
//        Bytes dst (bufSize);
//        toUtf (dst.data(), src, size);
//        this->_write (dst.data(), bufSize);
//        return *this;
    }

    /// \brief Дамп запроса.
    /// \param stream Поток, в который выводится дамп.
    void ClientQuery::dump (std::ostream &stream) const
    {
        for (const auto value : this->_content) {
            // stream << std::hex << std::setw(2) << value << " ";
            stream << value;
        }

        stream.flush();
    }

    /// \brief Кодирование запроса.
    /// \return Закодированный запрос.
    std::vector<unsigned char> ClientQuery::encode() const
    {
        std::vector<unsigned char> result;
        result.reserve (this->_content.size() + 10);
        const auto prefix = std::to_string (this->_content.size());
        const auto ptr = prefix.c_str();
        for (std::size_t i = 0; i < prefix.length(); i++) {
            result.push_back (static_cast<unsigned char>(ptr[i]));
        }
        result.push_back (0x0a);
        result.insert (result.end(), this->_content.begin(), this->_content.end());

        return result;
    }

    /// \brief Добавление перевода строки.
    /// \return `this`.
    ClientQuery& ClientQuery::newLine()
    {
        this->_write (0x0A);
        return *this;
    }

}
