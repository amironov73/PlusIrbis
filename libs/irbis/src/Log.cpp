// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#include <iostream>

namespace irbis {

/// \brief Вывод в лог.
void Log::write (const String &text)
{
    // std::cout << text << std::endl;
}

/// \brief Вывод в лог.
void Log::write (const std::string &text)
{
    // std::cout << text << std::endl;
}

/// \brief Вывод в лог.
void Log::write (const char *text)
{
    // std::cout << text << std::endl;
}


}
