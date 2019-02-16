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

void SubFieldList::add(wchar_t code, const std::wstring &value)
{
    _impl->subfields.push_back({code, value});
}

SubField* SubFieldList::begin() const
{
    return &(*_impl->subfields.begin());
}

void SubFieldList::clear()
{
    _impl->subfields.clear();
}


SubField* SubFieldList::end() const
{
    return &(*_impl->subfields.end());
}

size_t SubFieldList::size() const
{
    return _impl->subfields.size();
}

NAMESPACE_IRBIS_END
