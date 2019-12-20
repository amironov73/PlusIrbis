// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

std::vector<TermInfo> TermInfo::parse(const StringList &lines)
{
    std::vector<TermInfo> result;
    result.reserve(lines.size());
    for (const auto &line : lines) {
        if (!line.empty()) {
            const auto parts = maxSplit(line, L'#', 2);
            TermInfo term;
            term.count = fastParse32(parts[0]);
            if (parts.size() > 1) {
                term.text = parts[1];
            }
            result.push_back(term);
        }
    }

    return result;

}

std::wstring TermInfo::toString() const
{
    return std::to_wstring(this->count) + std::wstring(L"#")  + this->text;
}

}
