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

#include <map>

class Parser {
public:
    Parser(const char *sourceName, const char *pattern);

public:
    const Word &getCommand();
    const Word &getName();

    bool isName();
    bool isColor();
    bool isInteger();

    int  getInteger();
    RGBA getColor();
    uint32_t getDuration();
    uint32_t getTime();
    uint16_t getDate();

    bool hasArgument();
    void endCommand();
    void skipCommand();
    const Word startBlock();
    bool hasBlock();
    bool inBlock();
    void endBlock(const Word &indent);

public: // Error Context
    void reportError(int level, const char *message) { lexer.reportError(level, message); }
    int maxErrorLevel() { return lexer.maxErrorLevel(); }

private:
    Lexer lexer;
    Word indent;
    bool rewound;

private:
    const Word &next();
    const Word &peek();
    const Word &peeknext();
    int hexValue(char ch);
    bool isEOC(const Word &word);
};

#endif /* Parser_h */
