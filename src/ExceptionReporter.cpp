//
// Created by ocean on 25/9/2026.
//

#include "ExceptionReporter.h"

#include <format>
#include <iostream>

void ExceptionReporter::report(const LexerException &e) {
    auto &p = e.getPosition();
    auto file = p.file;
    CodeViewer *viewer = nullptr;

    if (file == nullptr) {
        viewer = manager_->getStdInCodeViewer();
        file = "@stdin";
    } else {
        viewer = manager_->getCodeViewer(file);
    }

    if (viewer == nullptr) {
        return;
    }

    const auto [line, content] = viewer->getLine(p.cursor);

    std::cerr << std::format("{}:{}: error: {}", file, line, e.what()) << std::endl;
    std::cerr << content;

    if (content.back() != '\n') {
        std::cerr << std::endl;
    }

    const auto col = viewer->getColIdx(p.cursor);
    for (int i = 1; i < col; ++i) {
        std::cerr << " ";
    }
    std::cerr << "^" << std::endl;
    std::cerr << "error: compilation failed" << std::endl;
}

void ExceptionReporter::report(const CodeViewerException &e) {
}
