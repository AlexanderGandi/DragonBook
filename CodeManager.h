//
// Created by ocean on 25/9/2026.
//

#ifndef DRAGONBOOK_CODEMANAGER_H
#define DRAGONBOOK_CODEMANAGER_H
#include <string>
#include <unordered_map>

#include "CodeViewer.h"


class CodeManager {
private:
    std::unordered_map<std::string, CodeViewer *> viewers_;

public:

};


#endif //DRAGONBOOK_CODEMANAGER_H
