#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/Parser.h"
#include "TestHelpers.h"

class ParserTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( ParserTests );
    CPPUNIT_TEST( testSimpleCommand );
    CPPUNIT_TEST( testExtraArguments );
    CPPUNIT_TEST( testInvalidInt );
    CPPUNIT_TEST( testValidInt );
    CPPUNIT_TEST( testInvalidColor );
    CPPUNIT_TEST( testInvalidColorDigit );
    CPPUNIT_TEST( testInvalidColorLength );
    CPPUNIT_TEST( testShortRGB );
    CPPUNIT_TEST( testShortRGBA );
    CPPUNIT_TEST( testLongRGB );
    CPPUNIT_TEST( testLongRGBA );
    CPPUNIT_TEST_SUITE_END();

    void testSimpleCommand() {
        Parser parser("ParserTests::testSimpleCommand", "solid");
        CPPUNIT_ASSERT(parser.getCommand() == "solid");
        parser.endCommand();
        CPPUNIT_ASSERT_EQUAL(0, parser.maxErrorLevel());
    }

    void testExtraArguments() {
        Parser parser("ParserTests::testExtraArguments", "solid blah");
        CPPUNIT_ASSERT(parser.getCommand() == "solid");
        parser.endCommand();
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidInt() {
        Parser parser("ParserTests::testInvalidInt", "nonsense");
        CPPUNIT_ASSERT_EQUAL(0, parser.getInteger());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testValidInt() {
        Parser parser("ParserTests::testValidInt", "1234");
        CPPUNIT_ASSERT_EQUAL(1234, parser.getInteger());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testInvalidColor() {
        Parser parser("ParserTests::testInvalidColor", "nonsense");
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColorDigit() {
        Parser parser("ParserTests::testInvalidColorDigit", "#00zz0000");
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColorLength() {
        Parser parser("ParserTests::testInvalidColorLength", "#12345");
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testShortRGB() {
        Parser parser("ParserTests::testShortRGB", "#123");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0x112233FF, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testShortRGBA() {
        Parser parser("ParserTests::testShortRGBA", "#abcd");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0xAABBCCDD, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testLongRGB() {
        Parser parser("ParserTests::testLongRGB", "#12345678");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0x12345678, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testLongRGBA() {
        Parser parser("ParserTests::testLongRGBA", "#09ABCDEF");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0x09ABCDEF, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }
};
