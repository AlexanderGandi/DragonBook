//
// Created by ocean on 18/9/2026.
//

#include <format>

#include "Lexer.h"
#include "Token.h"
#include "gtest/gtest.h"

void PrintTo(const TokenType tokenType, std::ostream *os) {
    *os << toString(tokenType) << "(" << tokenType << ")";
}

static void checkTokenFlow(Lexer &lexer, const std::vector<TokenType> &types) {
    auto current = lexer.currentToken();
    for (auto expect : types) {
        EXPECT_EQ(current->type, expect) << "Expect token type "
        << toString(expect) << ", but got " << toString(current->type);
        current = lexer.nextToken();
    }
}

TEST(LexerTest, CommentAndSpace) {
    const std::pair<const char *, std::vector<TokenType>> SAMPLES[] = {
        {"\n\n// hello", {TK_EOF}},
        {"/* hello world */", {TK_EOF}},
        {"   /* hello world */ \r\n\r // how are you", {TK_EOF}},
        {"/* hello */ */", {TK_MULT, TK_DIV, TK_EOF}}
    };

    for (auto &[code, flow] : SAMPLES) {
        SCOPED_TRACE(std::format("Testing: {}", code));
        try {
            auto lexer = Lexer(code, "");
            checkTokenFlow(lexer, flow);
        } catch (LexerException &e) {
            auto pos = e.getPosition();
            FAIL() << "Unexpected LexerException here: " << e.what() << "("
            << pos.line << ", " << pos.col << ")";
        }
    }
}

TEST(LexerTest, Symbol) {
    const std::pair<const char *, std::vector<TokenType>> CORRECT_SAMPLES[] = {
        {">>>>", {TK_UNSIGNED_RIGHT_SHIFT, TK_GREATER_THAN, TK_EOF}},
    };

    for (auto &[code, flow] : CORRECT_SAMPLES) {
        SCOPED_TRACE(std::format("Checking: {}", code));
        auto lexer = Lexer(code, "");
        checkTokenFlow(lexer, flow);
    }
}

TEST(LexerTest, Number) {
    const std::pair<const char *, std::vector<TokenType>> CORRECT_SAMPLES[] = {
        {"0", {TK_INT_LITERAL, TK_EOF}},
        {"0l", {TK_LONG_LITERAL, TK_EOF}},
        {"0xDadaCafe", {TK_INT_LITERAL, TK_EOF}},
        {"7", {TK_INT_LITERAL, TK_EOF}},
        {"12345", {TK_INT_LITERAL, TK_EOF}},
        {"0x456", {TK_INT_LITERAL, TK_EOF}},
        {"0XABCDEF", {TK_INT_LITERAL, TK_EOF}},
        {"01234", {TK_INT_LITERAL, TK_EOF}},
        {"123f", {TK_FLOAT_LITERAL, TK_EOF}},
        {"123F", {TK_FLOAT_LITERAL, TK_EOF}},
        {"12347d", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"12347D", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0f", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0d", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123f", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0123F", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0123d", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123D", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123.", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"123456.f", {TK_FLOAT_LITERAL, TK_EOF}},
        {"123456.F", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0123.d", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123.D", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123.f", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0123.F", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0123e0123", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123E0124", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0123e0123f", {TK_FLOAT_LITERAL, TK_EOF}},
        {"0123E0124D", {TK_DOUBLE_LITERAL, TK_EOF}},
        {"0x123E0124", {TK_INT_LITERAL, TK_EOF}},
        {"0x123E0124L", {TK_INT_LITERAL, TK_EOF}},

    };

    for (auto &[code, flow] : CORRECT_SAMPLES) {
        SCOPED_TRACE(std::format("Checking: {}", code));
        auto lexer = Lexer(code, "");
        checkTokenFlow(lexer, flow);
    }
}

TEST(LexerTest, InvalidNumber) {
    constexpr std::pair<const char *, const char *> ERROR_SAMPLES[] = {
        {"01238", "illegal digit on an octal literal"},
        {"0x123.456", "malformed floating-point literal"},
    };

    for (auto &[code, message] : ERROR_SAMPLES) {
        SCOPED_TRACE(std::format("Checking: {}", code));
        try {
            Lexer lexer(code, "");
            FAIL() << "Expected LexerException for invalid numeric literal";
        } catch (const LexerException &exception) {
            EXPECT_STREQ(exception.what(), message);
        }
    }
}
