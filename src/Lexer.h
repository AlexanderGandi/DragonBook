//
// Created by ocean on 17/9/2026.
//

#ifndef DRAGONBOOK_LEXER_H
#define DRAGONBOOK_LEXER_H
#include <string>
#include <unordered_map>
#include <vector>

#include "CodeViewer.h"
#include "Position.h"
#include "Token.h"
#include "LexerUtils.h"


class LexerException : public std::exception {
private:
    std::string message_;
    Position position_;

public:
    explicit LexerException(const std::string &&msg, const Position &pos)
        : message_(msg), position_(pos) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

    [[nodiscard]] const Position &getPosition() const noexcept {
        return position_;
    }
};


class Lexer {
    struct TokenNode {
        Token token;
        TokenNode *prev = nullptr;
        TokenNode *next = nullptr;

        TokenNode() = default;

        explicit TokenNode(const Token &tok) : token(tok) {}
    };


    TokenNode *head_;
    TokenNode *tail_;
    TokenNode *current_;
    CodeViewer *viewer_;

    inline static const std::unordered_map<std::string_view, TokenType> KEYWORDS_ {
        {"abstract", TK_KW_ABSTRACT},
        {"continue", TK_KW_CONTINUE},
        {"for", TK_KW_FOR},
        {"new", TK_KW_NEW},
        {"switch", TK_KW_SWITCH},
        {"assert", TK_KW_ASSERT},
        {"default", TK_KW_DEFAULT},
        {"if", TK_KW_IF},
        {"package", TK_KW_PACKAGE},
        {"synchronized", TK_KW_SYNCHRONIZED},
        {"boolean", TK_KW_BOOLEAN},
        {"do", TK_KW_DO},
        {"goto", TK_KW_GOTO},
        {"private", TK_KW_PRIVATE},
        {"this", TK_KW_THIS},
        {"break", TK_KW_BREAK},
        {"double", TK_KW_DOUBLE},
        {"implements", TK_KW_IMPLEMENTS},
        {"protected", TK_KW_PROTECTED},
        {"throw", TK_KW_THROW},
        {"byte", TK_KW_BYTE},
        {"else", TK_KW_ELSE},
        {"import", TK_KW_IMPORT},
        {"public", TK_KW_PUBLIC},
        {"throws", TK_KW_THROWS},
        {"case", TK_KW_CASE},
        {"enum", TK_KW_ENUM},
        {"instanceof", TK_KW_INSTANCEOF},
        {"return", TK_KW_RETURN},
        {"transient", TK_KW_TRANSIENT},
        {"catch", TK_KW_CATCH},
        {"extends", TK_KW_EXTENDS},
        {"int", TK_KW_INT},
        {"short", TK_KW_SHORT},
        {"try", TK_KW_TRY},
        {"char", TK_KW_CHAR},
        {"final", TK_KW_FINAL},
        {"interface", TK_KW_INTERFACE},
        {"static", TK_KW_STATIC},
        {"void", TK_KW_VOID},
        {"class", TK_KW_CLASS},
        {"finally", TK_KW_FINALLY},
        {"long", TK_KW_LONG},
        {"strictfp", TK_KW_STRICTFP},
        {"volatile", TK_KW_VOLATILE},
        {"const", TK_KW_CONST},
        {"float", TK_KW_FLOAT},
        {"native", TK_KW_NATIVE},
        {"super", TK_KW_SUPER},
        {"while", TK_KW_WHILE},
        {"true", TK_TRUE_LITERAL},
        {"false", TK_FALSE_LITERAL},
        {"null", TK_NULL_LITERAL},
    };

    static constexpr TokenType SPLITTABLE_TOKEN[] {
        TK_UNSIGNED_RIGHT_SHIFT_ASSIGN,
        TK_UNSIGNED_RIGHT_SHIFT,
        TK_RIGHT_SHIFT_ASSIGN,
        TK_LEFT_SHIFT_ASSIGN,
        TK_RIGHT_SHIFT,
        TK_GREATER_EQ,
        TK_LESS_EQ,
        TK_LEFT_SHIFT,
    };

    static constexpr std::pair<std::string_view, TokenType> SYMBOLS_[] {
        // operators
        {">>>=", TK_UNSIGNED_RIGHT_SHIFT_ASSIGN},
        {">>>", TK_UNSIGNED_RIGHT_SHIFT},
        {">>=", TK_RIGHT_SHIFT_ASSIGN},
        {"<<=", TK_LEFT_SHIFT_ASSIGN},
        {">>", TK_RIGHT_SHIFT},
        {">=", TK_GREATER_EQ},
        {"<=", TK_LESS_EQ},
        {"==", TK_EQUAL},
        {"!=", TK_NOT_EQUAL},
        {"<<", TK_LEFT_SHIFT},
        {"+=", TK_PLUS_ASSIGN},
        {"++", TK_PLUS_PLUS},
        {"-=", TK_MINUS_ASSIGN},
        {"--", TK_MINUS_MINUS},
        {"*=", TK_MULT_ASSIGN},
        {"/=", TK_DIV_ASSIGN},
        {"%=", TK_MOD_ASSIGN},
        {"&=", TK_BIT_AND_ASSIGN},
        {"|=", TK_BIT_OR_ASSIGN},
        {"^=", TK_BIT_XOR_ASSIGN},
        {"&&", TK_LOGIC_AND},
        {"||", TK_LOGIC_OR},
        {"&", TK_BIT_AND},
        {"|", TK_BIT_OR},
        {"^", TK_BIT_XOR},
        {"!", TK_LOGIC_NOT},
        {"~", TK_BIT_NOT},
        {"+", TK_PLUS},
        {"-", TK_MINUS},
        {"*", TK_MULT},
        {"/", TK_DIV},
        {"%", TK_MOD},
        {"?", TK_QUESTION},
        {":", TK_COLON},
        {"=", TK_ASSIGN},
        {"<", TK_LESS_THAN},
        {">", TK_GREATER_THAN},

        // separators
        {"(", TK_LEFT_PAREN},
        {")", TK_RIGHT_PAREN},
        {"{", TK_LEFT_BRACE},
        {"}", TK_RIGHT_BRACE},
        {"[", TK_LEFT_BRACKET},
        {"]", TK_RIGHT_BRACKET},
        {";", TK_SEMICOLON},
        {".", TK_DOT},
        {",", TK_COMMA}
    };

    TokenNode * appendTokenNode(const Token &tok);

    static constexpr bool isTokenSplittable(const Token &token) {
        const auto tokenType = token.type;
        for (auto type : SPLITTABLE_TOKEN) {
            if (tokenType == type) {
                return true;
            }
        }
        return false;
    }

    static constexpr std::string_view CONST_ESCAPE_SEQUENCE{"btnfr\"'\\"};

    // number handlers
    int matchHexPrefix(TokenType &type) const;
    int matchZeroPrefixedLiteral(TokenType &type) const;
    int matchDecimalOrFloatingLiteral(TokenType &type) const;

    [[nodiscard]] int consumeHexDigits(int &offset) const {
        const int n = offset;
        for (; isHexDigit(viewer_->peek(offset)); ++offset) {}
        return offset - n;
    }

    [[nodiscard]] int consumeDigits(int &offset) const {
        const int n = offset;
        for (; isDigit(viewer_->peek(offset)); ++offset) {}
        return offset - n;
    }

    [[nodiscard]] int consumeOctEscape(int &offset) const;
    [[nodiscard]] int parseEscapeChars(int &offset) const;


    void skipSpaceAndComment();
    void lexSymbol();
    void lexIdOrKeyword();
    void lexNumber();
    void lexCharLiteral();
    void lexString();

    void handleDot();
    void handleDigit();
    void handleJavaLetter();
    void handleSingleQuote();
    void handleDoubleQuote();
    void handlePunct();

    void lex();

public:
    explicit Lexer(CodeViewer *viewer);

    void splitCurrentToken();

    const Token *nextToken();
    [[nodiscard]] const Token *currentToken() const {
        return &(current_->token);
    }
};


#endif //DRAGONBOOK_LEXER_H
