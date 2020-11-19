//
//  Parser.cpp
//  LightStrings
//
//  Created by Guido Westenberg on 11/15/20.
//

#include "Parser.h"

#include <stdlib.h>
#include <stdio.h>

Parser *::Parser::create(const char *pattern) {
    return new Parser(pattern);
}

Parser::Parser(const char *pattern) {
    this->next = pattern;
}

const char *Parser::getWord() {
    wordSize = 0;
    skipWhitespace();
    while (!isWhitespace(*next)) {
        extendWord(*next++);
    }
    word[wordSize] = '\0';
    return wordSize > 0 ? word : NULL;
}

void Parser::extendWord(char ch) {
    if (wordSize < MAXWORD) word[wordSize++] = ch;
    else reportError("Word too long");
}

void Parser::skipWhitespace() {
    while (true) {
        if (*next == '\0') break;
        if (!isWhitespace(*next)) break;
        next++;
    }
}

bool Parser::isWhitespace(char ch) {
    switch (ch) {
    case ' ' : return true;
    case '\t': return true;
    case '\r': return true;
    case '\n': return true;
    case '\0': return true;
    default: return false;
    }
}

void Parser::reportError(const char *message) {
    printf("message");
}