// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

// ReSharper disable StringLiteralTypo

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

const std::string AlphabetTable::FileName = "ISISACW.TAB";

AlphabetTable::AlphabetTable(const std::vector<unsigned char>& bytes)
{
    throw NotImplementedException();
}

static unsigned char _bytes[] =
{
     38,  64,  65,  66,  67,  68,  69,  70,  71,  72,
     73,  74,  75,  76,  77,  78,  79,  80,  81,  82,
     83,  84,  85,  86,  87,  88,  89,  90,  97,  98,
     99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
    109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
    119, 120, 121, 122, 128, 129, 130, 131, 132, 133,
    134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153,
    154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
    164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
    174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
    184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
    194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
    204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
    214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
    244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
    254, 255
};

static AlphabetTable *_instance;

const AlphabetTable& AlphabetTable::instance()
{
    throw NotImplementedException();
}

bool AlphabetTable::isAlpha(const wchar_t &c) const
{
    throw NotImplementedException();
}

AlphabetTable AlphabetTable::parse(std::istream &stream)
{
    throw NotImplementedException();
}

AlphabetTable AlphabetTable::readLocalFile(const std::string &fileName)
{
    throw NotImplementedException();
}

std::wstring AlphabetTable::trimText(const std::wstring &text) const
{
    if (text.empty()) {
        return text;
    }

    if (isAlpha(text.at(0)) && isAlpha(text.at(text.size()-1))) {
        return text;
    }

    std::wstring result = text;
    while (result.length() && !isAlpha(result.at(0))) {
        // result.remove(0, 1);
    }

    while (result.length() && !isAlpha(result.at(result.size() - 1))) {
        // result.remove(result.size() - 1, 1);
    }

    return result;
}

std::vector<std::wstring> AlphabetTable::splitWords(const std::wstring &text) const
{
    std::vector<std::wstring> result;

    if (text.empty()) {
        return result;
    }

    std::wstring accumulator;
    for (const wchar_t &c : text) {
        if (isAlpha(c)) {
            accumulator.push_back(c);
        } else {
            if (!accumulator.empty()) {
                result.push_back(accumulator);
                accumulator.clear();
            }
        }
    }

    if (!accumulator.empty()) {
        result.push_back(accumulator);
    }

    return result;
}

bool AlphabetTable::verify(bool throwOnError) const
{
    bool result = !characters.empty();
    if (!result && throwOnError) {
        throw VerificationException();
    }

    return result;
}

}
