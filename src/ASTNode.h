//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_ASTNODE_H
#define DRAGONBOOK_ASTNODE_H
#include <cstdint>
#include <string_view>
#include <vector>

#include "Token.h"

enum class ASTNodeType {
    EMPTY,
    UNARY_OP,
    BINARY_OP,
};

struct ASTNode {
    ASTNodeType type{ASTNodeType::EMPTY};
    ASTNode *parent{nullptr};
    std::vector<ASTNode *> children;

    void addChild(ASTNode *node) {
        node->parent = this;
        children.push_back(node);
    }
};

struct Literal : public ASTNode {
    Token *token;
    explicit Literal(Token *token) : token(token) {}
};

struct BinaryOp : public ASTNode {
    Token *token;
    BinaryOp(Token *token, ASTNode *left, ASTNode *right);
};

struct UnaryOp : public ASTNode {
    Token *token;
    UnaryOp(Token *token, ASTNode *operand);
};

struct Type : public ASTNode {
    Token *token;
    explicit Type(Token *token) : token(token) {}
};


#endif //DRAGONBOOK_ASTNODE_H
