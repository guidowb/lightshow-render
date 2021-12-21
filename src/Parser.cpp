#include "Parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#define printf(fmt, ...) {}

class WordParser {
public:
    WordParser(const Word &word) : word(word), index(0) {};

    // These tests maintain position
    bool is(char ch) const { return word[index] == ch; }
    bool isDigit() const { return word[index] && isdigit(word[index]); }
    bool isAlpha() const { return word[index] && isalpha(word[index]); }
    bool isAlnum() const { return word[index] && isalnum(word[index]); }
    bool isEnded() const { return word[index] == '\0'; }
    bool isHexDigit() const;

    int length() const { return word.length(); }

    // These getters advance position
    void skip() { index++; }
    bool skip(char ch) { if (word[index] == ch) { index++; return true; } else return false; }
    int getDigit();

private:
    int index;
    const Word &word;
};

bool WordParser::isHexDigit() const {
    const char ch = word[index];
    if (isdigit(ch)) return true;
    if (ch >= 'a' && ch <= 'f') return true;
    if (ch >= 'A' && ch <= 'F') return true;
    return false;
}

int WordParser::getDigit() {
    char ch = word[index++];
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return 0;
}

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

const Word &Parser::getCommand() {
    if (!isName()) reportError(LEXER_ERROR, "Expected command");
    return next();
}

bool Parser::isName() {
    WordParser name(peeknext());
    if (!name.isAlpha()) return false;
    name.skip();
    while (name.isAlnum()) name.skip();
    if (!name.isEnded()) return false;
    return true;
}

const Word &Parser::getName() {
    if (!isName()) reportError(LEXER_ERROR, "Expected name");
    return next();
}

bool Parser::hasArgument() {
    const Word &word = peeknext();
    if (!word.isEOL()) return true;
    return false;
}

bool Parser::isInteger() {
    WordParser word(peeknext());
    if (word.is('-')) word.skip();
    while (word.isDigit()) word.skip();
    if (!word.isEnded()) return false;
    return true;
}

int Parser::getInteger() {
    WordParser word(next());
    bool negate = false;
    int value = 0;
    if (word.is('-')) { negate = true; word.skip(); }
    while (word.isDigit()) {
        value = value * 10 + word.getDigit();
    }
    if (!word.isEnded()) {
        reportError(LEXER_ERROR, "Expected integer");
        return 0;
    };
    if (negate) value = -value;
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

bool Parser::isColor() {
    WordParser word(peeknext());
    if (!word.is('#')) return false;
    word.skip();
    int len = 0;
    while (word.isHexDigit()) { len++; word.skip(); }
    if (!word.isEnded()) return false;
    if (len < 3 || len > 8 || len == 5 || len == 7) return false;
    return true;
}

RGBA Parser::getColor() {
    if (!isColor()) {
        reportError(LEXER_ERROR, "Expected color");
        return RGBA_NULL;
    }
    WordParser word(next());
    uint8_t r, g, b, a;
    word.skip();
    if (word.length() <= 5) {
        r = word.getDigit(); r = r * 16 + r;
        g = word.getDigit(); g = g * 16 + g;
        b = word.getDigit(); b = b * 16 + b;
        if (word.isHexDigit()) { a = word.getDigit(); a = a * 16 + a; }
        else a = 0x0ff;
    }
    else {
        r = word.getDigit() * 16 + word.getDigit();
        g = word.getDigit() * 16 + word.getDigit();
        b = word.getDigit() * 16 + word.getDigit();
        if (word.isHexDigit()) { a = word.getDigit() * 16 + word.getDigit(); }
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
