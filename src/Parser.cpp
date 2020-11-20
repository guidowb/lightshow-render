#include "Parser.h"

#include <stdlib.h>

Parser::Parser(const char *pattern) : lexer(pattern) {
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