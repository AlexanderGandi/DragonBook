//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_CODEVIEWER_H
#define DRAGONBOOK_CODEVIEWER_H
#include <cstdint>
#include <string>
#include <vector>

#include "Position.h"

enum class InputStream : int {
    STDIN,
    FILE,
};


class CodeViewer {
    std::string code_;
    std::string path_;
    InputStream stream_;
    const char *begin_{nullptr};
    const char *end_{nullptr};
    Position position_;
    std::vector<std::string> lines_;
    uint32_t view_[2]{};

    void readFromFile();
public:
    CodeViewer(const std::string &input, InputStream stream);
    void forward();
};


#endif //DRAGONBOOK_CODEVIEWER_H
