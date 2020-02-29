// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <iomanip>
#include <iostream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

/*!
    \class irbis::RawRecord

    "Сырая" запись нужна для экономии усилий на распаковку-запаковку.
*/

namespace irbis {

std::wostream& IRBIS_API operator << (std::wostream &stream, const RawRecord &record)
{
    stream << std::to_wstring(record.mfn) << String(L"#")
        << std::to_wstring(record.status) << std::endl;
    stream << String(L"0#") << std::to_wstring(record.version)
        << std::endl;
    for (const String &field : record.fields)
    {
        stream << field << std::endl;
    }

    return stream;
}

String RawRecord::encode (const String &delimiter) const
{
    String result;
    result.append (std::to_wstring(mfn));
    result.append (L"#");
    result.append (std::to_wstring(status));
    result.append (delimiter);
    result.append (L"0#");
    result.append (std::to_wstring(version));
    result.append (delimiter);

    for (const auto &field : fields) {
        result.append (field).append (delimiter);
    }

    return result;
}

void RawRecord::parseSingle(const StringList &lines)
{
    if (lines.empty()) {
        return;
    }

    const StringList parts1 = maxSplit(lines[0], '#', 2);
    mfn = fastParseUnsigned32(parts1[0]);
    status = 0;
    if (parts1.size() == 2) {
        status = fastParseUnsigned32(parts1[1]);
    }

    const StringList parts2 = maxSplit(lines[1], '#', 2);
    version = 0;
    if (parts2.size() == 2) {
        version = fastParseUnsigned32(parts2[1]);
    }

    fields.clear();
    for (std::size_t i = 2; i < lines.size(); i++) {
        fields.push_back(lines[i]);
    }
}

String RawRecord::toString() const
{
    return encode(L"\n");
}

}
