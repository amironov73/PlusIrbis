#include "catch.hpp"
#include "irbis.h"
#include "irbis_private.h"
#include "safeTests.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("IO_getCurrentDirectory_1", "[io]")
{
    const irbis::String dir = irbis::IO::getCurrentDirectory();
    CHECK(!dir.empty());
}

TEST_CASE("IO_getExtension_1", "[io]")
{
    CHECK(irbis::IO::getExtension(L"").empty());
    CHECK(irbis::IO::getExtension(L"file.ext") == L".ext");
    CHECK(irbis::IO::getExtension(L"file.ext2.ext1") == L".ext1");
    CHECK(irbis::IO::getExtension(L"file.").empty());
    CHECK(irbis::IO::getExtension(L"file_ext").empty());
    CHECK(irbis::IO::getExtension(L"dir.ext/file").empty());
    CHECK(irbis::IO::getExtension(L"").empty());
}

TEST_CASE("IO_getFileName_1", "[io]")
{
    CHECK(irbis::IO::getFileName(L"").empty());
    CHECK(irbis::IO::getFileName(L"dir/subdir/file.ext") == L"file.ext");
    CHECK(irbis::IO::getFileName(L"dir\\subdir\\file.ext") == L"file.ext");
    CHECK(irbis::IO::getFileName(L"dir/subdir/").empty());
    CHECK(irbis::IO::getFileName(L"dir\\subdir\\").empty());
    CHECK(irbis::IO::getFileName(L"/file.ext") == L"file.ext");
    CHECK(irbis::IO::getFileName(L"\\file.ext") == L"file.ext");
    CHECK(irbis::IO::getFileName(L"file.ext") == L"file.ext");
    CHECK(irbis::IO::getFileName(L"file.ext") == L"file.ext");
}

TEST_CASE("IO_getDirectory_1", "[io]")
{
    CHECK(irbis::IO::getDirectory(L"/file.ext") == L"/");
    CHECK(irbis::IO::getDirectory(L"").empty());
    CHECK(irbis::IO::getDirectory(L"dir/subdir/file.ext") == L"dir/subdir");
    CHECK(irbis::IO::getDirectory(L"/dir/subdir/file.ext") == L"/dir/subdir");
    CHECK(irbis::IO::getDirectory(L"dir\\subdir\\file.ext") == L"dir\\subdir");
    CHECK(irbis::IO::getDirectory(L"\\dir\\subdir\\file.ext") == L"\\dir\\subdir");
    CHECK(irbis::IO::getDirectory(L"file.ext").empty());
}

TEST_CASE("IO_convertSlashes_1", "[io]")
{
#ifdef IRBIS_WINDOWS

    irbis::String s(L"/path/to/file");
    CHECK(irbis::IO::convertSlashes(s) == L"\\path\\to\\file");

#else

    irbis::String s(L"\\path\\to\\file");
    CHECK(irbis::IO::convertSlashes(s) == L"/path/to/file");

#endif
}

TEST_CASE("IO_combinePath_1", "[io]")
{
    CHECK(irbis::IO::combinePath(L"dir", L"subdir") == L"dir/subdir");
}

TEST_CASE("IO_readInt32_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"Irbis64/Datai/IBIS/ibis.xrf");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    irbis::File file (path, L"rb");
    uint32_t value { 0u };
    REQUIRE(irbis::IO::readInt32 (file.getStream(), &value));
    CHECK(value == 0x015E34BCu);
}

TEST_CASE("IO_readInt64_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"Irbis64/Datai/IBIS/ibis.xrf");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist(path));
    irbis::File file (path, L"rb");
    uint64_t value { 0ull };
    REQUIRE (irbis::IO::readInt64(file.getStream(), &value));
    CHECK (value == 0x00000000015E34BCull);
}

TEST_CASE("IO_createDirectory_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    CHECK (irbis::IO::directoryExist (path));
}

TEST_CASE("IO_writeInt32_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    REQUIRE (irbis::IO::directoryExist (path));
    path = irbis::IO::combinePath (path, L"writeInt32.bin");
    irbis::IO::deleteFile(path);
    irbis::File file (path, L"wb");
    uint32_t value { 0x015E34BCu };
    CHECK (irbis::IO::writeInt32 (file.getStream(), value));
}

TEST_CASE("IO_writeInt64_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    REQUIRE (irbis::IO::directoryExist (path));
    path = irbis::IO::combinePath (path, L"writeInt64.bin");
    irbis::IO::deleteFile(path);
    irbis::File file (path, L"wb");
    uint64_t value { 0x00000000015E34BCull };
    CHECK (irbis::IO::writeInt64 (file.getStream(), value));
}

TEST_CASE("IO_trimLeadingSlashes_1", "[io]")
{
    irbis::String path(L"//some/path/");
    CHECK(irbis::IO::trimLeadingSlashes(path) == L"some/path/");
    CHECK(irbis::IO::trimLeadingSlashes(path) == L"some/path/");
    path = L"\\\\some\\path\\";
    CHECK(irbis::IO::trimLeadingSlashes(path) == L"some\\path\\");
    CHECK(irbis::IO::trimLeadingSlashes(path) == L"some\\path\\");
}

TEST_CASE("IO_trimTrailingingSlashes_1", "[io]")
{
    irbis::String path(L"//some/path/");
    CHECK(irbis::IO::trimTrailingSlashes(path) == L"//some/path");
    CHECK(irbis::IO::trimTrailingSlashes(path) == L"//some/path");
    path = L"\\\\some\\path\\";
    CHECK(irbis::IO::trimTrailingSlashes(path) == L"\\\\some\\path");
    CHECK(irbis::IO::trimTrailingSlashes(path) == L"\\\\some\\path");
}
