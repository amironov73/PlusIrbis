// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

std::wstring MenuEntry::toString() const
{
    if (comment.empty())
    {
        return code;
    }

    return code + L" - " + comment;
}


}
