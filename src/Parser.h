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
public:
    static Parser *create(const char *pattern);
    const char *getWord();

private:
    const char *next;
    char word[MAXWORD+1];
    int wordSize;

private:
    Parser(const char *pattern);
    void skipWhitespace();
    bool isWhitespace(char ch);
    void extendWord(char ch);
    void reportError(const char *message);
};

#endif /* Parser_h */
