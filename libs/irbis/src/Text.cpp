// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

const char Text::CrLf[] = { 0x0D, 0x0A };
const char Text::Lf[] = { 0x0A };

/// \brief Разделитель строк в ИРБИС.
const String Text::IrbisDelimiter = L"\u001F\u001E";

/// \brief Короткая версия разделителя строк.
const String Text::ShortDelimiter = L"\u001E";

/// \brief Разделитель строк в MSDOS.
const String Text::MsDosDelimiter = L"\r\n";

/// \brief Разделитель строк в UNIX.
const String Text::UnixDelimiter = L"\n";

/// \brief Разделитель элементов в строке ответа на поисковый запрос.
const String Text::SearchDelimiter = L"#";

/// \brief Замена разделителей текста с ИРБИС на MS-DOS.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
String Text::fromIrbisToDos (String &text)
{
    return replace (text, IrbisDelimiter, MsDosDelimiter);
}

/// \brief Замена разделителей текста с ИРБИС на MS-DOS.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
std::string Text::fromIrbisToUnix (std::string &text)
{
    return replace (text, "\x1F\x1E", "\n");
}

/// \brief Замена разделителей текста с ИРБИС на UNIX.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
String Text::fromIrbisToUnix (String &text)
{
    return replace (text, IrbisDelimiter, UnixDelimiter);
}

/// \brief Замена разделителей текста с MS-DOS на ИРБИС.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
String Text::fromDosToIrbis (String &text)
{
    return replace (text, MsDosDelimiter, IrbisDelimiter);
}

/// \brief Замена разделителей текста с MS-DOS на ИРБИС.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
String Text::fromUnixToIrbis (String &text)
{
    return replace (text, UnixDelimiter, IrbisDelimiter);
}

/// \brief Замена разделителей текста с MS-DOS на UNIX.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
String Text::fromDosToUnix (String &text)
{
    return replace (text, MsDosDelimiter, UnixDelimiter);
}

/// \brief Замена разделителей текста с UNIX на MS-DOS.
/// \param text Текст, подлежащий обработке.
/// \return Обработанный `text`.
String Text::fromUnixToDos (String &text)
{
    return replace (text, UnixDelimiter, MsDosDelimiter);
}

/// \brief Разбивка ответа сервера по строкам (полный вариант разделителя).
/// \param text Текст, подлежащий обработке.
/// \return Вектор строк. Содержит, кроме прочего, пустые строки.
StringList Text::fromFullDelimiter (const String &text)
{
    return split (text, IrbisDelimiter);
}

/// \brief Разбивка ответа сервера по строкам (сокращенный вариант разделителя).
/// \brief text Текст, подлежащий обработке.
/// \return Вектор строк. Содержит, кроме прочего, пустые строки.
///
/// Строки, приходящие в ответ на команду WriteRecord.
StringList Text::fromShortDelimiter (const String &text)
{
    return split (text, L'\u001E');
}

/// \brief Считываем весь файл как строку в кодировке ANSI.
/// \param filename Имя файла.
/// \return Прочитанный файл.
String Text::readAllAnsi (const String &filename)
{
    auto temp = File::readAllBytes (filename);
    auto result = cp1251_to_unicode (temp);
    return fromDosToUnix (result);
}

/// \brief Считываем весь файл как строку в кодировке UTF-8.
/// \param filename Имя файла.
/// \return Прочитанный файл.
String Text::readAllUtf (const String &filename)
{
    auto temp = File::readAllBytes (filename);
    auto result = fromUtf (temp);
    return fromDosToUnix (result);
}

/// \brief Считываем весь файл как массив строк в кодировке ANSI.
/// \param filename Имя файла.
/// \return Вектор строк.
StringList Text::readAnsiLines (const String &filename)
{
    StringList result;

    {
        File file (filename, L"rb");
        while (!file.eof()) {
            auto line = file.readLine();
            auto item = cp1251_to_unicode (line);
            result.push_back (item);
        }
    }

    return result;
}

/// \brief Считываем весь файл как массив строк в кодировке UTF-8.
/// \param filename Имя файла.
/// \return Вектор строк.
StringList Text::readUtfLines(const String &filename)
{
    StringList result;

    {
        File file (filename, L"rb");
        while (!file.eof()) {
            auto line = file.readLine();
            auto item = fromUtf (line);
            result.push_back (item);
        }
    }

    return result;
}

}
