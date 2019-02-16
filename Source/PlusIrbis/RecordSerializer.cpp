// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

RecordSerializer::RecordSerializer(std::wiostream &stream)
    : stream(stream) {
}

MarcRecord RecordSerializer::deserialize()
{
    throw NotImplementedException();

#if defined(NOTDEF)
    MarcRecord result;
    int fieldCount;
    stream >> fieldCount;
    for (int i = 0; i < fieldCount; ++i) {
        int tag;
        std::wstring value;
        stream >> tag >> value;
        result.add(tag, value);
        RecordField &field = result.fields.back();
        int subfieldCount;
        stream >> subfieldCount;
        for (int j = 0; j < subfieldCount; ++j) {
            wchar_t code;
            stream >> code >> value;
            field.add(code, value);
        }
    }

    return result;

#endif
}

void RecordSerializer::serialize(const MarcRecord &record)
{
    throw NotImplementedException();

#if defined(NOTDEF)
    const auto fieldCount = static_cast<int> (record.fields.size());
    stream << fieldCount;
    for (const RecordField &field : record.fields) {
        stream << field.tag << field.value;
        const auto subfieldCount = static_cast<unsigned short>(field.subfields.size()); //-V1029
        stream << subfieldCount;
        for (const SubField &subfield : field.subfields) {
            stream << subfield.code << subfield.value;
        }
    }
#endif
}

NAMESPACE_IRBIS_END
