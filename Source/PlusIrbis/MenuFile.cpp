// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

const std::wstring MenuFile::StopMarker = L"*****";
const std::vector<wchar_t> MenuFile::Separators = { ' ', '-', '=', ':' };

MenuFile& MenuFile::add(const std::wstring &code, const std::wstring &comment)
{
    entries.push_back({code, comment});

    return *this;
}

MenuEntry* MenuFile::getEntry(const std::wstring &code) const
{
    throw NotImplementedException();
}

MenuEntry* MenuFile::getEntrySensitive(const std::wstring &code) const
{
    throw NotImplementedException();
}

std::wstring* MenuFile::getValue(const std::wstring &code) const
{
    MenuEntry *entry = getEntry(code);
    std::wstring *result = entry ? &entry->comment : nullptr;

    return result;
}

std::wstring* MenuFile::getValueSensitive(const std::wstring &code) const
{
    MenuEntry *entry = getEntrySensitive(code);
    std::wstring *result = entry ? &entry->comment : nullptr;

    return result;
}

const std::wstring& MenuFile::getValue(const std::wstring &code, const std::wstring &defaultValue) const
{
    std::wstring *value = getValue(code);

    return value ? *value : defaultValue;
}

const std::wstring& MenuFile::getValueSensitive(const std::wstring &code, const std::wstring &defaultValue) const
{
    std::wstring *value = getValueSensitive(code);

    return value ? *value : defaultValue;
}

MenuFile MenuFile::parse(std::istream &stream)
{
    throw NotImplementedException();
}

MenuFile MenuFile::parseLocalFile(const std::wstring &filename /* const QTextCodec *encoding */)
{
    throw NotImplementedException();
}

NAMESPACE_IRBIS_END
