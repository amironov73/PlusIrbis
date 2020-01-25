// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

/*!

    \file

    \class irbis::PhantomRecord

    Фантомные записи не владеют памятью, на которую ссылаются.
    Они требуют гораздо меньших затрат.

    При необходимости фантомная запись может быть материализована.

 */

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Материализация фантомного подполя.
/// \return Созданное подполе.
SubField PhantomSubField::materialize() const
{
    SubField result;
    result.code = this->code;
    result.value = fromUtf (this->value);
    return result;
}

/// \brief Материализация фантомного поля.
/// \return Созданное поле.
RecordField PhantomField::materialize() const
{
    RecordField result (this->tag);
    result.value = fromUtf (this->value);
    for (const auto &subfield : this->subfields) {
        result.subfields.push_back(subfield.materialize());
    }
    return result;
}

/// \brief Материализация фантомной записи.
/// \return Созданная запись.
MarcRecord PhantomRecord::materialize() const
{
    MarcRecord result;
    result.mfn = this->mfn;
    result.status = this->status;
    result.version = this->version;
    for (const auto &field : this->fields) {
        result.fields.push_back(field.materialize());
    }
    return result;
}

}
