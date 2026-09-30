//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_PARSER_H
#define DRAGONBOOK_PARSER_H
#include "ASTNode.h"
#include "Environment.h"
#include "Lexer.h"


class ParserException : std::exception {
private:
    std::string message_;
    Position position_;
public:
    ParserException(std::string &&msg, const Position &pos) : message_(msg), position_(pos) {}
    [[nodiscard]] const char *what() const noexcept override { return message_.c_str(); }
    [[nodiscard]] const Position& getPosition() const noexcept { return position_; }
};

class Parser {
private:
    Lexer *lexer_;
    Environment *env_;

    ASTNode *expression();
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
    ASTNode *preIncrementExpression();
    ASTNode *preDecrementExpression();

    /**
     * @par PostfixExpression
     *      Primary
     *       ExpressionName
     *       PostIncrementExpression
     *       PostDecrementExpression
     *
     * @return pointer to ASTNode
     */
    ASTNode *postfixExpression();

    /**
     * Primary:
     *      PrimaryNoNewArray
     *      ArrayCreationExpression
     *
     *  PrimaryNoNewArray:
     *      Literal
     *      Type . class
     *      void . class
     *      this
     *      ClassName.this
     *      ( Expression )
     *      ClassInstanceCreationExpression
     *      FieldAccess
     *      MethodInvocation
     *      ArrayAccess
     * @return pointer to ASTNode
     */
    ASTNode *primary();
    ASTNode *assignmentOperator();

    ASTNode *type();

public:
    explicit Parser(Lexer *lexer, Environment *env) : lexer_(lexer), env_(env) {}

    ASTNode *parse();
};


#endif //DRAGONBOOK_PARSER_H
