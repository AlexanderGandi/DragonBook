//
// Created by ocean on 17/9/2026.
//

#include "Lexer.h"

#include <cstring>
#include <sstream>

static bool isHexDigit(const char c) {
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool isOctDigit(const char c) {
    return c >= '0' && c <= '7';
}

static bool isJavaLetter(const char c) {
    return isalpha(c) || c == '_' || c == '$';
}

void Lexer::forward() {
    if (p_.ptr < end_ && p_.ptr[0] == '\n') {
        p_.line += 1;
        p_.col = 0;
    }
    p_.ptr += 1;
    p_.col += 1;

    view_[0] = p_.ptr < end_ ? p_.ptr[0] : '\0';
    view_[1] = p_.ptr + 1 < end_ ? p_.ptr[1] : '\0';
}

Lexer::TokenNode * Lexer::appendTokenNode(const Token &tok) {
    auto node = new TokenNode(tok);
    node->prev = tail_->prev;
    node->next = tail_;
    tail_->prev->next = node;
    tail_->prev = node;
    return node;
}

void Lexer::skipSpaceAndComment() {
    while (true) {
        if (isspace(view_[0])) {
            forward();
            continue;
        }

        if (view_[0] == '/' && view_[1] == '/') {
            while (view_[0] != '\n' && view_[0] != '\0')
                forward();
            forward();
            continue;
        }

        if (view_[0] == '/' && view_[1] == '*') {
            forward(2);
            while (!(view_[0] == '*' && view_[1] == '/') && view_[0] != '\0')
                forward();
            forward(2);
            continue;
        }

        break;
    }
}

void Lexer::lexSymbol() {
    const auto start = p_;
    for (const auto &[symbol, tokenType] : SYMBOLS_) {
        if (p_.ptr + symbol.size() <= end_ && strncmp(p_.ptr, symbol.data(), symbol.size()) == 0) {
            forward(symbol.size() - 1);
            appendTokenNode({
                .type = tokenType, .begin = start, .end = p_
            });
            forward();
            return;
        }
    }

    throw LexerException("illegal character", p_);
}

void Lexer::lexIdOrKeyword() {
    auto start = p_;
    auto end = p_;
    while (isJavaLetter(view_[0]) || isdigit(view_[0])) {
        end = p_;
        forward();
    }
    const std::string_view identifier(start.ptr, end.ptr);
    auto it = KEYWORDS_.find(std::string(identifier));
    if (it != KEYWORDS_.end()) {
        appendTokenNode({
            .type = it->second, .begin = start, .end = end
        });
    } else {
        appendTokenNode({
            .type = TK_IDENTIFIER, .begin = start, .end = end
        });
    }
}

void Lexer::lexNumber() {
    enum {
        BEGIN,
        PREFIX_OR_DECIMAL,
        DECIMAL,
        OCTAL,
        HEX,
        FRAG,
        EXP,
        FLOATING_POSTFIX,
        INTEGER_POSTFIX,
        END,
    } status = BEGIN;
    auto start = p_;
    auto end = p_;
    TokenType type = TK_INT_LITERAL;

    while (status != END) {
        switch (status) {
            case BEGIN:
                if (view_[0] == '0') {
                    status = PREFIX_OR_DECIMAL;
                } else if (isdigit(view_[0])) {
                    status = DECIMAL;
                } else if (view_[0] == '.') {
                    type = TK_DOUBLE_LITERAL;
                    status = FRAG;
                } else {
                    status = END;
                    continue;
                }
                break;
            case PREFIX_OR_DECIMAL:
                if (view_[0] == 'x' || view_[0] == 'X') {
                    status = HEX;
                } else if (isdigit(view_[0])) {
                    status = OCTAL;
                } else if (view_[0] == '.') {
                    type = TK_DOUBLE_LITERAL;
                    status = FRAG;
                } else if (view_[0] == 'e' || view_[0] == 'E') {
                    type = TK_DOUBLE_LITERAL;
                    status = EXP;
                } else if (std::string_view("fFdD").find(view_[0]) != std::string_view::npos) {
                    type = TK_DOUBLE_LITERAL;
                    status = FLOATING_POSTFIX;
                    continue;
                } else if (std::string_view("lL").find(view_[0]) != std::string_view::npos) {
                    status = INTEGER_POSTFIX;
                    continue;
                } else {
                    status = END;
                    continue;
                }
                break;
            case DECIMAL:
                if (isdigit(view_[0])) {
                    break;
                }
                if (view_[0] == '.') {
                    status = FRAG;
                } else if (view_[0] == 'e' || view_[0] == 'E') {
                    status = EXP;
                } else if (std::string_view("lLfFdD").find(view_[0]) != std::string_view::npos) {
                    status = FLOATING_POSTFIX;
                    continue;
                } else {
                    status = END;
                    continue;
                }
                break;
            case OCTAL:
                if (isOctDigit(view_[0])) {
                    break;
                }
                if (view_[0] > '7' && view_[0] <= '9') {
                    throw LexerException("illegal digit on an octal literal", p_);
                }

                if (view_[0] == '.') {
                    type = TK_DOUBLE_LITERAL;
                    status = FRAG;
                } else if (view_[0] == 'e' || view_[0] == 'E') {
                    type = TK_DOUBLE_LITERAL;
                    status = EXP;
                } else if (std::string_view("fFdD").find(view_[0]) != std::string_view::npos) {
                    type = TK_DOUBLE_LITERAL;
                    status = FLOATING_POSTFIX;
                    continue;
                } else if (std::string_view("lL").find(view_[0]) != std::string_view::npos) {
                    status = INTEGER_POSTFIX;
                    continue;
                } else {
                    status = END;
                    continue;
                }
                break;
            case HEX:
                if (isHexDigit(view_[0])) {
                    break;
                }

                if (view_[0] == '.') {
                    throw LexerException("malformed floating-point literal", start);
                }

                if (std::string_view("lL").find(view_[0]) != std::string_view::npos) {
                    status = FLOATING_POSTFIX;
                    continue;
                } else {
                    status = END;
                    continue;
                }
                break;
            case FRAG:
                if (view_[0] == 'e' || view_[0] == 'E') {
                    status = EXP;
                } else if (std::string_view("fFdD").find(view_[0]) != std::string_view::npos) {
                    status = FLOATING_POSTFIX;
                    continue;
                } else {
                    status = END;
                    continue;
                }
                break;
            case EXP:
                if (isdigit(view_[0])) {
                    break;
                }
                if (std::string_view("fFdD").find(view_[0]) != std::string_view::npos) {
                    status = FLOATING_POSTFIX;
                    continue;
                } else {
                    status = END;
                    continue;
                }
                break;
            case FLOATING_POSTFIX:
                if (view_[0] == 'f' || view_[0] == 'F') {
                    type = TK_FLOAT_LITERAL;
                }
                if (view_[0] == 'd' || view_[0] == 'D') {
                    type = TK_DOUBLE_LITERAL;
                }
                status = END;
                break;
            case INTEGER_POSTFIX:
                if (view_[0] == 'l' || view_[0] == 'L') {
                    type = TK_LONG_LITERAL;
                }
                status = END;
                break;
            case END:
                continue;
        }
        end = p_;
        forward();
    }

    appendTokenNode({
        .type = type, .begin = start, .end = end
    });
}

void Lexer::handleDot() {
    if (isdigit(view_[1])) {
        lexNumber();
    } else {
        appendTokenNode({
            .type = TK_DOT, .begin = p_, .end = p_
        });
        forward();
    }
}

void Lexer::handleDigit() {
    lexNumber();
}

void Lexer::handleJavaLetter() {
    lexIdOrKeyword();
}

void Lexer::handlePunct() {
    lexSymbol();
}

void Lexer::lex() {
    skipSpaceAndComment();

    if (atEnd()) {
        appendTokenNode({
            .type = TK_EOF, .begin = p_, .end = p_
        });
        return;
    }

    if (isdigit(view_[0])) {
        handleDigit();
    } else if (view_[0] == '.') {
        handleDot();
    } else if (isJavaLetter(view_[0])) {
        handleJavaLetter();
    } else {
        handlePunct();
    }
}

void Lexer::parseLine() {
    std::stringstream ss(code_);
    std::string line;
    while (std::getline(ss, line)) {
        lines_.push_back(line);
    }
}

Lexer::Lexer(const std::string &code, const std::string &path) {
    code_ = code;
    p_.ptr = code.c_str();
    p_.file = path.c_str();
    begin_ = code.c_str();
    end_ = code.c_str() + code.size();
    path_ = path;
    head_ = new TokenNode();
    tail_ = new TokenNode();
    head_->next = tail_;
    tail_->prev = head_;
    current_ = head_;
    view_[0] = p_.ptr < end_ ? p_.ptr[0] : '\0';
    view_[1] = p_.ptr + 1 < end_ ? p_.ptr[1] : '\0';

    parseLine();

    nextToken();
}

void Lexer::splitCurrentToken() {
    if (!isTokenSplittable(current_->token)) {
        throw LexerException("can not split current token", current_->token.begin);
    }

}

const Token * Lexer::nextToken() {
    if (current_->next == tail_) {
        lex();
    }

    current_ = current_->next;
    return &(current_->token);
}

const char * Lexer::getLine(const Position &p) const {
    if (p.line >= lines_.size()) {
        throw LexerException("line number out of range", p);
    }

    return lines_[p.line].c_str();
}
