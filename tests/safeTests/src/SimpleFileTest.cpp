// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "bugprone-infinite-loop"

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

TEST_CASE("SimpleFile_create_2", "[file]")
{
    const auto tempDirectory = irbis::IO::getTempDirectoryNarrow();
    REQUIRE (irbis::IO::directoryExist (tempDirectory));
    auto fileName = irbis::IO::combinePath (tempDirectory, "irbis.tmp");
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

TEST_CASE("SimpleFile_insistCreate_1", "[file]")
{
    const auto tempDirectory = irbis::IO::getTempDirectory();
    REQUIRE (irbis::IO::directoryExist (tempDirectory));
    auto fileName = irbis::IO::combinePath (tempDirectory, L"irbis.tmp");
    irbis::IO::deleteFile (fileName);

    irbis::Byte original[] {3, 14, 15, 9, 26, 5};

    {
        auto file = irbis::SimpleFile::insistCreate (fileName);
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

TEST_CASE("SimpleFile_insistCreate_2", "[file]")
{
    const auto tempDirectory = irbis::IO::getTempDirectoryNarrow();
    REQUIRE (irbis::IO::directoryExist (tempDirectory));
    auto fileName = irbis::IO::combinePath (tempDirectory, "irbis.tmp");
    irbis::IO::deleteFile (fileName);

    irbis::Byte original[] {3, 14, 15, 9, 26, 5};

    {
        auto file = irbis::SimpleFile::insistCreate (fileName);
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
    path = irbis::IO::combinePath (path, L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    irbis::Byte buffer[100];
    auto file = irbis::SimpleFile::openRead (path);
    auto position = file.tell();
    CHECK (position == 0);
    auto read = file.read (buffer, irbis::size (buffer));
    CHECK (read == 100);
    CHECK (buffer[0] == 0x30);
    CHECK (buffer[1] == 0x30);
    position = file.tell();
    CHECK (position == 100);
    file.seek (0);
    position = file.tell();
    CHECK (position == 0);
    file.close();
}

TEST_CASE("SimpleFile_openRead_2", "[file]")
{
    auto path = irbis::toUtf (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "TEST1.ISO");
    irbis::IO::convertSlashes (path);
    irbis::Byte buffer[100];
    auto file = irbis::SimpleFile::openRead (path);
    auto position = file.tell();
    CHECK (position == 0);
    auto read = file.read (buffer, irbis::size (buffer));
    CHECK (read == 100);
    CHECK (buffer[0] == 0x30);
    CHECK (buffer[1] == 0x30);
    position = file.tell();
    CHECK (position == 100);
    file.seek (0);
    position = file.tell();
    CHECK (position == 0);
    file.close();
}

TEST_CASE("SimpleFile_insistOpenRead_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    irbis::Byte buffer[100];
    auto file = irbis::SimpleFile::insistOpenRead (path);
    auto position = file.tell();
    CHECK (position == 0);
    auto read = file.read (buffer, irbis::size (buffer));
    CHECK (read == 100);
    CHECK (buffer[0] == 0x30);
    CHECK (buffer[1] == 0x30);
    position = file.tell();
    CHECK (position == 100);
    file.seek (0);
    position = file.tell();
    CHECK (position == 0);
    file.close();
}

TEST_CASE("SimpleFile_insistOpenRead_2", "[file]")
{
    auto path = irbis::toUtf (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "TEST1.ISO");
    irbis::IO::convertSlashes (path);
    irbis::Byte buffer[100];
    auto file = irbis::SimpleFile::insistOpenRead (path);
    auto position = file.tell();
    CHECK (position == 0);
    auto read = file.read (buffer, irbis::size (buffer));
    CHECK (read == 100);
    CHECK (buffer[0] == 0x30);
    CHECK (buffer[1] == 0x30);
    position = file.tell();
    CHECK (position == 100);
    file.seek (0);
    position = file.tell();
    CHECK (position == 0);
    file.close();
}

TEST_CASE("SimpleFile_openWrite_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    auto file = irbis::SimpleFile::openWrite (path);
    auto position = file.tell();
    CHECK (position == 0);
    file.seek (86);
    position = file.tell();
    CHECK (position == 86);
    file.close();
}

TEST_CASE("SimpleFile_openWrite_2", "[file]")
{
    auto path = irbis::toUtf (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "TEST1.ISO");
    irbis::IO::convertSlashes (path);
    auto file = irbis::SimpleFile::openWrite (path);
    auto position = file.tell();
    CHECK (position == 0);
    file.seek (86);
    position = file.tell();
    CHECK (position == 86);
    file.close();
}

TEST_CASE("SimpleFile_insistOpenWrite_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    auto file = irbis::SimpleFile::insistOpenWrite (path);
    auto position = file.tell();
    CHECK (position == 0);
    file.seek (86);
    position = file.tell();
    CHECK (position == 86);
    file.close();
}

TEST_CASE("SimpleFile_insistOpenWrite_2", "[file]")
{
    auto path = irbis::toUtf (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "TEST1.ISO");
    irbis::IO::convertSlashes (path);
    auto file = irbis::SimpleFile::insistOpenWrite (path);
    auto position = file.tell();
    CHECK (position == 0);
    file.seek (86);
    position = file.tell();
    CHECK (position == 86);
    file.close();
}

TEST_CASE("SimpleFile_size_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    auto file = irbis::SimpleFile::openRead (path);
    const auto size = file.size();
    CHECK (size == 78096);
}

TEST_CASE("SimpleFile_readAll_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"org.mnu");
    REQUIRE (irbis::IO::fileExist(path));
    auto file = irbis::SimpleFile::openRead(path);
    auto all = file.readAll();
    CHECK_FALSE (all.empty());
    file.close();
}

TEST_CASE("SimpleFile_readChar_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"org.mnu");
    REQUIRE (irbis::IO::fileExist(path));
    auto file = irbis::SimpleFile::openRead(path);
    auto chr = file.readChar();
    CHECK (chr != -1);
    file.close();
}

TEST_CASE("SimpleFile_readInt32_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"Irbis64/Datai/IBIS/ibis.xrf");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    auto file = irbis::SimpleFile::openRead (path);
    uint32_t value = file.readInt32();
    CHECK (value == 0x015E34BCu);
    file.close();
}

TEST_CASE("SimpleFile_readInt64_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"Irbis64/Datai/IBIS/ibis.xrf");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist(path));
    auto file = irbis::SimpleFile::openRead(path);
    uint64_t value = file.readInt64();
    CHECK (value == 0x00000000015E34BCull);
    file.close();
}

TEST_CASE("SimpleFile_readNarrow_1", "[file]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"org.mnu");
    REQUIRE (irbis::IO::fileExist(path));
    auto file = irbis::SimpleFile::openRead(path);
    auto line = file.readNarrow();
    CHECK (line == "1");
    line = file.readNarrow();
    CHECK (line == "RU");
    file.close();
}

TEST_CASE("SimpleFile_writeInt32_1", "[file]")
{
    auto path = irbis::IO::getTempDirectory();
    REQUIRE (!path.empty());
    REQUIRE (irbis::IO::directoryExist (path));
    auto fileName = irbis::IO::combinePath (path, L"writeInt32.bin");
    irbis::IO::deleteFile (fileName);
    {
        auto file = irbis::SimpleFile::create (fileName);
        uint32_t value { 0x015E34BCu };
        file.writeInt32 (value);
        file.close();
    }
    irbis::IO::deleteFile (fileName);
}

TEST_CASE("SimpleFile_writeInt64_1", "[file]")
{
    auto path = irbis::IO::getTempDirectory();
    REQUIRE (!path.empty());
    REQUIRE (irbis::IO::directoryExist (path));
    auto fileName = irbis::IO::combinePath (path, L"writeInt64.bin");
    irbis::IO::deleteFile (fileName);
    {
        auto file = irbis::SimpleFile::create (fileName);
        uint64_t value { 0x00000000015E34BCull };
        file.writeInt64 (value);
        file.close();
    }
    irbis::IO::deleteFile (fileName);
}
