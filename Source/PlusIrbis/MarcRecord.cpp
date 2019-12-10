// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

MarcRecord& MarcRecord::add(wchar_t code, const std::wstring &value)
{
    fields.push_back({code, value});

    return *this;
}

bool MarcRecord::deleted() const
{
    return (status & RecordStatus::Deleted) != 0;
}

bool MarcRecord::verify(bool throwOnError) const
{
    bool result = true;
    for (const RecordField &field : fields) {
        if (!field.verify(throwOnError)) {
            result = false;
        }
    }

    if (!result && throwOnError) {
        throw VerificationException();
    }

    return result;
}

std::wostream& operator << (std::wostream &stream, const MarcRecord &record)
{
    stream << std::to_wstring(record.mfn) << std::wstring(L"#")
        << std::to_wstring(record.status) << std::endl;
    stream << std::wstring(L"0#") << record.version << std::endl;
    for (const RecordField &field : record.fields)
    {
        stream << field << std::endl;
    }

    return stream;
}

NAMESPACE_IRBIS_END
