// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"
#include "irbis_pft.h"
#include "irbis_private.h"

namespace irbis {

String PftToken::toString() const
{
    return this->text;
}

//=========================================================

PftNode::PftNode (const PftToken &token)
{
    this->column = token.column;
    this->line = token.line;
    this->text = token.text;
}

String PftNode::toString() const
{
    String result;
    for (const auto &child : children) {
        result.append (child.toString());
    }

    return result;
}

//=========================================================

/// \brief Номер текущей колонки (нумерация с 1).
/// \return Номер колонки.
std::size_t PftLexer::column() const noexcept
{
    return this->navigator->column();
}

/// \brief Достигнут ли конец текста?
/// \return true, если достигнут.
bool PftLexer::eot() const noexcept
{
    return this->navigator->eot();
}

/// \brief Номер текущей строки (нумерация с 1).
/// \return Номер строки.
std::size_t PftLexer::line() const noexcept
{
    return this->navigator->line();
}

/// \brief Подглядывание на один символ вперед.
/// \return Считанный символ либо EOT.
Char PftLexer::peekChar() const noexcept
{
    return this->navigator->peekCharNoCrLf();
}

/// \brief Чтение одного символа.
/// \return Считанный символ либо EOT.
Char PftLexer::readChar() noexcept
{
    return this->navigator->readCharNoCrLf();
}

/// \brief Чтение вплоть до указанного символа.
/// \param stopChar Стоп-символ.
/// \return Считанная строка.
String PftLexer::readTo (Char stopChar)
{
    String result;
    if (this->eot()) {
        return result;
    }

    while (true) {
        const auto c = this->readChar();
        if ((c == TextNavigator::EOT) || (c == stopChar)) {
            break;
        }
        result.push_back (c);
    }

    return result;
}

/// \brief Разбирает текст PFT-скрипта на лексемы (токены).
/// \param text Текст для разбора.
/// \return Вектор лексем.
PftTokenList PftLexer::tokenize (const String &text)
{
    PftTokenList result;
    this->navigator = makeUnique<TextNavigator> (text);
    while (!this->eot()) {
        this->navigator->skipWhitespace();
        if (this->eot()) {
            break;
        }
        auto c = this->readChar();
        Char c2 = 0;
        String value;
        auto kind = TokenKind::None;
        switch (c) {
            case '\'':
                kind = TokenKind::UnconditionalLiteral;
                value = this->readTo ('\'');
                break;

            case '"':
                kind = TokenKind::ConditionalLiteral;
                value = this->readTo ('"');
                break;

            case '|':
                kind = TokenKind::RepeatableLiteral;
                value = this->readTo ('|');
                break;

            case ':':
                kind = TokenKind::Colon;
                value = L":";
                if (this->peekChar() == ':') {
                    this->readChar();
                    value = L"::";
                }
                break;

            case ';':
                kind = TokenKind::Semicolon;
                value = L";";
                break;

            case ',':
                kind = TokenKind::Comma;
                value = L",";
                break;

            case '\\':
                kind = TokenKind::Backslash;
                value = L"\\";
                break;

            case '=':
                kind= TokenKind::Equals;
                value = L"=";
                if (this->peekChar() == '=') {
                    this->readChar();
                    value = L"==";
                }
                break;

            case '(':
                kind = TokenKind::LeftParenthesis;
                value = L"(";
                break;

            case ')':
                kind = TokenKind::RightParenthesis;
                value = L")";
                break;

            case '#':
                kind = TokenKind::Hash;
                value = L"#";
                break;

            case '%':
                kind = TokenKind::Percent;
                value = L"%";
                break;

            case '^':
                kind = TokenKind::Hat;
                value = L"^";
                break;

            case '+':
                kind = TokenKind::Plus;
                value = L"+";
                break;

            case '-':
                kind = TokenKind::Minus;
                value = L"-";
                break;

            case '*':
                kind = TokenKind::Star;
                value = L"*";
                break;

            case '_':
                kind = TokenKind::Underscore;
                value = L"_";
                break;

            case '~':
                kind = TokenKind::Tilda;
                value = L"~";
                if (this->peekChar() == '~') {
                    value = L"~~";
                    this->readChar();
                }

            case '`':
                kind = TokenKind::GraveAccent;
                value = this->readTo ('`');
                break;

            case '?':
                kind = TokenKind::Question;
                value = L"?";
                break;

            case '!':
                c2 = this->peekChar();
                if (c2 == '=') {
                    kind = TokenKind::NotEqual2;
                    value = L"!=";
                    this->readChar();
                    if (this->peekChar() == '=') {
                        value = L"!==";
                        this->readChar();
                    }
                }
                else if (c2 == '~') {
                    kind = TokenKind::NotEqual2;
                    value = L"!~";
                    this->readChar();
                    if (this->peekChar() == '~') {
                        value = L"!~~";
                        this->readChar();
                    }
                }
                else {
                    kind = TokenKind::Bang;
                    value = L"!";
                }
                break;

            case '/':
                c2 = this->peekChar();
                if (c2 == '*') {
                    this->readChar();
                    kind = TokenKind::Comment;
                    // TODO this->readTo ('\r', '\n');
                    value = this->readTo ('\n');
                }
                else {
                    kind = TokenKind::Slash;
                    value = L"/";
                }
                break;

            default:
                if (value.empty()) {
                    throw IrbisException();
                }

                toLower(value);
                if (value == L"abs") {
                    kind = TokenKind::Abs;
                }
                else if (value == L"all") {
                    kind = TokenKind::All;
                }
                else if (value == L"and") {
                    kind = TokenKind::And;
                }
                else if (value == L"any") {
                    kind = TokenKind::Any;
                }
                else if (value == L"blank") {
                    kind = TokenKind::Blank;
                }
                else {
                    throw IrbisException();
                }

                break;
        }

        if (kind == TokenKind::None) {
            throw IrbisException();
        }

        PftToken token;
        token.kind = kind;
        token.column = this->column();
        token.line = this->line();
        token.text = text;
        result.push_back (std::move (token));
    }

    return result;
}

/// \brief Чтение числа с плавающей точкой.
/// \return Прочитанное число либо пустая строка.
String PftLexer::readFloat()
{
    String result;

    auto dotFound = false;
    auto digitFound = false;
    auto c = this->peekChar();
    if (c == '.') {
        dotFound = true;
    }
    if (isDigit (c)) {
        digitFound = true;
    }
    if (! (dotFound || digitFound)) {
        return result;
    }
    result.push_back (c);
    this->readChar();

    while (true) {
        c = this->peekChar();
        if (!isDigit (c)) {
            break;
        }
        digitFound = true;
        result.push_back (c);
        this->readChar();
    }

    if (!dotFound && (c == '.')) {
        result.push_back (c);
        this->readChar();

        while (true) {
            c = this->peekChar();
            if (!isDigit (c)) {
                break;
            }
            digitFound = true;
            result.push_back (c);
            this->readChar();
        }
    }

    if (!digitFound) {
        return String();
    }

    if (c == 'e' || c == 'E') {
        result.push_back (c);
        this->readChar();
        digitFound = false;
        c = this->peekChar();

        if (c == '+' || c == '-') {
            result.push_back (c);
            this->readChar();
        }

        while (true) {
            c = this->peekChar();
            if (!isDigit (c)) {
                break;
            }
            digitFound = true;
            result.push_back (c);
            this->readChar();
        }

        if (!digitFound) {
            return String();
        }
    }

    return result;
}

/// \brief Чтение целого числа.
/// \return Прочитанное число либо пустая строка.
String PftLexer::readInteger()
{
    String result;

    auto c = this->peekChar();
    if (!isDigit (c)) {
        return result;
    }
    result.push_back (c);
    this->readChar();

    while (true) {
        c = this->peekChar();
        if (!isDigit (c)) {
            break;
        }
        result.push_back (c);
        this->readChar();
    }

    return result;
}

//=========================================================

/// \brief Разбор текста.
/// \param text Текст для разбора.
/// \return Признак успешности.
bool FieldSpecification::parse (const String &text)
{
    TextNavigator navigator (text);
    auto c = navigator.readCharNoCrLf();
    switch (c) {
        case 'd':
        case 'D':
            this->command = 'd';
            break;

        case 'g':
        case 'G':
            this->command = 'g';
            break;

        case 'n':
        case 'N':
            this->command = 'n';
            break;

        case 'v':
        case 'V':
            this->command = 'v';
            break;

        default:
            return false;
    }

    c = navigator.readCharNoCrLf();
    if (!isDigit (c)) {
        return false;
    }
    String s;
    s.push_back (c);
    while (true) {
        c = navigator.peekCharNoCrLf();
        if (!isDigit (c)) {
            break;
        }
        navigator.readCharNoCrLf();
        s.push_back (c);
    }
    this->tag = fastParse32 (s);

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();
    if (c == '@') {
        s.clear();
        navigator.readCharNoCrLf();
    }

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();

    if (c == '[') {
        navigator.readCharNoCrLf();
        navigator.skipWhitespace();
    }

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();

    if (c == '^') {
        navigator.readCharNoCrLf();
        if (navigator.eot()) {
            return false;
        }
    }

    if (this->command != 'v' && this->command != 'g') {
        return true;
    }

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();

    if (c == '*') {
        navigator.readCharNoCrLf();
        navigator.skipWhitespace();
        s.clear();
        while (true) {
            c = navigator.peekCharNoCrLf();
            if (!isDigit (c)) {
                break;
            }
            s.push_back (c);
            navigator.readCharNoCrLf();
        }
        if (s.empty()) {
            return false;
        }
        this->offset = fastParse32 (s);
    }

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();

    if (c == '.') {
        navigator.readCharNoCrLf();
        navigator.skipWhitespace();
        s.clear();
        while (true) {
            c = navigator.peekCharNoCrLf();
            if (!isDigit (c)) {
                break;
            }
            s.push_back (c);
            navigator.readCharNoCrLf();
        }
        if (s.empty()) {
            return false;
        }
        this->length = fastParse32 (s);
    }

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();

    if (c == '(') {
        navigator.readCharNoCrLf();
        navigator.skipWhitespace();
        s.clear();
    }

    return true;
}

/// \brief Разбор краткого варианта спецификации.
bool FieldSpecification::parseShort (const String &text)
{
    TextNavigator navigator (text);
    auto c = navigator.readCharNoCrLf();
    switch (c) {
        case 'g':
        case 'G':
            this->command = 'g';
            break;

        case 'v':
        case 'V':
            this->command = 'v';
            break;

        default:
            return false;
    }

    c = navigator.readCharNoCrLf();
    if (!isDigit (c)) {
        return false;
    }
    String s;
    s.push_back (c);
    while (true) {
        c = navigator.peekCharNoCrLf();
        if (!isDigit (c)) {
            break;
        }
        navigator.readCharNoCrLf();
        s.push_back (c);
    }
    this->tag = fastParse32 (s);

    navigator.skipWhitespace();
    c = navigator.peekCharNoCrLf();

    if (c == '^') {
        navigator.readCharNoCrLf();
        if (navigator.eot()) {
            return false;
        }
        this->subfield = navigator.readCharNoCrLf();
    }

    return true;
}

/// \brief Разбор спецификации поля в стиле Unifor.
bool FieldSpecification::parseUnifor (const String &text)
{
    TextNavigator navigator (text);
    auto c = navigator.readCharNoCrLf();
    switch (c) {
        case 'v':
        case 'V':
            this->command = 'v';
            break;

        default:
            return false;
    }

    c = navigator.readCharNoCrLf();
    if (!isDigit (c)) {
        return false;
    }
    String s;
    s.push_back (c);
    while (true) {
        c = navigator.peekCharNoCrLf();
        if (!isDigit (c)) {
            break;
        }
        navigator.readCharNoCrLf();
        s.push_back (c);
    }
    this->tag = fastParse32 (s);

    if (c == '^') {
        navigator.readCharNoCrLf();
        if (navigator.eot()) {
            return false;
        }
        this->subfield = navigator.readCharNoCrLf();
    }

    c = navigator.peekCharNoCrLf();

    if (c == '*') {
        navigator.readCharNoCrLf();
        navigator.skipWhitespace();
        s.clear();

        while (true) {
            c = navigator.peekCharNoCrLf();
            if (!isDigit (c)) {
                break;
            }
            navigator.readCharNoCrLf();
            s.push_back (c);
        }
        if (s.empty()) {
            return false;
        }
        this->offset = fastParse32 (s);
    }

    c = navigator.peekCharNoCrLf();

    if (c == '.') {
        navigator.readCharNoCrLf();
        navigator.skipWhitespace();
        s.clear();

        while (true) {
            c = navigator.peekCharNoCrLf();
            if (!isDigit (c)) {
                break;
            }
            navigator.readCharNoCrLf();
            s.push_back (c);
        }
        if (s.empty()) {
            return false;
        }
        this->length = fastParse32 (s);
    }

    return true;
}

/// \brief Преобразование в строку.
/// \return Строковое представление.
String FieldSpecification::toString() const
{
    String result;
    result.push_back (this->command);
    result.append (std::to_wstring (this->tag));
    if (!this->embedded.empty()) {
        result.push_back ('@');
        result.append (this->embedded);
    }
    if (this->subfield != 0) {
        result.push_back ('^');
        result.push_back (this->subfield);
    }
    if (this->offset != 0) {
        result.push_back ('*');
        result.append (std::to_wstring (this->offset));
    }
    if (this->length != 0) {
        result.push_back ('.');
        result.append (std::to_wstring (this->length));
    }
    if (paragraphIndent != 0) {
        result.push_back ('(');
        result.append (std::to_wstring (this->paragraphIndent));
        result.push_back (')');
    }
    return result;
}


//=========================================================

void PftProgram::execute (PftContext &context)
{
    // TODO implement
}

}
