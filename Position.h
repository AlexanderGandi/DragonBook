//
// Created by ocean on 17/9/2026.
//

#ifndef DRAGONBOOK_POSITION_H
#define DRAGONBOOK_POSITION_H


struct Position {
    const char *ptr = nullptr;
    int line = 1;
    int col = 1;
    const char *file = nullptr;
};


#endif //DRAGONBOOK_POSITION_H
