//
//  Lexer.cpp
//  LightStrings
//
//  Created by Guido Westenberg on 11/15/20.
//

#include "Lexer.h"

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

Lexer::Lexer(const char *pattern) {
    this->next = pattern;
    this->sawEOC = false;
    this->sawSOS = false;
    this->sawEOS = false;
    this->sawEOF = false;
    this->inCommand = false;
    this->inSequence = 0;
    this->errorLevel = LEXER_OK;
}

const char *Lexer::getWord() {
    wordSize = 0;
    skipWhitespace();
    if (sawEOC) { sawEOC = false; if (inCommand) { inCommand = false; return LEXER_EOC; }}
    if (sawSOS) { sawSOS = false; return LEXER_SOS; }
    if (sawEOS) { sawEOS = false; return LEXER_EOS; }
    if (sawEOF) { sawEOF = false; return LEXER_EOF; }
    while (!isWhitespace(*next)) {
        extendWord(*next++);
    }
    inCommand = true;
    word[wordSize] = '\0';
    return word;
}

void Lexer::extendWord(char ch) {
    if (wordSize < MAXWORD) word[wordSize++] = ch;
    else reportError(LEXER_ERROR, "Word too long");
}

void Lexer::skipWhitespace() {
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

bool Lexer::isWhitespace(char ch) {
    switch (ch) {
    case ' ' : return true;
    case '\t': return true;
    case '\r': return true;
    case '\n': return true;
    case '\0': return true;
    default: return false;
    }
}

void Lexer::reportError(int level, const char *message) {
    // What we want to get out of this is:
    // <inputfile>:<line>:<column>: ERROR: <message>
    // <input line containing failure>
    // <caret in the failing column>
    // The line and column reported should be the one where the last lexed word started
    errorLevel = std::max(errorLevel, level);
    printf("message");
}

int Lexer::maxErrorLevel() {
    return this->errorLevel;
}