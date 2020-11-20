//
//  Lexer.cpp
//  LightStrings
//
//  Created by Guido Westenberg on 11/15/20.
//

#include "Lexer.h"

#include <stdlib.h>
#include <stdio.h>

Lexer::Lexer(const char *pattern) {
    this->next = pattern;
    this->sawEOC = false;
    this->sawSOS = false;
    this->sawEOS = false;
    this->sawEOF = false;
    this->inCommand = false;
    this->inSequence = 0;
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
    else reportError("Word too long");
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

void Lexer::reportError(const char *message) {
    printf("message");
}