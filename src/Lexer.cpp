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

#define printf(fmt, ...) {}

Lexer::Lexer(const char *sourceName, const char *pattern) : word(pattern) {
    // Error Context
    this->sourceName = sourceName;
    this->position = pattern;
    this->errorLevel = LEXER_OK;
    this->lineNumber = 1;
    this->lineStart = this->position;
    skipWhitespace();
}

Word::Word(const char *start) {
    this->start = start;
    this->len = 0;
    this->m_isEOL = true;
    this->m_isEOF = false;
} 

void Word::reset(const char *start) {
    this->start = start;
    this->len = 0;
    this->m_isEOL = false;
    this->m_isEOF = false;
}

void Word::extend() {
    this->len++;
}

int Word::compare(const char other[]) const {
    for (int c = 0; true; c++) {
        if (c == len && other[c] == '\0') return 0;
        if (c == len) return -1;
        if (other[c] == '\0') return 1;
        if (start[c] == other[c]) continue;
        if (start[c] < other[c]) return -2;
        else return 2;
    }
}

int Word::compare(const Word &other) const {
    for (int c = 0; true; c++) {
        if (c == len && c == other.len) return 0;
        if (c == len) return -1;
        if (c == other.len) return 1;
        if (start[c] == other[c]) continue;
        if (start[c] < other[c]) return -2;
        else return 2;
    }
}

bool Word::operator==(const char other[]) const {
    return compare(other) == 0;
}

bool Word::operator==(const Word &other) const {
    return compare(other) == 0;
}

bool Word::operator>(const Word &other) const {
    return compare(other) == 1;
}

bool Word::operator<(const Word &other) const {
    return compare(other) == -1;
}

bool Word::operator>=(const Word &other) const {
    int result = compare(other);
    return result == 0 || result == 1;
}

bool Word::operator<=(const Word &other) const {
    int result = compare(other);
    return result == 0 || result == -1;
}

const char Word::operator[](const int index) const {
    if (index >= len) return '\0';
    else return start[index];
}

const Word &Lexer::next() {

    // EOF word is persistent
    if (word.isEOF()) {
        printf("\nlexer returns EOF\n");
        return word;
    }

    // Return whitespace word if it is significant
    word.reset(position);
    wordStart = position; // Error Context
    if (skipWhitespace()) {
        printf("\nlexer returns EOL\n");
        return word;
    }

    // Return string word
    word.reset(position);
    wordStart = position; // Error Context
    while (!isWhitespace(*position)) {
        word.extend();
        position++;
    }
    printf("\nlexer returns word \"%.*s\"\n", word.len, word.start);
    return word;
}

bool Lexer::skipWhitespace() {
    while (true) {
        switch (*position) {
        case '\0':
            word.reset(position);
            word.m_isEOL = true;
            word.m_isEOF = true;
            return true;
        case '\n':
            word.reset(position + 1);
            lineStart = position + 1; // Error Context
            lineNumber++;
            word.m_isEOL = true;
            break;
        case ' ': case '\t':
            word.extend();
            break;
        case '\r':
            break;
        default:
            return word.m_isEOL;
        }
        position++;
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