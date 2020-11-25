//
//  Parser.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Parser_h
#define Parser_h

#include "Lexer.h"
#include "LightShow.h"

class Parser {
public:
    Parser(const char *sourceName, const char *pattern);
    const Word &getCommand();
    int getInt();
    RGBA getColor();
    bool endOfBlock();
    bool endOfCommand();
    void endCommand();
    void reportError(int level, const char *message) { lexer.reportError(level, message); }
    int maxErrorLevel() { return lexer.maxErrorLevel(); }

private:
    Lexer lexer;
    const char *word;

private:
    int hexValue(char ch);
};

#endif /* Parser_h */
