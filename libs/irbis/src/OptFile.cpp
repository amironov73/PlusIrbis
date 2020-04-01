// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

/// \brief Разбор ответа сервера.
/// \param text Строки с ответом сервера.
void OptFile::parse (const StringList &text) {
    auto index = 0;
    for (const auto &line : text) {
        if (line.empty()) {
            break;
        }
        if (line[0] == L'*') {
            break;
        }
        ++index;
        if (index == 1) {
            this->tag = fastParse32 (line);
        }
        else if (index == 2) {
            this->length = fastParse32 (line);
        }
        else {
            const auto parts = maxSplit (line, L' ', 2);
            const auto key = parts[0];
            const auto value = trim (parts[1]);
            OptLine optLine { key, value };
            this->lines.push_back (optLine);
        }
    }
}

}
