//
//  Parser.cpp
//  LightStrings
//
//  Created by Guido Westenberg on 11/15/20.
//

#include "Parser.h"

#include <stdlib.h>
#include <stdio.h>

Parser *::Parser::create(const char *pattern) {
    return new Parser(pattern);;
}

Parser::Parser(const char *pattern) {
    this->next = pattern;
}

const char *Parser::getWord() {
    return NULL;
}