#include "Compiler.h"

#include <stdlib.h>
#include <string.h>

#define printf(fmt, ...) {}

Compiler::Compiler(const char *sourceName, const char *pattern) : parser(sourceName, pattern) {}

Renderer *Compiler::addLayer(Renderer *currentBlock, Renderer *layer) {
    if (currentBlock) return new LayeredRenderer(currentBlock, layer);
    else return layer;
}

Renderer *Compiler::compile() {
    return compileBlock();
}

Renderer *Compiler::compileCommand(Renderer *currentBlock) {

    const Word &command = parser.getCommand();
    if (command == "define")   return compileDefine(currentBlock);
    if (command == "solid")    return compileSolid(currentBlock);
    if (command == "dots")     return compileDots(currentBlock);
    if (command == "twinkle")  return compileTwinkle(currentBlock);
    if (command == "segment")  return compileSegment(currentBlock);
    if (command == "gradient") return compileGradient(currentBlock);
    if (command == "fade")     return compileFade(currentBlock);
    if (command == "after")    return compileAfter(currentBlock);
    if (command == "repeat")   return compileRepeat(currentBlock);
    if (command == "time")     return compileTime(currentBlock);
    if (command == "date")     return compileDate(currentBlock);

    parser.reportError(LEXER_ERROR, "Unknown command");
    parser.skipCommand();
    return new NullRenderer();
}

Renderer *Compiler::compileDefine(Renderer *currentBlock) {
    const Word &name = parser.getName();
    return currentBlock;
}

// When compiling a block, we don't know if it represents a simple
// sequence of layers, or if it includes scenes. Each command is
// therefore passed a reference to the block we've seen so far.
// Each command compiler is responsible for either continuing to add
// layers to that block, and returning it once its completed, OR,
// for capturing that block as a scene, and returning the scene
// renderer for it and all subsequent scenes.

Renderer *Compiler::compileBlock(Renderer *currentBlock) {
    const Word savedIndent = parser.startBlock();
    while (parser.inBlock()) {
        currentBlock = compileCommand(currentBlock);
    }
    parser.endBlock(savedIndent);
    return currentBlock;
}

Renderer *Compiler::compileCapturedBlock(Renderer *currentBlock) {
    if (parser.hasArgument()) return compileCommand(currentBlock);
    if (parser.hasBlock()) return compileBlock(currentBlock);
    // This is NOT a dup, because hasBlock has side effects :(
    return compileBlock(currentBlock);
}

Renderer *Compiler::compileSolid(Renderer *currentBlock) {
    RGBA color = parser.getColor();
    parser.endCommand();
    return addLayer(currentBlock, new SolidRenderer(color));
}

Renderer *Compiler::compileDots(Renderer *currentBlock) {
    Vector<RGBA> colors(10);
    int spacing = parser.getInteger();
    colors.append(parser.getColor());
    while (parser.hasArgument()) {
        colors.append(parser.getColor());
    }
    parser.endCommand();
    return addLayer(currentBlock, new DotsRenderer(spacing, colors));
}

Renderer *Compiler::compileTwinkle(Renderer *currentBlock) {
    RGBA color = parser.getColor();
    int tpm = parser.hasArgument() ? parser.getInteger() : 0;
    parser.endCommand();
    return addLayer(currentBlock, new TwinkleRenderer(color, tpm));
}

Renderer *Compiler::compileSegment(Renderer *currentBlock) {
    int from = parser.getInteger();
    int to = parser.getInteger();
    Renderer *renderer = compileCapturedBlock();
    return addLayer(currentBlock, new SegmentRenderer(from, to, renderer));
}

Renderer *Compiler::compileGradient(Renderer *currentBlock) {
    Vector<RGBA> colors(10);
    colors.append(parser.getColor());
    colors.append(parser.getColor());
    while (parser.hasArgument()) {
        colors.append(parser.getColor());
    }
    parser.endCommand();
    return addLayer(currentBlock, new GradientRenderer(colors));
}

Renderer *Compiler::compileFade(Renderer *before) {
    uint32_t duration = parser.getDuration();
    Renderer *after = compileCapturedBlock();
    return new FadeRenderer(before, after, duration);
}

Renderer *Compiler::compileAfter(Renderer *before) {
    uint32_t duration = parser.getDuration();
    Renderer *after = compileCapturedBlock(before);
    return new AfterRenderer(before, after, duration);
}

Renderer *Compiler::compileRepeat(Renderer *currentBlock) {
    Renderer *block = compileCapturedBlock();
    return addLayer(currentBlock, new RepeatRenderer(block));
}

Renderer *Compiler::compileTime(Renderer *currentBlock) {
    uint32_t from = parser.getTime();
    uint32_t to = parser.getTime();
    Renderer *block = compileCapturedBlock();
    return addLayer(currentBlock, new TimeRenderer(block, from, to));
}

Renderer *Compiler::compileDate(Renderer *currentBlock) {
    uint16_t from = parser.getDate();
    uint16_t to = parser.getDate();
    Renderer *block = compileCapturedBlock();
    return addLayer(currentBlock, new DateRenderer(block, from, to));
}
