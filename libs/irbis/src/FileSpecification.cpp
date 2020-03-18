// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

///
/// \class irbis::FileSpecification
///
/// \details Путь на файл `Αpath.Adbn.Afilename`.
///
///  `Αpath` – код путей:
///  * 0 – общесистемный путь.
///  * 1 – путь размещения сведений о базах данных сервера ИРБИС64
///  * 2 – путь на мастер-файл базы данных.
///  * 3 – путь на словарь базы данных.
///  * 10 – путь на параметрию базы данных.
///
///  `Adbn` – имя базы данных.
///
///  `Afilename` – имя требуемого файла с расширением.
///
///  В случае чтения ресурса по пути 0 и 1 имя базы данных не задается.
///

namespace irbis {

/// \brief Конструктор для спецификации системных файлов (вне баз данных).
/// \param path_ Код пути.
/// \param filename_ Имя файла (не пустое).
FileSpecification::FileSpecification(IrbisPath path_, const String &filename_)  // NOLINT(modernize-pass-by-value)
    : path(path_), filename(filename_)
{
}

/// \brief Конструктор для спецификации файлов в контексте определенной базы данных.
/// \param path_ Код пути.
/// \param database_ Имя базы данных (не пустое).
/// \param filename_ Имя файла (не пустое).
FileSpecification::FileSpecification(IrbisPath path_, const String &database_, const String &filename_)  // NOLINT(modernize-pass-by-value)
    : path(path_), database(database_), filename(filename_)
{
}

/// \brief Разбор текстового представления спецификации файла.
/// \param text Текстовое представление.
/// \return Разобранная спецификация.
FileSpecification FileSpecification::parse(const String &text)
{
    TextNavigator navigator(text);
    const auto path = static_cast<IrbisPath> (fastParse32(navigator.readTo('.')));
    const auto database = navigator.readTo('.').toString();
    auto fileName = navigator.remainingText().toString();
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

/// \brief Верификация спецификации файла.
/// \param throwException Бросать ли исключение при обнаружении ошибки?
/// \return Результат верификации.
bool FileSpecification::verify (bool throwException) const
{
    auto result = !filename.empty();

    result = result && ((path != IrbisPath::System) && (path != IrbisPath::Data)) == !database.empty();

    if (!result && throwException) {
        throw VerificationException();
    }

    return result;
}

/// \brief Строковое представление спецификации файла.
/// \return Строковое представление.
String FileSpecification::toString() const
{
    String result = this->filename;

    if (this->binaryFile) {
        result = L"@" + result;
    } else {
        if (!this->content.empty()) {
            result = result + L"&" + this->content;
        }
    }

    switch (this->path) {
    case IrbisPath::System:
    case IrbisPath::Data:
        result = std::to_wstring (static_cast<int> (this->path))
                + L".." + result;
        break;

    default:
        result = std::to_wstring (static_cast<int> (this->path))
                + L"." + this->database + L"." + result;
        break;
    }

    return result;
}

}
