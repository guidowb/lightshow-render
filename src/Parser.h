//
//  Parser.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Parser_h
#define Parser_h

#define MAXWORD 32

class Parser {
private:
    const char *next;
    char word[MAXWORD+1];

public:
    Parser(const char *pattern);
    const char *getWord();

public:
    static Parser *create(const char *pattern);
};

#endif /* Parser_h */
