// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_private.h"

#include <random>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

/// \brief Получение списка терминов в текущей базе данных с указанным префиксом.
/// \param prefix Префикс, для которого строится список терминов (например, "T=")
/// \return Термины, очищенные от префикса.
/// \warning Для больших баз данных выполнение операции может потребовать
/// много времени и занять слишком много оперативной памяти.
StringList ConnectionSearch::listTerms(const String &prefix)
{
    StringList result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto prefixSize = prefix.size();
    String startTerm = prefix;
    String lastTerm = startTerm;
    while (true) {
        const auto terms = this->readTerms(startTerm, 512);
        if (terms.empty()) {
            break;
        }

        for (const auto &term : terms) {
            String text = term.text;
            const auto begin = text.substr(0, prefixSize);
            if (begin != prefix) {
                goto DONE;
            }
            if (text != startTerm) {
                lastTerm = text;
                text = text.substr(prefixSize);
                result.push_back(text);
            }
        }
        startTerm = lastTerm;
    }

    DONE:
    return result;
}

std::vector<TermPosting> ConnectionSearch::readPostings (const PostingParameters &parameters)
{
    std::vector<TermPosting> result;

    if (!this->_checkConnection()) {
        return result;
    }

    const auto db = choose (parameters.database, this->database); // NOLINT(performance-unnecessary-copy-initialization)
    ClientQuery query (*this, "I");
    query.addAnsi (db).newLine();
    query.add (parameters.numberOfPostings).newLine();
    query.add (parameters.firstPosting).newLine();
    query.addFormat (parameters.format);
    if (!parameters.listOfTerms.empty()) {
        query.addUtf (parameters.term).newLine();
    } else {
        for (const auto &term : parameters.listOfTerms) {
            query.addUtf (term).newLine();
        }
    }

    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }

    const auto lines = response.readRemainingUtfLines();
    result = TermPosting::parse(lines);

    return result;
}

std::vector<TermInfo> ConnectionSearch::readTerms(const String &startTerm, int numberOfTerms = 100)
{
    auto parameters = TermParameters();
    parameters.startTerm = startTerm;
    parameters.numberOfTerms = numberOfTerms;

    return this->readTerms(parameters);
}

std::vector<TermInfo> ConnectionSearch::readTerms(const TermParameters &parameters)
{
    std::vector<TermInfo> result;

    if (!this->_checkConnection()) {
        return result;
    }

    const std::string command = parameters.reverseOrder ? "P" : "H";
    const auto db = choose (parameters.database, this->database);
    ClientQuery query (*this, command);
    query.addAnsi (db).newLine();
    query.addUtf (parameters.startTerm).newLine();
    query.add (parameters.numberOfTerms).newLine();
    query.addFormat (parameters.format);
    ServerResponse response (*this, query);
    if (!response.success()) {
        return result;
    }
    if (!response.checkReturnCode (3, -202, -203, -204)) {
        return result;
    }

    const auto lines = response.readRemainingUtfLines();
    result = TermInfo::parse (lines);

    return result;
}

MfnList ConnectionSearch::search (const Search &search)
{
    SearchParameters parameters {};
    parameters.database = this->database;
    parameters.searchExpression = search.toString();
    parameters.numberOfRecords = 0;
    parameters.firstRecord = 1;

    return this->search(parameters);
}

MfnList ConnectionSearch::search (const String &expression)
{
    SearchParameters parameters {};
    parameters.database = this->database;
    parameters.searchExpression = expression;
    parameters.numberOfRecords = 0;
    parameters.firstRecord = 1;

    return this->search(parameters);
}

MfnList ConnectionSearch::search (const SearchParameters &parameters)
{
    MfnList result {};
    if (!this->_checkConnection()) {
        return result;
    }

    const auto &databaseName = choose (parameters.database, this->database);
    ClientQuery query (*this, "K");
    query.addAnsi (databaseName).newLine()
            .addUtf (parameters.searchExpression).newLine()
            .add (parameters.numberOfRecords).newLine()
            .add (parameters.firstRecord).newLine()
            .addAnsi (parameters.formatSpecification).newLine()
            .add (parameters.minMfn).newLine()
            .add (parameters.maxMfn).newLine()
            .addAnsi (parameters.sequentialSpecification);

    ServerResponse response (*this, query);
    if (!response.checkReturnCode()) {
        return result;
    }
    const auto expected = response.readInteger();
    auto batchSize = 32000; // MAXPACKET
    if (expected < batchSize) {
        batchSize = expected;
    }
    for (auto i = 0; i < batchSize; i++) {
        const auto line = response.readAnsi();
        StringList parts = maxSplit(line, L'#', 2);
        const auto mfn = fastParse32(parts[0]);
        result.push_back(mfn);
    }

    return result;
}

}
