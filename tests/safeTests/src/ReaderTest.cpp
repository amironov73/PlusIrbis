// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("Reader_constructor_1", "[reader]")
{
    irbis::Reader reader;
    CHECK (reader.mfn == 0);
    CHECK (reader.record == nullptr);
    CHECK (reader.familyName.empty());
    CHECK (reader.firstName.empty());
    CHECK (reader.patronymic.empty());
    CHECK (reader.birth.empty());
    CHECK (reader.ticket.empty());
    CHECK (reader.gender.empty());
    CHECK (reader.category.empty());
    CHECK (reader.workplace.empty());
    CHECK (reader.education.empty());
    CHECK (reader.email.empty());
    CHECK (reader.phone.empty());
    CHECK (reader.enrollment.empty());
    CHECK (reader.registrations.empty());
    CHECK (reader.enabled.empty());
    CHECK (reader.disabled.empty());
    CHECK (reader.right.empty());
    CHECK (reader.remarks.empty());
    CHECK (reader.photo.empty());
    CHECK (reader.visits.empty());
    CHECK (reader.password.empty());
    CHECK (reader.fullName().empty());
}

TEST_CASE("Reader_parse_1", "[reader]")
{
    irbis::MarcRecord record;
    record.add (10, L"\u0418\u0432\u0430\u043D\u043E\u0432");
    record.add (11, L"\u0418\u0432\u0430\u043D");
    record.add (12, L"\u0418\u0432\u0430\u043D\u043E\u0432\u0438\u0447");
    record.add (21, L"20200302");
    record.add (30, L"31415926");
    record.add (23, L"\u043C");
    record.add (50, L"\u0441\u0442\u0443\u0434\u0435\u043D\u0442");
    record.add (15, L"\u0437\u043E\u043E\u043F\u0430\u0440\u043A");
    record.add (20, L"\u0441\u0440\u0435\u0434\u043D\u0435\u0435 \u0441\u043F\u0435\u0446\u0438\u0430\u043B\u044C\u043D\u043E\u0435");
    record.add (32, L"reader@library.com");
    record.add (17, L"212-85-06");
    irbis::Reader reader;
    reader.parse (record);
    CHECK (reader.familyName == record.fm (10));
    CHECK (reader.firstName  == record.fm (11));
    CHECK (reader.patronymic == record.fm (12));
    CHECK (reader.birth      == record.fm (21));
    CHECK (reader.ticket     == record.fm (30));
    CHECK (reader.gender     == record.fm (23));
    CHECK (reader.category   == record.fm (50));
    CHECK (reader.workplace  == record.fm (15));
    CHECK (reader.education  == record.fm (20));
    CHECK (reader.email      == record.fm (32));
    CHECK (reader.phone      == record.fm (17));
    CHECK (reader.record == &record);
    CHECK (reader.visits.empty());
    CHECK (reader.password.empty());
    CHECK (reader.remarks.empty());
    CHECK (reader.right.empty());
    CHECK (reader.enabled.empty());
    CHECK (reader.disabled.empty());
    CHECK (reader.enrollment.empty());
    CHECK (reader.mfn == record.mfn);
}

TEST_CASE("Reader_apply_1", "[reader]")
{
    irbis::Reader reader;
    reader.familyName = L"\u0418\u0432\u0430\u043D\u043E\u0432";
    reader.firstName  = L"\u0418\u0432\u0430\u043D";
    reader.patronymic = L"\u0418\u0432\u0430\u043D\u043E\u0432\u0438\u0447";
    reader.birth      = L"20200302";
    reader.ticket     = L"31415926";
    reader.gender     = L"\u043C";
    reader.category   = L"\u0441\u0442\u0443\u0434\u0435\u043D\u0442";
    reader.workplace  = L"\u0437\u043E\u043E\u043F\u0430\u0440\u043A";
    reader.education  = L"\u0441\u0440\u0435\u0434\u043D\u0435\u0435 \u0441\u043F\u0435\u0446\u0438\u0430\u043B\u044C\u043D\u043E\u0435";
    reader.email      = L"reader@library.com";
    reader.phone      = L"212-85-06";
    CHECK (reader.fullName() == L"\u0418\u0432\u0430\u043D\u043E\u0432 \u0418\u0432\u0430\u043D \u0418\u0432\u0430\u043D\u043E\u0432\u0438\u0447");
    irbis::MarcRecord record;
    reader.applyTo (record);
    CHECK (record.fm (10) == reader.familyName);
    CHECK (record.fm (11) == reader.firstName);
    CHECK (record.fm (12) == reader.patronymic);
    CHECK (record.fm (21) == reader.birth);
    CHECK (record.fm (30) == reader.ticket);
    CHECK (record.fm (23) == reader.gender);
    CHECK (record.fm (50) == reader.category);
    CHECK (record.fm (15) == reader.workplace);
    CHECK (record.fm (20) == reader.education);
    CHECK (record.fm (32) == reader.email);
    CHECK (record.fm (17) == reader.phone);
}
