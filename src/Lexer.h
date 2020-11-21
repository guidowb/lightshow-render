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

#define LEXER_OK      0
#define LEXER_INFO    1
#define LEXER_WARNING 2
#define LEXER_ERROR   3
#define LEXER_FATAL   4

class Lexer {
public:
    Lexer(const char *pattern);
    const char *getWord();
    void reportError(int level, const char *message);
    int maxErrorLevel();

private:
    const char *next;
    char word[MAXWORD+1];
    int wordSize;
    bool sawEOC, sawSOS, sawEOS, sawEOF;
    bool inCommand;
    int inSequence;
    int errorLevel;

private:
    void skipWhitespace();
    bool isWhitespace(char ch);
    void extendWord(char ch);
};

#endif /* Lexer_h */
