// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <random>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Получение списка баз данных.
/// \param ini
/// \param defaultFileName
/// \return
std::vector<DatabaseInfo> ConnectionContext::listDatabases (const IniFile &ini, const String &defaultFileName)
{
    std::vector<DatabaseInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

/// \brief Получение списка баз данных.
/// \param specification
/// \return
std::vector<DatabaseInfo> ConnectionContext::listDatabases (const FileSpecification &specification)
{
    std::vector<DatabaseInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto menu = this->readMenuFile (specification);
    if (menu.entries.empty()) {
        return result;
    }

    result = DatabaseInfo::parse(menu);

    return result;
}

/// \brief Получение списка файлов на сервере.
/// \param specification Спецификация файла.
/// \return Список файлов.
StringList ConnectionContext::listFiles (const FileSpecification &specification)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "!");
    query.add (specification);

    ServerResponse response (*this, query);
    const auto lines = response.readRemainingAnsiLines();
    for (const auto &line : lines) {
        auto converted = Text::fromFullDelimiter (line);
        for (const auto &item : converted) {
            if (!item.empty()) {
                result.push_back (item);
            }
        }
    }

    return result;
}

/// \brief Получение списка файлов на сервере.
/// \param specifications Вектор спецификаций файлов.
/// \return Список файлов.
StringList ConnectionContext::listFiles (const std::vector<FileSpecification> &specifications)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query(*this, "!");
    for (const auto &specification : specifications) {
        query.add (specification);
    }

    ServerResponse response(*this, query);
    const auto lines = response.readRemainingAnsiLines();
    for (const auto &line : lines) {
        auto converted = Text::fromFullDelimiter (line);
        for (const auto &item : converted) {
            if (!item.empty()) {
                result.push_back (item);
            }
        }
    }

    return result;
}

/// \brief Чтение двоичного файла с сервера.
/// \param specification
/// \return
Bytes ConnectionContext::readBinaryFile (const FileSpecification &specification)
{
    Bytes result;

    if (!this->_checkConnection()) {
        return result;
    }

    return result;
}

/// \brief Чтение INI-файла с сервера.
/// \param specification Спецификация файла.
/// \return Прочитанный INI-файл.
IniFile ConnectionContext::readIniFile (const FileSpecification &specification)
{
    const auto lines = this->readTextLines (specification);
    IniFile result;
    result.parse (lines);

    return result;
}

/// \brief Чтение MNU-файла с сервера.
/// \param specification
/// \return
MenuFile ConnectionContext::readMenuFile (const FileSpecification &specification)
{
    const auto lines = this->readTextLines (specification);
    MenuFile result;
    result.parse(lines);

    return result;
}

/// \brief Чтение текстового файла с сервера.
/// \param specification Спецификация файла.
/// \return Текст файла (пустой, если чтение не удалось).
String ConnectionContext::readTextFile (const FileSpecification &specification)
{
    String result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    query.add (specification);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    result = response.readAnsi();
    result = Text::fromIrbisToDos (result);

    return result;
}

/// \brief Чтение файла с сервера в кодировке ANSI.
/// \param specification Спецификация файла.
/// \return Содержимое файла (пустое, если чтение не удалось).
std::string ConnectionContext::readAnsiFile (const FileSpecification &specification)
{
    std::string result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    query.add (specification);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    result = response.getLine();
    result = Text::fromIrbisToUnix (result);

    return result;
}

/// \brief Чтение нескольких текстовых файлов с сервера.
/// \param specifications
/// \return
StringList ConnectionContext::readTextFiles (std::vector<FileSpecification> &specifications)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    for (const auto &specification : specifications) {
        query.add (specification).newLine();
    }

    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    const auto lines = response.readRemainingAnsiLines();
    for (String line : lines) {
        result.push_back (std::move (Text::fromIrbisToDos (line)));
    }

    return result;
}

/// \brief Чтение файла с сервера в виде вектора строк.
/// \param specification Спецификация файла.
/// \return Вектор строк (пустой, если чтение не удалось).
StringList ConnectionContext::readTextLines (const FileSpecification &specification)
{
    StringList result;
    if (!this->_checkConnection()) {
        return result;
    }

    ClientQuery query (*this, "L");
    query.add (specification);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }

    const auto text = response.readAnsi();
    result = Text::fromFullDelimiter (text);
    return result;
}

}
