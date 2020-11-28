#include "Compiler.h"

#include <stdlib.h>
#include <string.h>
#include <vector>

#define printf(fmt, ...) {}

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
        parser.reportError(LEXER_ERROR, "Unknown command");
        parser.skipCommand();
        return NULL;
    }
}

Renderer *Compiler::compileBlock() {
    const Word savedIndent = parser.startBlock();
    std::vector<Renderer *> renderers;
    while (parser.inBlock()) {
        printf("add renderer to block\n");
        renderers.push_back(compileCommand());
        if (renderers.size() > 3) break;
    }
    printf("block with %d renderers\n", (int) renderers.size());
    parser.endBlock(savedIndent);
    return new BlockRenderer(renderers);
}

Renderer *Compiler::compileSolid() {
    RGBA color = parser.getColor();
    parser.endCommand();
    return new SolidRenderer(color);
}

Renderer *Compiler::compileDots() {
    std::vector<RGBA> colors;
    int spacing = parser.getInteger();
    colors.push_back(parser.getColor());
    while (parser.hasArgument()) {
        colors.push_back(parser.getColor());
    }
    parser.endCommand();
    return new DotsRenderer(spacing, colors);
}
