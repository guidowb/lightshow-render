#include "Compiler.h"

#include <stdlib.h>
#include <string.h>
#include <vector>

Compiler::Compiler(const char *sourceName, const char *pattern) : parser(sourceName, pattern) {

}

Renderer *Compiler::compile() {
    return compileBlock();
}

Renderer *Compiler::compileCommand() {
    const Word &command = parser.getCommand();
    if (command == "solid") return compileSolid();
    else if (command == "dots") return compileDots();
    else {
        parser.reportError(LEXER_ERROR, "unknown command");
        parser.endCommand();
        return NULL;
    }
}

Renderer *Compiler::compileBlock() {
    std::vector<Renderer *> renderers;
    while (!parser.endOfBlock()) {
        renderers.push_back(compileCommand());
    }
    parser.endCommand();
    return new BlockRenderer(renderers);
}

Renderer *Compiler::compileSolid() {
    RGBA color = parser.getColor();
    parser.endCommand();
    return new SolidRenderer(color);
}

Renderer *Compiler::compileDots() {
    std::vector<RGBA> colors;
    int spacing = parser.getInt();
    colors.push_back(parser.getColor());
    while (!parser.endOfCommand()) {
        colors.push_back(parser.getColor());
    }
    parser.endCommand();
    return new DotsRenderer(spacing, colors);
}
