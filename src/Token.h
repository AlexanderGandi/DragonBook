//
// Created by ocean on 17/9/2026.
//

#ifndef DRAGONBOOK_TOKEN_H
#define DRAGONBOOK_TOKEN_H
#include "Position.h"

enum TokenType {
    // operators
    TK_PLUS,                        // +
    TK_PLUS_ASSIGN,                 // +=
    TK_PLUS_PLUS,                   // ++
    TK_MINUS,                       // -
    TK_MINUS_ASSIGN,                // -=
    TK_MINUS_MINUS,                 // --
    TK_MULT,                        // *
    TK_MULT_ASSIGN,                 // *=
    TK_DIV,                         // /
    TK_DIV_ASSIGN,                  // /=
    TK_MOD,                         // %
    TK_MOD_ASSIGN,                  // %=
    TK_BIT_AND,                     // &
    TK_BIT_AND_ASSIGN,              // &=
    TK_BIT_OR,                      // |
    TK_BIT_OR_ASSIGN,               // |=
    TK_BIT_XOR,                     // ^
    TK_BIT_XOR_ASSIGN,              // ^=
    TK_LOGIC_AND,                   // &&
    TK_LOGIC_OR,                    // ||
    TK_LOGIC_NOT,                   // !
    TK_BIT_NOT,                     // ~
    TK_QUESTION,                    // ?
    TK_COLON,                       // :
    TK_ASSIGN,                      // =
    TK_EQUAL,                       // ==
    TK_NOT_EQUAL,                   // !=
    TK_LESS_THAN,                   // <
    TK_GREATER_THAN,                // >
    TK_LESS_EQ,                     // <=
    TK_GREATER_EQ,                  // >=
    TK_RIGHT_SHIFT,                 // >>
    TK_UNSIGNED_RIGHT_SHIFT,        // >>>
    TK_LEFT_SHIFT,                  // <<
    TK_RIGHT_SHIFT_ASSIGN,          // >>=
    TK_UNSIGNED_RIGHT_SHIFT_ASSIGN, // >>>=
    TK_LEFT_SHIFT_ASSIGN,           // <<=

    TK_ANNOTATION,                  // @
    TK_ARROW,                       // ->

    // separators
    TK_COMMA,                       // ,
    TK_LEFT_PAREN,                  // (
    TK_RIGHT_PAREN,                 // )
    TK_LEFT_BRACE,                  // {
    TK_RIGHT_BRACE,                 // }
    TK_LEFT_BRACKET,                // [
    TK_RIGHT_BRACKET,               // ]
    TK_DOT,                         // .
    TK_SEMICOLON,                   // ;

    TK_IDENTIFIER,

    // literals
    TK_HEX_LITERAL,
    TK_OCT_LITERAL,
    TK_DECIMAL_LITERAL,
    TK_HEX_LONG_LITERAL,
    TK_OCT_LONG_LITERAL,
    TK_DECIMAL_LONG_LITERAL,
    TK_DECIMAL_FLOAT_LITERAL,
    TK_DECIMAL_DOUBLE_LITERAL,
    TK_HEX_FLOAT_LITERAL,
    TK_HEX_DOUBLE_LITERAL,
    TK_CHAR_LITERAL,
    TK_TRUE_LITERAL,
    TK_FALSE_LITERAL,
    TK_NULL_LITERAL,
    TK_STRING_LITERAL,

    // keywords
    TK_KW_ABSTRACT,
    TK_KW_CONTINUE,
    TK_KW_FOR,
    TK_KW_NEW,
    TK_KW_SWITCH,
    TK_KW_ASSERT,
    TK_KW_DEFAULT,
    TK_KW_IF,
    TK_KW_PACKAGE,
    TK_KW_SYNCHRONIZED,
    TK_KW_BOOLEAN,
    TK_KW_DO,
    TK_KW_GOTO,
    TK_KW_PRIVATE,
    TK_KW_THIS,
    TK_KW_BREAK,
    TK_KW_DOUBLE,
    TK_KW_IMPLEMENTS,
    TK_KW_PROTECTED,
    TK_KW_THROW,
    TK_KW_BYTE,
    TK_KW_ELSE,
    TK_KW_IMPORT,
    TK_KW_PUBLIC,
    TK_KW_THROWS,
    TK_KW_CASE,
    TK_KW_ENUM,
    TK_KW_INSTANCEOF,
    TK_KW_RETURN,
    TK_KW_TRANSIENT,
    TK_KW_CATCH,
    TK_KW_EXTENDS,
    TK_KW_INT,
    TK_KW_SHORT,
    TK_KW_TRY,
    TK_KW_CHAR,
    TK_KW_FINAL,
    TK_KW_INTERFACE,
    TK_KW_STATIC,
    TK_KW_VOID,
    TK_KW_CLASS,
    TK_KW_FINALLY,
    TK_KW_LONG,
    TK_KW_STRICTFP,
    TK_KW_VOLATILE,
    TK_KW_CONST,
    TK_KW_FLOAT,
    TK_KW_NATIVE,
    TK_KW_SUPER,
    TK_KW_WHILE,

    TK_EOF,
};

const char *toString(TokenType tokenType);

struct Token {
    TokenType type = TK_EOF;
    Span span;
};


#endif //DRAGONBOOK_TOKEN_H
