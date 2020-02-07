// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("Isbn_checkControlDigit_1", "[isbn]")
{
    CHECK (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5-02-003206-9")));
    CHECK (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5020032069")));
    CHECK (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5-01-001033-X")));
    CHECK (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"501001033X")));
    CHECK_FALSE (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5-02-0032239-5")));
    CHECK_FALSE (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5-85-202-063-X")));
    CHECK_FALSE (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5-01-00103X-3")));
    CHECK_FALSE (irbis::Isbn::checkControlDigit (irbis::WideSpan (L"5-01-00A033-X")));
}
