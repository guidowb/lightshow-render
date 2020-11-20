#include "../src/Lexer.h"

#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

class LexerTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( LexerTests );
    CPPUNIT_TEST( testSingleLine );
    CPPUNIT_TEST( testMultipleLines );
    CPPUNIT_TEST( testWhitespace );
    CPPUNIT_TEST( testEOFisPermanent );
    CPPUNIT_TEST_SUITE_END();

    void testSingleLine() {
        Lexer lexer("solid #000");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOC, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOF, lexer.getWord());
    };

    void testMultipleLines() {
        Lexer lexer("solid #000\nsolid #fff");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOC, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#fff", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOC, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOF, lexer.getWord());
    };

    void testWhitespace() {
        Lexer lexer("\nsolid #000\n\nsolid #fff\n");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOC, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#fff", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOC, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOF, lexer.getWord());
    };

    void testEOFisPermanent() {
        Lexer lexer("solid #000");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL("#000", lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOC, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOF, lexer.getWord());
        CPPUNIT_ASSERT_STRINGS_EQUAL(LEXER_EOF, lexer.getWord());
    };

    void CPPUNIT_ASSERT_STRINGS_EQUAL(const char *expected, const char *actual) {
        if (expected == NULL && actual == NULL) return;
        if (expected == NULL) { CPPUNIT_FAIL("expected NULL but received value"); }
        if (actual == NULL) { CPPUNIT_FAIL("expected value but received NULL"); }
        CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(actual));
    }
};