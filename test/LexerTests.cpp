#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/Lexer.h"
#include "TestHelpers.h"

class LexerTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( LexerTests );
    CPPUNIT_TEST( testSingleLine );
    CPPUNIT_TEST( testMultipleLines );
    CPPUNIT_TEST( testWhitespace );
    CPPUNIT_TEST( testEOFisPermanent );
    CPPUNIT_TEST_SUITE_END();

    void testSingleLine() {
        Lexer lexer("testSingleLine", "solid #000");
        CPPUNIT_ASSERT(lexer.getWord() == "solid");
        CPPUNIT_ASSERT(lexer.getWord() == "#000");
        CPPUNIT_ASSERT(lexer.getWord().isEOC());
        CPPUNIT_ASSERT(lexer.getWord().isEOF());
    };

    void testMultipleLines() {
        Lexer lexer("testMultipleLines", "solid #000\nsolid #fff");
        CPPUNIT_ASSERT(lexer.getWord() == "solid");
        CPPUNIT_ASSERT(lexer.getWord() == "#000");
        CPPUNIT_ASSERT(lexer.getWord().isEOC());
        CPPUNIT_ASSERT(lexer.getWord() == "solid");
        CPPUNIT_ASSERT(lexer.getWord() == "#fff");
        CPPUNIT_ASSERT(lexer.getWord().isEOC());
        CPPUNIT_ASSERT(lexer.getWord().isEOF());
    };

    void testWhitespace() {
        Lexer lexer("testWhiteSpace", "\nsolid #000\n\nsolid #fff\n");
        CPPUNIT_ASSERT(lexer.getWord() == "solid");
        CPPUNIT_ASSERT(lexer.getWord() == "#000");
        CPPUNIT_ASSERT(lexer.getWord().isEOC());
        CPPUNIT_ASSERT(lexer.getWord() == "solid");
        CPPUNIT_ASSERT(lexer.getWord() == "#fff");
        CPPUNIT_ASSERT(lexer.getWord().isEOC());
        CPPUNIT_ASSERT(lexer.getWord().isEOF());
    };

    void testEOFisPermanent() {
        Lexer lexer("testEOFisPermanent", "solid #000");
        CPPUNIT_ASSERT(lexer.getWord() == "solid");
        CPPUNIT_ASSERT(lexer.getWord() == "#000");
        CPPUNIT_ASSERT(lexer.getWord().isEOC());
        CPPUNIT_ASSERT(lexer.getWord().isEOF());
        CPPUNIT_ASSERT(lexer.getWord().isEOF());
    };
};
