#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/Lexer.h"
#include "TestHelpers.h"

class LexerTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( LexerTests );
    // CPPUNIT_TEST( testSingleLine );
    // CPPUNIT_TEST( testMultipleLines );
    // CPPUNIT_TEST( testWhitespace );
    // CPPUNIT_TEST( testEOFisPermanent );
    CPPUNIT_TEST_SUITE_END();

    void testSingleLine() {
        Lexer lexer("LexerTests::testSingleLine", "solid #000");
        CPPUNIT_ASSERT(lexer.next() == "solid");
        CPPUNIT_ASSERT(lexer.next() == "#000");
        CPPUNIT_ASSERT(lexer.next().isEOL());
        CPPUNIT_ASSERT(lexer.next().isEOF());
    };

    void testMultipleLines() {
        Lexer lexer("LexerTests::testMultipleLines", "solid #000\nsolid #fff");
        CPPUNIT_ASSERT(lexer.next() == "solid");
        CPPUNIT_ASSERT(lexer.next() == "#000");
        CPPUNIT_ASSERT(lexer.next().isEOL());
        CPPUNIT_ASSERT(lexer.next() == "solid");
        CPPUNIT_ASSERT(lexer.next() == "#fff");
        CPPUNIT_ASSERT(lexer.next().isEOL());
        CPPUNIT_ASSERT(lexer.next().isEOF());
    };

    void testWhitespace() {
        Lexer lexer("LexerTests::testWhiteSpace", "\nsolid #000\n\nsolid #fff\n");
        CPPUNIT_ASSERT(lexer.next() == "solid");
        CPPUNIT_ASSERT(lexer.next() == "#000");
        CPPUNIT_ASSERT(lexer.next().isEOL());
        CPPUNIT_ASSERT(lexer.next() == "solid");
        CPPUNIT_ASSERT(lexer.next() == "#fff");
        CPPUNIT_ASSERT(lexer.next().isEOL());
        CPPUNIT_ASSERT(lexer.next().isEOF());
    };

    void testEOFisPermanent() {
        Lexer lexer("LexerTests::testEOFisPermanent", "solid #000");
        CPPUNIT_ASSERT(lexer.next() == "solid");
        CPPUNIT_ASSERT(lexer.next() == "#000");
        CPPUNIT_ASSERT(lexer.next().isEOL());
        CPPUNIT_ASSERT(lexer.next().isEOF());
        CPPUNIT_ASSERT(lexer.next().isEOF());
    };
};
