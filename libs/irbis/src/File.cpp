// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

/*!
    \class irbis::File

    Объекты данного типа не перемещаются и не копируются!
 */

namespace irbis {

/// \brief Конструктор.
/// \param stream Уже открытый файл.
File::File(FILE *stream)
    : _stream(stream)
{
}

/// \brief Конструктор.
/// \param name Имя файла.
/// \param mode Режим доступа.
File::File(const char *name, const char *mode)
    : _stream(nullptr)
{
    this->_stream = ::fopen(name, mode);
    if (!this->_stream) {
        // TODO более информативное сообщение
        throw IrbisException();
    }
}

/// \brief Конструктор.
/// \param name Имя файла.
/// \param mode Режим доступа.
File::File(const std::string &name, const std::string &mode)
    : _stream(nullptr)
{
    this->_stream = ::fopen(name.c_str(), mode.c_str());
    if (!this->_stream) {
        // TODO более информативное сообщение
        throw IrbisException();
    }
}

/// \brief Конструктор.
/// \param name Имя файла.
/// \param mode Режим доступа.
File::File(const String &name, const String &mode)
    : _stream(nullptr)
{
    const auto ansiName = unicode_to_cp1251(name);
    const auto ansiMode = unicode_to_cp1251(mode);
    this->_stream = ::fopen(ansiName.c_str(), ansiMode.c_str());
    if (!this->_stream) {
        // TODO более информативное сообщение
        throw IrbisException();
    }
}

File::~File()
{
    if (this->_stream) {
        ::fclose(this->_stream);
        this->_stream = nullptr;
    }
}

/// \brief Чтение из файла.
/// \param buffer Буфер для данных.
/// \param count Количество считываемых байт.
/// \return Количество прочитанный байт.
std::size_t File::read(Byte *buffer, std::size_t count)
{
    return ::fread(buffer, 1, count, this->_stream);
}

/// \brief Получение текущей позиции в файле.
/// \return Позиция в файле.
int64_t File::tell()
{
    return ::ftell(this->_stream);
}

/// \brief Установка новой позиции в файле.
/// \param position Смещение в байтах от начала файла.
/// \return Смещение.
int64_t File::seek(int64_t position)
{
    // TODO handle truncation
    const auto truncated = static_cast<long>(position);
    return ::fseek(this->_stream, truncated, SEEK_SET);
}

/// \brief Запись в файл.
/// \param buffer Буфер с данными.
/// \param count Количество байт, подлежащих записи.
/// \return Количество реально записанных байт.
std::size_t File::write(const Byte *buffer, std::size_t count)
{
    return ::fwrite(buffer, 1, count, this->_stream);
}

}

