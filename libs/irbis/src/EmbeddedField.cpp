// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

const char EmbeddedField::DefaultCode = '1';

RecordFieldList EmbeddedField::getEmbeddedFields(const RecordField &field, char sign) {
    RecordFieldList result;
    for (const auto &subField : field.subfields) {
        if (subField.code == sign) {
            auto value = subField.value;
            auto tagValue = value.substr(0, 3);
            auto tag = fastParse32(tagValue);
            RecordField found;
            found.tag = tag;
            if (tagValue[0] == '0' && tagValue[1] == '0' && value.length() > 3) {
                found.value = value.substr(3);
            }
            result.push_back(found);
        } else {
            if (!result.empty()) {
                auto &last = result.back();
                last.add(subField.code, subField.value);
            }
        }
    }

    return result;
}

}
