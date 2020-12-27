#ifndef ARDUINO

#include "PrettyPrint.h"

const int INDENT = 3;

PrettyPrint::PrettyPrint() {
    currentIndent = 0;
}

void PrettyPrint::startObject(const char *kind) {
    printf("%s\n", kind);
    currentIndent += INDENT;
}

void PrettyPrint::endObject() {
    currentIndent -= INDENT;
}

void PrettyPrint::addProperty(const char *name, Renderer *value) {
    printf("%*s%s: ", currentIndent, "", name);
    value->serialize(*this);
}

void PrettyPrint::addProperty(const char *name, const char *value) {
    printf("%*s%s: \"%s\"\n", currentIndent, "", name, value);
}

void PrettyPrint::addProperty(const char *name, const RGBA &color) {
    printf("%*s%s: #%02x%02x%02x%02x\n", currentIndent, "", name, color.r, color.g, color.b, color.a);
}

void PrettyPrint::addProperty(const char *name, uint32_t value) {
    printf("%*s%s: %u\n", currentIndent, "", name, value);
}

void PrettyPrint::addProperty(const char *name, int ncolors, const RGBA *color) {
    printf("%*s%s[%d]: [ ", currentIndent, "", name, ncolors);
    for (int i = 0; i < ncolors; i++) {
        printf("#%02x%02x%02x%02x", color[i].r, color[i].g, color[i].b, color[i].a);
        if (i < ncolors - 1) printf(", ");
    }
    printf(" ]\n");
}

#endif