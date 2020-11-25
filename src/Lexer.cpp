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
    // Error Context
    this->sourceName = sourceName;
    this->next = pattern;
    this->errorLevel = LEXER_OK;
    this->lineNumber = 1;
    this->lineStart = this->next;
    this->word.m_isValid = false;
    skipWhitespace();
}

void Word::reset(const char *start) {
    this->start = start;
    this->len = 0;
    this->m_isEOC = false;
    this->m_isEOF = false;
    this->m_isEOS = false;
    this->m_isSOS = false;
}

void Word::extend() {
    this->len++;
}

bool Word::operator==(const char other[]) const {
    return !strncmp(this->start, other, this->len);
}

const char Word::operator[](const int index) const {
    if (index >= len) return '\0';
    else return start[index];
}

const Word &Lexer::getWord() {

    // Return prior word is it was ungotten
    if (word.m_isValid) {
        if (!word.m_isEOF) word.m_isValid = false;
        return word;
    }

    // Return whitespace word if it is significant
    word.reset(next);
    wordStart = next; // Error Context
    if (skipWhitespace()) {
        return word;
    }

    // Return string word
    word.reset(next);
    wordStart = next; // Error Context
    while (!isWhitespace(*next)) {
        word.extend();
        next++;
    }
    return word;
}

void Lexer::ungetWord() {
    word.m_isValid = true;
}

bool Lexer::skipWhitespace() {
    while (true) {
        switch (*next) {
        case '\0':
            word.m_isEOC = true;
            word.m_isEOS = true;
            word.m_isEOF = true;
            return true;
        case '\n':
            lineStart = next + 1;
            lineNumber++;
            word.m_isEOC = true;
            break;
        case ' ': 
            break;
        case '\t':
            break;
        case '\r':
            break;
        default:
            return word.m_isEOC || word.m_isEOS;
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