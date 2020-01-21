// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

void ParFile::assign (const String &path)
{
    assert(!path.empty());

    this->mst = path;
    this->xrf = path;
    this->cnt = path;
    this->l01 = path;
    this->l02 = path;
    this->n01 = path;
    this->n02 = path;
    this->ifp = path;
    this->any = path;
    this->pft = path;
    this->ext = path;
}

void ParFile::parse(const StringList &lines)
{
    std::map<int, String> dict;

    for (const auto &line : lines) {
        const auto parts = maxSplit(line, L'=', 2);
        const auto key = fastParse32(trim(parts[0]));
        const auto value = trim(parts[1]);
        dict[key] = value;
    }

    this->xrf = dict[1];
    this->mst = dict[2];
    this->cnt = dict[3];
    this->n01 = dict[4];
    this->n02 = dict[5];
    this->l01 = dict[6];
    this->l02 = dict[7];
    this->ifp = dict[8];
    this->any = dict[9];
    this->pft = dict[10];
    if (dict.find(11) != end(dict)) {
        this->ext = dict[11];
    }
}

std::map<int, String> ParFile::toDictionary() const
{
    std::map<int, String> result;

    result[1] = this->xrf;
    result[2] = this->mst;
    result[3] = this->cnt;
    result[4] = this->n01;
    result[5] = this->n02;
    result[6] = this->l01;
    result[7] = this->l02;
    result[8] = this->ifp;
    result[9] = this->any;
    result[10] = this->pft;
    result[11] = this->ext;

    return result;
}

}
