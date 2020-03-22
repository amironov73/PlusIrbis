// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE("PostingParameters_constructor_1", "[posting]")
{
    irbis::PostingParameters parameters {};
    CHECK (parameters.firstPosting     == 0);
    CHECK (parameters.numberOfPostings == 0);
    CHECK (parameters.database     .empty());
    CHECK (parameters.term         .empty());
    CHECK (parameters.listOfTerms  .empty());
    CHECK (parameters.format       .empty());
}

TEST_CASE("TermPosting_constructor_1", "[term]")
{
    irbis::TermPosting posting;
    CHECK (posting.count == 0);
    CHECK (posting.mfn == 0);
    CHECK (posting.occurrence == 0);
    CHECK (posting.tag == 0);
    CHECK (posting.text.empty());
}

TEST_CASE("TermPosting_parse_1", "[term]")
{
    irbis::StringList lines { L"1#2#3#4#First", L"2#3#4#5#Second", L"3#4#5#6#", L"" };
    const auto postings = irbis::TermPosting::parse (lines);
    REQUIRE (postings.size()      == 3);
    CHECK (postings[0].mfn        == 1);
    CHECK (postings[0].tag        == 2);
    CHECK (postings[0].occurrence == 3);
    CHECK (postings[0].count      == 4);
    CHECK (postings[0].text       == L"First");
    CHECK (postings[1].mfn        == 2);
    CHECK (postings[1].tag        == 3);
    CHECK (postings[1].occurrence == 4);
    CHECK (postings[1].count      == 5);
    CHECK (postings[1].text       == L"Second");
    CHECK (postings[2].mfn        == 3);
    CHECK (postings[2].tag        == 4);
    CHECK (postings[2].occurrence == 5);
    CHECK (postings[2].count      == 6);
    CHECK (postings[2].text.empty());
}

TEST_CASE("TermPosting_toString_1", "[term]")
{
    irbis::TermPosting posting;
    CHECK (posting.toString() == L"0#0#0#0#");

    posting.mfn        = 1;
    posting.tag        = 2;
    posting.occurrence = 3;
    posting.count      = 4;
    CHECK (posting.toString() == L"1#2#3#4#");

    posting.text = L"Text";
    CHECK (posting.toString() == L"1#2#3#4#Text");
}
