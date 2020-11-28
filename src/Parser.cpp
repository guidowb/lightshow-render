#include "Parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

//#define printf(fmt, ...) {}

Parser::Parser(const char *sourceName, const char *pattern) :
    lexer(sourceName, pattern),
    indent(lexer.peek())
{
    rewound = false;
}

const Word &Parser::next() {
    if (rewound) {
        rewound = false;
        return lexer.peek();
    }
    return lexer.next();
}

const Word &Parser::peeknext() {
    rewound = true;
    return lexer.next();
}

const Word &Parser::peek() {
    return lexer.peek();
}

int Parser::hexValue(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

const Word &Parser::getCommand() {
    const Word &command = next();
    if (!command.isString()) {
        reportError(LEXER_ERROR, "Expected command");
    }
    return command;
}

bool Parser::inCommand() {
    const Word &word = peeknext();
    if (!word.isEOL()) return true;
    printf("inCommand - indent %d (was %d)\n", word.length(), indent.length());
    if (word > indent) return true;
    return false;
}

int Parser::getInteger() {
    const Word &word = next();
    if (!word.isString() || !isdigit(word[0])) {
        reportError(LEXER_ERROR, "Expected integer");
        return 0;
    }
    int value = 0;
    for (int i = 0; const char ch = word[i]; i++) {
        if (!isdigit(ch)) {
            reportError(LEXER_ERROR, "Invalid digit in number constant");
            return 0;
        }
        value = value * 10 + (ch - '0');
    }
    return value;
}

RGBA Parser::getColor() {
    const Word &word = next();
    if (!word.isString() || word[0] != '#') {
        reportError(LEXER_ERROR, "Expected color");
        return RGBA_NULL;
    }
    int len = 0;
    for (int i = 1; const char ch = word[i]; i++) {
        len++;
        if (ch >= '0' && ch <= '9') continue;
        if (ch >= 'a' && ch <= 'f') continue;
        if (ch >= 'A' && ch <= 'F') continue;
        reportError(LEXER_ERROR, "Color constant must only contain hex digits");
        return RGBA_NULL;
    }
    if (len < 3 || len > 8 || len == 5 || len == 7) {
        reportError(LEXER_ERROR, "Invalid number of digits in color constant");
        return RGBA_NULL;
    }
    uint8_t r, g, b, a;
    if (len <= 4) {
        r = (hexValue(word[1]) << 4) + hexValue(word[1]);
        g = (hexValue(word[2]) << 4) + hexValue(word[2]);
        b = (hexValue(word[3]) << 4) + hexValue(word[3]);
        if (len == 4) a = (hexValue(word[4]) << 4) + hexValue(word[4]);
        else a = 0x0ff;
    }
    else {
        r = (hexValue(word[1]) << 4) + hexValue(word[2]);
        g = (hexValue(word[3]) << 4) + hexValue(word[4]);
        b = (hexValue(word[5]) << 4) + hexValue(word[6]);
        if (len == 8) a = (hexValue(word[7]) << 4) + hexValue(word[8]);
        else a = 0x0ff;
    }
    return (r << 24) | (g << 16) | (b << 8) | a;
}

bool Parser::isEOC(const Word &word) {
    if (!word.isEOL()) return false;
    if (word == indent) return true;
    if (word < indent) return true;
    if (word > indent) return false;
    reportError(LEXER_ERROR, "Inconsistent whitespace used for indentation");
    return true;
}

void Parser::endCommand() {
    const Word &word = next();
    if (!isEOC(word)) {
        reportError(LEXER_ERROR, "Extra arguments");
        skipCommand();
    }
}

void Parser::skipCommand() {
    while (!isEOC(next())) {
        next();
    }
}

bool Parser::inBlock() {
    const Word &word = peek();
    if (!word.isEOL()) {
        reportError(LEXER_FATAL, "Compiler tested for block here");
        return true;
    }
    if (word.isEOF()) return false;
    printf("inBlock - indent %d (was %d)\n", word.length(), indent.length());
    return word >= indent;
}

const Word Parser::startBlock() {
    const Word savedIndent = indent;
    const Word &word = peek();
    if (!word.isEOL()) {
        reportError(LEXER_FATAL, "Compiler believes a block starts here");
        return savedIndent;
    }
    printf("startBlock - indent %d (was %d)\n", word.length(), indent.length());
    indent = word;
    return savedIndent;
}

void Parser::endBlock(const Word &savedIndent) {
    indent = savedIndent;
}
