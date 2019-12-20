// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

namespace irbis {

const int XrfRecord64::RecordSize = sizeof(long long int) + sizeof (int);

bool XrfRecord64::deleted() const
{
    return (this->status & RecordStatus::Deleted) != 0;
}

bool XrfRecord64::locked() const
{
    return (this->status & RecordStatus::Locked) != 0;
}

std::wstring XrfRecord64::toString() const
{
    throw NotImplementedException();
}

}
