#include "Parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

//#define printf(fmt, ...) {}

class WordParser {
public:
    WordParser(const Word &word) : word(word), index(0) {};

    // These tests maintain position
    bool is(char ch) { return word[index] == ch; }
    bool isDigit() { return word[index] && isdigit(word[index]); }
    bool isEnded() { return word[index] == '\0'; }

    // These getters advance position
    bool skip(char ch) { if (word[index] == ch) { index++; return true; } else return false; }
    int getDigit() { return word[index++] - '0'; }

private:
    int index;
    const Word &word;
};

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
    if (rewound) return lexer.peek();
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
    if (command.isEOL()) {
        reportError(LEXER_ERROR, "Expected command");
    }
    return command;
}

bool Parser::hasArgument() {
    const Word &word = peeknext();
    if (!word.isEOL()) return true;
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

uint32_t Parser::getDuration() {
    const Word &word = next();
    if (!word.isString() || !isdigit(word[0])) {
        reportError(LEXER_ERROR, "Expected duration");
        return 0;
    }
    int index = 0;
    int value = 0;
    char ch;
    while ((ch = word[index]) && isdigit(ch)) {
        value = value * 10 + (ch - '0');
        index++;
    }
    if (ch && !word[index+1]) switch (ch) {
    case 's': return value * 1000;
    case 'm': return value * 1000 * 60;
    case 'h': return value * 1000 * 60 * 60;
    default: break;
    }
    reportError(LEXER_ERROR, "Duration must end with a valid unit");
    return 0;
}

uint32_t Parser::getTime() {

    int hour = 0;
    int minute = 0;
    int second = 0;

    WordParser word(next());

    if (word.isDigit()) hour = word.getDigit();
    else goto expectedTime;
    if (word.isDigit()) hour = hour * 10 + word.getDigit();
    if (!word.skip(':')) goto expectedTime;

    if (word.isDigit()) minute = word.getDigit();
    else goto expectedTime;
    if (word.isDigit()) minute = minute * 10 + word.getDigit();
    else goto expectedTime;

    if (word.skip(':')) {
        if (word.isDigit()) second = word.getDigit();
        else goto expectedTime;
        if (word.isDigit()) second = second * 10 + word.getDigit();
        else goto expectedTime;
    }

    if (!word.isEnded()) goto expectedTime;

    return hour * 3600 + minute * 60 + second;

expectedTime:
    reportError(LEXER_ERROR, "Expected time of the form HH:MM[:SS]");
    return 0;
}

uint16_t Parser::getDate() {

    int month = 0;
    int day = 0;

    WordParser word(next());

    if (word.isDigit()) month = word.getDigit();
    else goto expectedDate;
    if (word.isDigit()) month = month * 10 + word.getDigit();
    if (!word.skip('/')) goto expectedDate;
    if (month < 1 || month > 12) goto expectedDate;

    if (word.isDigit()) day = word.getDigit();
    else goto expectedDate;
    if (word.isDigit()) day = day * 10 + word.getDigit();
    else goto expectedDate;
    if (day < 1 || day > 31) goto expectedDate;

    if (!word.isEnded()) goto expectedDate;

    return (month << 8) + day;

expectedDate:
    reportError(LEXER_ERROR, "Expected date of the form MM/DD");
    return 0;
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
    uint32_t r, g, b, a;
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
    return RGBA(r, g, b, a);
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
    if (!word.isEOL()) {
        reportError(LEXER_ERROR, "Extra arguments");
        skipCommand();
    }
    else if (word > indent) {
        reportError(LEXER_ERROR, "Unexpected block");
        skipCommand();
    }
}

void Parser::skipCommand() {
    while (!isEOC(next())) {}
}

bool Parser::hasBlock() {
    const Word &word = peeknext();
    if (!word.isEOL()) return false;
    if (word < indent) return false;
    next();
    return true;
}

bool Parser::inBlock() {
    const Word &word = peek();
    if (!word.isEOL()) {
        reportError(LEXER_FATAL, "Compiler tested for block here");
        return true;
    }
    if (word.isEOF()) return false;
    return word >= indent;
}

const Word Parser::startBlock() {
    const Word savedIndent = indent;
    const Word &word = peek();
    if (!word.isEOL()) {
        reportError(LEXER_FATAL, "Compiler believes a block starts here");
        return savedIndent;
    }
    indent = word;
    return savedIndent;
}

void Parser::endBlock(const Word &savedIndent) {
    indent = savedIndent;
}
