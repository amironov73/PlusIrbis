// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

void OptFile::parse(const StringList &text) {
    for (const auto &line : text) {
        const auto parts = maxSplit(line, L' ', 2);
        const auto key = parts[0];
        const auto value = trim(parts[1]);
        OptLine optLine { key, value };
        this->lines.push_back(optLine);
    }
}

}
