//
// Created by ocean on 18/9/2026.
//

#include <format>
#include <string_view>

#include "../src/Lexer.h"
#include "../src/Token.h"
#include "gtest/gtest.h"

void PrintTo(const TokenType tokenType, std::ostream *os) {
    *os << toString(tokenType) << "(" << tokenType << ")";
}

static Lexer *createLexer(const std::string &code) {
    auto viewer = new CodeViewer(code, InputStream::STDIN);
    return new Lexer(viewer);
}

static void checkTokenFlow(Lexer *lexer, const std::vector<TokenType> &types) {
    auto current = lexer->currentToken();
    for (auto expect : types) {
        EXPECT_EQ(current->type, expect) << "Expect token type "
        << toString(expect) << ", but got " << toString(current->type);
        current = lexer->nextToken();
    }
}

struct NumberCase {
    const char *source;
    TokenType type;
};

static void checkNumberCases(const NumberCase *cases, const size_t count) {
    for (size_t i = 0; i < count; ++i) {
        const auto &[source, expectedType] = cases[i];
        SCOPED_TRACE(std::format("Checking: {}", source));

        auto lexer = createLexer(source);
        const auto *token = lexer->currentToken();
        ASSERT_EQ(token->type, expectedType);
        EXPECT_EQ(token->span.content, std::string_view(source));
        EXPECT_EQ(lexer->nextToken()->type, TK_EOF);
    }
}

struct LiteralCase {
    const char *source;
    TokenType type;
};

static void checkLiteralCases(const LiteralCase *cases, const size_t count) {
    for (size_t i = 0; i < count; ++i) {
        const auto &[source, expectedType] = cases[i];
        SCOPED_TRACE(std::format("Checking: {}", source));

        try {
            auto lexer = createLexer(source);
            const auto *token = lexer->currentToken();
            ASSERT_EQ(token->type, expectedType);
            EXPECT_EQ(token->span.content, std::string_view(source));
            EXPECT_EQ(lexer->nextToken()->type, TK_EOF);
        } catch (LexerException &e) {
            FAIL() << "unknown lex exception throws: " << e.what();
        }
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
            auto lexer = createLexer(code);
            checkTokenFlow(lexer, flow);
        } catch (LexerException &e) {
            auto pos = e.getPosition();
            FAIL() << "Unexpected LexerException here: " << e.what() << "("
            << pos.cursor.idx << ")";
        }
    }
}

TEST(LexerTest, Symbol) {
    const std::pair<const char *, std::vector<TokenType>> CORRECT_SAMPLES[] = {
        {">>>>", {TK_UNSIGNED_RIGHT_SHIFT, TK_GREATER_THAN, TK_EOF}},
    };

    for (auto &[code, flow] : CORRECT_SAMPLES) {
        SCOPED_TRACE(std::format("Checking: {}", code));
        auto lexer = createLexer(code);
        checkTokenFlow(lexer, flow);
    }
}

TEST(LexerTest, DecimalIntegerLiterals) {
    constexpr NumberCase cases[] = {
        {"0", TK_DECIMAL_LITERAL},
        {"123", TK_DECIMAL_LITERAL},
        {"0L", TK_DECIMAL_LONG_LITERAL},
        {"123L", TK_DECIMAL_LONG_LITERAL},
    };
    checkNumberCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, HexadecimalIntegerLiterals) {
    constexpr NumberCase cases[] = {
        {"0x0", TK_HEX_LITERAL},
        {"0x1A3F", TK_HEX_LITERAL},
        {"0XabcL", TK_HEX_LONG_LITERAL},
        {"0xDadaCafe", TK_HEX_LITERAL},
    };
    checkNumberCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, OctalIntegerLiterals) {
    constexpr NumberCase cases[] = {
        {"00", TK_OCT_LITERAL},
        {"0777", TK_OCT_LITERAL},
        {"0123L", TK_OCT_LONG_LITERAL},
    };
    checkNumberCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, DecimalFloatingPointLiterals) {
    constexpr NumberCase cases[] = {
        {"123.456", TK_DECIMAL_DOUBLE_LITERAL},
        {".456", TK_DECIMAL_DOUBLE_LITERAL},
        {"123.", TK_DECIMAL_DOUBLE_LITERAL},
        {"123.e10", TK_DECIMAL_DOUBLE_LITERAL},
        {".456e-10", TK_DECIMAL_DOUBLE_LITERAL},
        {"0123.456", TK_DECIMAL_DOUBLE_LITERAL},
        {"123f", TK_DECIMAL_FLOAT_LITERAL},
        {"123D", TK_DECIMAL_DOUBLE_LITERAL},
    };
    checkNumberCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, HexadecimalFloatingPointLiterals) {
    constexpr NumberCase cases[] = {
        {"0x1.0p-5D", TK_HEX_DOUBLE_LITERAL},
        {"0XFFp10", TK_HEX_DOUBLE_LITERAL},
        {"0x.A1p2f", TK_HEX_FLOAT_LITERAL},
        {"0x10.P+3", TK_HEX_DOUBLE_LITERAL},
        {"0x123p0124", TK_HEX_DOUBLE_LITERAL},
    };
    checkNumberCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, NumberBoundaries) {
    auto lexer = createLexer("123+456");
    EXPECT_EQ(lexer->currentToken()->type, TK_DECIMAL_LITERAL);
    EXPECT_EQ(lexer->currentToken()->span.content, "123");
    EXPECT_EQ(lexer->nextToken()->type, TK_PLUS);
    EXPECT_EQ(lexer->nextToken()->type, TK_DECIMAL_LITERAL);
    EXPECT_EQ(lexer->currentToken()->span.content, "456");
    EXPECT_EQ(lexer->nextToken()->type, TK_EOF);

    lexer = createLexer("123abc");
    EXPECT_EQ(lexer->currentToken()->type, TK_DECIMAL_LITERAL);
    EXPECT_EQ(lexer->currentToken()->span.content, "123");
    EXPECT_EQ(lexer->nextToken()->type, TK_IDENTIFIER);
    EXPECT_EQ(lexer->currentToken()->span.content, "abc");
    EXPECT_EQ(lexer->nextToken()->type, TK_EOF);

    lexer = createLexer(".");
    EXPECT_EQ(lexer->currentToken()->type, TK_DOT);
    EXPECT_EQ(lexer->currentToken()->span.content, ".");
    EXPECT_EQ(lexer->nextToken()->type, TK_EOF);
}

TEST(LexerTest, InvalidNumberLiterals) {
    constexpr std::pair<const char *, const char *> ERROR_SAMPLES[] = {
        {"0x123.456", "malformed floating-point literal"},
        {"0x.p123", "hexadecimal numbers must contain at least one hexadecimal digit"},
        {"0x12.p", "malformed floating-point literal"},
        {"01238", "illegal digit on an octal literal"},
    };

    for (auto &[code, message] : ERROR_SAMPLES) {
        SCOPED_TRACE(std::format("Checking: {}", code));
        try {
            auto lexer = createLexer(code);
            FAIL() << "Expected LexerException for invalid numeric literal";
        } catch (const LexerException &e) {
            EXPECT_STREQ(e.what(), message);
        }
    }
}

TEST(LexerTest, CharacterLiterals) {
    constexpr LiteralCase cases[] = {
        {"'a'", TK_CHAR_LITERAL},
        {"'Z'", TK_CHAR_LITERAL},
        {"'7'", TK_CHAR_LITERAL},
        {"' '", TK_CHAR_LITERAL},
        {"'\\n'", TK_CHAR_LITERAL},
        {"'\\''", TK_CHAR_LITERAL},
        {"'\\\\'", TK_CHAR_LITERAL},
        {"'\\141'", TK_CHAR_LITERAL},
        {"'\\u0041'", TK_CHAR_LITERAL},
    };
    checkLiteralCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, StringLiterals) {
    constexpr LiteralCase cases[] = {
        {"\"\"", TK_STRING_LITERAL},
        {"\"hello\"", TK_STRING_LITERAL},
        {"\"hello world\"", TK_STRING_LITERAL},
        {"\"123 + 456\"", TK_STRING_LITERAL},
        {"\"line\\ntext\"", TK_STRING_LITERAL},
        {"\"quote: \\\"\"", TK_STRING_LITERAL},
        {"\"backslash: \\\\\"", TK_STRING_LITERAL},
        {"\"octal: \\141\"", TK_STRING_LITERAL},
        {"\"unicode: \\u0041\"", TK_STRING_LITERAL},
    };
    checkLiteralCases(cases, sizeof(cases) / sizeof(cases[0]));
}

TEST(LexerTest, CharacterAndStringLiteralBoundaries) {
    auto lexer = createLexer("'a'\"text\"");
    EXPECT_EQ(lexer->currentToken()->type, TK_CHAR_LITERAL);
    EXPECT_EQ(lexer->currentToken()->span.content, "'a'");
    EXPECT_EQ(lexer->nextToken()->type, TK_STRING_LITERAL);
    EXPECT_EQ(lexer->currentToken()->span.content, "\"text\"");
    EXPECT_EQ(lexer->nextToken()->type, TK_EOF);
}

TEST(LexerTest, InvalidCharacterAndStringLiterals) {
    constexpr std::pair<const char *, const char *> errorSamples[] = {
        {"''", "empty character literal"},
        {"'ab'", "unclosed character literal"},
        {"'a", "unclosed character literal"},
        {"'\\q'", "illegal escape character"},
        {"\"unterminated", "unclosed string literal"},
        {"\"bad\\q\"", "illegal escape character"},
        {"\"\\\"", "unclosed string literal"},
        {"\'\\\'", "unclosed character literal"},
    };

    for (const auto &[source, message] : errorSamples) {
        SCOPED_TRACE(std::format("Checking: {}", source));
        try {
            auto lexer = createLexer(source);
            (void) lexer;
            FAIL() << "Expected LexerException for invalid literal";
        } catch (const LexerException &e) {
            EXPECT_STREQ(e.what(), message);
        }
    }
}
