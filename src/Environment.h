//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_SYMBOLTABLE_H
#define DRAGONBOOK_SYMBOLTABLE_H
#include <string>
#include <unordered_map>
#include <vector>

enum class SymbolScope {
    PUBLIC,
    PROTECT,
    PRIVATE,
    STACK,
};

struct Symbol {
    std::string name;
    std::string type;
    SymbolScope scope;
};

using SymbolTable = std::unordered_map<std::string, Symbol>;

class Environment {
    SymbolTable table_;
    std::vector<Environment *> children_;
    Environment *parent_{nullptr};

public:
    Environment() = default;
    explicit Environment(Environment *parent) : parent_(parent) {}

    void addChild(Environment *table) {
        table->parent_ = this;
        children_.push_back(table);
    }

    Symbol *get(const std::string &name) {
        Environment *current = this;
        while (current != nullptr) {
            auto &table = current->table_;
            auto it = table.find(name);
            if (it != table.end()) {
                return &(it->second);
            }
            current = current->parent_;
        }
        return nullptr;
    }
};


#endif //DRAGONBOOK_SYMBOLTABLE_H
