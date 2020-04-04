// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"
#include "irbis_internal.h"
#include "safeTests.h"

// ReSharper disable StringLiteralTypo

TEST_CASE("IO_getCurrentDirectory_1", "[io]")
{
    const auto dir = irbis::IO::getCurrentDirectory();
    CHECK_FALSE (dir.empty());
}

TEST_CASE("IO_getCurrentDirectory_2", "[io]")
{
    const auto dir = irbis::IO::getCurrentDirectoryNarrow();
    CHECK_FALSE (dir.empty());
}

TEST_CASE("IO_getExtension_1", "[io]")
{
    CHECK (irbis::IO::getExtension (L"").empty());
    CHECK (irbis::IO::getExtension (L".").empty());
    CHECK (irbis::IO::getExtension (L"file.ext") == L".ext");
    CHECK (irbis::IO::getExtension (L"file.ext2.ext1") == L".ext1");
    CHECK (irbis::IO::getExtension (L"file.").empty());
    CHECK (irbis::IO::getExtension (L"file_ext").empty());
    CHECK (irbis::IO::getExtension (L"dir.ext/file").empty());
}

TEST_CASE("IO_getExtension_2", "[io]")
{
    CHECK (irbis::IO::getExtension ("").empty());
    CHECK (irbis::IO::getExtension (".").empty());
    CHECK (irbis::IO::getExtension ("file.ext") == ".ext");
    CHECK (irbis::IO::getExtension ("file.ext2.ext1") == ".ext1");
    CHECK (irbis::IO::getExtension ("file.").empty());
    CHECK (irbis::IO::getExtension ("file_ext").empty());
    CHECK (irbis::IO::getExtension ("dir.ext/file").empty());
}

TEST_CASE("IO_getFileName_1", "[io]")
{
    CHECK (irbis::IO::getFileName (L"").empty());
    CHECK (irbis::IO::getFileName (L"dir/subdir/file.ext") == L"file.ext");
    CHECK (irbis::IO::getFileName (L"dir\\subdir\\file.ext") == L"file.ext");
    CHECK (irbis::IO::getFileName (L"dir/subdir/").empty());
    CHECK (irbis::IO::getFileName (L"dir\\subdir\\").empty());
    CHECK (irbis::IO::getFileName (L"/file.ext") == L"file.ext");
    CHECK (irbis::IO::getFileName (L"\\file.ext") == L"file.ext");
    CHECK (irbis::IO::getFileName (L"file.ext") == L"file.ext");
    CHECK (irbis::IO::getFileName (L"file.ext") == L"file.ext");
}

TEST_CASE("IO_getFileName_2", "[io]")
{
    CHECK (irbis::IO::getFileName ("").empty());
    CHECK (irbis::IO::getFileName ("dir/subdir/file.ext") == "file.ext");
    CHECK (irbis::IO::getFileName ("dir\\subdir\\file.ext") == "file.ext");
    CHECK (irbis::IO::getFileName ("dir/subdir/").empty());
    CHECK (irbis::IO::getFileName ("dir\\subdir\\").empty());
    CHECK (irbis::IO::getFileName ("/file.ext") == "file.ext");
    CHECK (irbis::IO::getFileName ("\\file.ext") == "file.ext");
    CHECK (irbis::IO::getFileName ("file.ext") == "file.ext");
    CHECK (irbis::IO::getFileName ("file.ext") == "file.ext");
}

TEST_CASE("IO_getDirectory_1", "[io]")
{
    CHECK(irbis::IO::getDirectory (L"/file.ext") == L"/");
    CHECK(irbis::IO::getDirectory (L"").empty());
    CHECK(irbis::IO::getDirectory (L"dir/subdir/file.ext") == L"dir/subdir");
    CHECK(irbis::IO::getDirectory (L"/dir/subdir/file.ext") == L"/dir/subdir");
    CHECK(irbis::IO::getDirectory (L"dir\\subdir\\file.ext") == L"dir\\subdir");
    CHECK(irbis::IO::getDirectory (L"\\dir\\subdir\\file.ext") == L"\\dir\\subdir");
    CHECK(irbis::IO::getDirectory (L"file.ext").empty());
}

TEST_CASE("IO_getDirectory_2", "[io]")
{
    CHECK(irbis::IO::getDirectory ("/file.ext") == "/");
    CHECK(irbis::IO::getDirectory ("").empty());
    CHECK(irbis::IO::getDirectory ("dir/subdir/file.ext") == "dir/subdir");
    CHECK(irbis::IO::getDirectory ("/dir/subdir/file.ext") == "/dir/subdir");
    CHECK(irbis::IO::getDirectory ("dir\\subdir\\file.ext") == "dir\\subdir");
    CHECK(irbis::IO::getDirectory ("\\dir\\subdir\\file.ext") == "\\dir\\subdir");
    CHECK(irbis::IO::getDirectory ("file.ext").empty());
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

TEST_CASE("IO_convertSlashes_2", "[io]")
{
#ifdef IRBIS_WINDOWS

    std::string s("/path/to/file");
    CHECK(irbis::IO::convertSlashes(s) == "\\path\\to\\file");

#else

    std::string s("\\path\\to\\file");
    CHECK(irbis::IO::convertSlashes(s) == "/path/to/file");

#endif
}

TEST_CASE("IO_combinePath_1", "[io]")
{
#ifdef IRBIS_WINDOWS
    CHECK(irbis::IO::combinePath(L"dir", L"subdir") == L"dir\\subdir");
#else
    CHECK(irbis::IO::combinePath(L"dir", L"subdir") == L"dir/subdir");
#endif
}

TEST_CASE("IO_combinePath_2", "[io]")
{
#ifdef IRBIS_WINDOWS
    CHECK(irbis::IO::combinePath("dir", "subdir") == "dir\\subdir");
#else
    CHECK(irbis::IO::combinePath("dir", "subdir") == "dir/subdir");
#endif
}

//TEST_CASE("IO_readInt32_1", "[io]")
//{
//    auto path = whereTestData();
//    REQUIRE (!path.empty());
//    path = irbis::IO::combinePath (path, L"Irbis64/Datai/IBIS/ibis.xrf");
//    irbis::IO::convertSlashes (path);
//    REQUIRE (irbis::IO::fileExist (path));
//    irbis::File file (path, L"rb");
//    uint32_t value { 0u };
//    REQUIRE (irbis::IO::readInt32 (file.getStream(), &value));
//    CHECK (value == 0x015E34BCu);
//}
//
//TEST_CASE("IO_readInt64_1", "[io]")
//{
//    auto path = whereTestData();
//    REQUIRE (!path.empty());
//    path = irbis::IO::combinePath (path, L"Irbis64/Datai/IBIS/ibis.xrf");
//    irbis::IO::convertSlashes (path);
//    REQUIRE (irbis::IO::fileExist(path));
//    irbis::File file (path, L"rb");
//    uint64_t value { 0ull };
//    REQUIRE (irbis::IO::readInt64(file.getStream(), &value));
//    CHECK (value == 0x00000000015E34BCull);
//}

TEST_CASE("IO_createDirectory_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    CHECK (irbis::IO::directoryExist (path));
    CHECK (irbis::IO::removeDirectory (path));
}

TEST_CASE("IO_createDirectory_2", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    CHECK (irbis::IO::directoryExist (path));
    CHECK (irbis::IO::removeDirectory (path));
}

TEST_CASE("IO_removeDirectory_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    CHECK (irbis::IO::directoryExist (path));
    CHECK (irbis::IO::removeDirectory (path));
}

TEST_CASE("IO_removeDirectory_2", "[io]")
{
    auto path = irbis::wide2string (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    CHECK (irbis::IO::directoryExist (path));
    CHECK (irbis::IO::removeDirectory (path));
}

//TEST_CASE("IO_writeInt32_1", "[io]")
//{
//    auto path = whereTestData();
//    REQUIRE (!path.empty());
//    path = irbis::IO::combinePath (path, L"temp");
//    irbis::IO::convertSlashes (path);
//    irbis::IO::createDirectory (path);
//    REQUIRE (irbis::IO::directoryExist (path));
//    auto fileName = irbis::IO::combinePath (path, L"writeInt32.bin");
//    irbis::IO::deleteFile (fileName);
//    {
//        irbis::File file (fileName, L"wb");
//        uint32_t value { 0x015E34BCu };
//        CHECK (irbis::IO::writeInt32 (file.getStream(), value));
//    }
//    irbis::IO::deleteFile (fileName);
//    irbis::IO::removeDirectory (path);
//}
//
//TEST_CASE("IO_writeInt64_1", "[io]")
//{
//    auto path = whereTestData();
//    REQUIRE (!path.empty());
//    path = irbis::IO::combinePath (path, L"temp");
//    irbis::IO::convertSlashes (path);
//    irbis::IO::createDirectory (path);
//    REQUIRE (irbis::IO::directoryExist (path));
//    auto fileName = irbis::IO::combinePath (path, L"writeInt64.bin");
//    irbis::IO::deleteFile (fileName);
//    {
//        irbis::File file (fileName, L"wb");
//        uint64_t value { 0x00000000015E34BCull };
//        CHECK (irbis::IO::writeInt64 (file.getStream(), value));
//    }
//    irbis::IO::deleteFile (fileName);
//    irbis::IO::removeDirectory (path);
//}

TEST_CASE("IO_trimLeadingSlashes_1", "[io]")
{
    irbis::String path (L"//some/path/");
    CHECK (irbis::IO::trimLeadingSlashes(path) == L"some/path/");
    CHECK (irbis::IO::trimLeadingSlashes(path) == L"some/path/");
    path = L"\\\\some\\path\\";
    CHECK (irbis::IO::trimLeadingSlashes(path) == L"some\\path\\");
    CHECK (irbis::IO::trimLeadingSlashes(path) == L"some\\path\\");
}

TEST_CASE("IO_trimLeadingSlashes_2", "[io]")
{
    std::string path ("//some/path/");
    CHECK (irbis::IO::trimLeadingSlashes(path) == "some/path/");
    CHECK (irbis::IO::trimLeadingSlashes(path) == "some/path/");
    path = R"(\\some\path\)";
    CHECK (irbis::IO::trimLeadingSlashes(path) == "some\\path\\");
    CHECK (irbis::IO::trimLeadingSlashes(path) == "some\\path\\");
}

TEST_CASE("IO_trimTrailingingSlashes_1", "[io]")
{
    irbis::String path (L"//some/path/");
    CHECK (irbis::IO::trimTrailingSlashes(path) == L"//some/path");
    CHECK (irbis::IO::trimTrailingSlashes(path) == L"//some/path");
    path = L"\\\\some\\path\\";
    CHECK (irbis::IO::trimTrailingSlashes(path) == L"\\\\some\\path");
    CHECK (irbis::IO::trimTrailingSlashes(path) == L"\\\\some\\path");
}

TEST_CASE("IO_trimTrailingingSlashes_2", "[io]")
{
    std::string path ("//some/path/");
    CHECK (irbis::IO::trimTrailingSlashes(path) == "//some/path");
    CHECK (irbis::IO::trimTrailingSlashes(path) == "//some/path");
    path = R"(\\some\path\)";
    CHECK (irbis::IO::trimTrailingSlashes(path) == "\\\\some\\path");
    CHECK (irbis::IO::trimTrailingSlashes(path) == "\\\\some\\path");
}

TEST_CASE("IO_getFileSize_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"TEST1.ISO");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist(path));
    const auto size = irbis::IO::getFileSize (path);
    CHECK (size == 78096ull);
}

TEST_CASE("IO_getFileSize_2", "[io]")
{
    auto path = irbis::wide2string (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "TEST1.ISO");
    irbis::IO::convertSlashes (path);
    REQUIRE (irbis::IO::fileExist (path));
    const auto size = irbis::IO::getFileSize (path);
    CHECK (size == 78096ull);
}

TEST_CASE("IO_createFile_1", "[io]")
{
    auto path = whereTestData();
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, L"temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    REQUIRE (irbis::IO::directoryExist (path));
    auto fileName = irbis::IO::combinePath (path, L"tempfile");
    irbis::IO::deleteFile (fileName);
    irbis::IO::createFile (fileName);
    irbis::IO::deleteFile (fileName);
    irbis::IO::removeDirectory (path);
}

TEST_CASE("IO_createFile_2", "[io]")
{
    auto path = irbis::wide2string (whereTestData());
    REQUIRE (!path.empty());
    path = irbis::IO::combinePath (path, "temp");
    irbis::IO::convertSlashes (path);
    irbis::IO::createDirectory (path);
    REQUIRE (irbis::IO::directoryExist (path));
    auto fileName = irbis::IO::combinePath (path, "tempfile");
    irbis::IO::deleteFile (fileName);
    irbis::IO::createFile (fileName);
    irbis::IO::deleteFile (fileName);
    irbis::IO::removeDirectory (path);
}

TEST_CASE("IO_getTempDirectory_1", "[io]")
{
    const auto temp = irbis::IO::getTempDirectory();
    CHECK_FALSE (temp.empty());
}

TEST_CASE("IO_getTempDirectory_2", "[io]")
{
    const auto temp = irbis::IO::getTempDirectoryNarrow();
    CHECK_FALSE (temp.empty());
}

TEST_CASE("IO_setCurrentDirectory_1", "[io]")
{
    const auto dir1 = irbis::IO::getCurrentDirectory();
    REQUIRE_FALSE (dir1.empty());
    irbis::IO::setCurrentDirectory (dir1);
    const auto dir2 = irbis::IO::getCurrentDirectory();
    CHECK (dir1 == dir2);
}

TEST_CASE("IO_setCurrentDirectory_2", "[io]")
{
    const auto dir1 = irbis::IO::getCurrentDirectoryNarrow();
    REQUIRE_FALSE (dir1.empty());
    irbis::IO::setCurrentDirectory (dir1);
    const auto dir2 = irbis::IO::getCurrentDirectoryNarrow();
    CHECK (dir1 == dir2);
}

