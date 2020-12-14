#include <stdlib.h>

#include "LightShow.h"
#include "Compiler.h"

void render(Renderer *renderer, Canvas *canvas) {
    ((Renderer *) renderer)->render(canvas);
}

Renderer *compile(const char *sourceName, const char *pattern) {
    Compiler compiler(sourceName, pattern);
    Renderer *renderer = compiler.compile();
    return renderer;
}
