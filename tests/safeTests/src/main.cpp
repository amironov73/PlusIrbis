// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "irbis.h"
#include "irbis_internal.h"
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

/// \brief Где у нас лежит ИРБИС64.
/// \return Путь к папке `Irbis64` (включая её саму).
irbis::String whereIrbis64()
{
    auto result = whereTestData();
    result = irbis::IO::combinePath(result, L"Irbis64");
    irbis::IO::convertSlashes(result);
    return result;
}

/// \brief Где у нас лежит `Datai` для ИРБИС64.
/// \return Путь к папке `Datai` (включая её саму).
irbis::String whereDatai()
{
    auto result = whereIrbis64();
    result = irbis::IO::combinePath(result, L"Datai");
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

/// \brief Где у нас временная директория.
/// \return Путь ко временной папке.
irbis::String whereTemp()
{
    return irbis::IO::getTempDirectory();
}

TEST_CASE("where am i", "[env]")
{
    std::wcout << L"current: " << irbis::IO::getCurrentDirectory() << std::endl;
    std::wcout << L"temp:    " << whereTemp() << std::endl;
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
