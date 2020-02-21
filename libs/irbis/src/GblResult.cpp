// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

/// \brief Разбор ответа сервера.
/// \param lines Вектор строк.
void GblResult::parse (const StringList& lines)
{
    for (const auto &line : lines) {
        if (!line.empty()) {
            throw NotImplementedException();
        }
    }
}


}
