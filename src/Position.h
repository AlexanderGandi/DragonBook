//
// Created by ocean on 17/9/2026.
//

#ifndef DRAGONBOOK_POSITION_H
#define DRAGONBOOK_POSITION_H
#include <string_view>


struct Cursor {
    int idx = 0;
};

struct Line {
    int line;
    std::string_view content;
};

struct Position {
    Cursor cursor;
    const char *file = nullptr;
};

struct Span {
    std::string_view content;
    Cursor begin;
    Cursor end;
    const char *file = nullptr;

    [[nodiscard]] Position getStartPosition() const {
        return Position {
            .cursor = begin,
            .file = file
        };
    }
};


#endif //DRAGONBOOK_POSITION_H
