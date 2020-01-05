// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

///
/// \file Search.cpp
/// \brief Методы, связанные с построителем поисковых запросов.
///

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"

namespace irbis {

/// \brief Отбор всех (не удалённых) документов в базе.
/// \return Построенное выражение.
///
/// Фактически строит запрос `I=$`.
Search Search::all()
{
    Search result;
    result._buffer = L"I=$";

    return result;
}

/// \brief Логическое И.
/// \param text Добавляемое выражение.
/// \return Построенное выражение.
Search& Search::and_(const String &text)
{
    this->_buffer = String(L"(")
            + this->_buffer
            + String(L" * ")
            + Search::wrap(text)
            + String(L")");
    return *this;
}

/// \brief Логическое И.
/// \param text1 Добавляемое выражение 1.
/// \param text2 Добавляемое выражение 2.
/// \return Построенное выражение.
Search& Search::and_(const String &text1, const String &text2)
{
    this->_buffer = String(L"(")
            + this->_buffer
            + String(L" * ")
            + Search::wrap(text1)
            + String(L" * ")
            + Search::wrap(text2)
            + String(L")");
    return *this;
}

/// \brief Логическое И.
/// \param text1 Добавляемое выражение 1.
/// \param text2 Добавляемое выражение 2.
/// \param text3 Добавляемое выражение 3.
/// \return Построенное выражение.
Search& Search::and_(const String &text1, const String &text2, const String &text3)
{
    this->_buffer = String(L"(")
            + this->_buffer
            + String(L" * ")
            + Search::wrap(text1)
            + String(L" * ")
            + Search::wrap(text2)
            + String(L" * ")
            + Search::wrap(text3)
            + String(L")");
    return *this;
}

/// \brief Логическое И.
/// \param item Добавляемое выражение.
/// \return Построенное выражение.
Search& Search::and_(const Search &item)
{
    return this->and_(item.toString());
}

/// \brief Логическое И.
/// \param item1 Добавляемое выражение 1.
/// \param item2 Добавляемое выражение 2.
/// \return Построенное выражение.
Search& Search::and_(const Search &item1, const Search &item2)
{
    return this->and_(item1.toString(), item2.toString());
}

/// \brief Логическое И.
/// \param item1 Добавляемое выражение 1.
/// \param item2 Добавляемое выражение 2.
/// \param item3 Добавляемое выражение 3.
/// \return Построенное выражение.
Search& Search::and_(const Search &item1, const Search &item2, const Search &item3)
{
    return this->and_(item1.toString(), item2.toString(), item3.toString());
}

/// \brief Поиск по совпадению с заданным значений.
/// \param prefix Префикс терминов в словаре.
/// \param text Искомое значение.
/// \return Возвращает построитель запросов для последующих цепочечных вызовов.
Search Search::equals(const String &prefix, const String &text)
{
    Search result;
    result._buffer = Search::wrap(prefix + text);
    return result;
}

/// \brief Поиск по совпадению с одним из перечисленных значений.
/// \param prefix Префикс терминов в словаре.
/// \param text1 Искомое значение 1.
/// \param text2 Искомое значение 2.
/// \return Возвращает построитель запросов для последующих цепочечных вызовов.
Search Search::equals(const String &prefix, const String &text1, const String &text2)
{
    Search result;
    result._buffer = String(L"(")
            + Search::wrap(prefix + text1)
            + String(L" + ")
            + Search::wrap(prefix + text2)
            + String(L")");
    return result;
}

/// \brief Поиск по совпадению с одним из перечисленных значений.
/// \param prefix Префикс терминов в словаре.
/// \param text1 Искомое значение 1.
/// \param text2 Искомое значение 2.
/// \param text3 Искомое значение 3.
/// \return Возвращает построитель запросов для последующих цепочечных вызовов.
Search Search::equals(const String &prefix, const String &text1, const String &text2, const String &text3)
{
    Search result;
    result._buffer = String(L"(")
            + Search::wrap(prefix + text1)
            + String(L" + ")
            + Search::wrap(prefix + text2)
            + String(L" + ")
            + Search::wrap(prefix + text3)
            + String(L")");
    return result;
}

/// \brief Определяет, нужно ли заключать в кавычки заданный текст.
/// \param text Текст, который нужно обернуть (или не обернуть) кавычками.
/// \return `true` если текст необходимо заключить в кавычки.
bool Search::needWrap(const String &text) noexcept
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

/// \brief Логическое НЕ.
/// \param text Правая часть оператора НЕ.
/// \return Возвращает построитель запросов для последующих цепочечных вызовов.
Search& Search::not_(const String &text)
{
    this->_buffer = String (L"(")
            + this->_buffer
            + String (L" ^ ")
            + Search::wrap(text)
            + String (L")");
    return *this;
}

/// \brief Логическое НЕ.
/// \param text Правая часть оператора НЕ.
/// \return Возвращает построитель запросов для последующих цепочечных вызовов.
Search& Search::not_(const Search &item)
{
    return this->not_(item.toString());
}

/// \brief Логическое ИЛИ.
/// \param item Добавляемое выражение.
/// \return Построенное выражение.
Search& Search::or_(const String &text)
{
    this->_buffer = String (L"(")
            + this->_buffer
            + String (L" + ")
            + Search::wrap(text)
            + String (L")");
    return *this;
}

/// \brief Логическое ИЛИ.
/// \param item Добавляемое выражение 1.
/// \param item Добавляемое выражение 2.
/// \return Построенное выражение.
Search& Search::or_(const String &text1, const String &text2)
{
    this->_buffer = String (L"(")
            + this->_buffer
            + String (L" * ")
            + Search::wrap(text1)
            + String (L" * ")
            + Search::wrap(text2)
            + String (L")");
    return *this;
}

/// \brief Логическое ИЛИ.
/// \param item Добавляемое выражение 1.
/// \param item Добавляемое выражение 2.
/// \param item Добавляемое выражение 3.
/// \return Построенное выражение.
Search& Search::or_(const String &text1, const String &text2, const String &text3)
{
    this->_buffer = String(L"(")
            + this->_buffer
            + String(L" + ")
            + Search::wrap(text1)
            + String(L" * ")
            + Search::wrap(text2)
            + String(L" + ")
            + Search::wrap(text3)
            + String(L")");
    return *this;
}

/// \brief Логическое ИЛИ.
/// \param item Добавляемое выражение.
/// \return Построенное выражение.
Search& Search::or_(const Search &item)
{
    return this->or_(item.toString());
}

/// \brief Логическое ИЛИ.
/// \param item Добавляемое выражение 1.
/// \param item Добавляемое выражение 2.
/// \return Построенное выражение.
Search& Search::or_(const Search &item1, const Search &item2)
{
    return this->or_(item1.toString(), item2.toString());
}

/// \brief Логическое ИЛИ.
/// \param item Добавляемое выражение 1.
/// \param item Добавляемое выражение 2.
/// \param item Добавляемое выражение 3.
/// \return Построенное выражение.
Search& Search::or_(const Search &item1, const Search &item2, const Search &item3)
{
    return this->or_(item1.toString(), item2.toString(), item3.toString());
}

/// \brief Оператор "в том же поле".
/// \param item Правая часть оператора.
/// \return Построенное выражение.
Search& Search::sameField(const String &text)
{
    this->_buffer = String(L"(")
            + this->_buffer
            + String (L" (G) ")
            + Search::wrap(text)
            + String (L")");
    return *this;
}

/// \brief Логическое "в том же повторении поля".
/// \param item Правая часть оператора.
/// \return Построенное выражение.
Search& Search::sameRepeat(const String &text)
{
    this->_buffer = String (L"(")
            + this->_buffer
            + String (L" (F) ")
            + Search::wrap(text)
            + String (L")");
    return *this;
}

/// \brief Выдача текста построенного выражения.
/// \return Текстовое представление.
String Search::toString() const noexcept
{
    return this->_buffer;
}

/// \brief Закавычивает (или нет) заданный текст.
/// \param text Текст для заключения в кавычки.
/// \return Закавыченный текст.
String Search::wrap(const String &text)
{
    if (Search::needWrap(text)) {
        return String (L"\"")
        + text
        + String (L"\"");
    }

    return text;
}

/// \brief Закавычивает (или нет) заданный текст.
/// \param text Текст для заключения в кавычки.
/// \return Закавыченный текст.
String Search::wrap(const Search &item)
{
    return Search::wrap (item.toString());
}

/// \brief Поиск по ключевому слову.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search keyword(const String &value1)
{
    return Search::equals(L"K=", value1);
}

/// \brief Поиск по автору.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search author(const String &value1)
{
    return Search::equals(L"A=", value1);
}

/// \brief Поиск по заглавию.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search title(const String &value1)
{
    return Search::equals(L"T=", value1);
}

/// \brief Поиск по издательству.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search publisher(const String &value1)
{
    return Search::equals(L"O=", value1);
}

/// \brief Поиск по месту издания.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search place(const String &value1)
{
    return Search::equals(L"MI=", value1);
}

/// \brief Поиск по предметной рубрике.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search subject(const String &value1)
{
    return Search::equals(L"S=", value1);
}

/// \brief Поиск по языку текста.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search language(const String &value1)
{
    return Search::equals(L"J=", value1);
}

/// \brief Поиск по году издания.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search year(const String &value1)
{
    return Search::equals(L"G=", value1);
}

/// \brief Поиск по заглавию журнала.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search magazine(const String &value1)
{
    return Search::equals(L"TJ=", value1);
}

/// \brief Поиск по виду документа.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search documentKind(const String &value1)
{
    return Search::equals(L"V=", value1);
}

/// \brief Поиск по УДК.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search udc(const String &value1)
{
    return Search::equals(L"U=", value1);
}

/// \brief Поиск по ББК.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search bbk(const String &value1)
{
    return Search::equals(L"BBK=", value1);
}

/// \brief Поиск по разделу знаний.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search rzn(const String &value1)
{
    return Search::equals(L"RZN=", value1);
}

/// \brief Поиск по месту хранения экземпляра.
/// \param value1 Искомое значение.
/// \return Построенный запрос.
Search mhr(const String &value1)
{
    return Search::equals(L"MHR=", value1);
}

}
