// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

static MfnList parseLine(const std::wstring &line)
{
    MfnList result;

    if (line.empty()) {
        return result;
    }

    StringList items = split(line, L'\u001E');
    for (const auto &item : items) {
        auto mfn = fastParse32(item);
        result.push_back(mfn);
    }

    return result;
}

void DatabaseInfo::parse(ServerResponse &response)
{
    this->logicallyDeletedRecords = parseLine(response.readAnsi());
    this->physicallyDeletedRecords = parseLine(response.readAnsi());
    this->nonActualizedRecords = parseLine(response.readAnsi());
    this->lockedRecords = parseLine(response.readAnsi());
    this->maxMfn = fastParse32(response.readAnsi());
    this->databaseLocked = fastParse32(response.readAnsi()) != 0;
}

std::vector<DatabaseInfo> DatabaseInfo::parse(MenuFile &menu)
{
    std::vector<DatabaseInfo> result;
    for (const MenuEntry &entry: menu.entries) {
        auto name = entry.code;
        auto description = entry.comment;
        auto readOnly = false;
        if (name[0] == L'-') {
            name = name.substr(1);
            readOnly = true;
        }

        DatabaseInfo info;
        info.name = name;
        info.description = description;
        info.readOnly = readOnly;
        result.push_back(info);
    }

    return result;
}

std::wstring DatabaseInfo::toString() const
{
    if (this->description.empty()) {
        return name;
    }

    return this->name + L" - " + this->description;
}

}
