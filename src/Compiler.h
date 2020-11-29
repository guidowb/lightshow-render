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
    Renderer *compileBlock();
    Renderer *compileCommand();
    Renderer *compileCommandOrBlock();
    Renderer *compileSolid();
    Renderer *compileDots();
    Renderer *compileTwinkle();
    Renderer *compileSegment();
    Renderer *compileGradient();
};

#endif /* Compiler_h */
