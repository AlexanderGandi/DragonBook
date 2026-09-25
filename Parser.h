//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_PARSER_H
#define DRAGONBOOK_PARSER_H
#include "ASTNode.h"
#include "Lexer.h"


class Parser {
private:
    Lexer *lexer_;

    ASTNode *expr();
    ASTNode *assignment();
    ASTNode *conditional();
    ASTNode *logicalOr();
    ASTNode *logicalAnd();
    ASTNode *exclusiveOrExpression();
    ASTNode *andExpression();
    ASTNode *equalityExpression();
    ASTNode *relationalExpression();
    ASTNode *shiftExpression();
    ASTNode *additiveExpression();
    ASTNode *multiplicativeExpression();
    ASTNode *unaryExpression();
    ASTNode *unaryExpressionNotPlusMinus();
    ASTNode *postfixExpression();
    ASTNode *primary();
    ASTNode *assignmentOperator();

public:
    explicit Parser(Lexer *lexer) : lexer_(lexer) {}
};


#endif //DRAGONBOOK_PARSER_H
