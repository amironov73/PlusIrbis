// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_internal.h"

/*!
    \var irbis::TermPosting::mfn
    \brief MFN записи с искомым термином.

    \var irbis::TermPosting::tag
    \brief Метка поля с искомым термином.

    \var irbis::TermPosting::occurrence
    \brief Номер повторения поля.

    \var irbis::TermPosting::count
    \brief Смещение (номер слова) в повторении поля.

    \var irbis::TermPosting::text
    \brief Результат форматирования (если есть).

 */

namespace irbis {

/// \brief Разбор ответа сервера.
/// \param lines Ответ сервера.
/// \return Вектор постингов.
std::vector<TermPosting> TermPosting::parse (const StringList &lines)
{
    std::vector<TermPosting> result;
    result.reserve(lines.size());
    for (const auto &line : lines) {
        const auto parts = maxSplit(line, L'#', 5);
        if (parts.size() < 4) {
            break;
        }
        TermPosting posting;
        posting.mfn        = fastParse32 (parts[0]);
        posting.tag        = fastParse32 (parts[1]);
        posting.occurrence = fastParse32 (parts[2]);
        posting.count      = fastParse32 (parts[3]);
        if (parts.size() > 4) {
            posting.text = parts[4];
        }
        result.push_back(posting);
    }

    return result;
}

/// \brief Текстовое представление постинга.
/// \return Текстовое представление.
std::wstring TermPosting::toString() const
{
    return std::to_wstring (this->mfn)
        + std::wstring (L"#")
        + std::to_wstring (this->tag)
        + std::wstring (L"#")
        + std::to_wstring (this->occurrence)
        + std::wstring (L"#")
        + std::to_wstring (this->count)
        + std::wstring (L"#")
        + this->text;
}

}
