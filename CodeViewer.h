//
// Created by ocean on 19/9/2026.
//

#ifndef DRAGONBOOK_CODEVIEWER_H
#define DRAGONBOOK_CODEVIEWER_H

#include <string>
#include <vector>

#include "Position.h"

enum class InputStream : int {
    STDIN,
    FILE,
};


class CodeViewerException : public std::exception {
private:
    std::string message_;

public:
    explicit CodeViewerException(const std::string &&msg)
        : message_(msg) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }
};


class CodeViewer {
    std::string code_;
    std::string_view codeView_;
    std::string path_;
    InputStream stream_;
    Cursor c_;
    std::vector<std::string_view> lines_;
    std::vector<int> lineStarts_;

    void parseLine();
    [[nodiscard]] int findLineByIndex(int i) const;

public:

    CodeViewer(const std::string &input, InputStream stream);
    void forward();
    void forward(const int n) {
        for (int i = 0; i < n; i++) {
            forward();
        }
    }

    Span eat(const int n) {
        const auto temp = c_;
        forward(n);
        return {
            .content = codeView_.substr(temp.idx, n),
            .begin = temp,
            .end = c_
        };
    }

    [[nodiscard]] char peek() const {
        return c_.idx < codeView_.size() ? codeView_[c_.idx] : '\0';
    }

    [[nodiscard]] char peek(const int n) const {
        if (atEnd(n)) {
            return '\0';
        }
        return codeView_[c_.idx + n];
    }

    [[nodiscard]] std::string_view slice(int n) const {
        n = c_.idx + n < codeView_.size() ? n : (codeView_.size() - c_.idx);
        return codeView_.substr(c_.idx, n);
    }

    [[nodiscard]] bool atEnd() const { return c_.idx >= codeView_.size(); }
    [[nodiscard]] bool atEnd(const int n) const { return c_.idx + n >= codeView_.size(); }

    [[nodiscard]] Line getLine(const Cursor &cursor) const;
    [[nodiscard]] Line getLine(int line) const;
    [[nodiscard]] const Position getPosition() const { return { .cursor = c_, .file = path_.c_str() }; }
    [[nodiscard]] const Position getPosition(int n) const {
        return {
            .cursor = {c_.idx + n},
            .file = path_.c_str()
        };
    }
};


#endif //DRAGONBOOK_CODEVIEWER_H
