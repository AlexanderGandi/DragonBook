#include <iostream>

#include "CodeManager.h"
#include "ExceptionReporter.h"
#include "Lexer.h"

int main() {
    std::string code = "public class Main {\n"
"    public static void main(String[] args) {\n"
"        var a = 012834;\n"
"        System.out.println(\"a = \" + a);\n"
"    }\n"
"}";
    auto manager = new CodeManager();
    auto reporter = new ExceptionReporter(manager);
    manager->addCodeViewer(code, InputStream::STDIN);

    auto viewer = manager->getStdInCodeViewer();

    try {
        auto lexer = new Lexer(viewer);
        while (lexer->currentToken()->type != TK_EOF) {
            lexer->nextToken();
        }
    } catch (LexerException &e) {
        reporter->report(e);
    }

    return 0;
}
