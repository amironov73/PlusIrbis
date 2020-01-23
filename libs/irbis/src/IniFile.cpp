// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <algorithm>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

/// \brief Модифицирована ли данная строка?
/// \return `true` если строка модифицирована.
///
/// Состояние "строка модифицирована" устанавливается
/// при вызове `setKey` и `setValue`.
/// \see IniLine::setKey(), IniLine::setValue()
bool IniLine::modified() const noexcept
{
    return this->_modified;
}

/// \brief Установка состояния строки "не модифицирована".
void IniLine::notModified() noexcept
{
    this->_modified = false;
}

/// \brief Смена ключа строки.
/// \param newKey Новый ключ строки.
///
/// Устанавливает состояние "строка модифицирована".
void IniLine::setKey(const String &newKey) noexcept
{
    this->key = newKey;
    this->_modified = true;
}

/// \brief Смена значения строки.
/// \param newValue Новое значение строки.
///
/// Устанавливает состояние "строка модифицирована".
void IniLine::setValue(const String &newValue) noexcept
{
    this->value = newValue;
    this->_modified = true;
}

/// \brief Формирует текстовое представление строки.
/// \return Ключ=значение.
String IniLine::toString() const
{
    return this->key + L"=" + this->value;
}

/// \brief Очищает секцию (удаляет все строки).
/// \return `this`.
IniSection& IniSection::clear()
{
    this->lines.clear();
    this->notModified();
    return *this;
}

/// \brief Содержит ли данная секция строку с указанным ключом?
/// \param key Искомый ключ.
/// \return `true` если содержит.
bool IniSection::containsKey(const String &key) const noexcept
{
    return std::any_of(begin(this->lines), end(this->lines),
            [&key] (const IniLine &x) { return sameString(x.key, key); });
}

/// \brief Получение индекса строки с указанным ключом.
/// \param key Искомый ключ.
/// \return Индекс строки, либо отрицательное значение,
/// если строка не найдена.
std::ptrdiff_t IniSection::getIndex(const String &key) const noexcept
{
    for (std::size_t i=0; i < this->lines.size(); i++) {
        if (sameString(this->lines[i].key, key)) {
            return i;
        }
    }

    return -1;
}

/// \brief Поиск строки с указанным ключом.
/// \param key Искомый ключ.
/// \return Указатель на найденную строку, либо `nullptr`.
IniLine* IniSection::getLine(const String &key) const noexcept
{
    for (const auto &line : this->lines) {
        if (sameString(line.key, key)) {
            return const_cast<IniLine*>(&line);
        }
    }

    return nullptr;
}

/// \brief Получение значения строки с указанным ключом.
/// \param key Искомый ключ.
/// \param defaultValue Значение, возвращаемое, если строка не найдена.
/// \return Найденное значение либо значение по умолчанию.
const String& IniSection::getValue(const String &key, const String &defaultValue) const noexcept
{
    const auto line = this->getLine(key);
    return line ? line->value : defaultValue;
}

/// \brief Есть ли модифицированные строки в данной секции?
/// \return `true`, если модифицированные строки есть.
bool IniSection::modified() const noexcept
{
    return std::any_of(begin(this->lines), end(this->lines),
            [] (const IniLine &x) { return x.modified(); });
}

/// \brief Пометка всех строк в данной секции как немодифицированных.
void IniSection::notModified()
{
    std::for_each(begin(this->lines), end(this->lines),
            [] (IniLine &x) { x.notModified(); });
}

/// \brief Удаление строки с указанным ключом.
/// \param key Искомый ключ.
/// \return `this`.
IniSection& IniSection::removeValue(const String &key)
{
    auto index = this->getIndex(key);
    if (index >= 0) {
        this->lines.erase(begin(this->lines) + index);
    }

    return *this;
}

/// \brief Установка значения строки с указанным ключом.
/// \param key Искомый ключ.
/// \param value Новое значение строки.
/// \return `this`.
IniSection& IniSection::setValue(const String &key, const String &value)
{
    IniLine *line = this->getLine(key);
    if (line) {
        line->setValue(value);
    } else {
        IniLine item;
        item.key = key;
        item.value = value;
        this->lines.push_back(item);
    }

    return *this;
}

/// \brief Получение текстового представления секции.
/// \return Текстовое представление.
String IniSection::toString() const
{
    String result;
    if (!this->name.empty()) {
        result = String(L"[") + this->name + String (L"]\n");
    }

    for(const auto &line : lines) {
        result.append(line.toString());
        result.append(L"\n");
    }

    return result;
}

/// Получение значения строки с указанным ключом.
/// \param index Искомый ключ.
/// \return Значение найденной строки либо пустая строка.
const String& IniSection::operator[] (const String &index) const noexcept
{
    return this->getValue(index, L"");
}

/// \brief Удаление всех секций из файла.
/// \return `this`.
IniFile& IniFile::clear()
{
    this->sections.clear();
    return *this;
}

/// \brief Есть ли секция с указанным именем?
/// \param name Искомое имя секции.
/// \return `true` если секция существует.
bool IniFile::containsSection (const String &name) const noexcept
{
    return std::any_of
        (
            begin(this->sections),
            end(this->sections),
            [&name] (const IniSection &x) { return sameString(x.name, name); }
        );
}

/// \brief Создание секции с указанным именем.
/// \param name Имя секции.
/// \return Созданная либо найденная секция.
///
/// Если секция с указанным именем уже существует, возвращается именно она,
/// создания новой секции не происходит.
IniSection& IniFile::createSection(const String &name)
{
    auto section = this->getSection(name);
    if (section) {
        return *section;
    }

    IniSection result { name };
    this->sections.push_back(result);
    return this->sections.back();
}

/// \brief Есть ли модифицированные секции?
/// \return `true`, если есть модифицированные секции.
bool IniFile::modified() const noexcept
{
    return std::any_of(begin(this->sections), end(this->sections),
            [] (const IniSection &x) { return x.modified(); } );
}

/// \brief Установка состояния "не модифицировано" для всех секций.
void IniFile::notModified()
{
    std::for_each(begin(this->sections), end(this->sections),
            [] (IniSection &x) { x.notModified(); });
}

/// \brief Получение индекса секции с указанным именем.
/// \param name Искомое имя секции.
/// \return Индекс секции либо отрицательное число, если секция не найдена.
std::ptrdiff_t IniFile::getIndex(const String &name) const noexcept
{
    for (std::size_t i = 0; i < this->sections.size(); i++) {
        if (sameString(this->sections[i].name, name)) {
            return i;
        }
    }
    return -1;
}

/// \brief Получение указателя на секцию с указанным именем.
/// \param name Искомое имя секции.
/// \return Указатель на найденную секцию, либо `nullptr`, если секция не найдена.
IniSection* IniFile::getSection(const String &name) const noexcept
{
    for (const auto &section : this->sections) {
        if (sameString(section.name, name)) {
            return const_cast<IniSection*>(&section);
        }
    }

    return nullptr;
}

/// \brief Получение значения строки с указанным ключом в указанной секции.
/// \param sectionName Имя секции.
/// \param keyName Ключ строки.
/// \param defaultValue Значение, возвращаемое, если строка не найдена.
/// \return Значение найденной строки либо значение по умолчанию.
const String& IniFile::getValue(const String &sectionName, const String &keyName, const String &defaultValue) const noexcept
{
    const auto section = this->getSection(sectionName);
    return section ? section->getValue(keyName, defaultValue) : defaultValue;
}

/// \brief Разбор текстового представления INI-файла.
/// \param lines Строки INI-файла.
void IniFile::parse(const StringList &lines)
{
    IniSection *section = nullptr;

    for (const auto &line : lines) {
        String trimmed = trim(line);
        if (trimmed.empty()) {
            continue;
        }

        if (trimmed[0] == L'[') {
            auto name = trimmed.substr(1, trimmed.size()-2);
            section = &this->createSection(name);
        } else if (section) {
            const auto parts = maxSplit(trimmed, L'=', 2);
            auto key = trim(parts[0]);
            auto value = trim(safeAt(parts, 1));
            IniLine item;
            item.key = key;
            item.value = value;
            section->lines.push_back(item);
        }
    }
}

/// \brief Удаление секции с указанным именем.
/// \param sectionName Имя секции.
/// \return `this`.
IniFile& IniFile::removeSection(const String &sectionName)
{
    const auto index = this->getIndex(sectionName);
    if (index >= 0) {
        sections.erase(begin(this->sections) + index);
    }
    return *this;
}

/// \brief Удаление строки с указанным ключом в указанной секции.
/// \param sectionName Имя секции.
/// \param keyName Ключ.
/// \return `this`.
IniFile& IniFile::removeValue(const String &sectionName, const String &keyName)
{
    const auto section = this->getSection(sectionName);
    if (section) {
        section->removeValue(keyName);
    }
    return *this;
}

/// \brief Установка значения строки с указанным ключом в указанной секции.
/// \param sectionName Имя секции.
/// \param keyName Ключ.
/// \param value Значение.
/// \return `this`.
IniFile& IniFile::setValue(const String &sectionName, const String &keyName, const String &value)
{
    auto section = this->createSection(sectionName);
    section.setValue(keyName, value);
    return *this;
}

//void IniFile::write(const String &filename, QTextCodec *encoding) const;

//void IniFile::writeModifiedOnly(QTextStream &stream) const;

}