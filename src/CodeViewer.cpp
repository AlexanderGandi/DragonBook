//
// Created by ocean on 19/9/2026.
//

#include "CodeViewer.h"

#include <format>
#include <fstream>
#include <sstream>

static std::string readFromFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw CodeViewerException("Can not open file " + path);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    auto content = ss.str();
    file.close();
    return content;
}

void CodeViewer::parseLine() {
    lineStarts_.push_back(0);
    for (auto i = 0; i < codeView_.size(); ++i) {
        if (codeView_[i] == '\n') {
            lines_.push_back(
                codeView_.substr(lineStarts_.back(), i - lineStarts_.back() + 1)
            );
            if (i + 1 < codeView_.size()) {
                lineStarts_.push_back(i + 1);
            }
        }
    }
}

int CodeViewer::findLineByIndex(const int i) const {
    if (i >= code_.size()) {
        throw CodeViewerException(std::format("index {} out of range", i));
    }
    const auto len = lineStarts_.size();
    for (auto start = 0; start < len && lineStarts_[start] < i; ++start) {
        if (start + 1 < len && lineStarts_[start + 1] >= i) {
            return start;
        }
    }
    return lineStarts_.size();
}

CodeViewer::CodeViewer(const std::string &input, InputStream stream)
: stream_(stream) {
    switch (stream_) {
        case InputStream::STDIN:
            code_ = input;
            break;
        case InputStream::FILE:
            path_ = input;
            code_ = readFromFile(path_);
            break;
    }
    codeView_ = code_;

    parseLine();
}

void CodeViewer::forward() {
    if (c_.idx == codeView_.size()) {
        return;
    }

    ++c_.idx;
}

Line CodeViewer::getLine(const Cursor &cursor) const {
    auto line = findLineByIndex(cursor.idx);
    return {
        line,
        lines_[line]
    };
}

Line CodeViewer::getLine(int line) const {
    if (line < 1 || line >= lines_.size()) {
        throw CodeViewerException(std::format("line index {} out of range", line));
    }
    return {
        line,
        lines_[line]
    };
}
