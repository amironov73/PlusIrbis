// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

Search Search::all()
{
    Search result;
    result._buffer = L"I=$";

    return result;
}

Search& Search::and_(const std::wstring &text)
{
    this->_buffer = std::wstring(L"(")
            + this->_buffer
            + std::wstring(L" * ")
            + Search::wrap(text)
            + std::wstring(L")");
    return *this;
}

Search& Search::and_(const std::wstring &text1, const std::wstring &text2)
{
    this->_buffer = std::wstring(L"(")
            + this->_buffer
            + std::wstring(L" * ")
            + Search::wrap(text1)
            + std::wstring(L" * ")
            + Search::wrap(text2)
            + std::wstring(L")");
    return *this;
}

Search& Search::and_(const std::wstring &text1, const std::wstring &text2, const std::wstring &text3)
{
    this->_buffer = std::wstring(L"(")
            + this->_buffer
            + std::wstring(L" * ")
            + Search::wrap(text1)
            + std::wstring(L" * ")
            + Search::wrap(text2)
            + std::wstring(L" * ")
            + Search::wrap(text3)
            + std::wstring(L")");
    return *this;
}

Search& Search::and_(const Search &item)
{
    return this->and_(item.toString());
}

Search& Search::and_(const Search &item1, const Search &item2)
{
    return this->and_(item1.toString(), item2.toString());
}

Search& Search::and_(const Search &item1, const Search &item2, const Search &item3)
{
    return this->and_(item1.toString(), item2.toString(), item3.toString());
}

Search Search::equals(const std::wstring &prefix, const std::wstring &text)
{
    Search result;
    result._buffer = Search::wrap(prefix + text);
    return result;
}

Search Search::equals(const std::wstring &prefix, const std::wstring &text1, const std::wstring &text2)
{
    Search result;
    result._buffer = std::wstring(L"(")
            + Search::wrap(prefix + text1)
            + std::wstring(L" + ")
            + Search::wrap(prefix + text2)
            + std::wstring(L")");
    return result;
}

Search Search::equals(const std::wstring &prefix, const std::wstring &text1, const std::wstring &text2, const std::wstring &text3)
{
    Search result;
    result._buffer = std::wstring(L"(")
            + Search::wrap(prefix + text1)
            + std::wstring(L" + ")
            + Search::wrap(prefix + text2)
            + std::wstring(L" + ")
            + Search::wrap(prefix + text3)
            + std::wstring(L")");
    return result;
}

bool Search::needWrap(const std::wstring &text)
{
    if (text.empty()) {
        return true;
    }

    wchar_t c = text[0];
    if (c == L'"' || c == L'(') {
        return false;
    }

    return contains(text, L' ')
           || contains(text, L'+')
           || contains(text, L'*')
           || contains(text, L'^')
           || contains(text, L'#')
           || contains(text, L'(')
           || contains(text, L')')
           || contains(text, L'"');

}

Search& Search::not_(const std::wstring &text)
{
    this->_buffer = std::wstring (L"(")
            + this->_buffer
            + std::wstring (L" ^ ")
            + Search::wrap(text)
            + std::wstring (L")");
    return *this;
}

Search& Search::not_(const Search &item)
{
    return this->not_(item.toString());
}

Search& Search::or_(const std::wstring &text)
{
    this->_buffer = std::wstring (L"(")
            + this->_buffer
            + std::wstring (L" + ")
            + Search::wrap(text)
            + std::wstring (L")");
    return *this;
}

Search& Search::or_(const std::wstring &text1, const std::wstring &text2)
{
    this->_buffer = std::wstring (L"(")
            + this->_buffer
            + std::wstring (L" * ")
            + Search::wrap(text1)
            + std::wstring (L" * ")
            + Search::wrap(text2)
            + std::wstring (L")");
    return *this;
}

Search& Search::or_(const std::wstring &text1, const std::wstring &text2, const std::wstring &text3)
{
    this->_buffer = std::wstring(L"(")
            + this->_buffer
            + std::wstring(L" + ")
            + Search::wrap(text1)
            + std::wstring(L" * ")
            + Search::wrap(text2)
            + std::wstring(L" + ")
            + Search::wrap(text3)
            + std::wstring(L")");
    return *this;
}

Search& Search::or_(const Search &item)
{
    return this->or_(item.toString());
}

Search& Search::or_(const Search &item1, const Search &item2)
{
    return this->or_(item1.toString(), item2.toString());
}

Search& Search::or_(const Search &item1, const Search &item2, const Search &item3)
{
    return this->or_(item1.toString(), item2.toString(), item3.toString());
}

Search& Search::sameField(const std::wstring &text)
{
    this->_buffer = std::wstring(L"(")
            + this->_buffer
            + std::wstring (L" (G) ")
            + Search::wrap(text)
            + std::wstring (L")");
    return *this;
}

Search& Search::sameRepeat(const std::wstring &text)
{
    this->_buffer = std::wstring (L"(")
            + this->_buffer
            + std::wstring (L" (F) ")
            + Search::wrap(text)
            + std::wstring (L")");
    return *this;
}

std::wstring Search::toString() const
{
    return this->_buffer;
}

std::wstring Search::wrap(const std::wstring &text)
{
    if (Search::needWrap(text)) {
        return std::wstring (L"\"")
        + text
        + std::wstring (L"\"");
    }

    return text;
}

std::wstring Search::wrap(const Search &item)
{
    return Search::wrap (item.toString());
}

Search keyword(const std::wstring &value1)
{
    return Search::equals(L"K=", value1);
}

Search author(const std::wstring &value1)
{
    return Search::equals(L"A=", value1);
}

Search title(const std::wstring &value1)
{
    return Search::equals(L"T=", value1);
}

Search publisher(const std::wstring &value1)
{
    return Search::equals(L"O=", value1);
}

Search place(const std::wstring &value1)
{
    return Search::equals(L"MI=", value1);
}

Search subject(const std::wstring &value1)
{
    return Search::equals(L"S=", value1);
}

Search language(const std::wstring &value1)
{
    return Search::equals(L"J=", value1);
}

Search year(const std::wstring &value1)
{
    return Search::equals(L"G=", value1);
}

Search magazine(const std::wstring &value1)
{
    return Search::equals(L"TJ=", value1);
}

Search documentKind(const std::wstring &value1)
{
    return Search::equals(L"V=", value1);
}

Search udc(const std::wstring &value1)
{
    return Search::equals(L"U=", value1);
}

Search bbk(const std::wstring &value1)
{
    return Search::equals(L"BBK=", value1);
}

Search rzn(const std::wstring &value1)
{
    return Search::equals(L"RZN=", value1);
}

Search mhr(const std::wstring &value1)
{
    return Search::equals(L"MHR=", value1);
}

}
