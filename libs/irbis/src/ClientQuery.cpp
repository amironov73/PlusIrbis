// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <iostream>
#include <iomanip>

namespace irbis {

/// \brief Конструктор клиентского запроса.
/// \param connection Подключение, для которого формируется запрос.
/// \param commandCode Код команды.
ClientQuery::ClientQuery(const Connection &connection, const std::string &commandCode)
{
    this->addAnsi(commandCode).newLine();
    this->addAnsi(connection.workstation).newLine();
    this->addAnsi(commandCode).newLine();
    this->add(connection.clientId).newLine();
    this->add(connection.queryId).newLine();
    this->addAnsi(connection.password).newLine();
    this->addAnsi(connection.username).newLine();
    this->newLine();
    this->newLine();
    this->newLine();
}

/// \brief Добавление информации к запросу.
/// \param bytes Массив с информацией.
/// \param size Количество байт.
void ClientQuery::_write(const Byte *bytes, size_t size)
{
    for (size_t i = 0; i < size; i++)     {
        this->_content.push_back(bytes[i]);
    }
}

/// \brief Добавление байта к запросу.
/// \param byte Добавляемый байт.
void ClientQuery::_write(Byte byte)
{
    _content.push_back(byte);
}

/// \brief Добавление целого числа к запросу.
/// \param value Добавляемое значение.
/// \return this.
ClientQuery& ClientQuery::add(int value)
{
    const auto s = std::to_string(value);
    return this->addAnsi(s);
}

/// \brief Добавление файловой спецификации к запросу.
/// \param specification Спецификация файла.
/// \return this.
ClientQuery& ClientQuery::add(const FileSpecification &specification)
{
    return this->addAnsi(specification.toString());
}

/// \brief Добавление записи к запросу.
/// \param record Добавляемая запись.
/// \param delimiter Разделитель элементов записи.
/// \return `this`.
ClientQuery& ClientQuery::add(const MarcRecord &record, const String &delimiter=L"\u001E")
{
    return this->addUtf(record.encode(delimiter));
}

/// \brief Добавление строки в кодировке ANSI.
/// \param text Добавляемый текст.
/// \return `this`.
/// \warning Предполагается, что переданный текст в кодировке ANSI!
ClientQuery& ClientQuery::addAnsi(const std::string &text)
{
    const size_t size = text.length();
    if (!size)
    {
        return *this;
    }

    const Byte *ptr = reinterpret_cast<const Byte*>(text.c_str());
    this->_write(ptr, size);
    return *this;
}

/// \brief Добавление строки в кодировке ANSI.
/// \param text Добавляемый текст.
/// \return `this`.
ClientQuery& ClientQuery::addAnsi(const String &text)
{
    const auto size = text.length();
    if (!size) {
        return *this;
    }

    const auto *src = text.c_str();
    Bytes dst(size);
    unicode_to_cp1251(dst.data(), src, size);
    this->_write(dst.data(), size);
    return *this;
}

/// \brief Добавление формата к запросу.
/// \param format Спецификация формата.
/// \return Был ли добавлен формат?
bool ClientQuery::addFormat(const String &format)
{
    if (format.empty()) {
        this->newLine();
        return false;
    }

    const auto prepared = prepareFormat(trimStart(format));

    if (format[0] == '@') {
        this->addAnsi(format);
    } else if (format[0] == '!') {
        this->addUtf(prepared);
    } else {
        this->addUtf(String (L"!") + prepared);
    }

    this->newLine();
    return true;
}

/// \brief Добавление строки в формате UTF-8.
/// \param text Добавляемый текст.
/// \return this.
ClientQuery& ClientQuery::addUtf(const String &text)
{
    const auto size = text.length();
    if (!size) {
        return *this;
    }

    const auto src = text.c_str();
    const auto bufSize = countUtf(src, size);
    Bytes dst(bufSize);
    toUtf(dst.data(), src, size);
    this->_write(dst.data(), bufSize);
    return *this;
}

/// \brief Дамп запроса.
/// \param stream Поток, в который выводится дамп.
void ClientQuery::dump(std::ostream &stream) const
{
    for (const auto value : this->_content) {
        stream << std::hex << std::setw(2) << value << " ";
    }
}

/// \brief Кодирование запроса.
/// \return Закодированный запрос.
Bytes ClientQuery::encode() const
{
    Bytes result;
    result.reserve(this->_content.size() + 10);
    const auto prefix = std::to_string(this->_content.size());
    const auto ptr = prefix.c_str();
    for (size_t i = 0; i < prefix.length(); i++) {
        result.push_back(static_cast<Byte>(ptr[i]));
    }
    result.push_back(0x0a);
    result.insert(result.end(), this->_content.begin(), this->_content.end());

    return result;
}

/// \brief Добавление перевода строки.
/// \return `this`.
ClientQuery& ClientQuery::newLine()
{
    this->_write(0x0A);
    return *this;
}

}
