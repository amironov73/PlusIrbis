// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

namespace irbis
{

RawRecord::RawRecord()
    : mfn(0), status(0), version(0)
{
}

std::wostream& operator << (std::wostream &stream, const RawRecord &record)
{
    stream << record.mfn << L"#" << record.status << std::endl;
    stream << L"0#" << record.version << std::endl;
    for (const std::wstring &field : record.fields)
    {
        stream << field << std::endl;
    }

    return stream;
}

}

