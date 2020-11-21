//
//  Parser.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Parser_h
#define Parser_h

#include "Lexer.h"

class Parser {
public:
    Parser(const char *sourceName, const char *pattern);
    const char *getCommand();
    void endCommand();
    int maxErrorLevel() { return lexer.maxErrorLevel(); }

private:
    Lexer lexer;
    const char *word;

private:
    const char *getWord();
    void pushBack(const char *word);
};

#endif /* Parser_h */