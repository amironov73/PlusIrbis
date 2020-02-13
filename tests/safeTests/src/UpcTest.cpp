// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("Upc12_computeCheckDigit_1", "[ean]")
{
    CHECK (irbis::Upc12::computeCheckDigit (irbis::CharSpan ("041689300494")) == '4');
    CHECK (irbis::Upc12::computeCheckDigit (irbis::WideSpan (L"041689300494")) == L'4');
}

TEST_CASE("Upc12_checkControlDigit_1", "[ean]")
{
    CHECK (irbis::Upc12::checkControlDigit (irbis::CharSpan ("041689300494")));
    CHECK (irbis::Upc12::checkControlDigit (irbis::WideSpan (L"041689300494")));
}
