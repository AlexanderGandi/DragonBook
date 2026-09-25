//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_ASTNODE_H
#define DRAGONBOOK_ASTNODE_H
#include <cstdint>
#include <string_view>
#include <vector>

enum ASTNodeType {
    N_EMPTY,
    N_UNARY_OP,
    N_BINARY_OP,
};

struct ASTNode {
    ASTNodeType type{N_EMPTY};
    ASTNode *parent{nullptr};
    std::vector<ASTNode *> children;

    std::string_view op;
    int8_t byteValue{0};
    int16_t shortValue{0};
    int32_t intValue{0};
    int64_t longValue{0};

    void addChild(ASTNode *node) {
        node->parent = this;
        children.push_back(node);
    }
};


#endif //DRAGONBOOK_ASTNODE_H
