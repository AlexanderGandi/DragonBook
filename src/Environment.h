//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_SYMBOLTABLE_H
#define DRAGONBOOK_SYMBOLTABLE_H
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum Modifier : uint16_t {
    M_PUBLIC = 0x01,
    M_PROTECTED = 0x02,
    M_PRIVATE = 0x04,
    M_FINAL = 0x08,
    M_STATIC = 0x10,
};

enum class SymbolClass : uint16_t {
    CLASS,
    METHOD,
    VARIABLE,
    PACKAGE,
};

struct Symbol {
    std::string name;
    std::string dataType;
    uint16_t flags{0};
    SymbolClass clazz{SymbolClass::VARIABLE};
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

    void define(const Symbol &symbol) {
        table_[symbol.name] = symbol;
    }

    Symbol *resolve(const std::string &name) {
        auto it = table_.find(name);
        if (it != table_.end()) {
            return &(it->second);
        }
        if (parent_ != nullptr) {
            return parent_->resolve(name);
        }
        return nullptr;
    }

    Environment *getParent() const { return parent_; }
};


#endif //DRAGONBOOK_SYMBOLTABLE_H
