//
//  Compiler.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Compiler_h
#define Compiler_h

#include "Render.h"
#include "Parser.h"

class Compiler {
public:
    Compiler(const char *sourceName, const char *pattern);
    Renderer *compile();
    int maxErrorLevel() { return parser.maxErrorLevel(); }

private:
    Parser parser;

private:
    Renderer *compileSolid();
};

#endif /* Compiler_h */
