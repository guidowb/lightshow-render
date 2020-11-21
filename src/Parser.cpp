#include "Parser.h"

#include <stdlib.h>
#include <string.h>

Parser::Parser(const char *sourceName, const char *pattern) : lexer(sourceName, pattern) {
    word = NULL;
}

const char *Parser::getWord() {
    const char *word = this->word;
    this->word = NULL;
    if (word != NULL) return word;
    return lexer.getWord();
}

void Parser::pushBack(const char *word) {
    this->word = word;
}

const char *Parser::getCommand() {
    const char *command = getWord();
    return command;
}

int Parser::hexValue(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

RGBA Parser::getColor() {
    const char *word = getWord();
    if (*word++ != '#') {
        reportError(LEXER_ERROR, "color constant must start with '#'");
        return RGBA_NULL;
    }
    int len;
    const char *digit = word;
    for (len = 0; len < 9; len++, digit++) {
        if (*digit == '\0') break;
        if (*digit >= '0' && *digit <= '9') continue;
        if (*digit >= 'a' && *digit <= 'f') continue;
        if (*digit >= 'A' && *digit <= 'F') continue;
        reportError(LEXER_ERROR, "color constant must only contain hex digits");
        return RGBA_NULL;
    }
    if (len < 3 || len > 8 || len == 5 || len == 7) {
        reportError(LEXER_ERROR, "invalid number of digits in color constant");
        return RGBA_NULL;
    }
    uint8_t r, g, b, a;
    if (len <= 4) {
        r = (hexValue(word[0]) << 4) + hexValue(word[0]);
        g = (hexValue(word[1]) << 4) + hexValue(word[1]);
        b = (hexValue(word[2]) << 4) + hexValue(word[2]);
        if (len == 4) a = (hexValue(word[3]) << 4) + hexValue(word[3]);
        else a = 0x0ff;
    }
    else {
        r = (hexValue(word[0]) << 4) + hexValue(word[1]);
        g = (hexValue(word[2]) << 4) + hexValue(word[3]);
        b = (hexValue(word[4]) << 4) + hexValue(word[5]);
        if (len == 8) a = (hexValue(word[6]) << 4) + hexValue(word[7]);
        else a = 0x0ff;
    }
    return (r << 24) | (g << 16) | (b << 8) | a;
}

void Parser::endCommand() {
    const char *next = getWord();
    if (strcmp(next, LEXER_EOC)) {
        reportError(LEXER_ERROR, "extra arguments");
    }
    while (strcmp(next, LEXER_EOC)) next = getWord();
}