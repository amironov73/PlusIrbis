// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//==============================================================================
// Это простое приложение предназначено для разработки и отладки шаблонов,
// определенных в заголовочных файлах irbis.h и irbis_internal.h.
// Больше от него никакой пользы.
//==============================================================================

#include <iostream>

#include "irbis.h"
#include "irbis_internal.h"

#include <cctype>
#include <limits>
#include <iterator>

void testSystem();
void testMmap();
void testByteRange();
void testTextRange();

int main()
{
    std::cout << "=============================================" << std::endl;

    try
    {
        //testTextRange();
        testMmap ();
    }
    catch (const std::exception &exception)
    {
        std::cout << exception.what() << std::endl;
    }

    std::cout << "=============================================" << std::endl;

    return 0;
}
