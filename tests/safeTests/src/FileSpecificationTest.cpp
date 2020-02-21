// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

TEST_CASE ("FileSpecification_constructor_1", "[file]")
{
    irbis::FileSpecification specification;
    CHECK (specification.path == irbis::IrbisPath::System);
    CHECK_FALSE (specification.binaryFile);
    CHECK (specification.database.empty());
    CHECK (specification.filename.empty());
    CHECK (specification.content.empty());
}

TEST_CASE ("FileSpecification_constructor_2", "[file]")
{
    irbis::FileSpecification specification (irbis::IrbisPath::System, L"file.ext");
    CHECK (specification.path == irbis::IrbisPath::System);
    CHECK_FALSE (specification.binaryFile);
    CHECK (specification.database.empty());
    CHECK (specification.filename == L"file.ext");
    CHECK (specification.content.empty());
}

TEST_CASE ("FileSpecification_constructor_3", "[file]")
{
    irbis::FileSpecification specification (irbis::IrbisPath::MasterFile, L"IBIS", L"file.ext");
    CHECK (specification.path == irbis::IrbisPath::MasterFile);
    CHECK_FALSE (specification.binaryFile);
    CHECK (specification.database == L"IBIS");
    CHECK (specification.filename == L"file.ext");
    CHECK (specification.content.empty());
}

TEST_CASE ("FileSpecification_constructor_4", "[file]")
{
    irbis::FileSpecification specification1 (irbis::IrbisPath::MasterFile, L"IBIS", L"file.ext");
    irbis::FileSpecification specification2 (specification1);
    CHECK (specification2.path == specification1.path);
    CHECK (specification2.binaryFile == specification1.binaryFile);
    CHECK (specification2.database == specification1.database);
    CHECK (specification2.filename == specification1.filename);
    CHECK (specification2.content == specification1.content);
}

TEST_CASE("FileSpecification_parse_1", "[file]")
{
    irbis::FileSpecification specification = irbis::FileSpecification::parse (L"2.IBIS.brief.pft");
    CHECK (specification.path == irbis::IrbisPath::MasterFile);
    CHECK_FALSE (specification.binaryFile);
    CHECK (specification.database == L"IBIS");
    CHECK (specification.filename == L"brief.pft");
    CHECK (specification.content.empty());
}

TEST_CASE("FileSpecification_parse_2", "[file]")
{
    irbis::FileSpecification specification = irbis::FileSpecification::parse (L"0..iri.mnu");
    CHECK (specification.path == irbis::IrbisPath::System);
    CHECK_FALSE (specification.binaryFile);
    CHECK (specification.database.empty());
    CHECK (specification.filename == L"iri.mnu");
    CHECK (specification.content.empty());
}

TEST_CASE("FileSpecification_parse_3", "[file]")
{
    irbis::FileSpecification specification = irbis::FileSpecification::parse (L"2.IBIS.@doclad99.doc");
    CHECK (specification.path == irbis::IrbisPath::MasterFile);
    CHECK (specification.binaryFile);
    CHECK (specification.database == L"IBIS");
    CHECK (specification.filename == L"doclad99.doc");
    CHECK (specification.content.empty());
}

TEST_CASE("FileSpecification_parse_4", "[file]")
{
    irbis::FileSpecification specification = irbis::FileSpecification::parse (L"2.IBIS.brief.pft&Hello");
    CHECK (specification.path == irbis::IrbisPath::MasterFile);
    CHECK_FALSE (specification.binaryFile);
    CHECK (specification.database == L"IBIS");
    CHECK (specification.filename == L"brief.pft");
    CHECK (specification.content == L"Hello");
}

TEST_CASE("FileSpecification_verify_1", "[file]")
{
    irbis::FileSpecification specification;
    CHECK (!specification.verify (false));

    specification.path = irbis::IrbisPath ::MasterFile;
    specification.filename = L"brief.pft";
    CHECK (!specification.verify (false));

    specification.database = L"IBIS";
    CHECK (specification.verify (false));

    specification.path = irbis::IrbisPath::System;
    CHECK (!specification.verify (false));
}

TEST_CASE("FileSpecification_toString_1", "[file]")
{
    irbis::FileSpecification specification;
    CHECK (specification.toString() == L"0..");

    specification.path = irbis::IrbisPath::MasterFile;
    specification.filename = L"brief.pft";
    CHECK (specification.toString() == L"2..brief.pft");

    specification.database = L"IBIS";
    CHECK (specification.toString() == L"2.IBIS.brief.pft");

    specification.binaryFile = true;
    CHECK (specification.toString() == L"2.IBIS.@brief.pft");

    specification.binaryFile = false;
    specification.content = L"Hello";
    CHECK (specification.toString() == L"2.IBIS.brief.pft&Hello");
}

