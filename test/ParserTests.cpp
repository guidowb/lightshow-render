#include "../src/Parser.h"

#include <stdlib.h>
#include <cppunit/extensions/HelperMacros.h>

class ParserTests : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( ParserTests );
    CPPUNIT_TEST( testSimpleCommand );
    CPPUNIT_TEST( testExtraArguments );
    CPPUNIT_TEST( testInvalidColor );
    CPPUNIT_TEST( testInvalidColorDigit );
    CPPUNIT_TEST( testInvalidColorLength );
    CPPUNIT_TEST( testShortRGB );
    CPPUNIT_TEST( testShortRGBA );
    CPPUNIT_TEST( testLongRGB );
    CPPUNIT_TEST( testLongRGBA );
    CPPUNIT_TEST_SUITE_END();

    void testSimpleCommand() {
        Parser parser("testSimpleCommand", "solid");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser.getCommand());
        parser.endCommand();
        CPPUNIT_ASSERT_EQUAL(0, parser.maxErrorLevel());
    }

    void testExtraArguments() {
        Parser parser("testExtraArguments", "solid blah");
        CPPUNIT_ASSERT_STRINGS_EQUAL("solid", parser.getCommand());
        parser.endCommand();
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColor() {
        Parser parser("testInvalidColor", "nonsense");
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColorDigit() {
        Parser parser("testInvalidColorDigit", "#00zz0000");
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testInvalidColorLength() {
        Parser parser("testInvalidColorLength", "#12345");
        CPPUNIT_ASSERT_EQUAL(RGBA_NULL, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_ERROR, parser.maxErrorLevel());
    }

    void testShortRGB() {
        Parser parser("testShortRGB", "#123");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0x112233FF, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testShortRGBA() {
        Parser parser("testShortRGBA", "#abcd");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0xAABBCCDD, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testLongRGB() {
        Parser parser("testLongRGB", "#12345678");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0x12345678, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }

    void testLongRGBA() {
        Parser parser("testLongRGBA", "#09ABCDEF");
        CPPUNIT_ASSERT_EQUAL((uint32_t) 0x09ABCDEF, parser.getColor());
        CPPUNIT_ASSERT_EQUAL(LEXER_OK, parser.maxErrorLevel());
    }
};
