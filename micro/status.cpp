// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "micro.h"

namespace irbis
{
    RecordStatus operator | (RecordStatus left, RecordStatus right)
    {
        return static_cast <RecordStatus > (
            static_cast <std::underlying_type<RecordStatus>::type> (left)
            | static_cast <std::underlying_type<RecordStatus>::type> (right)
        );
    }

    RecordStatus operator & (RecordStatus left, RecordStatus right)
    {
        return static_cast <RecordStatus > (
            static_cast <std::underlying_type<RecordStatus>::type> (left)
            & static_cast <std::underlying_type<RecordStatus>::type> (right)
        );
    }

}
