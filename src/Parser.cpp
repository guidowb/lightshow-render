#include "Parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

Parser::Parser(const char *sourceName, const char *pattern) : lexer(sourceName, pattern) {
    word = NULL;
}

const Word &Parser::getCommand() {
    const Word &command = lexer.getWord();
    if (!command.isString()) {
        lexer.ungetWord();
        reportError(LEXER_ERROR, "expected command");
    }
    return command;
}

int Parser::hexValue(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

int Parser::getInt() {
    const Word &word = lexer.getWord();
    if (!word.isString() || !isdigit(word[0])) {
        lexer.ungetWord();
        reportError(LEXER_ERROR, "expected integer");
        return 0;
    }
    int value = 0;
    for (int i = 0; const char ch = word[i]; i++) {
        if (!isdigit(ch)) {
            reportError(LEXER_ERROR, "invalid digit in number constant");
            return 0;
        }
        value = value * 10 + (ch - '0');
    }
    return value;
}

RGBA Parser::getColor() {
    const Word &word = lexer.getWord();
    if (!word.isString() || word[0] != '#') {
        lexer.ungetWord();
        reportError(LEXER_ERROR, "expected color");
        return RGBA_NULL;
    }
    int len = 0;
    for (int i = 1; const char ch = word[i]; i++) {
        len++;
        if (ch >= '0' && ch <= '9') continue;
        if (ch >= 'a' && ch <= 'f') continue;
        if (ch >= 'A' && ch <= 'F') continue;
        reportError(LEXER_ERROR, "color constant must only contain hex digits");
        return RGBA_NULL;
    }
    if (len < 3 || len > 8 || len == 5 || len == 7) {
        reportError(LEXER_ERROR, "invalid number of digits in color constant");
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

bool Parser::endOfBlock() {
    const Word &word = lexer.getWord();
    lexer.ungetWord();
    return word.isEOS();
}

bool Parser::endOfCommand() {
    const Word &word = lexer.getWord();
    lexer.ungetWord();
    return word.isEOC();
}

void Parser::endCommand() {
    const Word &word = lexer.getWord();
    if (!word.isEOC()) {
        reportError(LEXER_ERROR, "extra arguments");
    }
    while (!lexer.getWord().isEOC()) {}
}