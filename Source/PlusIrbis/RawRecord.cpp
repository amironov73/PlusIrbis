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

NAMESPACE_IRBIS_END
