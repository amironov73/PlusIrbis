// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <sys/stat.h>
#include <fcntl.h>

#ifdef IRBIS_WINDOWS
#include <cwchar>
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

MarcRecord* Iso2709::readRecord(FILE *device, Encoding *encoding)
{
	throw NotImplementedException();
}

void Iso2709::writeRecord(FILE *device, const MarcRecord &record, Encoding *encoding)
{
	throw NotImplementedException();
}

}
