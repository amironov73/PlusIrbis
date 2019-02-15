// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

class RecordFieldListImpl
{
public:
};

RecordFieldList::RecordFieldList()
    : _impl(new RecordFieldListImpl())
{
}

RecordFieldList::~RecordFieldList()
{
    delete _impl;
}


NAMESPACE_IRBIS_END
