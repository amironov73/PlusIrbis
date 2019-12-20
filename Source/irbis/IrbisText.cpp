// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

const char IrbisText::CrLf[] = { 0x0D, 0x0A };
const char IrbisText::Lf[] = { 0x0A };

// Разделитель строк в ИРБИС.
const std::wstring IrbisText::IrbisDelimiter = L"\u001F\u001E";

// Короткая версия разделителя строк.
const std::wstring IrbisText::ShortDelimiter = L"\u001E";

//Разделитель строк в MSDOS.
const std::wstring IrbisText::MsDosDelimiter = L"\r\n";

// Разделитель строк в UNIX.
const std::wstring IrbisText::UnixDelimiter = L"\n";

// Разделитель элементов в строке ответа на поисковый запрос.
const std::wstring IrbisText::SearchDelimiter = L"#";

// Замена разделителей текста с ИРБИС на MS-DOS.
std::wstring IrbisText::fromIrbisToDos (std::wstring &text)
{
    return replace(text, IrbisDelimiter, MsDosDelimiter);
}

// Замена разделителей текста с ИРБИС на UNIX.
std::wstring IrbisText::fromIrbisToUnix (std::wstring &text)
{
    return replace(text, IrbisDelimiter, UnixDelimiter);
}

// Замена разделителей текста с MS-DOS на ИРБИС.
std::wstring IrbisText::fromDosToIrbis (std::wstring &text)
{
    return replace(text, MsDosDelimiter, IrbisDelimiter);
}

// Замена разделителей текста с MS-DOS на UNIX.
std::wstring IrbisText::fromDosToUnix(std::wstring &text)
{
    return replace(text, MsDosDelimiter, UnixDelimiter);
}

// Разбивка ответа сервера по строкам (полный вариант разделителя).
StringList IrbisText::fromFullDelimiter (const std::wstring &text)
{
    throw NotImplementedException();
    //return text.split(IrbisDelimiter);
}

// Строки, приходящие в ответ на команду WriteRecord.
StringList IrbisText::fromShortDelimiter(const std::wstring &text)
{
    return split(text, L'\u001E');
}

// Считываем весь файл как строку в кодировке ANSI.
std::wstring IrbisText::readAllAnsi(const std::wstring &filename)
{
    throw NotImplementedException();
    //QFile file(filename);
    //if (!file.open(QIODevice::ReadOnly)) {
    //    throw IrbisException();
    //}

    //QByteArray bytes = file.readAll();
    //std::wstring result = IrbisEncoding::ansi()->toUnicode(bytes);

    //return result;
}

// Считываем весь файл как строку в кодировке UTF-8.
std::wstring IrbisText::readAllUtf(const std::wstring &filename)
{
    throw NotImplementedException();
    //QFile file(filename);
    //if (!file.open(QIODevice::ReadOnly)) {
    //    throw IrbisException();
    //}

    //QByteArray bytes = file.readAll();
    //std::wstring result = IrbisEncoding::ansi()->toUnicode(bytes);

    //return result;
}

// Считываем весь файл как массив строк в кодировке ANSI.
StringList IrbisText::readAnsiLines(const std::wstring &filename)
{
    throw NotImplementedException();
    //QFile file(filename);
    //if (!file.open(QIODevice::ReadOnly)) {
    //    throw IrbisException();
    //}

    //QStringList result;
    //QTextCodec *ansi = IrbisEncoding::ansi();
    //while (true) {
    //    QByteArray bytes = file.readLine();
    //    if (bytes.isEmpty()) {
    //        break;
    //    }

    //    std::wstring line = ansi->toUnicode(bytes);
    //    result.append(line);
    //}

    //return result;
}

// Считываем весь файл как массив строк в кодировке UTF-8.
StringList IrbisText::readUtfLines(const std::wstring &filename)
{
    throw NotImplementedException();
    //QFile file(filename);
    //if (!file.open(QIODevice::ReadOnly)) {
    //    throw IrbisException();
    //}

    //QStringList result;
    //QTextCodec *utf = IrbisEncoding::utf();
    //while (true) {
    //    QByteArray bytes = file.readLine();
    //    if (bytes.isEmpty()) {
    //        break;
    //    }

    //    std::wstring line = utf->toUnicode(bytes);
    //    result.append(line);
    //}

    //return result;
}

}
