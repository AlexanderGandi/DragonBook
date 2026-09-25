//
// Created by ocean on 17/9/2026.
//

#include "Lexer.h"

#include <format>
#include <sstream>



Lexer::TokenNode * Lexer::appendTokenNode(const Token &tok) {
    auto node = new TokenNode(tok);
    node->prev = tail_->prev;
    node->next = tail_;
    tail_->prev->next = node;
    tail_->prev = node;
    return node;
}

int Lexer::matchHexPrefix(TokenType &type) const {
    auto prefix = viewer_->slice(2);
    if (prefix != "0x" && prefix != "0X") {
        return 0;
    }
    int n = 2;
    int hexNumeralCnt = 0;
    int hexFracCnt = 0;
    type = TK_HEX_LITERAL;
    hexNumeralCnt = consumeHexDigits(n);
    char c;

    if (viewer_->peek(n) == '.') {
        type = TK_HEX_DOUBLE_LITERAL;
        ++n;
        hexFracCnt = consumeHexDigits(n);

        c = viewer_->peek(n);
        if (c != 'p' && c != 'P') {
            throw LexerException("malformed floating-point literal", viewer_->getPosition());
        }
    }
    c = viewer_->peek(n);
    if (c == 'p' || c == 'P') {
        if (hexNumeralCnt == 0 && hexFracCnt == 0) {
            throw LexerException("hexadecimal numbers must contain at least one hexadecimal digit", viewer_->getPosition());
        }
        type = TK_HEX_DOUBLE_LITERAL;
        c = viewer_->peek(++n);
        if (c == '+' || c == '-') {
            ++n;
        }
        auto expCnt = consumeDigits(n);
        if (expCnt == 0) {
            throw LexerException("malformed floating-point literal", viewer_->getPosition());
        }
    }

    c = viewer_->peek(n);
    if (isLongSuffix(c) && type == TK_HEX_LITERAL) {
        type = TK_HEX_LONG_LITERAL;
        ++n;
    } else if (isFloatSuffix(c)) {
        type = TK_HEX_FLOAT_LITERAL;
        ++n;
    } else if (isDoubleSuffix(c)) {
        type = TK_HEX_DOUBLE_LITERAL;
        ++n;
    }
    return n;
}

int Lexer::matchZeroPrefixedLiteral(TokenType &type) const {
    auto c = viewer_->peek();
    if (c != '0') {
        return 0;
    }
    auto n = 1;
    type = TK_DECIMAL_LITERAL;
    int fracCnt = 0;
    int intCnt = 1;
    int nonOctDigit = -1;

    while (isDigit(c = viewer_->peek(n))) {
        type = TK_OCT_LITERAL;
        if (!isOctDigit(c) && nonOctDigit == -1) {
            nonOctDigit = n;
        }
        ++n;
        ++intCnt;
    }

    if (viewer_->peek(n) == '.') {
        type = TK_DECIMAL_DOUBLE_LITERAL;
        ++n;
        fracCnt = consumeDigits(n);
    }

    c = viewer_->peek(n);
    if (c == 'e' || c == 'E') {
        if (intCnt == 0 && fracCnt == 0) {
            throw LexerException("malformed floating-point literal", viewer_->getPosition());
        }
        type = TK_DECIMAL_DOUBLE_LITERAL;
        c = viewer_->peek(++n);
        if (c == '+' || c == '-') {
            ++n;
        }
        auto expCnt = consumeDigits(n);
        if (expCnt == 0) {
            throw LexerException("malformed floating-point literal", viewer_->getPosition());
        }
    }

    c = viewer_->peek(n);
    if (isLongSuffix(c)) {
        if (type == TK_OCT_LITERAL) {
            type = TK_OCT_LONG_LITERAL;
            ++n;
        } else if (type == TK_DECIMAL_LITERAL) {
            type = TK_DECIMAL_LONG_LITERAL;
            ++n;
        }
    } else if (isFloatSuffix(c)) {
        type = TK_DECIMAL_FLOAT_LITERAL;
        ++n;
    } else if (isDoubleSuffix(c)) {
        type = TK_DECIMAL_DOUBLE_LITERAL;
        ++n;
    }

    if (nonOctDigit > 0 && (type == TK_OCT_LITERAL || type == TK_OCT_LONG_LITERAL)) {
        throw LexerException("illegal digit on an octal literal", viewer_->getPosition(nonOctDigit));
    }

    return n;
}

int Lexer::matchDecimalOrFloatingLiteral(TokenType &type) const {
    type = TK_DECIMAL_LITERAL;
    auto n = 0;
    auto intCnt = consumeDigits(n);
    int fracCnt = 0;

    if (viewer_->peek(n) == '.') {
        type = TK_DECIMAL_DOUBLE_LITERAL;
        ++n;
        fracCnt = consumeDigits(n);
    }

    auto c = viewer_->peek(n);
    if (c == 'e' or c == 'E') {
        if (intCnt == 0 && fracCnt == 0) {
            throw LexerException("malformed floating-point literal", viewer_->getPosition());
        }
        type = TK_DECIMAL_DOUBLE_LITERAL;
        c = viewer_->peek(++n);
        if (c == '+' || c == '-') {
            ++n;
        }
        auto expCnt = consumeDigits(n);
        if (expCnt == 0) {
            throw LexerException("malformed floating-point literal", viewer_->getPosition());
        }
    }

    if (isLongSuffix(c) && type == TK_DECIMAL_LITERAL) {
        type = TK_DECIMAL_LONG_LITERAL;
        ++n;
    } else if (isFloatSuffix(c)) {
        type = TK_DECIMAL_FLOAT_LITERAL;
        ++n;
    } else if (isDoubleSuffix(c)) {
        type = TK_DECIMAL_DOUBLE_LITERAL;
        ++n;
    }

    return n;
}

int Lexer::consumeOctEscape(int &offset) const {
    const auto c1 = viewer_->peek(offset);
    const auto c2 = viewer_->peek(offset + 1);
    const auto c3 = viewer_->peek(offset + 2);

    if (!isOctDigit(c1)) {
        return 0;
    }

    if (!isOctDigit(c2)) {
        offset += 1;
        return 1;
    }

    if (!isOctDigit(c3) || c1 < '0' || c1 > '3') {
        offset += 2;
        return 2;
    }

    offset += 3;
    return 3;
}

int Lexer::parseEscapeChars(int &offset) const {
    auto n = 1;
    auto i = offset + 1;
    auto c = viewer_->peek(i);
    if (CONST_ESCAPE_SEQUENCE.find(c) != std::string_view::npos) {
        ++n;
        ++i;
    } else if (isOctDigit(c)) {
        n += consumeOctEscape(i);
    } else if (c == 'u') {
        while ((c = viewer_->peek(++i)) == 'u') {}
        for (int j = 0; j < 4; ++j) {
            if (!isHexDigit(viewer_->peek(i + j))) {
                throw LexerException("illegal unicode escape", viewer_->getPosition(i + j));
            }
        }
        i += 4;
        n += 4;
    } else {
        throw LexerException("illegal escape character", viewer_->getPosition(i));
    }

    offset = i;
    return n;
}

void Lexer::skipSpaceAndComment() {
    while (true) {
        if (isspace(viewer_->peek())) {
            viewer_->forward();
            continue;
        }

        if (viewer_->slice(2) == "//") {
            while (viewer_->peek() != '\0' && viewer_->peek() != '\n')
                viewer_->forward();
            viewer_->forward();
            continue;
        }

        if (viewer_->slice(2) == "/*") {
            viewer_->forward(2);
            while (viewer_->peek() != '\0' && viewer_->slice(2) != "*/")
                viewer_->forward();
            viewer_->forward(2);
            continue;
        }

        break;
    }
}

void Lexer::lexSymbol() {
    for (const auto &[symbol, tokenType] : SYMBOLS_) {
        auto peek = viewer_->slice(symbol.size());
        if (peek == symbol) {
            appendTokenNode({
                .type = tokenType, .span = viewer_->eat(symbol.size())
            });
            return;
        }
    }

    throw LexerException("illegal character", viewer_->getPosition());
}

void Lexer::lexIdOrKeyword() {
    int n = 0;
    for (n = 0; isJavaLetter(viewer_->peek(n)) || isDigit(viewer_->peek(n)); ++n) {}

    const auto span = viewer_->eat(n);
    const auto it = KEYWORDS_.find(span.content);

    appendTokenNode({
        .type = it == KEYWORDS_.end() ? TK_IDENTIFIER : it->second,
        .span = span
    });
}

void Lexer::lexNumber() {
    TokenType type;
    int n = 0;
    if ((n = matchHexPrefix(type)) != 0) {
        appendTokenNode({
            .type = type, .span = viewer_->eat(n)
        });
    } else if ((n = matchZeroPrefixedLiteral(type)) != 0) {
        appendTokenNode({
            .type = type, .span = viewer_->eat(n)
        });
    } else if ((n = matchDecimalOrFloatingLiteral(type)) != 0) {
        appendTokenNode({
            .type = type, .span = viewer_->eat(n)
        });
    } else {
        throw LexerException("fail to lexing a number", viewer_->getPosition());
    }
}

void Lexer::lexCharLiteral() {
    int n = 1;
    auto c = viewer_->peek(n);

    if (c == '\'') {
        throw LexerException("empty character literal", viewer_->getPosition());
    }

    if (c != '\\') {
        if (viewer_->peek(++n) != '\'') {
            throw LexerException("unclosed character literal", viewer_->getPosition());
        }
        appendTokenNode({
            .type = TK_CHAR_LITERAL, .span = viewer_->eat(n + 1)
        });
        return;
    }

    (void)parseEscapeChars(n);
    c = viewer_->peek(n++);
    if (c != '\'') {
        throw LexerException("unclosed character literal", viewer_->getPosition());
    }

    appendTokenNode({
        .type = TK_CHAR_LITERAL, .span = viewer_->eat(n + 1)
    });
}

void Lexer::lexString() {
    auto n = 1;
    auto c = viewer_->peek(n);
    while (c != '\0' && c != '"') {
        if (c == '\\') {
            (void)parseEscapeChars(n);
        } else {
            ++n;
        }
        c = viewer_->peek(n);
    }

    if (c != '"') {
        throw LexerException("unclosed string literal", viewer_->getPosition());
    }

    appendTokenNode({
        .type = TK_STRING_LITERAL, .span = viewer_->eat(n + 1)
    });
}

void Lexer::handleDot() {
    if (isDigit(viewer_->peek(1))) {
        lexNumber();
    } else {
        appendTokenNode({
            .type = TK_DOT, .span = viewer_->eat(1),
        });
    }
}

void Lexer::handleDigit() {
    lexNumber();
}

void Lexer::handleJavaLetter() {
    lexIdOrKeyword();
}

void Lexer::handleSingleQuote() {
    lexCharLiteral();
}

void Lexer::handleDoubleQuote() {
    lexString();
}

void Lexer::handlePunct() {
    lexSymbol();
}

void Lexer::lex() {
    skipSpaceAndComment();

    if (viewer_->atEnd()) {
        appendTokenNode({
            .type = TK_EOF, .span = viewer_->eat(1)
        });
        return;
    }

    auto c = viewer_->peek();

    if (isdigit(c)) {
        handleDigit();
    } else if (c == '.') {
        handleDot();
    } else if (c == '\'') {
        handleSingleQuote();
    } else if (c == '"') {
        handleDoubleQuote();
    } else if (isJavaLetter(c)) {
        handleJavaLetter();
    } else {
        handlePunct();
    }
}

Lexer::Lexer(CodeViewer *viewer) : viewer_(viewer) {
    head_ = new TokenNode();
    tail_ = new TokenNode();
    head_->next = tail_;
    tail_->prev = head_;
    current_ = head_;

    nextToken();
}

void Lexer::splitCurrentToken() {
    if (!isTokenSplittable(current_->token)) {
        throw LexerException("can not split current token", current_->token.span.getStartPosition());
    }

}

const Token * Lexer::nextToken() {
    if (current_->next == tail_) {
        lex();
    }

    current_ = current_->next;
    return &(current_->token);
}
