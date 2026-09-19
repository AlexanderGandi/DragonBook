//
// Created by ocean on 19/9/2026.
//

#include "CodeViewer.h"

CodeViewer::CodeViewer(const std::string &input, InputStream stream) : stream_(stream) {
    switch (stream) {
        case InputStream::STDIN:
            code_ = input;
        case InputStream::FILE:
            readFromFile();
    }

    position_.ptr = code_.c_str();

    begin_ = code_.c_str();
    end_ = code_.c_str() + code_.size();
}

void CodeViewer::forward() {
    auto it = code_.begin();
    position_.ptr = it.base();
}
