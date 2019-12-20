// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <algorithm>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

bool IniLine::modified() const
{
    return this->_modified;
}

void IniLine::notModified()
{
    this->_modified = false;
}

void IniLine::setKey(const std::wstring &newKey)
{
    this->key = newKey;
    this->_modified = true;
}

void IniLine::setValue(const std::wstring &newValue)
{
    this->value = newValue;
    this->_modified = true;
}

std::wstring IniLine::toString() const
{
    return this->key + L"=" + this->value;
}

IniSection& IniSection::clear()
{
    this->lines.clear();
    this->notModified();
    return *this;
}

bool IniSection::containsKey(const std::wstring &key) const
{
    return std::any_of(begin(this->lines), end(this->lines),
            [&key] (const auto &x) {return sameString(x.key, key); });
}

size_t IniSection::getIndex(const std::wstring &key) const
{
    for (size_t i=0; i < this->lines.size(); i++) {
        if (sameString(this->lines[i].key, key)) {
            return i;
        }
    }

    return -1;
}

IniLine* IniSection::getLine(const std::wstring &key) const
{
    for (const auto &line : this->lines) {
        if (sameString(line.key, key)) {
            return const_cast<IniLine*>(&line);
        }
    }

    return nullptr;
}

const std::wstring& IniSection::getValue(const std::wstring &key, const std::wstring &defaultValue) const
{
    const auto line = this->getLine(key);
    return line ? line->value : defaultValue;
}

bool IniSection::modified() const
{
    return std::any_of(begin(this->lines), end(this->lines),
            [] (const auto &x) { return x.modified(); });
}

void IniSection::notModified()
{
    std::for_each(begin(this->lines), end(this->lines),
            [] (auto &x) { x.notModified(); });
}

IniSection& IniSection::removeValue(const std::wstring &key)
{
    size_t index = this->getIndex(key);
    if (index >= 0) {
        this->lines.erase(begin(this->lines) + index);
    }

    return *this;
}

IniSection& IniSection::setValue(const std::wstring &key, const std::wstring &value)
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

std::wstring IniSection::toString() const
{
    std::wstring result;
    if (!this->name.empty()) {
        result = std::wstring(L"[") + this->name + std::wstring (L"]\n");
    }

    for(const auto &line : lines) {
        result.append(line.toString());
        result.append(L"\n");
    }

    return result;
}

const std::wstring& IniSection::operator[] (const std::wstring &index) const
{
    return this->getValue(index, L"");
}

IniFile& IniFile::clear()
{
    this->sections.clear();
    return *this;
}

bool IniFile::containsSection (const std::wstring &name) const
{
    return std::any_of
        (
            begin(this->sections),
            end(this->sections),
            [&name] (const auto &x) { return sameString(x.name, name); }
        );
}

IniSection& IniFile::createSection(const std::wstring &name)
{
    IniSection *section = this->getSection(name);
    if (section) {
        return *section;
    }

    IniSection result { name };
    this->sections.push_back(result);
    return this->sections.back();
}

bool IniFile::modified() const
{
    return std::any_of(begin(this->sections), end(this->sections),
            [] (const auto &x) { return x.modified(); } );
}

void IniFile::notModified()
{
    std::for_each(begin(this->sections), end(this->sections),
            [] (auto &x) { x.notModified(); });
}

size_t IniFile::getIndex(const std::wstring &name) const
{
    for (size_t i = 0; i < this->sections.size(); i++) {
        if (sameString(this->sections[i].name, name)) {
            return i;
        }
    }
    return -1;
}

IniSection* IniFile::getSection(const std::wstring &name) const
{
    for (const auto &section : this->sections) {
        if (sameString(section.name, name)) {
            return const_cast<IniSection*>(&section);
        }
    }

    return nullptr;
}

const std::wstring& IniFile::getValue(const std::wstring &sectionName, const std::wstring &keyName, const std::wstring &defaultValue) const
{
    const auto *section = this->getSection(sectionName);
    return section ? section->getValue(keyName, defaultValue) : defaultValue;
}

void IniFile::parse(const StringList &list)
{
    // TODO implement
}

IniFile& IniFile::removeSection(const std::wstring &sectionName)
{
    const auto index = this->getIndex(sectionName);
    if (index >= 0) {
        sections.erase(begin(this->sections) + index);
    }
    return *this;
}

IniFile& IniFile::removeValue(const std::wstring &sectionName, const std::wstring &keyName)
{
    const auto section = this->getSection(sectionName);
    if (section) {
        section->removeValue(keyName);
    }
    return *this;
}

IniFile& IniFile::setValue(const std::wstring &sectionName, const std::wstring &keyName, const std::wstring &value)
{
    auto section = this->createSection(sectionName);
    section.setValue(keyName, value);
    return *this;
}

//void IniFile::write(const std::wstring &filename, QTextCodec *encoding) const;

//void IniFile::writeModifiedOnly(QTextStream &stream) const;

}