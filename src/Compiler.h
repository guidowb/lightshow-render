//
//  Compiler.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Compiler_h
#define Compiler_h

#include "LightShow.h"
#include "Parser.h"
#include "Renderers.h"

class Compiler {
public:
    Compiler(const char *sourceName, const char *pattern);
    Renderer *compile();
    int maxErrorLevel() { return parser.maxErrorLevel(); }

private:
    Parser parser;
    Renderer *firstScene;
    Renderer *currentScene;

private:
    class SavedContext {
    public:
        SavedContext(Compiler *compiler);
        ~SavedContext();

    private:
        Compiler *compiler;
        Renderer *firstScene;
        Renderer *currentScene;
    };

private:
    Renderer *addLayer(Renderer *layer);
    Renderer *newScene(Renderer *scene);

private:
    Renderer *compileDefine();
    Renderer *compileBlock();
    Renderer *compileCapturedBlock(Renderer *initial = NULL);
    Renderer *compileSequence();
    Renderer *compileCommand();
    Renderer *compileSolid();
    Renderer *compileDots();
    Renderer *compileTwinkle();
    Renderer *compileSegment();
    Renderer *compileGradient();
    Renderer *compileFade();
    Renderer *compileAfter();
    Renderer *compileRepeat();
    Renderer *compileTime();
    Renderer *compileDate();
};

#endif /* Compiler_h */
