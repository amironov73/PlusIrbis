// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

NAMESPACE_IRBIS_BEGIN

class SubFieldListImpl
{
public:
    std::list<SubField> subfields;
};

SubFieldList::SubFieldList()
    : _impl(new SubFieldListImpl())
{
}

SubFieldList::~SubFieldList()
{
    delete _impl;
}


NAMESPACE_IRBIS_END
