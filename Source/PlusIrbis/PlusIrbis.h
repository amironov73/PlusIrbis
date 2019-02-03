// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <ios>
#include <istream>
#include <list>

#ifdef PLUSIRBIS_LIBRARY
#define PLUSIRBIS_EXPORTS __declspec(dllexport)
#else
#define PLUSIRBIS_EXPORTS __declspec(dllimport)
#endif

//=========================================================

namespace irbis
{

class BaseException;
class MarcRecord;
class NetworkException;
class RecordField;
class SubField;
class VerificationException;

//=========================================================

class PLUSIRBIS_EXPORTS BaseException : public std::exception
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS MarcRecord
{
public:
    std::wstring database;
    int mfn;
    int status;
    int version;
    std::list<RecordField> fields;

    MarcRecord();

    bool deleted() const;
    bool verify(bool throwOnError) const;

    friend std::wostream& operator << (std::wostream &stream, const MarcRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS NetworkException : public BaseException
{
public:
};

//=========================================================

class PLUSIRBIS_EXPORTS RawRecord
{
public:
    std::wstring database;
    int mfn;
    int status;
    int version;
    std::list<std::wstring> fields;

    RawRecord();

    friend std::wostream& operator << (std::wostream &stream, const RawRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordField
{
public:
    const static int NoTag;

    int tag;
    std::wstring value;
    std::list<SubField> subfields;

    RecordField(int tag = NoTag, const std::wstring &value = L"");

    RecordField& add(wchar_t code, const std::wstring &value = L"");
    RecordField& clear();
    bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

    friend std::wostream& operator << (std::wostream &stream, const RecordField &field);
};

//=========================================================

enum RecordStatus
{
    LogicallyDeleted = 1,
    PhysicallyDeleted = 2,
    Deleted = LogicallyDeleted | PhysicallyDeleted,
    Absent = 4,
    NonActualized = 8,
    Last = 32,
    Locked = 64
};

//=========================================================

class PLUSIRBIS_EXPORTS SubField
{
public:
    const static wchar_t NoCode;

    wchar_t code;
    std::wstring value;

    SubField(wchar_t code = NoCode, const std::wstring &value = L"");

    bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

    friend std::wostream& operator << (std::wostream &stream, const SubField &subfield);
};

//=========================================================

class PLUSIRBIS_EXPORTS VerificationException : public BaseException
{
public:
};


}

