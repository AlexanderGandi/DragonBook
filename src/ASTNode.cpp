//
// Created by ocean on 19/9/2026.
//

#include "ASTNode.h"

BinaryOp::BinaryOp(Token *token, ASTNode *left, ASTNode *right) : token(token) {
    addChild(left);
    addChild(right);
}

UnaryOp::UnaryOp(Token *token, ASTNode *operand) : token(token) {
    addChild(operand);
}
