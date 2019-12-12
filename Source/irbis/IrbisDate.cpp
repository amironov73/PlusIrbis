// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

IrbisDate::IrbisDate(const std::wstring &text_)  // NOLINT(modernize-pass-by-value)
    : text(text_)
{
}

IrbisDate::IrbisDate(std::chrono::system_clock::time_point date_)
    : date(date_)
{
}

std::chrono::system_clock::time_point IrbisDate::convert(const std::wstring &text)
{
    throw NotImplementedException();
}

std::wstring IrbisDate::convert(const std::chrono::system_clock::time_point &date)
{
    throw NotImplementedException();
}

std::wstring IrbisDate::today()
{
    throw NotImplementedException();
}

IrbisDate* IrbisDate::safeParse(const std::wstring &text)
{
    throw NotImplementedException();
}

}
