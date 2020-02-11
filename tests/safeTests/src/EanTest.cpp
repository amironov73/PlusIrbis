// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("Ean13_computeCheckDigit_1", "[ean]")
{
    CHECK (irbis::Ean13::computeCheckDigit (irbis::CharSpan ("4600051000057")) == '7');
    CHECK (irbis::Ean13::computeCheckDigit (irbis::WideSpan (L"4600051000057")) == L'7');
}

TEST_CASE("Ean13_checkControlDigit_1", "[ean]")
{
    CHECK (irbis::Ean13::checkControlDigit (irbis::CharSpan ("4600051000057")));
    CHECK (irbis::Ean13::checkControlDigit (irbis::WideSpan (L"4600051000057")));
}
TEST_CASE("Ean8_computeCheckDigit_1", "[ean]")
{
    CHECK (irbis::Ean8::computeCheckDigit (irbis::CharSpan ("46009333")) == '3');
    CHECK (irbis::Ean8::computeCheckDigit (irbis::WideSpan (L"46009333")) == L'3');
}

TEST_CASE("Ean8_checkControlDigit_1", "[ean]")
{
    CHECK (irbis::Ean8::checkControlDigit (irbis::CharSpan ("46009333")));
    CHECK (irbis::Ean8::checkControlDigit (irbis::WideSpan (L"46009333")));
}