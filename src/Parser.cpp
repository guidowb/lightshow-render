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

void Parser::endCommand() {
    const char *next = getWord();
    if (strcmp(next, LEXER_EOC)) {
        lexer.reportError(LEXER_ERROR, "extra arguments");
    }
    while (strcmp(next, LEXER_EOC)) next = getWord();
}