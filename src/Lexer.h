//
//  Lexer.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Lexer_h
#define Lexer_h

#define MAXWORD 32

#define LEXER_EOC "EOC" // End of Command
#define LEXER_SOS "SOS" // Start of Sequence
#define LEXER_EOS "EOS" // End of Sequence
#define LEXER_EOF "EOF" // End of File

class Lexer {
public:
    Lexer(const char *pattern);
    const char *getWord();

private:
    const char *next;
    char word[MAXWORD+1];
    int wordSize;
    bool sawEOC, sawSOS, sawEOS, sawEOF;
    bool inCommand;
    int inSequence;

private:
    void skipWhitespace();
    bool isWhitespace(char ch);
    void extendWord(char ch);
    void reportError(const char *message);
};

#endif /* Lexer_h */
