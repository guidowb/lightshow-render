#include "Compiler.h"

#include <stdlib.h>
#include <string.h>

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
    else if (command == "twinkle") return compileTwinkle();
    else if (command == "segment") return compileSegment();
    else if (command == "gradient") return compileGradient();
    else {
        parser.reportError(LEXER_ERROR, "Unknown command");
        parser.skipCommand();
        return new NullRenderer();
    }
}

Renderer *Compiler::compileBlock() {
    const Word savedIndent = parser.startBlock();
    Vector<Renderer *> renderers(20);
    while (parser.inBlock()) {
        printf("add renderer to block\n");
        renderers.append(compileCommand());
    }
    printf("block with %d renderers\n", (int) renderers.size());
    parser.endBlock(savedIndent);
    return new BlockRenderer(renderers);
}

Renderer *Compiler::compileCommandOrBlock() {
    if (parser.hasArgument()) return compileCommand();
    if (parser.hasBlock()) return compileBlock();
    return compileBlock();
}

Renderer *Compiler::compileSolid() {
    RGBA color = parser.getColor();
    parser.endCommand();
    return new SolidRenderer(color);
}

Renderer *Compiler::compileDots() {
    Vector<RGBA> colors(10);
    int spacing = parser.getInteger();
    colors.append(parser.getColor());
    while (parser.hasArgument()) {
        colors.append(parser.getColor());
    }
    parser.endCommand();
    return new DotsRenderer(spacing, colors);
}

Renderer *Compiler::compileTwinkle() {
    RGBA color = parser.getColor();
    int tpm = parser.hasArgument() ? parser.getInteger() : 0;
    parser.endCommand();
    return new TwinkleRenderer(color, tpm);
}

Renderer *Compiler::compileSegment() {
    int from = parser.getInteger();
    int to = parser.getInteger();
    Renderer *renderer = compileCommandOrBlock();
    return new SegmentRenderer(from, to, renderer);
}

Renderer *Compiler::compileGradient() {
    Vector<RGBA> colors(10);
    colors.append(parser.getColor());
    colors.append(parser.getColor());
    while (parser.hasArgument()) {
        colors.append(parser.getColor());
    }
    parser.endCommand();
    return new GradientRenderer(colors);
}
