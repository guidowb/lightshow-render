#include "Compiler.h"

#include <stdlib.h>
#include <string.h>

Compiler::Compiler(const char *sourceName, const char *pattern) : parser(sourceName, pattern) {

}

Renderer *Compiler::compile() {
    const char *command = parser.getCommand();
    if (!strcmp("solid", command)) return compileSolid();
    else {
        parser.reportError(LEXER_ERROR, "unknown command");
        return NULL;
    }
}

Renderer *Compiler::compileSolid() {
    RGBA color = parser.getColor();
    return new SolidRenderer(color);
}

bool render(const char *sourceName, const char *pattern, Canvas *canvas) {
    Compiler compiler(sourceName, pattern);
    Renderer *renderer = compiler.compile();
    if (renderer == NULL) return false;
    renderer->render(canvas);
    delete renderer;
    return true;
}