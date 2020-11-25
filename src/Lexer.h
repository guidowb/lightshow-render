//
//  Lexer.h
//  LightShow
//
//  Created by Guido Westenberg on 11/15/20.
//

#ifndef Lexer_h
#define Lexer_h

#define LEXER_OK      0
#define LEXER_INFO    1
#define LEXER_WARNING 2
#define LEXER_ERROR   3
#define LEXER_FATAL   4

class Word {
public:
    bool isEOC() const { return m_isEOC; }
    bool isEOF() const { return m_isEOF; }
    bool isEOS() const { return m_isEOS; }
    bool isSOS() const { return m_isSOS; }
    bool isString() const { return len > 0; }
    bool operator==(const char other[]) const;
    const char operator[](const int index) const;

private:
    const char *start;
    int len;
    bool m_isEOC;
    bool m_isEOF;
    bool m_isEOS;
    bool m_isSOS;
    bool m_isValid;

private:
    friend class Lexer;
    void reset(const char *start);
    void extend();
};

class Lexer {
public:
    Lexer(const char *sourceName, const char *pattern);
    const Word &getWord();
    void ungetWord();
    void reportError(int level, const char *message);
    int maxErrorLevel();

private:
    Word word;

private: // Error Context
    const char *sourceName;
    const char *next;
    int errorLevel;
    int lineNumber;
    const char *lineStart, *wordStart;

private:
    bool skipWhitespace();
    bool isWhitespace(char ch);
};

#endif /* Lexer_h */
