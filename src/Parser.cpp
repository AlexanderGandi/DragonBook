//
// Created by ocean on 19/9/2026.
//

#include "Parser.h"

ASTNode * Parser::expression() {
}

ASTNode * Parser::assignment() {
}

ASTNode * Parser::conditional() {
}

ASTNode * Parser::logicalOr() {
}

ASTNode * Parser::logicalAnd() {
}

ASTNode * Parser::exclusiveOrExpression() {
}

ASTNode * Parser::andExpression() {
}

ASTNode * Parser::equalityExpression() {
}

ASTNode * Parser::relationalExpression() {
}

ASTNode * Parser::shiftExpression() {
}

ASTNode * Parser::additiveExpression() {
}

ASTNode * Parser::multiplicativeExpression() {
}

ASTNode * Parser::unaryExpression() {
    auto token = lexer_->eatToken();

    switch (token->type) {
        case TK_PLUS_PLUS:
        case TK_MINUS_MINUS:
        case TK_PLUS:
        case TK_MINUS:
        case TK_LOGIC_NOT:
        case TK_BIT_NOT:
            return new UnaryOp(token, unaryExpression());
        case TK_IDENTIFIER:
            break;
        default:
            throw ParserException("fail to parse unary operator", token->span.getStartPosition());
    }
}

ASTNode * Parser::unaryExpressionNotPlusMinus() {
}

ASTNode * Parser::preIncrementExpression() {
}

ASTNode * Parser::preDecrementExpression() {
}

ASTNode * Parser::postfixExpression() {
    auto current = lexer_->currentToken();
    switch (current->type) {
        case TK_OCT_LITERAL:
        case TK_DECIMAL_LITERAL:
        case TK_HEX_LITERAL:
        case TK_OCT_LONG_LITERAL:
        case TK_DECIMAL_LONG_LITERAL:
        case TK_HEX_LONG_LITERAL:
        case TK_DECIMAL_DOUBLE_LITERAL:
        case TK_DECIMAL_FLOAT_LITERAL:
        case TK_HEX_DOUBLE_LITERAL:
        case TK_HEX_FLOAT_LITERAL:
        case TK_CHAR_LITERAL:
        case TK_STRING_LITERAL:
        case TK_NULL_LITERAL:
        case TK_BOOLEAN_LITERAL:
        case TK_KW_NEW:
        case TK_LEFT_PAREN:
            return primary();
    }
}

ASTNode * Parser::primary() {
    auto token = lexer_->eatToken();
    ASTNode *node = nullptr;
    switch (token->type) {
        case TK_OCT_LITERAL:
        case TK_DECIMAL_LITERAL:
        case TK_HEX_LITERAL:
        case TK_OCT_LONG_LITERAL:
        case TK_DECIMAL_LONG_LITERAL:
        case TK_HEX_LONG_LITERAL:
        case TK_DECIMAL_DOUBLE_LITERAL:
        case TK_DECIMAL_FLOAT_LITERAL:
        case TK_HEX_DOUBLE_LITERAL:
        case TK_HEX_FLOAT_LITERAL:
        case TK_CHAR_LITERAL:
        case TK_STRING_LITERAL:
        case TK_NULL_LITERAL:
        case TK_BOOLEAN_LITERAL:
            node = new Literal(token);
            break;
        case TK_KW_NEW:
            // TODO: new array expression
            break;
        case TK_LEFT_PAREN:
            node = expression();
            if (lexer_->currentToken()->type != TK_RIGHT_PAREN) {
                throw ParserException("')' expected", lexer_->currentToken()->span.getStartPosition());
            }
            break;
        default:
            throw ParserException("expect a primary", token->span.getStartPosition());
    }

    return node;
}

ASTNode * Parser::assignmentOperator() {

}

ASTNode * Parser::type() {
    auto token = lexer_->eatToken();
    switch (token->type) {
        case TK_KW_BYTE:
        case TK_KW_SHORT:
        case TK_KW_INT:
        case TK_KW_LONG:
        case TK_KW_CHAR:
        case TK_KW_FLOAT:
        case TK_KW_DOUBLE:
            return new Type(token);
        case TK_IDENTIFIER:
            break;
        default:
            throw ParserException("type name expected", token->span.getStartPosition());
    }
}

ASTNode * Parser::parse() {
}
