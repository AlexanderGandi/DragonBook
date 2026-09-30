//
// Created by ocean on 25/9/2026.
//

#include "CodeManager.h"

void CodeManager::addCodeViewer(const std::string &input, InputStream stream) {
    switch (stream) {
        case InputStream::STDIN:
            stdinViewer_ = new CodeViewer(input, stream);
            break;
        case InputStream::FILE:
            viewers_[input] = new CodeViewer(input, stream);
            break;
    }
}

CodeViewer * CodeManager::getCodeViewer(const std::string &file) {
    auto it = viewers_.find(file);
    if (it == viewers_.end()) {
        return nullptr;
    }

    return it->second;
}
