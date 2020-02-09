// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cstdarg>

namespace irbis {

/// \brief Конструктор.
/// \param connection Подключение.
/// \param query Полностью сформированный клиентский запрос.
///
/// Вычитывает ответ сервера до конца и сохраняет его во внутреннем буфере.
ServerResponse::ServerResponse (Connection &connection, ClientQuery &query)
{
    std::lock_guard<std::mutex> guard (connection._mutex);

    this->_connection = &connection;
    this->returnCode  = 0;
    this->_success    = false;
    this->_position   = 0;

    // Read the response
    auto &socket = *connection.socket;
    socket.host  = connection.host;
    socket.port  = connection.port;
    socket.open();

    auto encoded    = query.encode();
    const auto data = encoded.data();
    const auto size = encoded.size();
    socket.send (data, size);

    Byte buffer[2048];
    while(true) {
        const auto received = socket.receive(buffer, sizeof(buffer));
        if (received <= 0) {
            break;
        }
        _write(buffer, received);
    }
    socket.close();

    // decode the response
    this->command       = this->readAnsi();
    this->clientId      = this->readInteger();
    this->queryId       = this->readInteger();
    this->answerSize    = this->readInteger();
    this->serverVersion = this->readAnsi();
    this->readAnsi();
    this->readAnsi();
    this->readAnsi();
    this->readAnsi();
    this->readAnsi();

    // set the success flag
    this->_success = true;
}

/// \brief Проверка кода возврата последней операции, выполненной сервером.
/// \return Результат проверки: true означает успешное завершение операции.
bool ServerResponse::checkReturnCode()
{
    return this->checkReturnCode(0);
}

/// \brief Проверка, достигнут ли конец ответа сервера.
/// \return true, если достигнут конец, иначе false.
bool ServerResponse::eot() const
{
    return this->_position >= this->_content.size();
}

/// \brief Проверка кода возврата последней операции, выполненной сервером.
/// \param nargs Количество разрешённых кодов. Далее идут разрешённые коды (отрицательные числа, не означающие сбой при выполнении операции).
/// \return Результат проверки: true означает успешное завершение операии.
///
/// В любом случае у соединения выставляется значение поля lastError.
bool ServerResponse::checkReturnCode(int nargs, ...)
{
    this->_connection->lastError = 0;
    if (!this->_success) {
        this->_connection->lastError = -100002;
        return false;
    }

    va_list args;
    auto result = true;

    if (this->getReturnCode() < 0) {
        va_start(args, nargs);
        result = false;
        for(auto i = 0; i < nargs; i++) {
            const int code = va_arg(args, int);
            if (code == this->returnCode) {
                result = true;
                break;
            }
        }
        va_end(args);
        this->_connection->lastError = this->returnCode;
    }

    return result;
}

/// \brief Чтение строки без преобразования кодировок.
/// \return Прочитанная строка. Если достигнут конец ответа сервера, строка будет пустая.
std::string ServerResponse::getLine()
{
    std::string result;
    const auto size = this->_content.size();
    while (this->_position < size) {
        auto c = this->_content.at(_position);
        this->_position++;

        if (c == 13) {
            if (this->_position < size) {
                c = this->_content.at(_position);
                if (c == 10) {
                    this->_position++;
                }
            }
            break;
        }

        result.push_back(c);
    }

    return result;
}

/// \brief Чтение оставшейся части ответа сервера без преобразования кодировок.
/// \return Прочитанный ответ сервера. Если достигнут конец, строка будет пустая.
std::string ServerResponse::getRemaining()
{
    std::string result;
    const auto size = this->_content.size();
    if (this->_position < size) {
        const auto data = reinterpret_cast<const char*>(_content.data() + this->_position);
        std::size_t remaining = size - this->_position;
        // Убираем переводы строки в конце.
        while (remaining > 0) {
            const auto c = data[remaining-1];
            if (c != '\r' && c != '\n') {
                break;
            }
            remaining--;
        }
        const std::string s2(data, remaining);
        result = s2;
        this->_position = size;
    }

    return result;
}

/// \brief Получение кода возврата.
/// \return Полученный от сервера код возврата. Отрицательное значение свидетельствует о проблеме (но не всегда).
///
/// Получение кода возврата. Вызывается один раз в свой час и только тогда.
int ServerResponse::getReturnCode()
{
    this->returnCode = this->readInteger();
    return this->returnCode;
}

/// \brief Чтение строки в кодировке ANSI.
/// \return Полученная строка. Если достигнут конец ответа, строка будет пустой.
std::wstring ServerResponse::readAnsi()
{
    const auto line = this->getLine();
    return cp1251_to_unicode(line);
}

/// \brief Чтение целого числа.
/// \return Прочитанное число.
///
/// Предполагается, что число занимает всю строку.
int ServerResponse::readInteger()
{
    const auto line = this->getLine();
    return std::stoi(line);
}

/// \brief Чтение оставшихся строк в кодировке ANSI.
/// \return Вектор прочитанных строк.
StringList ServerResponse::readRemainingAnsiLines()
{
    const auto text = this->readRemainingAnsiText();
    return split (text, '\n');
}

/// \brief Чтение оставшегося текста в кодировке ANSI.
/// \return Прочитанный текст.
std::wstring ServerResponse::readRemainingAnsiText()
{
    const auto line = this->getRemaining();
    return cp1251_to_unicode (line);
}

/// \brief Чтение оставшихся строк в кодировке UTF-8.
/// \return Вектор прочитанных строк.
StringList ServerResponse::readRemainingUtfLines()
{
    StringList result;
    while (!this->eot()) {
        const auto line = readUtf();
        result.push_back (line);
    }
    return result;
}

/// \brief Чтение оставшихся строк в кодировке UTF-8.
/// \return Вектор прочитанных строк.
std::vector<std::string> ServerResponse::readRemainingLinesUtf()
{
    std::vector<std::string> result;
    while (!this->eot()) {
        const auto line = getLine();
        result.push_back (line);
    }
    return result;
}

/// \brief Чтение оставшегося текста в кодировке UTF-8.
/// \return Прочитанный текст.
String ServerResponse::readRemainingUtfText()
{
    const auto line = getRemaining();
    return fromUtf(line);
}

/// \brief Чтение строки в кодировке UTF-8.
/// \return Полученная строка. Если достигнут конец ответа, строка будет пустой.
String ServerResponse::readUtf()
{
    const auto line = this->getLine();
    return fromUtf(line);
}

/// \brief Сетевой обмен с сервером завершился успешно?
/// \return Признак успешности.
bool ServerResponse::success() const
{
    return this->_success;
}

void ServerResponse::_write(const Byte *bytes, std::size_t size)
{
    const auto newSize = this->_content.size() + size;
    if (this->_content.capacity() < newSize) {
        this->_content.reserve(newSize);
    }

    for (std::size_t i = 0; i < size; i++) {
        this->_content.push_back(bytes[i]);
    }
}

}
