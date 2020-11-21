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

Lexer::Lexer(const char *sourceName, const char *pattern) {
    this->sourceName = sourceName;
    this->next = pattern;
    this->sawEOC = false;
    this->sawSOS = false;
    this->sawEOS = false;
    this->sawEOF = false;
    this->inCommand = false;
    this->inSequence = 0;
    this->errorLevel = LEXER_OK;
    this->lineNumber = 1;
    this->lineStart = this->next;
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
    wordStart = next;
    while (true) {
        switch (*next) {
        case '\0':
            sawEOC = true; sawEOF = true; return;
        case '\n':
            lineStart = next + 1;
            lineNumber++;
            sawEOC = true; break;
        case ' ': 
            break;
        case '\t':
            break;
        default:
            wordStart = next;
            return;
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

const char *levelNames[] = { "OK", "INFO", "WARNING", "ERROR", "FATAL" };

void Lexer::reportError(int level, const char *message) {
    const char *levelName = levelNames[level];
    errorLevel = std::max(errorLevel, level);
    int columnNumber = wordStart - lineStart + 1;
    fprintf(stderr, "%s:%d:%d: %s: %s\n", sourceName, lineNumber, columnNumber, levelName, message);
    const char *c = lineStart;
    while (*c != '\n' && *c != '\0') { fprintf(stderr, "%c", *c++); }
    fprintf(stderr, "\n" );
    for (int count = 1; count < columnNumber; count++) { fprintf(stderr, " "); }
    fprintf(stderr, "^\n");
}

int Lexer::maxErrorLevel() {
    return this->errorLevel;
}