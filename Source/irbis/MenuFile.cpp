// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

const std::wstring MenuFile::StopMarker = L"*****";
const std::vector<wchar_t> MenuFile::Separators = { ' ', '-', '=', ':' };

MenuFile& MenuFile::add(const std::wstring &code, const std::wstring &comment)
{
    entries.push_back({code, comment});
    return *this;
}

MenuEntry* MenuFile::getEntry(const std::wstring &code) const
{
    for(const auto &entry : entries) {
        if (sameString(entry.code, code)) {
            return const_cast<MenuEntry*>(&entry);
        }
    }

    return nullptr;
}

MenuEntry* MenuFile::getEntrySensitive(const std::wstring &code) const
{
    for(const auto &entry : entries) {
        if (entry.code == code) {
            return const_cast<MenuEntry*>(&entry);
        }
    }

    return nullptr;
}

std::wstring* MenuFile::getValue(const std::wstring &code) const
{
    const auto entry = getEntry(code);
    const auto result = entry ? &entry->comment : nullptr;
    return result;
}

std::wstring* MenuFile::getValueSensitive(const std::wstring &code) const
{
    const auto entry = getEntrySensitive(code);
    const auto result = entry ? &entry->comment : nullptr;
    return result;
}

const std::wstring& MenuFile::getValue(const std::wstring &code, const std::wstring &defaultValue) const
{
    const auto value = getValue(code);
    return value ? *value : defaultValue;
}

const std::wstring& MenuFile::getValueSensitive(const std::wstring &code, const std::wstring &defaultValue) const
{
    const auto value = getValueSensitive(code);
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

void MenuFile::parseLocalFile(const std::wstring &filename /* const QTextCodec *encoding */)
{
    throw NotImplementedException();
}

}
