// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

std::wostream& operator << (std::wostream &stream, const RawRecord &record)
{
    stream << std::to_wstring(record.mfn) << std::wstring(L"#")
        << std::to_wstring(record.status) << std::endl;
    stream << std::wstring(L"0#") << std::to_wstring(record.version)
        << std::endl;
    for (const std::wstring &field : record.fields)
    {
        stream << field << std::endl;
    }

    return stream;
}

std::wstring RawRecord::encode(const std::wstring &delimiter) const
{
    std::wstring result;
    result.append(std::to_wstring(mfn));
    result.append(L"#");
    result.append(std::to_wstring(status));
    result.append(delimiter);
    result.append(L"0#");
    result.append(std::to_wstring(version));
    result.append(delimiter);

    for (const auto field: fields) {
        result.append(field).append(delimiter);

    }

    return result;
}

void RawRecord::parseSingle(const StringList &lines)
{
    if (lines.empty()) {
        return;
    }

    StringList parts = maxSplit(lines[0], '#', 2);
    mfn = fastParse32(parts[0]);
    status = 0;
    if (parts.size() == 2) {
        status = fastParse32(parts[1]);
    }

    parts = maxSplit(lines[1], '#', 2);
    version = 0;
    if (parts.size() == 2) {
        version = fastParse32(parts[1]);
    }

    fields.clear();
    for (unsigned i = 2; i < lines.size(); i++) {
        fields.push_back(lines[i]);
    }
}

std::wstring RawRecord::to_wstring() const
{
    return encode(L"\n");
}

NAMESPACE_IRBIS_END
