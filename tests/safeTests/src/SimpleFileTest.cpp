// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

TEST_CASE("SimpleFile_create_1", "[file]")
{
    const auto tempDirectory = irbis::IO::getTempDirectory();
    REQUIRE (irbis::IO::directoryExist (tempDirectory));
    auto fileName = irbis::IO::combinePath (tempDirectory, L"irbis.tmp");
    irbis::IO::deleteFile (fileName);

    irbis::Byte original[] {3, 14, 15, 9, 26, 5};

    {
        auto file = irbis::SimpleFile::create (fileName);
        file.write (original, irbis::size (original));
        file.close();
    }

    CHECK (irbis::IO::getFileSize (fileName) == irbis::size (original));

    {
        irbis::Byte buffer [10];
        auto file = irbis::SimpleFile::openRead (fileName);
        const auto read = file.read (buffer, irbis::size (buffer));
        CHECK (read == irbis::size (original));
        file.close();
    }

    irbis::IO::deleteFile (fileName);
}

TEST_CASE("SimpleFile_openRead_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"org.mnu");
    irbis::IO::convertSlashes (path);
    irbis::Byte buffer[100];
    auto file = irbis::SimpleFile::openRead (path);
    auto position = file.tell();
    CHECK (position == 0);
    auto read = file.read (buffer, irbis::size (buffer));
    CHECK (read == 86);
    CHECK (buffer[0] == 0x31);
    CHECK (buffer[1] == 0x0D);
    position = file.tell();
    CHECK (position == 86);
    file.seek (0);
    position = file.tell();
    CHECK (position == 0);
    file.close();
}

TEST_CASE("SimpleFile_openWrite_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"org.mnu");
    irbis::IO::convertSlashes (path);
    auto file = irbis::SimpleFile::openWrite (path);
    auto position = file.tell();
    CHECK (position == 0);
    file.seek (86);
    position = file.tell();
    CHECK (position == 86);
    file.close();
}
