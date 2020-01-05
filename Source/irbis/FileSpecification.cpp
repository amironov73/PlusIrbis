// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

FileSpecification::FileSpecification(int path_, const String &filename_)  // NOLINT(modernize-pass-by-value)
    : path(path_), filename(filename_)
{
}

FileSpecification::FileSpecification(int path_, const String &database_, const String &filename_)  // NOLINT(modernize-pass-by-value)
    : path(path_), database(database_), filename(filename_)
{
}

FileSpecification FileSpecification::parse(const String &text)
{
    TextNavigator navigator(text);
    const auto path = fastParse32(navigator.readTo('.'));

    const auto database = navigator.readTo('.');
    auto fileName = navigator.remainingText();
    const auto binaryFile = fileName[0] == L'@';
    if (binaryFile) {
        fileName = fileName.substr(1);
    }

    String content;
    const auto position = fileName.find(L'&');
    if (position != String::npos)
    {
        content = fileName.substr(position + 1);
        fileName = fileName.substr(0, position);
    }

    FileSpecification result;
    result.binaryFile = binaryFile;
    result.path = path;
    result.database = database;
    result.filename = fileName;
    result.content = content; //-V820

    return result;
}

bool FileSpecification::verify(bool throwException) const
{
    auto result = !filename.empty();

    if ((path != IrbisPath::System) && (path != IrbisPath::Data)) {
        result = result && !database.empty();
    }

    if (!result && throwException) {
        throw VerificationException();
    }

    return result;
}

std::wstring FileSpecification::toString() const
{
    String result = filename;

    if (binaryFile) {
        result = L"@" + result;
    } else {
        if (!content.empty()) {
            result = L"&" + result;
        }
    }

    switch (path) {
    case 0:
    case 1:
        result = std::to_wstring(path) + L".." + result;
        break;

    default:
        result = std::to_wstring(path) + L"." + database + L"." + result;
        break;
    }

//    if (!content.empty()) {
  //      QString copy(content);
    //    IrbisText::fromDosToIrbis(copy);
      //  result = result + "&" + copy;
    //}

    return result;
}


}
