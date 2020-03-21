// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("EmbeddedField_getEmbeddedFields_i", "[embedded]")
{
    irbis::RecordField field (461);
    const auto embedded = irbis::EmbeddedField::getEmbeddedFields (field);
    CHECK (embedded.empty());
}

TEST_CASE("EmbeddedField_getEmbeddedFields_2", "[embedded]")
{
    irbis::RecordField field (461);
    field.add ('1', L"2001#");
    field.add ('a', L"\u0417\u043B\u0430\u0442\u0430\u044F \u0446\u0435\u043F\u044C");
    field.add ('e', L"\u0417\u0430\u043F\u0438\u0441\u043A\u0438. \u041F\u043E\u0432\u0435\u0441\u0442\u0438. \u0420\u0430\u0441\u0441\u043A\u0430\u0437\u044B");
    field.add ('f', L"\u0411\u043E\u043D\u0434\u0430\u0440\u0438\u043D \u0421. \u0410.");
    field.add ('v', L"\u0421. 76-132");
    const auto embedded = irbis::EmbeddedField::getEmbeddedFields (field);
    CHECK (embedded.size() == 1);
    const irbis::RecordField &first = embedded[0];
    CHECK (first.tag == 200);
    CHECK (first.getFirstSubfieldValue ('a') == L"\u0417\u043B\u0430\u0442\u0430\u044F \u0446\u0435\u043F\u044C");
    CHECK (first.getFirstSubfieldValue ('e') == L"\u0417\u0430\u043F\u0438\u0441\u043A\u0438. \u041F\u043E\u0432\u0435\u0441\u0442\u0438. \u0420\u0430\u0441\u0441\u043A\u0430\u0437\u044B");
    CHECK (first.getFirstSubfieldValue ('f') == L"\u0411\u043E\u043D\u0434\u0430\u0440\u0438\u043D \u0421. \u0410.");
    CHECK (first.getFirstSubfieldValue ('v') == L"\u0421. 76-132");
}

TEST_CASE("EmbeddedField_getEmbeddedFields_3", "[embedded]")
{
    irbis::RecordField field (461);
    field.add ('1', L"2001#");
    field.add ('a', L"\u0417\u043B\u0430\u0442\u0430\u044F \u0446\u0435\u043F\u044C");
    field.add ('e', L"\u0417\u0430\u043F\u0438\u0441\u043A\u0438. \u041F\u043E\u0432\u0435\u0441\u0442\u0438. \u0420\u0430\u0441\u0441\u043A\u0430\u0437\u044B");
    field.add ('f', L"\u0411\u043E\u043D\u0434\u0430\u0440\u0438\u043D \u0421. \u0410.");
    field.add ('v', L"\u0421. 76-132");
    field.add ('1', L"2001#");
    field.add ('a', L"\u0420\u0443\u0441\u043B\u0430\u043D \u0438 \u041B\u044E\u0434\u043C\u0438\u043B\u0430");
    field.add ('f', L"\u041F\u0443\u0448\u043A\u0438\u043D \u0410. \u0421.");
    const auto embedded = irbis::EmbeddedField::getEmbeddedFields (field);
    CHECK (embedded.size() == 2);
    const irbis::RecordField &first = embedded[0];
    CHECK (first.tag == 200);
    CHECK (first.getFirstSubfieldValue ('a') == L"\u0417\u043B\u0430\u0442\u0430\u044F \u0446\u0435\u043F\u044C");
    CHECK (first.getFirstSubfieldValue ('e') == L"\u0417\u0430\u043F\u0438\u0441\u043A\u0438. \u041F\u043E\u0432\u0435\u0441\u0442\u0438. \u0420\u0430\u0441\u0441\u043A\u0430\u0437\u044B");
    CHECK (first.getFirstSubfieldValue ('f') == L"\u0411\u043E\u043D\u0434\u0430\u0440\u0438\u043D \u0421. \u0410.");
    CHECK (first.getFirstSubfieldValue ('v') == L"\u0421. 76-132");
    const irbis::RecordField &second = embedded[1];
    CHECK (second.tag == 200);
    CHECK (second.getFirstSubfieldValue ('a') == L"\u0420\u0443\u0441\u043B\u0430\u043D \u0438 \u041B\u044E\u0434\u043C\u0438\u043B\u0430");
    CHECK (second.getFirstSubfieldValue ('f') == L"\u041F\u0443\u0448\u043A\u0438\u043D \u0410. \u0421.");
}

TEST_CASE("EmbeddedField_getEmbeddedFields_4", "[embedded]")
{
    irbis::RecordField field (461);
    field.add ('1', L"001123456789");
    const auto embedded = irbis::EmbeddedField::getEmbeddedFields (field);
    CHECK (embedded.size() == 1);
    const irbis::RecordField &first = embedded[0];
    CHECK (first.tag == 1);
    CHECK (first.value == L"123456789");
}
