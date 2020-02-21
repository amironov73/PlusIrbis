// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

using irbis::Search;
using irbis::keyword;
using irbis::author;
using irbis::title;
using irbis::publisher;
using irbis::place;
using irbis::subject;
using irbis::language;
using irbis::year;
using irbis::magazine;
using irbis::documentKind;
using irbis::udc;
using irbis::bbk;
using irbis::rzn;
using irbis::mhr;

TEST_CASE("Search_all_1", "[search]")
{
    CHECK (Search::all().toString() == L"I=$");
}

TEST_CASE("Search_and_1", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .and_ (Search::equals (L"prefix2=", L"value2"))
       .toString() == L"(prefix1=value1 * prefix2=value2)");
}

TEST_CASE("Search_and_2", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .and_ (Search::equals (L"prefix2=", L"value2"),
               Search::equals (L"prefix3=", L"value3"))
       .toString() == L"(prefix1=value1 * prefix2=value2 * prefix3=value3)");
}

TEST_CASE("Search_and_3", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .and_ (Search::equals (L"prefix2=", L"value2"),
               Search::equals (L"prefix3=", L"value3"),
               Search::equals (L"prefix4=", L"value4"))
       .toString() == L"(prefix1=value1 * prefix2=value2 * prefix3=value3 * prefix4=value4)");
}

TEST_CASE("Search_and_4", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .and_ (L"prefix2=value2")
       .toString() == L"(prefix1=value1 * prefix2=value2)");
}

TEST_CASE("Search_and_5", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .and_ (L"prefix2=value2", L"prefix3=value3")
       .toString() == L"(prefix1=value1 * prefix2=value2 * prefix3=value3)");
}

TEST_CASE("Search_and_6", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .and_ (L"prefix2=value2", L"prefix3=value3", L"prefix4=value4")
       .toString() == L"(prefix1=value1 * prefix2=value2 * prefix3=value3 * prefix4=value4)");
}

TEST_CASE("Search_equals_1", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .toString() == L"prefix1=value1");
    CHECK (Search::equals (L"prefix1=", L"value 1")
       .toString() == L"\"prefix1=value 1\"");
}

TEST_CASE("Search_equals_2", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1", L"value2")
       .toString() == L"(prefix1=value1 + prefix1=value2)");
    CHECK (Search::equals (L"prefix1=", L"value 1", L"value 2")
       .toString() == L"(\"prefix1=value 1\" + \"prefix1=value 2\")");
}

TEST_CASE("Search_equals_3", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1", L"value2", L"value3")
       .toString() == L"(prefix1=value1 + prefix1=value2 + prefix1=value3)");
    CHECK (Search::equals (L"prefix1=", L"value 1", L"value 2", L"value 3")
       .toString() == L"(\"prefix1=value 1\" + \"prefix1=value 2\" + \"prefix1=value 3\")");
}

TEST_CASE("Search_needWrap_1", "[search]")
{
    CHECK (Search::needWrap (L""));
    CHECK (Search::needWrap (L" "));
    CHECK (Search::needWrap (L"1+2"));
    CHECK (!Search::needWrap (L"1"));
    CHECK (!Search::needWrap (L"\"1\""));
    CHECK (!Search::needWrap (L"(1)"));
}

TEST_CASE ("Search_not_1", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .not_ (L"prefix2=value2")
       .toString() == L"(prefix1=value1 ^ prefix2=value2)");
}

TEST_CASE ("Search_not_2", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .not_ (Search::equals (L"prefix2=", L"value2"))
       .toString() == L"(prefix1=value1 ^ prefix2=value2)");
}

TEST_CASE("Search_or_1", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
                   .or_ (Search::equals (L"prefix2=", L"value2"))
                   .toString() == L"(prefix1=value1 + prefix2=value2)");
}

TEST_CASE("Search_or_2", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
                   .or_ ( Search::equals (L"prefix2=", L"value2"),
                         Search::equals (L"prefix3=", L"value3"))
                   .toString() == L"(prefix1=value1 + prefix2=value2 + prefix3=value3)");
}

TEST_CASE("Search_or_3", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
                   .or_ (Search::equals (L"prefix2=", L"value2"),
                         Search::equals (L"prefix3=", L"value3"),
                         Search::equals (L"prefix4=", L"value4"))
                   .toString() == L"(prefix1=value1 + prefix2=value2 + prefix3=value3 + prefix4=value4)");
}

TEST_CASE("Search_or_4", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
                   .or_ (L"prefix2=value2")
                   .toString() == L"(prefix1=value1 + prefix2=value2)");
}

TEST_CASE("Search_or_5", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
                   .or_ (L"prefix2=value2", L"prefix3=value3")
                   .toString() == L"(prefix1=value1 + prefix2=value2 + prefix3=value3)");
}

TEST_CASE("Search_or_6", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
                   .or_ (L"prefix2=value2", L"prefix3=value3", L"prefix4=value4")
                   .toString() == L"(prefix1=value1 + prefix2=value2 + prefix3=value3 + prefix4=value4)");
}

TEST_CASE("Search_sameField_1", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .sameField (L"prefix2=value2")
       .toString() == L"(prefix1=value1 (G) prefix2=value2)");
}

TEST_CASE("Search_sameRepeat_1", "[search]")
{
    CHECK (Search::equals (L"prefix1=", L"value1")
       .sameRepeat (L"prefix2=value2")
       .toString() == L"(prefix1=value1 (F) prefix2=value2)");
}

TEST_CASE("Search_wrap_1", "[search]")
{
    CHECK (Search::wrap (L"")      == L"\"\"");
    CHECK (Search::wrap (L" ")     == L"\" \"");
    CHECK (Search::wrap (L"1+2")   == L"\"1+2\"");
    CHECK (Search::wrap (L"1")     == L"1");
    CHECK (Search::wrap (L"\"1\"") == L"\"1\"");
    CHECK (Search::wrap (L"(1)")   == L"(1)");

}

TEST_CASE("Search_wrap_2", "[search]")
{
    CHECK (Search::wrap (Search::equals(L"prefix=", L"value")) == L"prefix=value");
    CHECK (Search::wrap (Search::equals(L"prefix=", L"value 1")) == L"\"prefix=value 1\"");
}

TEST_CASE("Search_keyword_1", "[search]")
{
    CHECK (keyword(L"concrete").toString() == L"K=concrete");
    CHECK (keyword(L"concrete block").toString() == L"\"K=concrete block\"");
}

TEST_CASE("Search_author_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(author(L"Pushkin$")).toString() == L"(K=concrete * A=Pushkin$)");
}

TEST_CASE("Search_title_1", "[search]")
{
    CHECK (author(L"Pushkin$")
       .and_(title(L"Tales")).toString() == L"(A=Pushkin$ * T=Tales)");
}

TEST_CASE("Search_publisher_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(publisher(L"Nauka")).toString() == L"(K=concrete * O=Nauka)");
}

TEST_CASE("Search_place_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(place(L"Moscow")).toString() == L"(K=concrete * MI=Moscow)");
}

TEST_CASE("Search_subject_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(subject(L"Building")).toString() == L"(K=concrete * S=Building)");
}

TEST_CASE("Search_language_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(language(L"fra")).toString() == L"(K=concrete * J=fra)");
}

TEST_CASE("Search_year_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(year(L"2020")).toString() == L"(K=concrete * G=2020)");
}

TEST_CASE("Search_magazine_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(magazine(L"Architecture")).toString() == L"(K=concrete * TJ=Architecture)");
}

TEST_CASE("Search_documentKind_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(documentKind(L"i")).toString() == L"(K=concrete * V=i)");
}

TEST_CASE("Search_udc_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(udc(L"622$")).toString() == L"(K=concrete * U=622$)");
}

TEST_CASE("Search_bbk_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(bbk(L"33$")).toString() == L"(K=concrete * BBK=33$)");
}

TEST_CASE("Search_rzn_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(rzn(L"1")).toString() == L"(K=concrete * RZN=1)");
}

TEST_CASE("Search_mhr_1", "[search]")
{
    CHECK (keyword(L"concrete")
       .and_(mhr(L"A1")).toString() == L"(K=concrete * MHR=A1)");
}

