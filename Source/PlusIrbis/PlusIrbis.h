// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <cwchar>
#include <cwctype>
#include <algorithm>
#include <string>
#include <ios>
#include <list>

#define NAMESPACE_IRBIS_BEGIN namespace irbis {
#define NAMESPACE_IRBIS_END }

#ifdef PLUSIRBIS_LIBRARY
#define PLUSIRBIS_EXPORTS __declspec(dllexport)
#else
#define PLUSIRBIS_EXPORTS __declspec(dllimport)
#endif

//=========================================================

NAMESPACE_IRBIS_BEGIN

class BaseException;
class Format;
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

class PLUSIRBIS_EXPORTS Format final
{
public:
    static std::wstring removeComments(const std::wstring &text);
    static std::wstring prepareFormat(const std::wstring &text);
};

//=========================================================

class PLUSIRBIS_EXPORTS MarcRecord final
{
public:
    int mfn = 0;
    int status = 0;
    int version = 0;
    std::list<RecordField> fields;
    std::wstring database;

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

class PLUSIRBIS_EXPORTS RawRecord final
{
public:
    int mfn = 0;
    int status = 0;
    int version = 0;
    std::list<std::wstring> fields;
    std::wstring database;

    friend std::wostream& operator << (std::wostream &stream, const RawRecord &record);
};

//=========================================================

class PLUSIRBIS_EXPORTS RecordField final
{
public:
    const static int NoTag;

    int tag = 0;
    std::wstring value;
    std::list<SubField> subfields;

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

class PLUSIRBIS_EXPORTS SubField final
{
public:
    const static wchar_t NoCode;

    wchar_t code = L'\0';
    std::wstring value;

    bool empty() const;
    bool verify(bool throwOnError) const;
    std::wstring wstr() const;

    friend std::wostream& operator << (std::wostream &stream, const SubField &subfield);
};

//=========================================================

class PLUSIRBIS_EXPORTS VerificationException final
    : public BaseException
{
public:
};

//=========================================================

// Utilities

bool PLUSIRBIS_EXPORTS sameChar(wchar_t first, wchar_t second);
bool PLUSIRBIS_EXPORTS sameString(const std::wstring &first, const std::wstring &second);

bool PLUSIRBIS_EXPORTS contains(const std::wstring &text, const std::wstring &fragment);

int PLUSIRBIS_EXPORTS fastParse32(const std::wstring &text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text);
int PLUSIRBIS_EXPORTS fastParse32(const wchar_t *text, int length);

NAMESPACE_IRBIS_END
