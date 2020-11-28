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
    Word(const char *start);

public:
    bool isEOL() const { return m_isEOL; }
    bool isEOF() const { return m_isEOF; }
    bool isString() const { return len > 0; }
    bool operator==(const char other[]) const;
    bool operator==(const Word &other) const;
    bool operator<(const Word &other) const;
    bool operator>(const Word &other) const;
    bool operator>=(const Word &other) const;
    const char operator[](const int index) const;
    int length() const { return len; }

private:
    const char *start;
    int len;
    bool m_isEOL;
    bool m_isEOF;

private:
    friend class Lexer;
    void reset(const char *start);
    void extend();
};

class Lexer {
public:
    Lexer(const char *sourceName, const char *pattern);
    const Word &peek() { return word; }
    const Word &next();
    void reportError(int level, const char *message);
    int maxErrorLevel();

private:
    Word word;

private: // Error Context
    const char *sourceName;
    const char *position;
    int errorLevel;
    int lineNumber;
    const char *lineStart, *wordStart;

private:
    bool skipWhitespace();
    bool isWhitespace(char ch);
};

#endif /* Lexer_h */
