#include "Compiler.h"

#include <stdlib.h>
#include <string.h>

#define printf(fmt, ...) {}

Compiler::SavedContext::SavedContext(Compiler *compiler) {
    this->compiler = compiler;
    this->firstScene = compiler->firstScene;
    this->currentScene = compiler->currentScene;
    compiler->firstScene = NULL;
    compiler->currentScene = NULL;
}

Compiler::SavedContext::~SavedContext() {
    compiler->firstScene = firstScene;
    compiler->currentScene = currentScene;
}

Compiler::Compiler(const char *sourceName, const char *pattern) : parser(sourceName, pattern) {
    this->firstScene = NULL;
    this->currentScene = NULL;
}

Renderer *Compiler::addLayer(Renderer *layer) {
    if (currentScene) currentScene = new LayeredRenderer(currentScene, layer);
    else currentScene = layer;
    return currentScene;
}

Renderer *Compiler::newScene(Renderer *scene) {
    currentScene = scene;
    return currentScene;
}

Renderer *Compiler::cutScene() {
    if (!firstScene) firstScene = currentScene;
    return currentScene;
}

Renderer *Compiler::compile() {
    return compileBlock();
}

Renderer *Compiler::compileCommand() {

    const Word &command = parser.getCommand();
    if (command == "define")   return compileDefine();
    if (command == "solid")    return compileSolid();
    if (command == "dots")     return compileDots();
    if (command == "twinkle")  return compileTwinkle();
    if (command == "segment")  return compileSegment();
    if (command == "gradient") return compileGradient();
    if (command == "fade")     return compileFade();
    if (command == "after")    return compileAfter();
    if (command == "repeat")   return compileRepeat();
    if (command == "time")     return compileTime();
    if (command == "date")     return compileDate();

    parser.reportError(LEXER_ERROR, "Unknown command");
    parser.skipCommand();
    return newScene(new NullRenderer());
}

Renderer *Compiler::compileDefine() {
    const Word &name = parser.getName();
    
    return currentScene;
}

Renderer *Compiler::compileBlock() {
    const Word savedIndent = parser.startBlock();
    while (parser.inBlock()) compileCommand();
    parser.endBlock(savedIndent);
    return currentScene;
}

Renderer *Compiler::compileCapturedBlock(Renderer *initial) {
    const SavedContext savedContext(this);
    if (initial) addLayer(initial);
    if (parser.hasArgument()) return compileCommand();
    if (parser.hasBlock()) return compileBlock();
    if (parser.inBlock()) return compileBlock();
    if (firstScene) return new DuplicateRenderer(firstScene);
    parser.reportError(LEXER_ERROR, "Expected block");
    return newScene(new NullRenderer());
}

Renderer *Compiler::compileSolid() {
    RGBA color = parser.getColor();
    parser.endCommand();
    return addLayer(new SolidRenderer(color));
}

Renderer *Compiler::compileDots() {
    Vector<RGBA> colors(10);
    int spacing = parser.getInteger();
    colors.append(parser.getColor());
    while (parser.hasArgument()) {
        colors.append(parser.getColor());
    }
    parser.endCommand();
    return addLayer(new DotsRenderer(spacing, colors));
}

Renderer *Compiler::compileTwinkle() {
    RGBA color = parser.getColor();
    int tpm = parser.hasArgument() ? parser.getInteger() : 0;
    parser.endCommand();
    return addLayer(new TwinkleRenderer(color, tpm));
}

Renderer *Compiler::compileSegment() {
    int from = parser.getInteger();
    int to = parser.getInteger();
    Renderer *renderer = compileCapturedBlock();
    return addLayer(new SegmentRenderer(from, to, renderer));
}

Renderer *Compiler::compileGradient() {
    Vector<RGBA> colors(10);
    colors.append(parser.getColor());
    colors.append(parser.getColor());
    while (parser.hasArgument()) {
        colors.append(parser.getColor());
    }
    parser.endCommand();
    return addLayer(new GradientRenderer(colors));
}

Renderer *Compiler::compileFade() {
    uint32_t duration = parser.getDuration();
    Renderer *before = cutScene();
    Renderer *after = compileCapturedBlock();
    return newScene(new FadeRenderer(before, after, duration));
}

Renderer *Compiler::compileAfter() {
    uint32_t duration = parser.getDuration();
    Renderer *before = currentScene;
    Renderer *after = compileCapturedBlock(before);
    return newScene(new AfterRenderer(before, after, duration));
}

Renderer *Compiler::compileRepeat() {
    Renderer *block = compileCapturedBlock();
    return addLayer(new RepeatRenderer(block));
}

Renderer *Compiler::compileTime() {
    uint32_t from = parser.getTime();
    uint32_t to = parser.getTime();
    Renderer *block = compileCapturedBlock();
    return addLayer(new TimeRenderer(block, from, to));
}

Renderer *Compiler::compileDate() {
    uint16_t from = parser.getDate();
    uint16_t to = parser.getDate();
    Renderer *block = compileCapturedBlock();
    return addLayer(new DateRenderer(block, from, to));
}
