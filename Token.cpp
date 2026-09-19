//
// Created by ocean on 17/9/2026.
//

#include "Token.h"

namespace {
constexpr const char *TOKEN_TYPE_NAMES[] = {
    "TokenType::TK_LEFT_PAREN",
    "TokenType::TK_RIGHT_PAREN",
    "TokenType::TK_LEFT_BRACE",
    "TokenType::TK_RIGHT_BRACE",
    "TokenType::TK_LEFT_BRACKET",
    "TokenType::TK_RIGHT_BRACKET",
    "TokenType::TK_LINE_COMMENT",
    "TokenType::TK_BLOCK_COMMENT_BEGIN",
    "TokenType::TK_BLOCK_COMMENT_END",
    "TokenType::TK_PLUS",
    "TokenType::TK_MINUS",
    "TokenType::TK_MULT",
    "TokenType::TK_DIV",
    "TokenType::TK_DOT",
    "TokenType::TK_SEMICOLON",
    "TokenType::TK_ARROW",
    "TokenType::TK_ASSIGN",
    "TokenType::TK_EQUAL",
    "TokenType::TK_NOT_EQUAL",
    "TokenType::TK_LESS_THAN",
    "TokenType::TK_GREATER_THAN",
    "TokenType::TK_LESS_EQ",
    "TokenType::TK_GREATER_EQ",
    "TokenType::TK_RIGHT_SHIFT",
    "TokenType::TK_UNSIGNED_RIGHT_SHIFT",
    "TokenType::TK_LEFT_SHIFT",
    "TokenType::TK_IDENTIFIER",
    "TokenType::TK_INT_LITERAL",
    "TokenType::TK_LONG_LITERAL",
    "TokenType::TK_FLOAT_LITERAL",
    "TokenType::TK_DOUBLE_LITERAL",
    "TokenType::TK_KW_IMPORT",
    "TokenType::TK_KW_PACKAGE",
    "TokenType::TK_KW_CLASS",
    "TokenType::TK_KW_PUBLIC",
    "TokenType::TK_KW_PRIVATE",
    "TokenType::TK_KW_PROTECTED",
    "TokenType::TK_KW_STATIC",
    "TokenType::TK_KW_SHORT",
    "TokenType::TK_KW_INT",
    "TokenType::TK_KW_LONG",
    "TokenType::TK_KW_FLOAT",
    "TokenType::TK_KW_DOUBLE",
    "TokenType::TK_KW_CHAR",
    "TokenType::TK_KW_BYTE",
    "TokenType::TK_EOF",
};
}

const char *toString(TokenType tokenType) {
    return TOKEN_TYPE_NAMES[static_cast<int>(tokenType)];
}
