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
    this->sawEOC = false;
    this->sawSOS = false;
    this->sawEOS = false;
    this->sawEOF = false;
    this->inCommand = false;
    this->inSequence = 0;
}


const char *Parser::getWord() {
    wordSize = 0;
    skipWhitespace();
    if (sawEOC) { sawEOC = false; if (inCommand) { inCommand = false; return PARSER_EOC; }}
    if (sawSOS) { sawSOS = false; return PARSER_SOS; }
    if (sawEOS) { sawEOS = false; return PARSER_EOS; }
    if (sawEOF) { sawEOF = false; return PARSER_EOF; }
    while (!isWhitespace(*next)) {
        extendWord(*next++);
    }
    inCommand = true;
    word[wordSize] = '\0';
    return word;
}

void Parser::extendWord(char ch) {
    if (wordSize < MAXWORD) word[wordSize++] = ch;
    else reportError("Word too long");
}

void Parser::skipWhitespace() {
    while (true) {
        switch (*next) {
        case '\0': sawEOC = true; sawEOF = true; return;
        case '\n': sawEOC = true; break;
        case ' ':  break;
        case '\t': break;
        default: return;
        }
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