// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

std::wstring MenuEntry::to_wstring() const
{
    if (comment.empty())
    {
        return code;
    }

    return code + L" - " + comment;
}


NAMESPACE_IRBIS_END
