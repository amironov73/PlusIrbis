// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

/// \brief Маркер конца меню.
const String MenuFile::StopMarker = L"*****";

/// \brief Разделители для кода меню.
const Chars MenuFile::Separators = { ' ', '-', '=', ':' };

//=========================================================

/// \brief Конверсия в текстовое представление.
/// \return Текстовое представление.
String MenuEntry::toString() const
{
    if (comment.empty()) {
        return code;
    }

    return code + L" - " + comment;
}

//=========================================================

/// \brief Откусывание от кода фрагмента, начинающегося с разделителя.
/// \param code Код.
/// \return Очищенный код.
static String trimCode (const String &code)
{
    for (std::size_t i = 0; i < code.size(); ++i) {
        const auto c = code[i];
        for (const auto separator : MenuFile::Separators) {
            if (c == separator) {
                return code.substr (0, i);
            }
        }
    }

    return code;
}

/// \brief Добавление пары "код-комментарий" в конец меню.
/// \param code Код.
/// \param comment Комментарий.
/// \return this.
MenuFile& MenuFile::add (const String &code, const String &comment)
{
    this->entries.emplace_back (code, comment);
    return *this;
}

/// \brief Добавление пары "код-комментарий" в конец меню.
/// \param code Код.
/// \param comment Комментарий.
/// \return this.
MenuFile& MenuFile::add (String &&code, String &&comment)
{
    this->entries.emplace_back (std::move (code), std::move (comment));
    return *this;
}

/// \brief Поиск пары с указанным кодом без учета регистра символов.
/// \param code Искомый код.
/// \return Найденная пара либо nullptr.
MenuEntry* MenuFile::getEntry (const String &code) const
{
    for(const auto &entry : this->entries) {
        if (sameString (entry.code, code)) {
            return const_cast<MenuEntry*> (&entry);
        }
    }

    auto trimmed = trim (code);
    for(const auto &entry : this->entries) {
        if (sameString (trim (entry.code), trimmed)) {
            return const_cast<MenuEntry*> (&entry);
        }
    }

    trimmed = trimCode (trimmed);
    for(const auto &entry : this->entries) {
        if (sameString (trimCode (entry.code), trimmed)) {
            return const_cast<MenuEntry*> (&entry);
        }
    }

    return nullptr;
}

/// \brief Поиск пары с указанным кодом с учетом регистра символов.
/// \param code Искомый код.
/// \return Найденная пара либо nullptr.
MenuEntry* MenuFile::getEntrySensitive (const String &code) const
{
    for(const auto &entry : this->entries) {
        if (entry.code == code) {
            return const_cast<MenuEntry*> (&entry);
        }
    }

    auto trimmed = trim (code);
    for(const auto &entry : this->entries) {
        if (trim (entry.code) == trimmed) {
            return const_cast<MenuEntry*> (&entry);
        }
    }

    trimmed = trimCode (code);
    for(const auto &entry : this->entries) {
        if (trimCode (entry.code) == trimmed) {
            return const_cast<MenuEntry*> (&entry);
        }
    }

    return nullptr;
}

/// \brief Получение значения для указанного кода без учета регистра символов.
/// \param code Искомый код.
/// \return Найденное значение.
String MenuFile::getValue (const String &code) const
{
    const auto entry = this->getEntry (code);
    auto result = entry ? entry->comment : String();
    return result;
}

/// \brief Получение значения для указанного кода с учетом регистра символов.
/// \param code Искомый код.
/// \return Найденное значение.
String MenuFile::getValueSensitive (const String &code) const
{
    const auto entry = this->getEntrySensitive (code);
    auto result = entry ? entry->comment : String();
    return result;
}

/// \brief Получение значения для указанного кода без учета регистра символов.
/// \param code Искомый код.
/// \param defaultValue Значение по умолчанию.
/// \return Найденное значение.
String MenuFile::getValue (const String &code, const String &defaultValue) const
{
    const auto value = this->getValue (code);
    return value.empty() ? defaultValue : value;
}

/// \brief Получение значения для указанного кода с учетом регистра символов.
/// \param code Искомый код.
/// \param defaultValue Значение по умолчанию.
/// \return Найденное значение.
String MenuFile::getValueSensitive (const String &code, const String &defaultValue) const
{
    const auto value = this->getValueSensitive  (code);
    return value.empty() ? defaultValue : value;
}

/// \brief Разбор текстового представления.
/// \param Строки текста.
void MenuFile::parse (const StringList &lines)
{
    auto ptr = std::begin (lines);
    const auto end = std::end (lines);
    while (ptr != end) {
        const auto &code = *ptr;
        if (code.empty()) {
            break;
        }
        if (code == StopMarker) {
           break;
        }
        ++ptr;
        if (ptr != end) {
            const auto &comment = *ptr;
            if (comment == StopMarker) {
                break;
            }
            this->add (code, comment);
            ++ptr;
        }
    }
}

/// \brief Чтение меню из текстового файла.
/// \param filename Имя файла.
void MenuFile::parseLocalFile (const String &filename)
{
    const auto lines = Text::readAnsiLines (filename);
    this->parse (lines);
}

}
