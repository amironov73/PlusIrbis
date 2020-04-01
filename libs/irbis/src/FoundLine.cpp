// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

/// \class irbis::FoundLine
/// \details Содержит MFN найденной записи и опциональный текст,
/// например, результат расформатирования,
/// разделённые символом '#'.

namespace irbis {

/// \brief Разбор строки.
void FoundLine::parse (const String &line)
{
    const auto parts = maxSplit (line, L'#', 2);
    this->mfn = fastParse32 (parts[0]);
    this->description = safeAt (parts, 1);
}


}
