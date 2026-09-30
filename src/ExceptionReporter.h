//
// Created by ocean on 25/9/2026.
//

#ifndef DRAGONBOOK_EXCEPTIONREPORTER_H
#define DRAGONBOOK_EXCEPTIONREPORTER_H
#include "CodeManager.h"
#include "Lexer.h"


class ExceptionReporter {
private:
    CodeManager *manager_;
public:
    explicit ExceptionReporter(CodeManager *manager) : manager_(manager) {}
    void report(const LexerException &e);
    void report(const CodeViewerException &e);
};


#endif //DRAGONBOOK_EXCEPTIONREPORTER_H
