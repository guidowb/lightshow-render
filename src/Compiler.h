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

private:
    Renderer *addLayer(Renderer *currentBlock, Renderer *layer);

private:
    Renderer *compileBlock(Renderer *currentBlock = NULL);
    Renderer *compileCapturedBlock(Renderer *currentBlock = NULL);
    Renderer *compileSequence(Renderer *currentBlock);
    Renderer *compileCommand(Renderer *currentBlock);
    Renderer *compileSolid(Renderer *currentBlock);
    Renderer *compileDots(Renderer *currentBlock);
    Renderer *compileTwinkle(Renderer *currentBlock);
    Renderer *compileSegment(Renderer *currentBlock);
    Renderer *compileGradient(Renderer *currentBlock);
    Renderer *compileFade(Renderer *currentBlock);
    Renderer *compileAfter(Renderer *currentBlock);
    Renderer *compileRepeat(Renderer *currentBlock);
};

#endif /* Compiler_h */
