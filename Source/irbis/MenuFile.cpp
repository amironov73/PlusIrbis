// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

const String MenuFile::StopMarker = L"*****";
const Chars MenuFile::Separators = { ' ', '-', '=', ':' };

MenuFile& MenuFile::add(const String &code, const String &comment)
{
    this->entries.push_back({ code, comment });
    return *this;
}

MenuEntry* MenuFile::getEntry(const String &code) const noexcept
{
    for(const auto &entry : this->entries) {
        if (sameString(entry.code, code)) {
            return const_cast<MenuEntry*>(&entry);
        }
    }

    return nullptr;
}

MenuEntry* MenuFile::getEntrySensitive(const String &code) const noexcept
{
    for(const auto &entry : this->entries) {
        if (entry.code == code) {
            return const_cast<MenuEntry*>(&entry);
        }
    }

    return nullptr;
}

String* MenuFile::getValue(const String &code) const noexcept
{
    const auto entry = this->getEntry(code);
    const auto result = entry ? &entry->comment : nullptr;
    return result;
}

String* MenuFile::getValueSensitive(const String &code) const noexcept
{
    const auto entry = this->getEntrySensitive(code);
    const auto result = entry ? &entry->comment : nullptr;
    return result;
}

const String& MenuFile::getValue(const String &code, const String &defaultValue) const noexcept
{
    const auto value = this->getValue(code);
    return value ? *value : defaultValue;
}

const String& MenuFile::getValueSensitive(const String &code, const String &defaultValue) const noexcept
{
    const auto value = this->getValueSensitive(code);
    return value ? *value : defaultValue;
}

void MenuFile::parse(std::istream &stream)
{
    throw NotImplementedException();
}

void MenuFile::parse(const StringList &lines)
{
    throw NotImplementedException();
}

void MenuFile::parseLocalFile(const String &filename /* const QTextCodec *encoding */)
{
    throw NotImplementedException();
}

}
