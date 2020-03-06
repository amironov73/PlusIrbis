// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <cassert>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

/*!
    \file ParFile.cpp

    \class irbis::ParFile

    Каждой базе данных ИРБИС соответствует один .par-файл.
    Этот файл содержит набор путей к файлам базы данных ИРБИС.
    Имя .par-файла соответствует имени базы данных.

    .par-файл представляет собой текстовый файл, состоящий
    из 11 строк. Каждая строка представляет собой путь,
    указывающий местонахождение соответствующих файлов базы данных.
    Примечание: до версии 2011.1 включительно .par-файлы включают
    в себя 10 строк. 11-я строка добавлена в версии 2012.1.

    В исходном состоянии системы .par-файл содержит относительные
    пути размещения файлов базы данных – относительно основной
    директории системы <IRBIS_SERVER_ROOT>.

    Фактически в ИРБИС принят принцип хранения всех файлов
    базы данных в одной папке, поэтому .par-файлы содержат
    один и тот же путь, повторяющийся в каждой строке.

    Как правило, PAR-файлы располагаются в подпапке DataI внутри
    папки IRBIS64, в которую установлен сервер ИРБИС
    (но их расположение может быть переопределено параметром
    DataPath в irbis_server.ini).

    Пример файла IBIS.PAR:

    ```
    1=.\datai\ibis\
    2=.\datai\ibis\
    3=.\datai\ibis\
    4=.\datai\ibis\
    5=.\datai\ibis\
    6=.\datai\ibis\
    7=.\datai\ibis\
    8=.\datai\ibis\
    9=.\datai\ibis\
    10=.\datai\ibis\
    11=f:\webshare\
    ```

    ```
    Параметр | Назначение
    ---------|-----------
           1 | Путь к файлу XRF
           2 | MST
           3 | CNT
           4 | N01
           5 | N02 (только для ИРБИС32)
           6 | L01
           7 | L02 (только для ИРБИС32)
           8 | IFP
           9 | ANY
          10 | FDT, FST, FMT, PFT, STW, SRT
          11 | появился в версии 2012:
             | расположение внешних объектов (поле 951)
    ```

 */

namespace irbis {

/// \brief Присваивание пути.
/// \param path Предполагается, что все файлы базы располагаются по указанному пути.
void ParFile::assign (const String &path)
{
    assert (!path.empty());

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

/// \brief Разбор ответа сервера.
/// \param lines Строки с ответом сервера.
void ParFile::parse (const StringList &lines)
{
    std::map<int, String> dict;

    for (const auto &line : lines) {
        if (line.empty()) {
            continue;
        }
        const auto parts = maxSplit (line, L'=', 2);
        if (parts.size() != 2) {
            continue;
        }
        const auto key = fastParse32 (trim(parts[0]));
        const auto value = trim (parts[1]);
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
    if (dict.find (11) != end (dict)) {
        this->ext = dict[11];
    }
}

/// \brief Превращение в словарь.
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

/// \brief Чтение из локального файла.
/// \param path
/// \return
ParFile ParFile::readLocalFile (const String &path)
{
    assert (!path.empty());
    const auto lines = Text::readAnsiLines (path);
    ParFile result;
    result.parse (lines);
    return result;
}

}
