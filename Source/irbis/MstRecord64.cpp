// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

bool MstRecord64::deleted() const
{
    return (this->leader.status & RecordStatus::Deleted) != 0;
}

MarcRecord MstRecord64::toMarcRecord() const
{
    MarcRecord result;
    result.status = 0; // TODO obtain the status
    result.version = 0; // TODO obtain the version
    //result.fields.reserve(dictionary.size());
    for (const auto &entry : dictionary) {
        RecordField field;
        field.tag = entry.tag;
        // field.parse(entry.text); // TODO parse the field
        result.fields.push_back(field);
    }

    return result;
}

}
