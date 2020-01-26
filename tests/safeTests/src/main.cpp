#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

#include <iostream>
#include <string>

/// \brief Где у нас лежат данные для тестов.
/// \return Путь к папке `testData` (включая её саму).
irbis::String whereTestData()
{
    auto result = irbis::IO::getCurrentDirectory();
    while (true) {
        if (result.empty()) {
            throw irbis::IrbisException();
        }
        auto candidate = irbis::IO::combinePath(result, L"testData");
        if (irbis::IO::directoryExist(candidate)) {
            return irbis::IO::convertSlashes(candidate);
        }
        result = irbis::IO::getDirectory(result);
    }
}

/// \brief Где у нас лежит `Datai` для ИРБИС64.
/// \return Путь к папке `Datai` (включая её саму).
irbis::String whereDatai()
{
    auto result = whereTestData();
    result = irbis::IO::combinePath(result, L"Irbis64/Datai");
    irbis::IO::convertSlashes(result);
    return result;
}

/// \brief Где у нас лежит `Deposit` для ИРБИС64.
/// \return Путь к папке `Deposit` (включая её саму).
irbis::String whereDeposit()
{
    auto result = whereDatai();
    result = irbis::IO::combinePath(result, L"Deposit");
    irbis::IO::convertSlashes(result);
    return result;
}

/// \brief Где у нас лежит `IBIS` для ИРБИС64.
/// \return Путь к папке `IBIS` (включая её саму).
irbis::String whereIbis()
{
    auto result = whereDatai();
    result = irbis::IO::combinePath(result, L"IBIS");
    irbis::IO::convertSlashes(result);
    return result;
}

TEST_CASE("where am i", "[env]")
{
    std::wcout << L"current: " << irbis::IO::getCurrentDirectory() << std::endl;
    std::wcout << L"tests:   " << whereTestData() << std::endl;
    std::wcout << L"Datai:   " << whereDatai()    << std::endl;
    std::wcout << L"IBIS:    " << whereIbis()     << std::endl;
}

TEST_CASE("libraryVersion_1", "[version]")
{
    REQUIRE(irbis::libraryVersion() != 0);
}

TEST_CASE("libraryVersionString_1", "[version]")
{
    REQUIRE(!irbis::libraryVersionString().empty());
}
