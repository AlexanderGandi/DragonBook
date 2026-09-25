//
// Created by ocean on 25/9/2026.
//

#ifndef DRAGONBOOK_EXCEPTIONREPORTER_H
#define DRAGONBOOK_EXCEPTIONREPORTER_H
#include "Lexer.h"


class ExceptionReporter {
public:
    ExceptionReporter() = default;
    void report(LexerException &e);
    void report(CodeViewerException &e);
};


#endif //DRAGONBOOK_EXCEPTIONREPORTER_H
