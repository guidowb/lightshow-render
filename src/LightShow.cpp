#include "LightShow.h"

#include <stdlib.h>

#include "Compiler.h"

bool render(const char *sourceName, const char *pattern, Canvas *canvas) {
    Compiler compiler(sourceName, pattern);
    Renderer *renderer = compiler.compile();
    if (renderer == NULL) return false;
    renderer->render(canvas);
    delete renderer;
    return true;
}