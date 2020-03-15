#ifndef PLUSIRBIS_IRBIS_PFT_H
#define PLUSIRBIS_IRBIS_PFT_H

#include "irbis.h"

#include <memory>
#include <utility>

namespace irbis {

//=========================================================

class FieldSpecification;
class IndexSpecification;
class PftContext;
class PftField;
class PftGroup;
class PftLexer;
class PftNode;
class PftNumeric;
class PftToken;

using PftNodeList = std::vector<PftNode>;
using PftTokenList = std::vector<PftToken>;

//=========================================================

/// \brief Разновидности токенов.
enum class TokenKind {
    None,
    A,
    Abs,
    All,
    At,
    And,
    Any,
    Backslash,
    Bang,
    Blank,
    Break,
    C,
    Ceil,
    Colon,
    Comma,
    Comment,
    ConditionalLiteral,
    Div,
    Do,
    EatClose,
    EatOpen,
    Else,
    Empty,
    End,
    Equals,
    Eval,
    F,
    Fmt,
    False,
    Fi,
    First,
    Floor,
    For,
    ForEach,
    Frac,
    From,
    Global,
    GraveAccent,
    Hash,
    Hat,
    Have,
    Identifier,
    If,
    In,
    L,
    Last,
    LeftCurly,
    LeftParenthesis,
    LeftSquare,
    Less,
    LessEqual,
    Local,
    Mfn,
    Mpl,
    Minus,
    More,
    MoreEqual,
    Nl,
    Not,
    NotEqual1,
    NotEqual2,
    Number,
    Order,
    P,
    Parallel,
    Percent,
    Plus,
    Pow,
    Proc,
    Question,
    Ravr,
    RepeatableLiteral,
    RightCurly,
    RightSquare,
    RightParenthesis,
    Ref,
    Rmax,
    Rmin,
    Round,
    Rsum,
    S,
    Select,
    Semicolon,
    Sign,
    Slash,
    Star,
    Then,
    True,
    Trunc,
    Tilda,
    TripleCurly,
    TripleLess,
    Underscore,
    Unifor,
    UnconditionalLiteral,
    V,
    Val,
    Variable,
    Where,
    While,
    With,
    X
};

//=========================================================

/// \brief Токен.
class IRBIS_API PftToken {
public:
    TokenKind kind { TokenKind::None }; ///< Вид токена.
    std::size_t column { 0 };           ///< Номер колонки (нумерация с 1).
    std::size_t line { 0 };             ///< Номер строки (нумерация с 1).
    String text;                        ///< Связанный текст.

    String toString() const;
};

//=========================================================

/// \brief Вид индекса для поля.
enum class IndexKind
{
    None,           ///< Нет индекса.
    Literal,        ///< Простой литерал, например, 3.
    Expression,     ///< Выражение, например, $x + 1.
    LastRepeat,     ///< Последнее повторение, кодируется `*`.
    NewRepeat,      ///< Новое повторение, кодируется `+`.
    CurrentRepeat,  ///< Текущее повторение, кодируется `.`.
    AllRepeats      ///< Все повторения, кодируется `-`.
};

//=========================================================

/// \brief Спецификация индекса для полей.
class IRBIS_API IndexSpecification final
{
public:
    IndexKind kind { IndexKind::None };  ///< Вид индекса.
    int literal { 0 };                   ///< Значение литерала.
    String expression;                   ///< Выражение.
    std::unique_ptr<PftNumeric> program; ///< Компилированное выражение.
};

//=========================================================

/// \brief Спецификация поля/подполя.
class IRBIS_API FieldSpecification final
{
public:
    char command { 0 };        ///< Код команды (обязательно в нижнем регистре)
    String embedded;           ///< Метка встроенного поля.
    int firstLine { 0 };       ///< Красная строка.
    int paragraphIndent;       ///< Общий абзацный отступ.
    int offset { 0 };          ///< Смещение.
    int length { 0 };          ///< Длина.
    IndexSpecification repeat; ///< Спецификация повторения.
    int tag { 0 };             ///< Метка поля
    Char subfield { 0 };       ///< Спецификация подполя.
    String raw;                ///< Сырая строка.

    bool   parse       (const String &text);
    bool   parseShort  (const String &text);
    bool   parseUnifor (const String &text);
    String toString() const;
};

//=========================================================

/// \brief Разбирает PFT-скрипт на лексемы (токены).
class IRBIS_API PftLexer final
{
public:

    std::size_t column() const noexcept;
    bool eot() const noexcept;
    std::size_t line() const noexcept;
    PftTokenList tokenize (const String &text);

private:
    std::unique_ptr<TextNavigator> navigator;

    Char peekChar() const noexcept;
    Char readChar() noexcept;
    String readFloat();
    String readInteger();
    String readTo (Char stop);
};

//=========================================================

/// \brief Контекст исполнения PFT-скрипта.
class IRBIS_API PftContext
{
public:
    PftContext *parent { nullptr };            ///< Родительский контекст.
    MarcRecord *record { nullptr };            ///< Ассоциированная запись.
    MarcRecord *alternativeRecord { nullptr }; ///< Альтернативная запись.
    PftGroup   *currentGroup { nullptr };      ///< Текущая группа.
    int index { 0 };                           ///< Текущий номер повторения.
    bool outputFlag { false };                 ///< Признак, что был осуществлен вывод поля.
    PftField *currentField { nullptr };        ///< Текущее поле.
};

//=========================================================

/// \brief Узел AST-дерева.
class IRBIS_API PftNode
{
public:
    PftNode *parent { nullptr }; ///< Родительский узел.
    PftNodeList children;        ///< Узлы-потомки (если есть).
    std::size_t column { 0 };    ///< Номер колонки (нумерация с 1).
    std::size_t line { 0 };      ///< Номер строки (нумерация с 1).
    String text;                 ///< Ассоциированный текст.

    PftNode() = default;
    explicit PftNode (const PftToken &token);

    virtual void execute (PftContext &context) {};
    virtual String toString () const;
};

//=========================================================

class IRBIS_API PftGroup
{
public:
};

//=========================================================

class IRBIS_API PftField
{
public:
};

//=========================================================

/// \brief Корневой узел AST-дерева.
class IRBIS_API PftProgram : public PftNode
{
public:

    void execute (PftContext &context) override;
};

}

#endif //PLUSIRBIS_IRBIS_PFT_H
