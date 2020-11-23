#include "Compiler.h"

#include <stdlib.h>
#include <string.h>
#include <vector>

Compiler::Compiler(const char *sourceName, const char *pattern) : parser(sourceName, pattern) {

}

Renderer *Compiler::compile() {
    const char *command = parser.getCommand();
    if (!strcmp("solid", command)) return compileSolid();
    else if (!strcmp("dots", command)) return compileDots();
    else {
        parser.reportError(LEXER_ERROR, "unknown command");
        return NULL;
    }
}

Renderer *Compiler::compileSolid() {
    RGBA color = parser.getColor();
    return new SolidRenderer(color);
}

Renderer *Compiler::compileDots() {
    std::vector<RGBA> colors;
    int spacing = parser.getInt();
    colors.push_back(parser.getColor());
    while (!parser.endOfCommand()) {
        colors.push_back(parser.getColor());
    }
    return new DotsRenderer(spacing, colors);
}
